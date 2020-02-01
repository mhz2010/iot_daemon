/*****************************************************************************/
/**
*
* @file daemon.h
*       守护管理
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who                  Date      Changes
* ----- -------------------- ---------- ---------------------------------------
* 1.0   Ma Haozhi            2020/02/02 first release
*       mhz2010@126.com
*       Li Xiaojie
*       linsa049962@163.com
*
* </pre>
*
******************************************************************************/

#ifndef __DAEMON_H__
#define __DAEMON_H__

/*----------------------------------------------------------------------*/
/*                                                                      */
/*                                  配置信息                              */
/*                                                                      */
/*----------------------------------------------------------------------*/

/*-------------------------------Daemon版本-------------------------------*/
#define DAEMON_VER_PARAM_NUM            2
#define DAEMON_VER_MAJOR                MAJOR_VER
#define DAEMON_VER_MINOR                MINOR_VER

/*-------------------------------可配置参数--------------------------------*/

#define DAEMON_CTRL_PARAM_NUM           3

/*配置边界*/
#define MIN_DAEMON_AUTO_REBOOT_PERIOD   (3600)
#define MIN_SYSTEM_BOOT_CONFIRM_TIME    (0)
#define MIN_SYSTEM_EXIT_WAIT_TIME       (5)

#define MAX_DAEMON_AUTO_REBOOT_PERIOD   (0x7FFFFFFF)
#define MAX_SYSTEM_BOOT_CONFIRM_TIME    (300)
#define MAX_SYSTEM_EXIT_WAIT_TIME       (300)

/*默认配置*/

/*sec daemon自从重启系统周期*/
#define DAEMON_AUTO_REBOOT_PERIOD       (6*3600)
/*sec 联系正常喂狗一定时间后认为程序启动成功*/
#define SYSTEM_BOOT_CONFIRM_TIME        (30)
/*sec 发送主模块退出信号后至强制终止主模块间的等待时间*/
#define SYSTEM_EXIT_WAIT_TIME           (10)

/*--------------------------------固定参数-------------------------------*/

/*将重启时间偏离指定时间范围,防止对采集造成影响*/
#define DAEMON_AUTO_REBOOT_TIME_BASE    (5*60)  /*启动窗口周期*/
#define DAEMON_AUTO_REBOOT_TIME_OFFSET  (3*60)  /*启动窗口起始举例周期起始偏移*/
#define DAEMON_AUTO_REBOOT_TIME_WINDOW  (30)    /*启动窗口宽度*/

/*系统重载/恢复/失败处理*/
#define SYSTEM_MAX_RECOVER_CNT          1       /*最大固件恢复次数*/
#define SYSTEM_MAX_RELOAD_CNT           3       /*最大固件加载尝试次数*/
#define SYSTEN_FAIL_REBOOT_WAIT_TIME    (600)   /*系统失效重启前等待时间*/

/*----------------------------------------------------------------------*/
/*                                                                      */
/*                                DAEMON控制                             */
/*                                                                      */
/*----------------------------------------------------------------------*/

/*守护进程控制标志*/
#define DAEMON_CTRL_IDLE                0
#define DAEMON_CTRL_REBOOT              1
#define DAEMON_CTRL_EXIT                2

/*----------------------------------------------------------------------*/
/*                                                                      */
/*                                主模块控制                              */
/*                                                                      */
/*----------------------------------------------------------------------*/

/*主模块状态标志*/
#define SYSTEM_STATE_STOP               0
#define SYSTEM_STATE_LOAD               1
#define SYSTEM_STATE_RUN                2
#define SYSTEM_SATAE_LOAD_FAIL          3

/*主模块控制标志*/
#define SYSTEM_CTRL_IDLE                0
#define SYSTEM_CTRL_RELOAD              1
#define SYSTEM_CTRL_RECOVER             2

/*OTA标志*/
#define SYSTEM_OTA_IDLE                 0
#define SYSTEM_OTA_UPDATE               1
#define SYSTEM_OTA_CONFIRM              2

/*----------------------------------------------------------------------*/
/*                                                                      */
/*                                  制块控                               */
/*                                                                      */
/*----------------------------------------------------------------------*/

typedef struct{

    /*config*/
    unsigned int daemon_auto_reboot_period; /*自动重启时间周期*/
    unsigned int system_boot_confirm_time;  /*正常喂狗指定时间后判定系统运行正常*/
    unsigned int system_exit_wait_time;     /*发送主模块退出信号后至强制杀死主模块间的等待时间*/

    /*daemon ctrl*/
    unsigned int daemon_load_time;          /*守护模块加载时间*/
    unsigned int daemon_ctrl;               /*守护模块重启/退出控制标志*/

    /*system ctrl and state*/
    unsigned int system_state;              /*程序是否已经加载*/
    unsigned int system_ctrl;               /*程序重启请求*/
    unsigned int system_OTA_state;          /*程序重启请求*/

    /*cnt and time stamp*/
    unsigned int system_recover_cnt;        /*系统恢复计数*/
    unsigned int system_reload_cnt;         /*程序加载尝试计数*/
    unsigned int system_recover_time;       /*系统恢复时间*/
    unsigned int system_load_time;          /*程序加载时间(not used)*/
    unsigned int system_boot_time;          /*程序启动时间(完成第一次喂狗)*/

    /*system pid*/
    pid_t        system_pid;                /*主系统进程号*/

}DAEMON_CTRL;

extern DAEMON_CTRL __daemon_ctrl;

/*----------------------------------------------------------------------*/
/*                                                                      */
/*                              daemon 基础操作                          */
/*                                                                      */
/*----------------------------------------------------------------------*/
unsigned int ini_daemon_ctrl();
unsigned int daemon_load_system();

unsigned int daemon_exit();                     /*程序出口,不会返回*/
unsigned int daemon_reboot();                   /*程序出口,不会返回*/
unsigned int daemon_check_auto_reboot();
unsigned int daemon_load_system();

unsigned int set_daemon_ctrl(unsigned int param_cnt,unsigned int * param);
unsigned int get_daemon_ctrl(unsigned int param_buffer_len,unsigned int * param);
unsigned int get_daemon_version(unsigned int param_buffer_len, unsigned int * param);


unsigned int dump_daemon_ctrl_param(DAEMON_CTRL * daemon_ctrl);
unsigned int dump_daemon_ctrl(DAEMON_CTRL * daemon_ctrl);

/*----------------------------------------------------------------------*/
/*                                                                      */
/*                              daemon 处理                              */
/*                                                                      */
/*----------------------------------------------------------------------*/

/*daemon ctrl*/
unsigned int daemon_ctrl_process();

/*daemon OTA*/
unsigned int daemon_OTA_process();

/*daemon message*/
unsigned int daemon_heartbeat_process();
unsigned int daemon_ctrl_msg_process();

/*system and ctrl*/
unsigned int daemon_system_state_process();
unsigned int daemon_system_ctrl_process();

/*log*/
unsigned int daemon_log_process();

#endif
