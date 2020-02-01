/*****************************************************************************/
/**
*
* @file mq_manage.h
*       管理守护模块消息队列
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

#ifndef __MQ_MANAGE_H__
#define __MQ_MANAGE_H__

#include "../env.h"
#include "../debug.h"

/*----------------------------------------------------------------------*/
/*                                                                      */
/*                              MQ number                               */
/*                                                                      */
/*----------------------------------------------------------------------*/

/*IOT_MQ_BASE_ID is set by makefile*/

#define IOT_MQ_NUM      (10)                    /*由IOT_BASE_MQ开始的若干消息队列由系统占用*/

#define DAEMON_HEARTBEAT_MQ (IOT_MQ_BASE_ID+1)  /*守护模块-主模块间消息通道*/
#define DAEMON_CTRL_MQ      (IOT_MQ_BASE_ID+2)  /*守护模块-控制通道*/

/*----------------------------------------------------------------------*/
/*                                                                      */
/*                              MQ management                           */
/*                                                                      */
/*----------------------------------------------------------------------*/

unsigned int ini_mq();                      /*释放系统所有队列,初始化*/
unsigned int clear_mq(unsigned int mq_key); /*释放指定队列*/

unsigned int create_daemon_mq ();           /*创建daemon消息队列*/
unsigned int release_daemon_mq();           /*释放daemon消息队列*/

/*----------------------------------------------------------------------*/
/*                                                                      */
/*                              MSG_HEARTBEAT                           */
/*                                                                      */
/*----------------------------------------------------------------------*/

#define MSG_HEARTBEAT_TYPE_REQ      1 /*system -> daemon*/
#define MSG_HEARTBEAT_TYPE_RSPD     2 /*daemon -> system*/

/*运行控制 for IOT_ctrl*/
#define IOT_CTRL_EXIT       0
#define IOT_CTRL_RUN        1
#define IOT_CTRL_PAUSE      2

/*运行状态 for IOT_state*/
#define IOT_STATE_EXIT      0
#define IOT_STATE_RUN       1
#define IOT_STATE_PAUSE     2

/*IOT状态 for IOT_OTA_FLAG*/
#define IOT_OTA_IDLE        0
#define IOT_OTA_REQ         1

typedef struct {
    long int     msg_type;      /*消息类型*/
    int          IOT_pid;       /*主模块反馈的PID号*/
    unsigned int IOT_state;     /*主模块状态*/
    unsigned int IOT_ctrl;      /*守护模块-主模块指令*/
    unsigned int IOT_OTA_flag;  /*主模块OTA状态*/
}MSG_HEARTBEAT;

MSG_HEARTBEAT * create_heartbeat_msg();
unsigned int destroy_heartbeat_msg(MSG_HEARTBEAT ** msg);
unsigned int dump_heartbeat_msg(MSG_HEARTBEAT * msg);

MSG_HEARTBEAT * get_heartbeat_msg(unsigned int type);
unsigned int put_heartbeat_msg(unsigned int type, MSG_HEARTBEAT * msg);
unsigned int heartbeat_msg_ctrl(unsigned int iot_ctrl);

/*----------------------------------------------------------------------*/
/*                                                                      */
/*                              MSG_DAEMON_CTRL                         */
/*                                                                      */
/*----------------------------------------------------------------------*/

/*控制类型*/
#define IOT_DAEMON_CTRL_NULL        0
#define IOT_DAEMON_CTRL_EXIT        1 /*daemon退出(关闭WDI)*/
#define IOT_DAEMON_CTRL_REBOOT      2 /*daemon重启(等待狗叫复位)*/
#define IOT_DAEMON_CTRL_EXIT_MAIN   3 /*尚未启用*/
#define IOT_DAEMON_CTRL_REBOOT_MAIN 4 /*尚未启用*/
#define IOT_DAEMON_CTRL_SET_PARAM   5 /*进行参数配置*/
#define IOT_DAEMON_CTRL_GET_PARAM   6 /*获取参数配置*/

/*消息类型*/
#define MSG_DAEMON_CTRL_TYPE_REQ    1 /*x -> daemon*/
#define MSG_DAEMON_CTRL_TYPE_RSPD   2 /*daemon -> x*/

/*参数配置类型*/
#define IOT_DAEMON_PARAM_NULL       0
#define IOT_DAEMON_PARAM_VER        1
#define IOT_DAEMON_PARAM_DAEMON     2
#define IOT_DAEMON_PARAM_LOG        3
#define IOT_DAEMON_PARAM_HEARTBEAT  4

/*最大参数数量*/
#define IOT_DAEMON_PARAM_NUM        12

/*64B*/
typedef struct {
    long int     msg_type;
    unsigned int iot_daemon_ctrl;
    unsigned int iot_daemon_param_type;
    unsigned int iot_daemon_param_num;
    unsigned int iot_daemon_param[IOT_DAEMON_PARAM_NUM];
}MSG_DAEMON_CTRL;

MSG_DAEMON_CTRL * create_daemon_ctrl_msg();
unsigned int destroy_daemon_ctrl_msg(MSG_DAEMON_CTRL ** msg);
MSG_DAEMON_CTRL * get_daemon_ctrl_msg(unsigned int type);
unsigned int put_daemon_ctrl_msg(unsigned int type, MSG_DAEMON_CTRL * msg);
unsigned int dump_daemon_ctrl_msg(MSG_DAEMON_CTRL * msg);

#endif
