/*****************************************************************************/
/**
*
* @file log_File.h
*       日志处理
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

#ifndef __LOG_MANAGE_H__
#define __LOG_MANAGE_H__

#include "../env.h"
#include "../debug.h"

/*----------------------------------------------------------------------*/
/*                                                                      */
/*                             日志操作控制结构                            */
/*                                                                      */
/*----------------------------------------------------------------------*/
#define LOG_FILE_PARAM_NUM          (3)

#define MIN_LOG_FILE_LIFESPAN       (1)
#define MIN_LOG_FILE_SWITCH_PERIOD  (1)
#define MIN_LOG_FILE_CLEAR_PERIOD   (1)

#define MAX_LOG_FILE_LIFESPAN       (10000)
#define MAX_LOG_FILE_SWITCH_PERIOD  (10000)
#define MAX_LOG_FILE_CLEAR_PERIOD   (10000)

#define LOG_FILE_LIFESPAN           (3)     /*day*/
#define LOG_FILE_SWITCH_PERIOD      (1)     /*day*/
#define LOG_FILE_CLEAR_PERIOD       (1)     /*day*/

typedef struct{
    unsigned int log_file_lifespan;         /*最大日志文件存在周期    day*/
    unsigned int log_file_switch_period;    /*日志文件切换时间间隔    day*/
    unsigned int log_file_clear_period;     /*日志文件检查时间间隔    day*/

    unsigned int last_clear_time;           /*上次日志文件检查时间    day*/
    unsigned int last_switch_time;          /*上次日志文件切换时间    day*/
}DAEMON_LOG_CTRL;

/*----------------------------------------------------------------------*/
/*                                                                      */
/*                              日志操作函数                              */
/*                                                                      */
/*----------------------------------------------------------------------*/
unsigned int ini_daemon_log_ctrl();
unsigned int set_daemon_log_ctrl(unsigned int param_cnt,unsigned int * param);
unsigned int get_daemon_log_ctrl(unsigned int param_buffer_len,unsigned int * param);

unsigned int check_daemon_log_name(char * log_name);
unsigned int clear_daemon_log_file();
unsigned int switch_daemon_log_file();
unsigned int close_daemon_log_file();
unsigned int check_daemon_switch_time();
unsigned int check_daemon_clear_time();

unsigned int dump_daemon_log_ctrl(DAEMON_LOG_CTRL * ctrl);

#endif
