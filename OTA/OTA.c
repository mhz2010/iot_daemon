
#include "OTA.h"
#include "../path_manage/path_manage.h"

/*********************************************************************/
/*
*           OTA_firmware_switch
*              固件切换
*
* @param
*
* @return   unsigned int
*               成功:     IOT_SUCCEESE
*               失败:     IOT_FAILED
*
* @note
*
**********************************************************************/
unsigned int OTA_firmware_switch(unsigned int switch_mode){

    /*获得路径*/
    char * src_path = NULL;
    char * dst_path = NULL;

    switch (switch_mode) {

        case OTA_MODE_BACKUP_ACTIVE :   /*active -> temp*/
            src_path = get_path (NUM_ACTIVE_BOOT_FILE, 0);
            dst_path = get_path (NUM_TEMP_BOOT_FILE, 0);
            break;
        case OTA_MODE_UPDATE_BACKUP:    /*temp -> backup*/
            src_path = get_path (NUM_TEMP_BOOT_FILE, 0);
            dst_path = get_path (NUM_BACKUP_BOOT_FILE, 0);
            break;
        case OTA_MODE_ROLLBACK_ACTIVE:  /*temp -> active*/
            src_path = get_path (NUM_TEMP_BOOT_FILE, 0);
            dst_path = get_path (NUM_ACTIVE_BOOT_FILE, 0);
            break;
        case OTA_MODE_OTA:              /*OTA -> active*/
            src_path = get_path (NUM_OTA_FILE, 0);
            dst_path = get_path (NUM_ACTIVE_BOOT_FILE, 0);
            break;
        case OTA_MODE_RECOVER_ACTIVE:   /*backup -> active*/
            src_path = get_path (NUM_BACKUP_BOOT_FILE, 0);
            dst_path = get_path (NUM_ACTIVE_BOOT_FILE, 0);
            break;
        default :
            src_path = NULL;
            dst_path = NULL;
    }

    if((src_path == NULL) || (dst_path == NULL)){
        IOT_ERR("%s : NULL src or dst path.\n",__FUNCTION__);
        free(dst_path);
        free(src_path);
        return IOT_FAILED;
    }
    else{
    }

    /*判定源文件/目标路径是否存在*/
    if(IOT_FAILED == is_file_exist(src_path)){
        IOT_ERR("%s : src not existed!\n",__FUNCTION__);
        free(dst_path);
        free(src_path);
        return IOT_FAILED;
    }
    else{
    }

    if(IOT_FAILED == check_file_dir(dst_path)){
        IOT_ERR("%s : src not existed!\n",__FUNCTION__);
        free(dst_path);
        free(src_path);
        return IOT_FAILED;
    }
    else{
    }

    /*开始切换文件*/
    unlink(dst_path);                                   /*删除目标*/
    exec_shell_cmd_cp_rename(dst_path, src_path);       /*替换*/

    /*如果为OTA模式,则添加执行权限*/
    if(switch_mode == OTA_MODE_OTA){
        exec_shell_chmod_run(dst_path);                 /*增加写权限*/
    }
    else{
    }

    free(dst_path);
    free(src_path);

    return IOT_SUCCEESE;
}

unsigned int OTA_delete_firmware(unsigned int type){

    /*获得路径*/
    char * src_path = NULL;

    switch (type) {

        case OTA_FIRMWARE_TYPE_ACTIVE :     /*active*/
            src_path = get_path (NUM_ACTIVE_BOOT_FILE, 0);
            break;
        case OTA_FIRMWARE_TYPE_BACKUP:      /*backup*/
            src_path = get_path (NUM_BACKUP_BOOT_FILE, 0);
            break;
        case OTA_FIRMWARE_TYPE_TEMP:        /*temp*/
            src_path = get_path (NUM_TEMP_BOOT_FILE, 0);
            break;
        case OTA_FIRMWARE_TYPE_OTA:         /*OTA*/
            src_path = get_path (NUM_OTA_FILE, 0);
            break;
        default :
            src_path = NULL;
    }

    if(src_path == NULL){
        IOT_ERR("%s : Illegal firmware type.\n", __FUNCTION__);
        return IOT_FAILED;
    }
    else{
        IOT_MSG("%s : unlink %s\n", __FUNCTION__,src_path);
        unlink(src_path);
        free(src_path);
        return IOT_SUCCEESE;
    }

}




/*********************************************************************/
/*
*           OTA_update_firmware
*              将OTA程序替换主程序
*
* @param
*           unsigned int OTA_flag   OTA标志——当前是否有OTA版本
*           unsigned int OTA_enable OTA使能标志
*
* @return   unsigned int
*               成功:     IOT_SUCCEESE
*               软失败:    IOT_SOFT_FAIL
*               失败:     IOT_FAILED
*
* @note
*
**********************************************************************/
unsigned int OTA_update_firmware() {

    /*备份固件*/
    if( IOT_FAILED == OTA_firmware_switch(OTA_MODE_BACKUP_ACTIVE) ) {
        IOT_ERR("%s : OTA backup active firmware (active -> temp) failed\n", \
                __FUNCTION__);
        OTA_delete_firmware(OTA_FIRMWARE_TYPE_TEMP); /*确保删除temp固件*/
        return IOT_FAILED;
    }
    else{
        IOT_MSG("%s : OTA backup active firmware (active -> temp) OK\n", \
                __FUNCTION__);
    }

    /*更新固件*/
    if( IOT_FAILED == OTA_firmware_switch(OTA_MODE_OTA) ) {
        IOT_ERR("%s : OTA update active firmware (OTA -> active) failed\n", \
                __FUNCTION__);
        OTA_firmware_switch(OTA_MODE_ROLLBACK_ACTIVE);
        OTA_delete_firmware(OTA_FIRMWARE_TYPE_TEMP);
        OTA_delete_firmware(OTA_FIRMWARE_TYPE_OTA);
        return IOT_FAILED;
    }
    else{
        IOT_MSG("%s : OTA update firmware (active -> temp / OTA -> active) OK\n", \
                __FUNCTION__);
        OTA_delete_firmware(OTA_FIRMWARE_TYPE_OTA);
    }

    return IOT_SUCCEESE;
}

unsigned int OTA_confirm_firmware(){

    /*更新备份固件*/
    if( IOT_FAILED == OTA_firmware_switch(OTA_MODE_UPDATE_BACKUP) ) {
        IOT_ERR("%s : OTA update backup firmware (temp -> backup) failed\n", \
                __FUNCTION__);
        OTA_delete_firmware(OTA_FIRMWARE_TYPE_TEMP); /*确保删除temp固件*/
        return IOT_FAILED;
    }
    else{
        IOT_MSG("%s : OTA update backup firmware (temp -> backup) OK\n", \
                __FUNCTION__);
        OTA_delete_firmware(OTA_FIRMWARE_TYPE_TEMP); /*确保删除temp固件*/
    }

    return IOT_SUCCEESE;

}

/*********************************************************************/
/*
*           OTA_rollback_firmware
*
*
* @param
*
* @return   unsigned int
*               成功:     IOT_SUCCEESE
*               软失败:    IOT_SOFT_FAIL
*               失败:     IOT_FAILED
*
* @note
*
**********************************************************************/
unsigned int OTA_rollback_firmware() {

    /*更新备份固件*/
    if( IOT_FAILED == OTA_firmware_switch(OTA_MODE_ROLLBACK_ACTIVE) ) {
        IOT_ERR("%s : OTA roll backup firmware (temp -> active) failed\n", \
                __FUNCTION__);
        OTA_delete_firmware(OTA_FIRMWARE_TYPE_TEMP); /*确保删除temp固件*/
        return IOT_FAILED;
    }
    else{
        IOT_MSG("%s : OTA roll backup firmware (temp -> active) OK\n", \
                __FUNCTION__);
        OTA_delete_firmware(OTA_FIRMWARE_TYPE_TEMP); /*确保删除temp固件*/
    }

    return IOT_SUCCEESE;
}

unsigned int OTA_recover_firmware(){

    /*更新备份固件*/
    if( IOT_FAILED == OTA_firmware_switch(OTA_MODE_RECOVER_ACTIVE) ) {
        IOT_ERR("%s : OTA recover firmware (backup -> active) failed\n", \
                __FUNCTION__);
        return IOT_FAILED;
    }
    else{
        IOT_MSG("%s : OTA recover firmware (backup -> active) OK\n", \
                __FUNCTION__);
    }

    return IOT_SUCCEESE;

}
