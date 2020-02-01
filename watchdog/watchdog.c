
#include <linux/watchdog.h>
#include "watchdog.h"

int __watchdog_fd = -1;

/*********************************************************************/
/*
*           ini_watchdog
*               使能看门狗
*
* @param
*           unsigned int time   喂狗超时时间
*
* @return   unsigned int
*               成功: IOT_SUCCEESE
*               失败: IOT_FAILED
*
* @note
*
**********************************************************************/
unsigned int ini_watchdog(unsigned int time) {

#ifndef WDT_ENABLE
        return IOT_SUCCEESE;
#endif

    /*open watchdog file*/
    __watchdog_fd = open("/dev/watchdog",O_RDWR);
    if(__watchdog_fd < 0){
        IOT_WARN("%s : open /dev/watchdog FILED.\n",__FUNCTION__);
        return IOT_FAILED;
    }
    else{
        IOT_INFO("%s : open /dev/watchdog OK.\n",__FUNCTION__);
    }

    /*set time*/
    if(ioctl(__watchdog_fd,WDIOC_SETTIMEOUT,&time) < 0){
        IOT_WARN("%s : watchdog set timeout failed.\n",__FUNCTION__);
        return IOT_FAILED;
    }
    else{
        IOT_INFO("%s : watchdog set timeout %d.\n",__FUNCTION__, time);
    }

    return IOT_SUCCEESE;
}

/*********************************************************************/
/*
*           release_watchdog
*               关闭看门狗
*
* @param
*
* @return   unsigned int
*               成功: IOT_SUCCEESE
*               失败: IOT_FAILED
*
* @note
*
**********************************************************************/
unsigned int release_watchdog() {

#ifndef WDT_ENABLE
        return IOT_SUCCEESE;
#endif

    unsigned int ret = IOT_SUCCEESE;

    if(__watchdog_fd < 0) {
        IOT_WARN("%s : watchdog not opened.\n",__FUNCTION__);
        __watchdog_fd = -1;
        return IOT_SUCCEESE;
    }
    else{
    }

    /*disable watchdog*/
    if(1 !=  write(__watchdog_fd,"V",1)) { /*Magic Close feature*/
        IOT_WARN("%s : watchdog disable failed.\n",__FUNCTION__);
        ret = IOT_FAILED;
    }
    else{
        IOT_INFO("%s : watchdog disable OK.\n",__FUNCTION__);
    }

    /*close watchdog*/
    if( 0 > close(__watchdog_fd)) {
        IOT_WARN("%s : watchdog close failed.\n",__FUNCTION__);
        ret = IOT_FAILED;
    }
    else{
        IOT_INFO("%s : watchdog close OK.\n",__FUNCTION__);
    }

    __watchdog_fd = -1;

    return ret;
}

/*********************************************************************/
/*
*           set_keeptime_watchdog
*               设置喂狗超时时间
*
* @param
*           unsigned int time   喂狗超时时间
*
* @return   unsigned int
*               成功: IOT_SUCCEESE
*               失败: IOT_FAILED
*
* @note
*
**********************************************************************/
unsigned int set_keeptime_watchdog(unsigned int time) {

#ifndef WDT_ENABLE
    return IOT_SUCCEESE;
#endif

    /*set time*/
    if(ioctl(__watchdog_fd,WDIOC_SETTIMEOUT,&time) < 0){
        IOT_WARN("%s : watchdog set timeout failed.\n",__FUNCTION__);
        return IOT_FAILED;
    }
    else{
        IOT_INFO("%s : watchdog set timeout %d.\n",__FUNCTION__, time);
        return IOT_SUCCEESE;
    }

}

/*********************************************************************/
/*
*           feed_watchdog
*               喂狗
*
* @param
*
*
* @return   unsigned int
*               成功: IOT_SUCCEESE
*               失败: IOT_FAILED
*
* @note
*
**********************************************************************/
unsigned int feed_watchdog(){

#ifndef WDT_ENABLE
        return IOT_SUCCEESE;
#endif

    static unsigned int cnt = 0; /*防止喂狗错误输出过于频繁*/

    if(__watchdog_fd < 0) {
        if( (cnt%100) == 0) {
            IOT_WARN("%s : watchdog not opened.\n",__FUNCTION__);
        }
        else{
            cnt ++;
        }
        return IOT_FAILED;
    }
    else{
    }

    if(ioctl(__watchdog_fd,WDIOC_KEEPALIVE) < 0){
        if( (cnt%300) == 0 ) {
            IOT_WARN("%s : feed watchdog failed.\n",__FUNCTION__);
        }
        else{
            cnt ++;
        }
        return IOT_FAILED;
    }
    else{
        return IOT_SUCCEESE;
    }

}




