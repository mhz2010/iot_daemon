
#include "firmware.h"
#include "../path_manage/path_manage.h"
#include "../daemon/daemon.h"

/*********************************************************************/
/*
*           load_main_module
*               加载主程序
*
* @param
*
* @return   unsigned int
*               成功: IOT_SUCCEESE
*               失败: IOT_FAILED
*
* @note     全部采用在当前session中启动新进程的方式
*
**********************************************************************/
void load_main_module() {

    /*获取BIN文件路径*/
    char * active_path = get_path(NUM_ACTIVE_BOOT_FILE,0);
    if(active_path == NULL) {
        IOT_ERR ("%s : get active boot file path FAILED\n",__FUNCTION__);
        return;
    }
    else{
        IOT_INFO ("%s : active boot file path %s\n",__FUNCTION__, active_path);
    }

    IOT_INFO ("%s : loading boot file %s\n",__FUNCTION__,active_path);

    /*创建子进程*/
    pid_t sys_id = fork();
    if(sys_id < 0){
        IOT_ERR("%s : sys fork FAILED.\n",__FUNCTION__);
        free(active_path);
        return;
    }
    /*father*/
    else if(sys_id > 0){
        __daemon_ctrl.system_pid = sys_id;
        free(active_path);
        IOT_INFO("%s : system_reload_cnt %d\n",\
                 __FUNCTION__, __daemon_ctrl.system_reload_cnt);
    }
    /*system*/
    else{
        IOT_INFO("%s : system path %s\n",__FUNCTION__,active_path);
        execl("/bin/bash", "bash", "-c", active_path, NULL);
    }

    return;
}

/*********************************************************************/
/*
*           kill_main_module
*               终止主程序
*
* @param
*
* @return   unsigned int
*               成功: IOT_SUCCEESE
*               失败: IOT_FAILED
*
* @note
*
**********************************************************************/
void kill_main_module() {

    if(__daemon_ctrl.system_pid > 0) {
        char buffer[256];
        sprintf(buffer,"kill -KILL %d",(int)__daemon_ctrl.system_pid );
        IOT_MSG("%s : Removal residual main module processes %d, cmd %s\n", \
                __FUNCTION__, (int)__daemon_ctrl.system_pid,buffer);
        system(buffer);
        usleep(1000*1000);
        __daemon_ctrl.system_pid = -1;
    }
    else{
        __daemon_ctrl.system_pid = -1;
    }

    return ;
}
