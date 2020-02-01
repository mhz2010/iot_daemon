/*****************************************************************************/
/**
*
* @file env.h
*           采集器守护线程环境设置
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

#ifndef __DAEMON_ENV_H__
#define __DAEMON_ENV_H__

#include <errno.h>
#include <unistd.h> /*for usleep*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <ctype.h>
#include <time.h>

#include <pthread.h>
#include <dirent.h>
#include <signal.h>
#include <inttypes.h>

#include <sys/msg.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>

/*成功失败标志*/
#define IOT_SUCCEESE            0
#define IOT_SOFT_FAIL           0xFFFFFFEE
#define IOT_FAILED              0xFFFFFFFF

/*版本号*/
#define MAJOR_VER               1
#define MINOR_VER               0

#define IOT_DAEMON_VER          "Iot_Daemon V1.0"

//#define LOG_FILE

#endif

