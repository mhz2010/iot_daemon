
#include "env.h"
#include "debug.h"

#include "./daemon/daemon.h"
#include "./log_manage/log_manage.h"
#include "./watchdog/watchdog.h"
#include "./mq_manage/mq_manage.h"
#include "./heartbeat/heartbeat.h"

/*********************************************************************/
/*
*           main
*               守护进程入口
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
int main () {

    IOT_MSG("IOT Daemon system\n");
    IOT_MSG("Version : %s\n",IOT_DAEMON_VER);

    /*初始化看门狗*/
#ifndef WDT_ENABLE
    IOT_WARN("%s : Watchdog disabled.\n",__FUNCTION__);
#else
    IOT_MSG("%s : Initializing Watchdog.\n",__FUNCTION__);
    if(ini_watchdog(WDI_KEEP_TIME) == IOT_FAILED) {
        IOT_WARN("%s : ini watchdog FAILED.\n",__FUNCTION__);
    }
    else{
        IOT_MSG("%s : ini watchdog OK.\n",__FUNCTION__);
    }
#endif

    /*初始化日志控制块*/
    ini_daemon_log_ctrl();

    /*初始化守护控制块*/
    ini_daemon_ctrl();

    /*初始化心跳与喂狗时限控制*/
    ini_heartbeat_ctrl();

    /*初始化消息队列*/
    if(IOT_FAILED == ini_mq()) {
        IOT_WARN("%s : ini_mq FAILED, waiting for dog reboot.\n",__FUNCTION__);
        while(1){ sleep(1); }
    }
    else{
        IOT_MSG("%s : ini_mq OK.\n",__FUNCTION__);
    }

    while(1){

        /*喂狗*/
        IOT_INFO("feed_watchdog...\n");
        feed_watchdog();

        /*处理日志*/
        IOT_INFO("daemon_log_process...\n");
        daemon_log_process();

        /*处理守护模块控制信息*/
        IOT_INFO("daemon_ctrl_msg_process...\n");
        daemon_ctrl_msg_process();

        /*处理守护模块控制信息*/
        IOT_INFO("daemon_ctrl_process...\n");
        daemon_ctrl_process();

        /*处理心跳信息*/
        IOT_INFO("daemon_heartbeat_process...\n");
        daemon_heartbeat_process();

        /*处理OTA信息*/
        IOT_INFO("daemon_OTA_process...\n");
        daemon_OTA_process();

        /*system state*/
        IOT_INFO("daemon_system_state_process...\n");
        daemon_system_state_process();

        /*system ctrl*/
        IOT_INFO("daemon_system_ctrl_process...\n");
        daemon_system_ctrl_process();

#if 0
        IOT_INFO("daemon_ctrl after daemon_system_ctrl_process...\n");
        dump_daemon_ctrl(&__daemon_ctrl);
#endif

        IOT_INFO("daemon while finished\n\n");

        usleep(1000*1000);
    }

    return 0;
}

