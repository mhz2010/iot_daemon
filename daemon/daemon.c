
#include "../path_manage/path_manage.h"
#include "../firmware/firmware.h"
#include "../mq_manage/mq_manage.h"
#include "../heartbeat/heartbeat.h"
#include "../OTA/OTA.h"
#include "../watchdog/watchdog.h"
#include "../log_manage/log_manage.h"

#include "daemon.h"

DAEMON_CTRL __daemon_ctrl;

/*----------------------------------------------------------------------*/
/*                                                                      */
/*                              daemon退出或重启                          */
/*                                                                      */
/*----------------------------------------------------------------------*/

/*********************************************************************/
/*
*           ini_daemon_ctrl
*               初始化daemon_ctrl记录
*
* @param    void
*
* @return   unsigned int
*               成功:IOT_SUCCEESE
*               失败:
*
* @note
*
**********************************************************************/
unsigned int ini_daemon_ctrl(){

    memset(&__daemon_ctrl,0,sizeof(DAEMON_CTRL));

    /*config*/
    __daemon_ctrl.daemon_auto_reboot_period = DAEMON_AUTO_REBOOT_PERIOD;
    __daemon_ctrl.system_boot_confirm_time = SYSTEM_BOOT_CONFIRM_TIME;
    __daemon_ctrl.system_exit_wait_time = SYSTEM_EXIT_WAIT_TIME;

    /*daemon ctrl*/
    __daemon_ctrl.daemon_load_time      = get_time_sec();
    __daemon_ctrl.daemon_ctrl           = DAEMON_CTRL_IDLE;

    /*boot flag*/
    __daemon_ctrl.system_state          = SYSTEM_STATE_STOP;
    __daemon_ctrl.system_ctrl           = SYSTEM_CTRL_IDLE;
    __daemon_ctrl.system_OTA_state      = SYSTEM_OTA_IDLE;

    /*time and cnt*/
    __daemon_ctrl.system_reload_cnt     = 0;    /*程序加载尝试计数*/
    __daemon_ctrl.system_recover_cnt    = 0;
    __daemon_ctrl.system_recover_time   = get_time_sec();
    __daemon_ctrl.system_load_time      = 0;
    __daemon_ctrl.system_boot_time      = 0;

    /*system pid*/
    __daemon_ctrl.system_pid            = -1;

    return IOT_SUCCEESE;
}

/*********************************************************************/
/*
*           daemon_exit
*               退出
*
* @param    void
*
* @return   unsigned int
*               成功:IOT_SUCCEESE
*               失败:IOT_FAILED
*
* @note
*
**********************************************************************/
unsigned int daemon_exit(){

    IOT_WARN("%s : rcv daemon exit cmd, exitting...\n",__FUNCTION__);
    IOT_WARN("%s : main modlue exit cmd sent\n",__FUNCTION__);

    /*发送主模块退出信号*/
    heartbeat_msg_ctrl(IOT_CTRL_EXIT);
    unsigned int time = SYSTEM_EXIT_WAIT_TIME;
    while(time!=0) {
        IOT_WARN("%s : waiting %d seconds for main modlue exit\n",\
                 __FUNCTION__, time);
        usleep(1000*1000);
        time--;
        feed_watchdog();
    }

    kill_main_module(); /*强制终止主进程*/
    release_watchdog(); /*停止看门狗功能*/

    IOT_WARN("%s : Iot_Daemon exited\n",__FUNCTION__);

    close_daemon_log_file();

    /*log操作*/
    exit (0);
}

/*********************************************************************/
/*
*           daemon_reboot
*               重启系统
*
* @param    void
*
* @return   unsigned int
*               成功:IOT_SUCCEESE
*               失败:IOT_FAILED
*
* @note
*
**********************************************************************/
unsigned int daemon_reboot(){

    IOT_WARN("%s : rcv daemon reboot cmd, exitting...\n",__FUNCTION__);
    IOT_WARN("%s : main modlue exit cmd sent\n",__FUNCTION__);

    /*发送主模块退出信号*/
    heartbeat_msg_ctrl(IOT_CTRL_EXIT);
    unsigned int time = SYSTEM_EXIT_WAIT_TIME;
    while(time!=0) {
        IOT_WARN("%s : waiting %d seconds for main modlue exit\n",\
                 __FUNCTION__, time);
        usleep(1000*1000);
        time--;
        feed_watchdog();
    }

    /*强制终止主进程*/
    kill_main_module();

    IOT_WARN("%s : Iot_Daemon Reboot\n",__FUNCTION__);
    IOT_WARN("%s : feed_watchdog stoped, halt system and reboot.\n",__FUNCTION__);

    unsigned int wdi_reboot_keep_time = get_reboot_keep_time();
    set_keeptime_watchdog(wdi_reboot_keep_time);

#ifdef HALT_SYS
    IOT_WARN("%s : halt system.\n",__FUNCTION__);
    close_daemon_log_file();
    system("reboot");
#else
    close_daemon_log_file();
#endif

    exit (0);
}

/*********************************************************************/
/*
*           daemon_check_auto_reboot
*               周期重启系统
*
* @param
*
* @return
*
* @note
*
**********************************************************************/
unsigned int daemon_check_auto_reboot() {

    static unsigned int loop = 0;

    unsigned int time = get_time_sec();
    unsigned int daemon_load_time = __daemon_ctrl.daemon_load_time;
    unsigned int auto_reboot_period = __daemon_ctrl.daemon_auto_reboot_period;

    /*检查自动重启*/
    if( (time-daemon_load_time) > auto_reboot_period) {

        unsigned int byte = time/DAEMON_AUTO_REBOOT_TIME_BASE;
        unsigned int bit = time - byte * DAEMON_AUTO_REBOOT_TIME_BASE;

        unsigned int period_offset = bit;

        unsigned int down_window = DAEMON_AUTO_REBOOT_TIME_OFFSET;
        unsigned int up_window   = DAEMON_AUTO_REBOOT_TIME_OFFSET+DAEMON_AUTO_REBOOT_TIME_WINDOW;

        if( (period_offset >= down_window) && (period_offset <= up_window) ) {
            IOT_MSG("%s : DAEMON_AUTO_REBOOT_PERIOD reached, reboot.\n",__FUNCTION__);
            IOT_MSG("%s : period_offset = %d, down_window = %d, up_window = %d.\n",\
                    __FUNCTION__,period_offset,down_window,up_window);
            __daemon_ctrl.daemon_ctrl = DAEMON_CTRL_REBOOT;
            loop = 0;
        }
        else{
            if(loop % 10 == 0)
            {
                IOT_INFO("%s : DAEMON_AUTO_REBOOT_PERIOD reached, waiting for DAEMON_AUTO_REBOOT_TIME_WINDOW.\n",__FUNCTION__);
                IOT_INFO("%s : period_offset = %d, down_window = %d, up_window = %d.\n",__FUNCTION__,period_offset,down_window,up_window);
            }
            else{}
            loop++;
        }
    }
    else{
    }

    return IOT_SUCCEESE;
}

unsigned int daemon_load_system(){

    if(__daemon_ctrl.system_OTA_state == SYSTEM_OTA_UPDATE){
        return IOT_SUCCEESE;
    }
    else{
    }

    IOT_MSG("%s : reloading...\n",__FUNCTION__);
    kill_main_module();

    ini_mq();
    set_heartbeat_time();
    set_system_load_time();
    load_main_module();

    return IOT_SUCCEESE;
}

/*设置daemon控制参数*/
unsigned int set_daemon_ctrl(unsigned int param_num,unsigned int * param){

    if(param_num != DAEMON_CTRL_PARAM_NUM){
        IOT_ERR("%s : err param_num\n", __FUNCTION__);
        return IOT_FAILED;
    }
    else{}

    if(param == NULL){
        IOT_ERR("%s : NULL param\n", __FUNCTION__);
        return IOT_FAILED;
    }
    else{}

    if((param[0]>=MIN_DAEMON_AUTO_REBOOT_PERIOD)&&(param[0]<=MAX_DAEMON_AUTO_REBOOT_PERIOD)){
        __daemon_ctrl.daemon_auto_reboot_period = param[0];
    }
    else{
    }

    if((param[1]>=MIN_SYSTEM_BOOT_CONFIRM_TIME)&&(param[1]<=MAX_SYSTEM_BOOT_CONFIRM_TIME)){
        __daemon_ctrl.system_boot_confirm_time = param[1];
    }
    else{
    }

    if((param[2]>=MIN_SYSTEM_EXIT_WAIT_TIME)||(param[2]<=MAX_SYSTEM_EXIT_WAIT_TIME)){
        __daemon_ctrl.system_exit_wait_time = param[2];
    }
    else{
    }


    return IOT_SUCCEESE;
}

unsigned int get_daemon_ctrl(unsigned int param_buffer_len,unsigned int * param){

    if(param_buffer_len < DAEMON_CTRL_PARAM_NUM){
        IOT_ERR("%s : param buffer len too short\n",__FUNCTION__);
        return IOT_FAILED;
    }
    else{}

    if(param == NULL){
        IOT_ERR("%s : NULL param buffer\n",__FUNCTION__);
        return IOT_FAILED;
    }
    else{}

    memset(param, 0 ,sizeof(unsigned int) * param_buffer_len);
    param[0] = __daemon_ctrl.daemon_auto_reboot_period;
    param[1] = __daemon_ctrl.system_boot_confirm_time;
    param[2] = __daemon_ctrl.system_exit_wait_time;

    return DAEMON_CTRL_PARAM_NUM;
}

unsigned int get_daemon_version(unsigned int param_buffer_len,unsigned int * param){

    if(param_buffer_len < DAEMON_VER_PARAM_NUM){
        IOT_ERR("%s : param buffer len too short\n",__FUNCTION__);
        return IOT_FAILED;
    }
    else{}

    if(param == NULL){
        IOT_ERR("%s : NULL param buffer\n",__FUNCTION__);
        return IOT_FAILED;
    }
    else{}

    memset(param, 0 ,sizeof(unsigned int) * param_buffer_len);
    param[0] = DAEMON_VER_MAJOR;
    param[1] = DAEMON_VER_MINOR;

    return DAEMON_VER_PARAM_NUM;
}

unsigned int dump_daemon_ctrl_param(DAEMON_CTRL * daemon_ctrl){

    printf("dump_daemon_ctrl_param...\n");

    if(daemon_ctrl!=NULL){
        printf("    daemon_auto_reboot_period   : %d sec.\n",\
               daemon_ctrl->daemon_auto_reboot_period);
        printf("    system_boot_confirm_time    : %d sec.\n",\
               daemon_ctrl->system_boot_confirm_time);
        printf("    system_exit_wait_time       : %d sec.\n",\
               daemon_ctrl->system_exit_wait_time);
    }
    else{
        printf("    NULL\n");
    }
    return IOT_SUCCEESE;
}

unsigned int dump_daemon_ctrl(DAEMON_CTRL * daemon_ctrl){

    printf("dump_daemon_ctrl...\n");

    if(daemon_ctrl != NULL){
        printf("    daemon_auto_reboot_period   : %d sec.\n",daemon_ctrl->daemon_auto_reboot_period);
        printf("    system_boot_confirm_time    : %d sec.\n",daemon_ctrl->system_boot_confirm_time);
        printf("    system_exit_wait_time       : %d sec.\n",daemon_ctrl->system_exit_wait_time);
        printf("\n");
        printf("    daemon_load_time            : %d sec.\n",daemon_ctrl->daemon_load_time);
        printf("    daemon_ctrl                 : %d .\n",daemon_ctrl->daemon_ctrl);
        printf("\n");
        printf("    system_state                : %d .\n",daemon_ctrl->system_state);
        printf("    system_ctrl                 : %d .\n",daemon_ctrl->system_ctrl);
        printf("    system_OTA_state            : %d .\n",daemon_ctrl->system_OTA_state);
        printf("\n");
        printf("    system_recover_cnt          : %d .\n",daemon_ctrl->system_recover_cnt);
        printf("    system_reload_cnt           : %d .\n",daemon_ctrl->system_reload_cnt);
        printf("    system_recover_time         : %d sec.\n",daemon_ctrl->system_recover_time);
        printf("    system_load_time            : %d sec.\n",daemon_ctrl->system_load_time);
        printf("    system_boot_time            : %d sec.\n",daemon_ctrl->system_boot_time);
        printf("\n");
        printf("    system_pid                  : %d .\n",(int)daemon_ctrl->system_pid);
    }
    else{
        printf("    NULL\n");
    }
    return IOT_SUCCEESE;

}

/*----------------------------------------------------------------------*/
/*                                                                      */
/*                              daemon 处理                              */
/*                                                                      */
/*----------------------------------------------------------------------*/

/*********************************************************************/
/*
*           daemon_daemon_ctrl_process
*               处理守护模块控制
*
* @param    void
*
* @return   unsigned int
*               成功:IOT_SUCCEESE
*               失败:IOT_FAILED
*
* @note
*           考虑当处于OTA状态时是否执行
*           考虑当system正在引导过程中是否执行
*
**********************************************************************/
unsigned int daemon_ctrl_process(){

    /*处理指令daemon_ctrl*/
    if(__daemon_ctrl.daemon_ctrl == DAEMON_CTRL_EXIT){
        daemon_exit();
    }
    else if(__daemon_ctrl.daemon_ctrl == DAEMON_CTRL_REBOOT) {
        daemon_reboot();
    }
    else{
        __daemon_ctrl.daemon_ctrl = DAEMON_CTRL_IDLE;
    }

    /*检查auto_reboot*/
    daemon_check_auto_reboot();
    if(__daemon_ctrl.daemon_ctrl == DAEMON_CTRL_EXIT){
        daemon_exit();
    }
    else if(__daemon_ctrl.daemon_ctrl == DAEMON_CTRL_REBOOT) {
        daemon_reboot();
    }
    else{
        __daemon_ctrl.daemon_ctrl = DAEMON_CTRL_IDLE;
    }

    return IOT_SUCCEESE;
}

/*********************************************************************/
/*
*           daemon_OTA_process
*               处理OTA
*
* @param    void
*
* @return   unsigned int
*               成功:IOT_SUCCEESE
*               失败:IOT_FAILED
*
* @note
*
**********************************************************************/
unsigned int daemon_OTA_process(){

    /*如果当前没有OTA,直接反馈成功*/
    if(__daemon_ctrl.system_OTA_state == SYSTEM_OTA_IDLE){
    }

    /*如果当前为等待OTA更新固件状态,并且已经发送系统重新加载指令,则更新固件*/
    else if(__daemon_ctrl.system_OTA_state == SYSTEM_OTA_UPDATE){

        /*如果未处于等待重新加载状态,说明主程序尚未结束,不可进行固件操作*/
        if(__daemon_ctrl.system_ctrl != SYSTEM_CTRL_RELOAD){
        }
        else{
            if(OTA_update_firmware() == IOT_SUCCEESE){ /*固件更新成功,进入确认阶段*/
                IOT_MSG("%s : OTA_update_firmware SUCCEESE\n", __FUNCTION__);
                __daemon_ctrl.system_recover_cnt = 0;
                __daemon_ctrl.system_reload_cnt = 0;
                __daemon_ctrl.system_OTA_state = SYSTEM_OTA_CONFIRM;
            }
            else{                                       /*固件更新失败,退出OTA*/
                IOT_ERR("%s : OTA_update_firmware FAILED\n", __FUNCTION__);
                __daemon_ctrl.system_OTA_state = SYSTEM_OTA_IDLE;
            }
        }

    }
    /*如果当前为等待OTA确认状态,并且已经正确加载系统,则确认OTA更改*/
    else if(__daemon_ctrl.system_OTA_state == SYSTEM_OTA_CONFIRM){

        /*程序进入运行状态,说明新版本OTA成功,进行确认*/
        if(__daemon_ctrl.system_state == SYSTEM_STATE_RUN){
            IOT_MSG("%s : OTA_confirm_firmware.\n", __FUNCTION__);
            OTA_confirm_firmware();
            __daemon_ctrl.system_OTA_state = SYSTEM_OTA_IDLE;
        }

        /*如果出现回滚操作,则说明OTA固件失败,进行回滚*/
        else if(__daemon_ctrl.system_ctrl == SYSTEM_CTRL_RECOVER){

            /*从temp文件恢复active*/
            if(OTA_rollback_firmware() == IOT_SUCCEESE){
                IOT_MSG("%s : OTA_rollback_firmware OK\n", __FUNCTION__);
                __daemon_ctrl.system_reload_cnt = 0;
                __daemon_ctrl.system_recover_cnt = 0;
                __daemon_ctrl.system_recover_time = get_time_sec();
            }

            /*从backup文件恢复active*/
            else{
                IOT_MSG("%s : OTA_rollback_firmware failed, try OTA_recover_firmware.\n", __FUNCTION__);
                OTA_recover_firmware();
                __daemon_ctrl.system_reload_cnt = 0;
                __daemon_ctrl.system_recover_cnt ++;
                __daemon_ctrl.system_recover_time = get_time_sec();
            }

            __daemon_ctrl.system_OTA_state = SYSTEM_OTA_IDLE;
            __daemon_ctrl.system_ctrl = SYSTEM_CTRL_IDLE;
            __daemon_ctrl.system_state = SYSTEM_STATE_STOP;
        }
        else{
        }
    }
    else{
        __daemon_ctrl.system_OTA_state = SYSTEM_OTA_IDLE;
    }
    return IOT_SUCCEESE;
}

/*********************************************************************/
/*
*           daemon_heartbeat_process
*               处理心跳信息
*
* @param
*
* @return
*
* @note
*
**********************************************************************/
unsigned int daemon_heartbeat_process(){

    /*记录心跳信号是否曾经到达*/
    static unsigned int heatbeat_arrived_flag = IOT_FAILED;

    MSG_HEARTBEAT * msg;
    unsigned int msg_flag = IOT_SUCCEESE;

    do{
        /*接收心跳信息*/
        msg = get_heartbeat_msg(MSG_HEARTBEAT_TYPE_REQ);
        if(msg != NULL) {

            IOT_INFO("%s : heartbeat receiverd\n",__FUNCTION__);

            msg_flag = IOT_SUCCEESE;

            set_heartbeat_time(); /*重置心跳    */

            /*如果为首次心跳,则记录首次心跳到来时间*/
            if((__daemon_ctrl.system_state == SYSTEM_STATE_LOAD) \
                    && (heatbeat_arrived_flag == IOT_FAILED)){

                __daemon_ctrl.system_boot_time = get_time_sec();
                IOT_MSG("%s : first heatbeat arrived, time = %d.\n",\
                        __FUNCTION__,__daemon_ctrl.system_boot_time);
                heatbeat_arrived_flag = IOT_SUCCEESE;
            }
            else{
            }

            /*如果心跳到达时间超过启动确认时间,则清除重载和恢复计数,确认为运行状态*/
            if(get_time_sec() - __daemon_ctrl.system_boot_time > __daemon_ctrl.system_boot_confirm_time){
                __daemon_ctrl.system_reload_cnt = 0;
                __daemon_ctrl.system_recover_cnt = 0;
                __daemon_ctrl.system_state = SYSTEM_STATE_RUN;
            }
            else{}

            /*处理OTA*/
            if(msg->IOT_OTA_flag == IOT_OTA_REQ) {
                if(__daemon_ctrl.system_OTA_state == SYSTEM_OTA_IDLE){
                    IOT_MSG("%s : OTA signal received\n",__FUNCTION__);
                    __daemon_ctrl.system_OTA_state = SYSTEM_OTA_UPDATE;
                    __daemon_ctrl.system_reload_cnt = 0;
                    __daemon_ctrl.system_recover_cnt = 0;
                    __daemon_ctrl.system_state = SYSTEM_STATE_RUN;/*直接确认为运行状态*/
                    heartbeat_msg_ctrl(IOT_CTRL_EXIT);/*发送主模块退出信号*/
                }
                else{}
            }
            else{}
            destroy_heartbeat_msg(&msg);
        }
        else{
            msg_flag = IOT_FAILED;
        }
    }while(msg_flag == IOT_SUCCEESE);

    /*如果心跳已经超时,则进行处理*/
    if(check_heartbeat() == IOT_FAILED){
        if(__daemon_ctrl.system_state == SYSTEM_STATE_LOAD){
            __daemon_ctrl.system_state = SYSTEM_SATAE_LOAD_FAIL;
            heatbeat_arrived_flag = IOT_FAILED;
        }
        else if(__daemon_ctrl.system_state == SYSTEM_STATE_RUN){
            __daemon_ctrl.system_state = SYSTEM_STATE_STOP;
            heatbeat_arrived_flag = IOT_FAILED;
        }
        /*stop 或 fail状态*/
        else{
            heatbeat_arrived_flag = IOT_FAILED;
        }
    }
    else{}

    return IOT_SUCCEESE;
}

/*********************************************************************/
/*
*           daemon_ctrl_msg_process
*               处理守护模块控制消息
*
* @param    void
*
* @return   unsigned int
*               成功:IOT_SUCCEESE
*               失败:IOT_FAILED
*
* @note
*
**********************************************************************/
unsigned int daemon_ctrl_msg_process(){

    /*获取控制信号*/
    MSG_DAEMON_CTRL * msg;
    msg = get_daemon_ctrl_msg(MSG_DAEMON_CTRL_TYPE_REQ);
    if(msg == NULL) {
        return IOT_SUCCEESE;
    }
    else{}

    if(msg -> iot_daemon_ctrl == IOT_DAEMON_CTRL_EXIT){
        IOT_WARN("%s : iot daemon ctrl EXIT\n",__FUNCTION__);
        __daemon_ctrl.daemon_ctrl = DAEMON_CTRL_EXIT;
    }
    else if(msg -> iot_daemon_ctrl == IOT_DAEMON_CTRL_REBOOT){
        IOT_WARN("%s : iot daemon ctrl REBOOT\n",__FUNCTION__);
        __daemon_ctrl.daemon_ctrl = DAEMON_CTRL_REBOOT;
    }
    else if(msg -> iot_daemon_ctrl == IOT_DAEMON_CTRL_SET_PARAM){
        IOT_INFO("%s : set iot daemon param\n",__FUNCTION__);
        if(msg ->iot_daemon_param_type == IOT_DAEMON_PARAM_DAEMON){
            set_daemon_ctrl(msg->iot_daemon_param_num,msg->iot_daemon_param);
        }
        else if(msg ->iot_daemon_param_type == IOT_DAEMON_PARAM_LOG){
            set_daemon_log_ctrl(msg->iot_daemon_param_num,msg->iot_daemon_param);
        }
        else if(msg ->iot_daemon_param_type == IOT_DAEMON_PARAM_HEARTBEAT){
            set_heartbeat_ctrl(msg->iot_daemon_param_num, msg->iot_daemon_param);
        }
        else{
            IOT_INFO("%s : unknown param type\n",__FUNCTION__);
        }
    }
    else if(msg -> iot_daemon_ctrl == IOT_DAEMON_CTRL_GET_PARAM){

        IOT_INFO("%s : get iot daemon param\n",__FUNCTION__);

        MSG_DAEMON_CTRL msg_rspd;
        memset(&msg_rspd,0,sizeof(MSG_DAEMON_CTRL));
        msg_rspd.iot_daemon_ctrl = IOT_DAEMON_CTRL_GET_PARAM;
        msg_rspd.iot_daemon_param_type = msg -> iot_daemon_param_type;

        unsigned int flag = IOT_FAILED;

        if(msg -> iot_daemon_param_type == IOT_DAEMON_PARAM_VER){
            IOT_INFO("%s : param type Version\n",__FUNCTION__);
            flag = get_daemon_version(IOT_DAEMON_PARAM_NUM,msg_rspd.iot_daemon_param);
            msg_rspd.iot_daemon_param_num = flag;
        }
        else if(msg -> iot_daemon_param_type == IOT_DAEMON_PARAM_DAEMON){
            IOT_INFO("%s : param type DAEMON\n",__FUNCTION__);
            flag = get_daemon_ctrl(IOT_DAEMON_PARAM_NUM,msg_rspd.iot_daemon_param);
            msg_rspd.iot_daemon_param_num = flag;
        }
        else if(msg ->iot_daemon_param_type == IOT_DAEMON_PARAM_LOG){
            IOT_INFO("%s : param type LOG\n",__FUNCTION__);
            flag = get_daemon_log_ctrl(IOT_DAEMON_PARAM_NUM,msg_rspd.iot_daemon_param);
            msg_rspd.iot_daemon_param_num = flag;
        }
        else if(msg ->iot_daemon_param_type == IOT_DAEMON_PARAM_HEARTBEAT){
            IOT_INFO("%s : param type HEARTBEAT\n",__FUNCTION__);
            flag = get_heartbeat_ctrl(IOT_DAEMON_PARAM_NUM, msg_rspd.iot_daemon_param);
            msg_rspd.iot_daemon_param_num = flag;
        }
        else{
            IOT_INFO("%s : unknown param type\n",__FUNCTION__);
            flag = IOT_FAILED;
        }

        if(flag != IOT_FAILED){
            IOT_INFO("%s : replay iot daemon param\n",__FUNCTION__);
            put_daemon_ctrl_msg(MSG_DAEMON_CTRL_TYPE_RSPD, &msg_rspd);
        }
        else{
            IOT_ERR("%s : get iot daemon param failed\n", __FUNCTION__);
        }
    }
    else{
        IOT_ERR("%s : unknown daemon ctrl type\n", __FUNCTION__);
    }

    destroy_daemon_ctrl_msg(& msg);

    return IOT_SUCCEESE;

}

/*********************************************************************/
/*
*           daemon_system_state_process
*               处理system状态信息
*
* @param    void
*
* @return   unsigned int
*               成功:IOT_SUCCEESE
*               失败:
*
* @note
*
**********************************************************************/
unsigned int daemon_system_state_process(){

    static unsigned int first_rcv_flag = IOT_SUCCEESE;

    /*如果系统停止,则进行重新加载*/
    if(__daemon_ctrl.system_state == SYSTEM_STATE_STOP){
        __daemon_ctrl.system_ctrl = SYSTEM_CTRL_RELOAD;
        first_rcv_flag = IOT_SUCCEESE;
    }

    /*如果系统加载失败,则进行重新加载*/
    else if(__daemon_ctrl.system_state == SYSTEM_SATAE_LOAD_FAIL){
        if(__daemon_ctrl.system_reload_cnt < SYSTEM_MAX_RELOAD_CNT){
            __daemon_ctrl.system_ctrl = SYSTEM_CTRL_RELOAD;
            first_rcv_flag = IOT_SUCCEESE;
        }
        else{
            if(first_rcv_flag == IOT_SUCCEESE){
                IOT_WARN("%s : system_reload_cnt exceeds, try recover firmware.\n", __FUNCTION__);
                first_rcv_flag = IOT_FAILED;
            }
            else{
            }

            __daemon_ctrl.system_ctrl = SYSTEM_CTRL_RECOVER;
        }
    }
    else if(__daemon_ctrl.system_state == SYSTEM_STATE_RUN){
        first_rcv_flag = IOT_SUCCEESE;
    }
    else if(__daemon_ctrl.system_state == SYSTEM_STATE_LOAD){
        first_rcv_flag = IOT_SUCCEESE;
    }
    else{
        __daemon_ctrl.system_state = SYSTEM_STATE_STOP;
        first_rcv_flag = IOT_SUCCEESE;
    }

    return IOT_SUCCEESE;
}

/*********************************************************************/
/*
*           daemon_system_ctrl_process
*               处理心跳信息
*
* @param    void
*
* @return   unsigned int
*               成功:IOT_SUCCEESE
*               失败:
*
* @note
*
**********************************************************************/
unsigned int daemon_system_ctrl_process(){

    static unsigned int rcv_cnt_exceed_flag = IOT_FAILED;

    if(__daemon_ctrl.system_ctrl == SYSTEM_CTRL_RELOAD){
        if(__daemon_ctrl.system_OTA_state == SYSTEM_OTA_UPDATE){
        }
        else if(__daemon_ctrl.system_state == SYSTEM_STATE_LOAD){ /*说明正在进行加载*/
        }
        else{
            __daemon_ctrl.system_ctrl  = SYSTEM_CTRL_IDLE;
            __daemon_ctrl.system_state = SYSTEM_STATE_LOAD;
            __daemon_ctrl.system_reload_cnt ++;
            IOT_WARN("%s : load system, system_reload_cnt %d.\n",__FUNCTION__, __daemon_ctrl.system_reload_cnt);
            daemon_load_system();
        }
        rcv_cnt_exceed_flag = IOT_FAILED;
    }
    else if(__daemon_ctrl.system_ctrl == SYSTEM_CTRL_RECOVER){

        do{
            if(__daemon_ctrl.system_OTA_state == SYSTEM_OTA_CONFIRM){
                IOT_WARN("%s : system failed in SYSTEM_OTA_CONFIRM state.\n", __FUNCTION__);
                rcv_cnt_exceed_flag = IOT_FAILED;
                break;
            }
            else{}

            if(__daemon_ctrl.system_recover_cnt < SYSTEM_MAX_RECOVER_CNT) {
                rcv_cnt_exceed_flag = IOT_FAILED;
                __daemon_ctrl.system_reload_cnt = 0;
                __daemon_ctrl.system_recover_cnt ++;
                __daemon_ctrl.system_recover_time = get_time_sec();
                __daemon_ctrl.system_ctrl = SYSTEM_CTRL_IDLE;
                __daemon_ctrl.system_state = SYSTEM_STATE_STOP;
                IOT_WARN("%s : system failed, recover firmware, system_recover_cnt %d.\n", \
                         __FUNCTION__, __daemon_ctrl.system_recover_cnt);
                OTA_recover_firmware();
            }
            else{

                /*更新rcv时间*/
                if(rcv_cnt_exceed_flag == IOT_FAILED){
                    IOT_WARN("%s : system_recover_cnt exceeded, waiting %d secs for reboot\n",\
                             __FUNCTION__,SYSTEN_FAIL_REBOOT_WAIT_TIME);
                    __daemon_ctrl.system_recover_time = get_time_sec();
                    rcv_cnt_exceed_flag = IOT_SUCCEESE;
                }
                else{
                }

                unsigned int last_recover_time = __daemon_ctrl.system_recover_time;
                unsigned int interval = get_time_sec() - last_recover_time;
                if(interval > SYSTEN_FAIL_REBOOT_WAIT_TIME){
                    IOT_WARN("%s : system failed, sent reboot cmd\n", __FUNCTION__);
                    __daemon_ctrl.daemon_ctrl = DAEMON_CTRL_REBOOT;
                }
                else{
                    unsigned int remain_time = SYSTEN_FAIL_REBOOT_WAIT_TIME - interval;
                    if(remain_time % 300 == 0){
                        IOT_WARN("%s : system failed, reboot in %d sec\n", __FUNCTION__, remain_time);
                    }
                    else{}
                }
            }
        }while(0);
    }
    else{
        __daemon_ctrl.system_ctrl = SYSTEM_CTRL_IDLE;
        rcv_cnt_exceed_flag = IOT_FAILED;
    }

    return IOT_SUCCEESE;
}

/*********************************************************************/
/*
*           daemon_log_process
*               处理日志信息
*
* @param    void
*
* @return   unsigned int
*               成功:IOT_SUCCEESE
*               失败:
*
* @note
*
**********************************************************************/
unsigned int daemon_log_process(){

    if(IOT_SUCCEESE == check_daemon_switch_time()) {
        switch_daemon_log_file();
    }
    else{}

    if(IOT_SUCCEESE == check_daemon_clear_time()) {
        clear_daemon_log_file();
    }
    else{}

    return IOT_SUCCEESE;
}
