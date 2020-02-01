/*****************************************************************************/
/**
*
* @file heatbeat.h
*           心跳处理逻辑
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who                  Date      Changes
* ----- -------------------- ---------- ---------------------------------------
* 1.0   Ma Haozhi            2020/02/02 first releass
*       mhz2010@126.com
*       Li Xiaojie
*       linsa049962@163.com
*
* </pre>
*
******************************************************************************/

#ifndef __HEARTBEAT_H__
#define __HEARTBEAT_H__

#include "../env.h"
#include "../debug.h"

/*----------------------------------------------------------------------*/
/*                                                                      */
/*                              心跳控制数据                              */
/*                                                                      */
/*----------------------------------------------------------------------*/
#define HEARTBEAT_CTRL_PARAM_NUM        4       /*心跳处理配置参数数量*/

#define WDI_KEEP_TIME                   20
#define WDI_REBOOT_KEEP_TIME            5
#define HEARTBEAT_LOST_LIMIT            5
#define LOAD_EXEMPT_TIME                35

/*设置范围*/
#define MIN_WDI_KEEP_TIME               20
#define MIN_WDI_REBOOT_KEEP_TIME        5
#define MIN_HEARTBEAT_LOST_LIMIT        5
#define MIN_LOAD_EXEMPT_TIME            10

#define MAX_WDI_KEEP_TIME               3600
#define MAX_WDI_REBOOT_KEEP_TIME        120
#define MAX_HEARTBEAT_LOST_LIMIT        3600
#define MAX_LOAD_EXEMPT_TIME            120

typedef struct{
    unsigned int wdi_keep_time;                 /*看门狗最大喂狗间隔*/
    unsigned int wdi_reboot_keep_time;          /*重启过程中看门狗喂狗间隔*/
    unsigned int heartbeat_lost_limit;          /*主模块最大无心跳间隔,超过此值将重启主模块*/
    unsigned int load_exempt_time;              /*系统启动后喂狗豁免时间*/

    unsigned int system_load_time;              /*主模块加载时间*/
    unsigned int heartbeat_time;                /*主模块喂狗时间*/
}DAEMON_HEARTBEAT_CTRL;

/*----------------------------------------------------------------------*/
/*                                                                      */
/*                              心跳控制接口                              */
/*                                                                      */
/*----------------------------------------------------------------------*/

void ini_heartbeat_ctrl();
void set_system_load_time();
void set_heartbeat_time();
unsigned int set_heartbeat_ctrl(unsigned int param_num,unsigned int * param);
unsigned int get_heartbeat_ctrl(unsigned int param_buffer_len,unsigned int * param);
unsigned int check_heartbeat();
unsigned int get_reboot_keep_time();

#endif
