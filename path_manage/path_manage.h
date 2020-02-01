/*****************************************************************************/
/**
*
* @file path_manage.h
*           采集器守护线程路径管理
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

#ifndef __PATH_MANAGE_H__
#define __PATH_MANAGE_H__

#include "../debug.h"
#include "../env.h"

/*----------------------------------------------------------------------*/
/*                                                                      */
/*                                目录结构                                */
/*                                                                      */
/*----------------------------------------------------------------------*/

/*DAEMON_ROOT目录内容*/
#define DAEMON_PATH                     "daemon"    /*守护进程序与启动脚本*/
#define FIRMWARE_PATH                   "firmware"  /*固件目录*/
#define LOG_PATH                        "log"       /*日志目录*/
#define TOOLS_PATH                      "tools"     /*工具程序目录*/

/*CLCT_ROOT目录内容*/
#define MAIN_MODULE_FIRMWARE_PATH       "firmware"  /*固件目录*/
/*主程序firmware子目录*/
#define OTA_PATH                        "OTA"       /*OTA接收程序目录*/

/*----------------------------------------------------------------------*/
/*                                                                      */
/*                                文件路径                                */
/*                                                                      */
/*----------------------------------------------------------------------*/

/*守护程序路径*/
#define fmt_daemon_boot_file "%s/%s/daemon_sys" /*DAEMON_ROOT DAEMON_PATH */

/*守护程序中主模块路径*/
#define fmt_active_boot_file "%s/%s/active_sys" /*DAEMON_ROOT FIRMWARE_PATH*/

/*守护程序中主模块备份路径*/
#define fmt_backup_boot_file "%s/%s/backup_sys" /*DAEMON_ROOT FIRMWARE_PATH*/

/*守护程序中主模块临时备份路径-用于OTA过程*/
#define fmt_temp_boot_file  "%s/%s/temp_sys"    /*DAEMON_ROOT FIRMWARE_PATH*/

/*守护程序日志目录*/
#define fmt_daemon_log_dir  "%s/%s"             /*DAEMON_ROOT LOG_PATH*/

/*主模块OTA路径*/
#define fmt_ota_file "%s/%s/%s/ota_sys"         /*CLCT_ROOT FIRMWARE_PATH OTA_PATH*/

/*----------------------------------------------------------------------*/
/*                                                                      */
/*                                文件代码                              */
/*                                                                      */
/*----------------------------------------------------------------------*/

#define NUM_DAEMON_BOOT_FILE                0
#define NUM_ACTIVE_BOOT_FILE                1
#define NUM_BACKUP_BOOT_FILE                2
#define NUM_TEMP_BOOT_FILE                  3
#define NUM_OTA_FILE                        4
#define NUM_DAEMON_LOG_DIR                  5

/*----------------------------------------------------------------------*/
/*                                                                      */
/*                                路径接口                              */
/*                                                                      */
/*----------------------------------------------------------------------*/

char * get_path(unsigned int type,unsigned int time);   /*计算目标文件路径*/
char * get_dir_name(const char * path);                 /*根据路径截取所在目录*/
char * get_file_name(const char * path);                /*根据路径截取文件名*/
char * gen_log_file_name();

unsigned int is_file_exist(const char * path) ;         /*确定文件是否存在*/
unsigned int check_file_dir(const char * file_path);    /*判定文件夹是否存在,不存在则建立*/

unsigned int get_file_size(const char * path);          /*获取文件大小*/

char * get_text_file (const char * path);               /*加载文件*/
char * get_bin_file(const char * path);

unsigned int put_text_file(const char * path, \
                           const char * file_text);     /*写入文件*/
unsigned int put_bin_file(const char * path, \
                          const char * bin_text, unsigned int len);
unsigned int get_time_sec();

/*----------------------------------------------------------------------*/
/*                                                                      */
/*                                bash指令                               */
/*                                                                      */
/*----------------------------------------------------------------------*/

unsigned int exec_shell_cmd_cp_rename(char * d_file, char * s_file);
unsigned int exec_shell_chmod_run(char * s_file);/*添加可执行权限*/
unsigned int exec_shell_mkdir_p(char * dir_path);
unsigned int exec_shell_umount(char * fs_path);

#endif
