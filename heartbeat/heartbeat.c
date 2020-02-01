
#include "heartbeat.h"
#include "../watchdog/watchdog.h"
#include "../daemon/daemon.h"
#include "../path_manage/path_manage.h"

DAEMON_HEARTBEAT_CTRL __daemon_heartbeat_ctrl;

/*初始化心跳控制数据结构*/
void ini_heartbeat_ctrl(){

    memset(&__daemon_heartbeat_ctrl,0,sizeof(DAEMON_HEARTBEAT_CTRL));

    /*看门狗喂狗超时时间*/
    __daemon_heartbeat_ctrl.wdi_keep_time           = WDI_KEEP_TIME;
    /*重启过程中的看门狗喂狗超时时间*/
    __daemon_heartbeat_ctrl.wdi_reboot_keep_time    = WDI_REBOOT_KEEP_TIME;
    /*主模块最大喂狗间隔*/
    __daemon_heartbeat_ctrl.heartbeat_lost_limit    = HEARTBEAT_LOST_LIMIT;
    /*主模块启动喂狗豁免时间*/
    __daemon_heartbeat_ctrl.load_exempt_time        = LOAD_EXEMPT_TIME;

    /*主模块加载时间*/
    __daemon_heartbeat_ctrl.system_load_time        = 0;
    /*主模块上次喂狗时间*/
    __daemon_heartbeat_ctrl.heartbeat_time          = 0;
}

/*设置主系统启动时间*/
void set_system_load_time() {
    __daemon_heartbeat_ctrl.system_load_time = get_time_sec();
}
/*设置心跳时间*/
void set_heartbeat_time(){
    __daemon_heartbeat_ctrl.heartbeat_time = get_time_sec();
}
/*设置心跳控制参数*/
unsigned int set_heartbeat_ctrl(unsigned int param_num,unsigned int * param){

    if(param_num != HEARTBEAT_CTRL_PARAM_NUM){
        return IOT_FAILED;
    }
    else{}

    if(param == NULL){
        return IOT_FAILED;
    }
    else{}

    unsigned int wdi_keep_time = __daemon_heartbeat_ctrl.wdi_keep_time;

    if((param[0]>=MIN_WDI_KEEP_TIME)&&(param[0]<=MAX_WDI_KEEP_TIME)){
        __daemon_heartbeat_ctrl.wdi_keep_time = param[0];
    }
    else{
    }

    if((param[1]>=MIN_WDI_REBOOT_KEEP_TIME)&&(param[1]<=MAX_WDI_REBOOT_KEEP_TIME)){
        __daemon_heartbeat_ctrl.wdi_reboot_keep_time = param[1];
    }
    else{
    }

    if((param[2]>=MIN_HEARTBEAT_LOST_LIMIT)||(param[2]<=MAX_HEARTBEAT_LOST_LIMIT)){
        __daemon_heartbeat_ctrl.heartbeat_lost_limit = param[2];
    }
    else{
    }

    if((param[3]>=MIN_LOAD_EXEMPT_TIME)||(param[3]<=MAX_LOAD_EXEMPT_TIME)){
        __daemon_heartbeat_ctrl.load_exempt_time = param[3];
    }
    else{
    }

    if(wdi_keep_time != __daemon_heartbeat_ctrl.wdi_keep_time){
        set_keeptime_watchdog(__daemon_heartbeat_ctrl.wdi_keep_time);
    }
    else{
    }

    return IOT_SUCCEESE;
}

unsigned int get_heartbeat_ctrl(unsigned int param_buffer_len,unsigned int * param){

    if(param_buffer_len < HEARTBEAT_CTRL_PARAM_NUM){
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
    param[0] = __daemon_heartbeat_ctrl.wdi_keep_time;
    param[1] = __daemon_heartbeat_ctrl.wdi_reboot_keep_time;
    param[2] = __daemon_heartbeat_ctrl.heartbeat_lost_limit;
    param[3] = __daemon_heartbeat_ctrl.load_exempt_time;

    return HEARTBEAT_CTRL_PARAM_NUM;
}

/*检查是否心跳停止超时*/
unsigned int check_heartbeat() {

    static unsigned int first_exceed_flag = IOT_SUCCEESE;

    /*系统启动后由一定时间的豁免期,即使不喂狗也会判定正常*/
    unsigned int time = get_time_sec();
    unsigned int load_time_interval = time - __daemon_heartbeat_ctrl.system_load_time;
    unsigned int heart_beat_interval = time - __daemon_heartbeat_ctrl.heartbeat_time;

    /*检查系统启动豁免时间情况*/
    unsigned int load_exempt_time_remain = 0;
    if((load_time_interval < __daemon_heartbeat_ctrl.load_exempt_time) \
            && (heart_beat_interval > __daemon_heartbeat_ctrl.heartbeat_lost_limit)) {
        load_exempt_time_remain = __daemon_heartbeat_ctrl.load_exempt_time - load_time_interval;
        if(load_exempt_time_remain %5 == 0){
            IOT_MSG("%s : main module boot exempt time remain %d secs.\n", \
                    __FUNCTION__, load_exempt_time_remain);
        }
        else{}
        first_exceed_flag = IOT_SUCCEESE;
        return IOT_SUCCEESE;
    }
    else{
    }

    /*检查喂狗间隔*/
    if(heart_beat_interval > __daemon_heartbeat_ctrl.heartbeat_lost_limit) {
        if(first_exceed_flag == IOT_SUCCEESE){
            IOT_WARN("%s : heartbeat signal has lost in %d secs.\n", \
                     __FUNCTION__, heart_beat_interval );
            first_exceed_flag = IOT_FAILED;
        }
        else {
            if(heart_beat_interval % 300 == 0){
                IOT_WARN("%s : heartbeat signal has lost in %d secs.\n", \
                         __FUNCTION__, heart_beat_interval );
            }
            else{
            }
        }
        return IOT_FAILED;
    }
    else{
        first_exceed_flag = IOT_SUCCEESE;
        return IOT_SUCCEESE;
    }
}
unsigned int get_reboot_keep_time(){
    return __daemon_heartbeat_ctrl.wdi_reboot_keep_time;
}
