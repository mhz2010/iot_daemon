/*****************************************************************************/
/**
*
* @file firmware.h
*           固件更新
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

#ifndef __FIRMWARE_H__
#define __FIRMWARE_H__

#include "../env.h"
#include "../debug.h"

void load_main_module();
void kill_main_module();

#endif
