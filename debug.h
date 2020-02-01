/*****************************************************************************/
/**
*
* @file debug.h
*   采集器守护线程调试接口
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

#ifndef __DAEMON_DEBUG_H__
#define __DAEMON_DEBUG_H__

/*----------------------------------------------------------------------*/
/*                                                                      */
/*                              iot debug                               */
/*                                                                      */
/*----------------------------------------------------------------------*/

#define IOT_DAEMON_DEBUG
#define IOT_DAEMON_DEBUG_LEVEL 3

#ifdef IOT_DAEMON_DEBUG
    #define iot_debug_cond(cond, fmt, args...)      \
        do {                    \
        if (cond){          \
            printf(fmt, ##args); \
            fflush(stdout); \
            fflush(stderr);} \
    } while (0)

#else
    #define iot_debug_cond(cond, fmt, args...)      \
        do {                    \
    } while (0)
#endif

#define IOT_INFO(fmt,args...)  iot_debug_cond((IOT_DAEMON_DEBUG_LEVEL>4), "IoT DAEMON INFO: " fmt,  ##args)
#define IOT_DBG(fmt,args...)  iot_debug_cond((IOT_DAEMON_DEBUG_LEVEL>3), "IoT DAEMON DBG : " fmt,  ##args)
#define IOT_MSG(fmt,args...)  iot_debug_cond((IOT_DAEMON_DEBUG_LEVEL>2), "IoT DAEMON MSG : " fmt,  ##args)
#define IOT_WARN(fmt,args...)  iot_debug_cond((IOT_DAEMON_DEBUG_LEVEL>1), "IoT DAEMON WARN: " fmt,  ##args)
#define IOT_ERR(fmt,args...)  iot_debug_cond((IOT_DAEMON_DEBUG_LEVEL>0), "IoT DAEMON ERR : " fmt,  ##args)

/*数据表格打印宽度*/
#define TABLE_DUMP_WIDTH    5

#endif
