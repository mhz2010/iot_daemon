/*****************************************************************************/
/**
*
* @file OTA.h
*       固件更新
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

#ifndef __OTA_H__
#define __OTA_H__

#include "../env.h"
#include "../debug.h"

/*----------------------------------------------------------------------*/
/*                                                                      */
/*                          OTA 模式  (固件切换)                          */
/*                                                                      */
/*----------------------------------------------------------------------*/
#define OTA_MODE_BACKUP_ACTIVE      0       /*active -> temp*/
#define OTA_MODE_UPDATE_BACKUP      1       /*temp -> backup*/
#define OTA_MODE_ROLLBACK_ACTIVE    2       /*temp -> active*/
#define OTA_MODE_OTA                3       /*OTA -> active*/
#define OTA_MODE_RECOVER_ACTIVE     4       /*backup -> active*/

unsigned int OTA_firmware_switch(unsigned int switch_mode);

#define OTA_FIRMWARE_TYPE_ACTIVE    0
#define OTA_FIRMWARE_TYPE_BACKUP    1
#define OTA_FIRMWARE_TYPE_TEMP      2
#define OTA_FIRMWARE_TYPE_OTA       3

unsigned int OTA_delete_firmware(unsigned int type);

/*----------------------------------------------------------------------*/
/*                                                                      */
/*                              OTA 流程控制                              */
/*                                                                      */
/*----------------------------------------------------------------------*/
unsigned int OTA_update_firmware();
unsigned int OTA_confirm_firmware();
unsigned int OTA_rollback_firmware();
unsigned int OTA_recover_firmware();

#endif
