/*****************************************************************************/
/**
*
* @file watchdog.h
*           采集器守护线程看门狗操作
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

#ifndef __WATCHDOG_H__
#define __WATCHDOG_H__

#include "../env.h"
#include "../debug.h"

unsigned int ini_watchdog(unsigned int time);
unsigned int release_watchdog();
unsigned int set_keeptime_watchdog(unsigned int time);
unsigned int feed_watchdog();

#endif
