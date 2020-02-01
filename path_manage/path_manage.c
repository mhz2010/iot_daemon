
#include "path_manage.h"

/*----------------------------------------------------------------------*/
/*                                                                      */
/*                               具体文件路径生成                           */
/*                                                                      */
/*----------------------------------------------------------------------*/

char * get_daemon_boot_file();
char * get_active_boot_file();
char * get_backup_boot_file();
char * get_temp_boot_file();
char * get_ota_file();
char * get_daemon_log_dir();

/*********************************************************************/
/*
*           get_XXX_file
*               获取特定文件的文件路径和文件名
*
* @param
*
* @return   char *
*               成功: path
*               失败: NULL
*
* @note     分配内存，所有路径使用完成后需要释放
*
**********************************************************************/
char * get_daemon_boot_file() {

    unsigned int len = strlen(fmt_daemon_boot_file) + strlen(DAEMON_ROOT) \
                       + strlen (DAEMON_PATH) + 1;

    char * path_buffer = (char *)malloc(len);
    if(NULL == path_buffer) {
        IOT_ERR("%s : file path malloc FAILED.\n",__FUNCTION__);
        return NULL;
    }
    else{
    }

    sprintf(path_buffer, fmt_daemon_boot_file, DAEMON_ROOT, DAEMON_PATH);

    return path_buffer;

}

char * get_active_boot_file() {

    unsigned int len = strlen(fmt_active_boot_file) + strlen(DAEMON_ROOT) \
                       + strlen (FIRMWARE_PATH) + 1;

    char * path_buffer = (char *)malloc(len);
    if(NULL == path_buffer) {
        IOT_ERR("%s : file path malloc FAILED.\n",__FUNCTION__);
        return NULL;
    }
    else{
    }

    sprintf(path_buffer, fmt_active_boot_file, DAEMON_ROOT, FIRMWARE_PATH);

    return path_buffer;

}

char * get_backup_boot_file() {

    unsigned int len = strlen(fmt_backup_boot_file) + strlen(DAEMON_ROOT) \
                       + strlen (FIRMWARE_PATH) +1;

    char * path_buffer = (char *)malloc(len);
    if(NULL == path_buffer) {
        IOT_ERR("%s : file path malloc FAILED.\n",__FUNCTION__);
        return NULL;
    }
    else{
    }

    sprintf(path_buffer, fmt_backup_boot_file, DAEMON_ROOT, FIRMWARE_PATH);

    return path_buffer;

}

char * get_temp_boot_file() {

    unsigned int len = strlen(fmt_temp_boot_file) + strlen(DAEMON_ROOT) \
                       + strlen (FIRMWARE_PATH) + 1;

    char * path_buffer = (char *)malloc(len);
    if(NULL == path_buffer) {
        IOT_ERR("%s : file path malloc FAILED.\n",__FUNCTION__);
        return NULL;
    }
    else{
    }

    sprintf(path_buffer, fmt_temp_boot_file, DAEMON_ROOT, FIRMWARE_PATH);

    return path_buffer;

}

char * get_ota_file() {

    unsigned int len = strlen(fmt_ota_file) + strlen(CLCT_ROOT) \
                       + strlen (FIRMWARE_PATH) +strlen (OTA_PATH) +1;

    char * path_buffer = (char *)malloc(len);
    if(NULL == path_buffer) {
        IOT_ERR("%s : file path malloc FAILED.\n",__FUNCTION__);
        return NULL;
    }
    else{
    }

    sprintf(path_buffer, fmt_ota_file, CLCT_ROOT, FIRMWARE_PATH, OTA_PATH);

    return path_buffer;

}

char * get_daemon_log_dir() {

    unsigned int len = strlen(fmt_daemon_log_dir) + strlen(DAEMON_ROOT) \
                       + strlen (LOG_PATH) + 1;

    char * path_buffer = (char *)malloc(len);
    if(NULL == path_buffer) {
        IOT_ERR("%s : file path malloc FAILED.\n",__FUNCTION__);
        return NULL;
    }
    else{
    }

    sprintf(path_buffer, fmt_daemon_log_dir, DAEMON_ROOT, LOG_PATH );

    return path_buffer;

}

/*----------------------------------------------------------------------*/
/*                                                                      */
/*                                路径接口                               */
/*                                                                      */
/*----------------------------------------------------------------------*/

/*********************************************************************/
/*
*           get_path
*           根据文件类型与参数生成目标文件绝对路径名
*
* @param
*
* @return   char *
*               成功: path
*               失败: NULL
*
* @note
*
**********************************************************************/
char * get_path(unsigned int type,unsigned int time) {

    char * str_path = NULL;

    switch (type) {

        case NUM_DAEMON_BOOT_FILE :
            str_path = get_daemon_boot_file();
            break;
        case NUM_ACTIVE_BOOT_FILE :
            str_path = get_active_boot_file();
            break;
        case NUM_BACKUP_BOOT_FILE :
            str_path = get_backup_boot_file();
            break;
        case NUM_TEMP_BOOT_FILE :
            str_path = get_temp_boot_file();
            break;
        case NUM_OTA_FILE :
            str_path = get_ota_file();
            break;
        case NUM_DAEMON_LOG_DIR :
            str_path = get_daemon_log_dir();
            break;

        default :
            IOT_ERR("%s : UNKNOWN path type.\n",__FUNCTION__);
            break;
    }

    return str_path;
}

/*********************************************************************/
/*
*           get_dir_name
*               获取文件所在目录名
*
* @param
*       const char * path
*
* @return
*       char *
*               成功:
*               失败: NULL
*
* @note
*       分配内存，不释放形参
*       通过最后一个/进行检测
*
**********************************************************************/
char * get_dir_name(const char * path){

    if(path == NULL) {
        IOT_ERR("%s : NULL pointer.\n",__FUNCTION__);
        return NULL;
    }
    else{
    }

    char * ptr = strrchr(path,'/');
    if(ptr == NULL) {
        return NULL;
    }
    else{
    }

    unsigned int str_len = strlen(path)-strlen(ptr);
    char * str_dir_path = (char *)malloc(str_len +1);
    if(str_dir_path == NULL) {
        IOT_ERR("%s : malloc failed.\n",__FUNCTION__);
        return NULL;
    }
    else{
        memset(str_dir_path, 0 ,str_len +1);
    }

    strncpy(str_dir_path,path,str_len);

    return str_dir_path;
}

/*********************************************************************/
/*
*           get_file_name
*               获取文件名
*
* @param
*
* @return   unsigned int
*               成功:
*               失败: NULL
*
* @note     分配内存，不释放形参
*           通过最后一个/进行检测
*           todo 可能有问题,需要验证开头是否多了/
*
**********************************************************************/
char * get_file_name(const char * path) {//分配内存

    if(path == NULL) {
        IOT_ERR("%s : NULL pointer.\n",__FUNCTION__);
        return NULL;
    }
    else{
    }

    char * ptr = strrchr(path,'/');
    if(ptr == NULL) {
        return NULL;
    }
    else{
    }

    unsigned int str_len = strlen(ptr);
    char * str_file_name = (char *)malloc(str_len +1);
    if(str_file_name == NULL) {
        IOT_ERR("%s : str_file_name malloc failed.\n",__FUNCTION__);
        return NULL;
    }
    else{
        memset(str_file_name, 0 ,str_len +1);
    }

    strncpy(str_file_name,ptr,str_len);

    return str_file_name;

}

char * gen_log_file_name(){

    char * log_dir_path = get_path(NUM_DAEMON_LOG_DIR,0);   /*计算log dir路径*/
    if(log_dir_path == NULL){
        IOT_ERR("%s : get log dir path failed.\n",__FUNCTION__);
        return NULL;
    }
    else{
        IOT_INFO("%s : log dir path %s\n",__FUNCTION__,log_dir_path);
    }

    /*get time*/
/*
    tm_sec   秒 – 取值区间为[0,59]
    tm_min   分 - 取值区间为[0,59]
    tm_hour  时 - 取值区间为[0,23]
    tm_mday  一个月中的日期 - 取值区间为[1,31]
    tm_mon   月份（从一月开始，0代表一月） - 取值区间为[0,11]
    tm_year  年份，其值等于实际年份减去1900
    tm_wday  星期 – 取值区间为[0,6]，其中0代表星期天，1代表星期一，以此类推
    tm_yday  从每年的1月1日开始的天数 – 取值区间为[0,365]，其中0代表1月1日，1代表1月2日
*/

    time_t now;
    time(&now);
    struct tm * timenow = localtime(&now);   //do not free!

    unsigned int year   = timenow->tm_year + 1900;
    unsigned int month  = timenow->tm_mon + 1;
    unsigned int day    = timenow->tm_mday;
    unsigned int hour   = timenow->tm_hour;
    unsigned int min    = timenow->tm_min;
    unsigned int sec    = timenow->tm_sec;

    char * fmt_daemon_log_file = "%s/iot_daemon_%04d-%02d-%02d_%02d:%02d:%02d.log";
    unsigned int len = strlen(fmt_daemon_log_file) + strlen(log_dir_path) + 60 +1;
    char * path_buffer = (char *)malloc(len);
    if(NULL == path_buffer) {
        IOT_ERR("%s : malloc path_buffer.\n",__FUNCTION__);
        free(log_dir_path);
        return NULL;
    }
    else{
        sprintf(path_buffer, fmt_daemon_log_file, log_dir_path, \
                year, month, day, hour, min, sec);
        IOT_INFO("%s : log file path %s \n",__FUNCTION__,path_buffer);
        free(log_dir_path);
    }

    return path_buffer;
}

/*********************************************************************/
/*
*           is_file_exist
*               判定文件是否存在
*
* @param
*
* @return   unsigned int
*               成功: IOT_SUCCEESE
*               失败: IOT_FAILED
*
* @note     分配内存
*
**********************************************************************/
unsigned int is_file_exist(const char * path) {

    if(0 != access(path,F_OK) ) {
        return IOT_FAILED;
    }
    else{
    }

    return IOT_SUCCEESE;

}

/*********************************************************************/
/*
*           check_file_dir
*               判定文件目录是否存在，如果不存在则创建目录
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
unsigned int check_file_dir(const char * file_path) {

    if(file_path == NULL) {
        IOT_ERR("%s : NULL pointer.\n",__FUNCTION__);
        return IOT_FAILED;
    }
    else{
    }

    char * file_dir_path = get_dir_name(file_path);
    if(file_dir_path == NULL) {
        IOT_ERR("%s : get_dir_name failed, file_path %s.\n",__FUNCTION__,file_path);
        return IOT_FAILED;
    }
    else{
    }

    if ( IOT_FAILED == is_file_exist(file_dir_path) ) {
        if ( IOT_FAILED ==  exec_shell_mkdir_p(file_dir_path)) {
            IOT_ERR("%s : exec_shell_make_p %s failed.\n",__FUNCTION__,file_dir_path);
            free(file_dir_path);
            return IOT_FAILED;
        }
        else{
            free(file_dir_path);
        }
    }
    else{
        free(file_dir_path);
    }

    return IOT_SUCCEESE;
}

/*********************************************************************/
/*
*           get_file_size
*               获得文件长度
*
* @param
*       char *
*
* @return   unsigned int
*               成功: file size
*               失败: IOT_FAILED
*
* @note
*
**********************************************************************/
unsigned int get_file_size(const char * path) {

    struct stat info = { 0 };

    if (!path || !*path){
        return IOT_FAILED;
    }
    else{
    }

    if(0 != stat(path, &info) ) {
        IOT_ERR("%s : get stat failed.\n",__FUNCTION__);
        return IOT_FAILED;
    }
    else{
    }

    return (unsigned int)info.st_size;

}

/*********************************************************************/
/*
*           get_text_file
*               获得文件内容
*
* @param
*       char * path
*
* @return   char *
*               成功:
*               失败: NULL
*
* @note     分配内存,不释放形参
*
**********************************************************************/
char * get_text_file (const char * path) {

    if(path == NULL)  {
        IOT_ERR("%s : NULL file path.\n",__FUNCTION__);
        return NULL;
    }
    else{
    }

    unsigned int file_length = get_file_size(path);
    if(file_length == 0) {
        IOT_ERR("%s : NULL file.\n",__FUNCTION__);
        return NULL;
    }
    else{
    }

    FILE * fp = NULL;
    fp = fopen (path,"r");
    if(fp == NULL) {
        IOT_ERR("%s : NULL file pointer.\n",__FUNCTION__);
        return NULL;
    }
    else{
    }

    char * file_buff = (char *)malloc(file_length + 1);
    if(file_buff == NULL) {
        IOT_ERR("%s : malloc failed .\n",__FUNCTION__);
        return NULL;
    }
    else{
    }

    memset(file_buff, 0 , file_length + 1);
    size_t read_length = fread(file_buff, file_length, 1,fp);
    if(read_length != 1) {
        IOT_WARN("%s : WARN, read file %s length not mathch.\n",__FUNCTION__, path);
    }
    else{
    }

    fclose(fp);

    return file_buff;
}

/*********************************************************************/
/*
*           get_bin_file
*               获得二进制文件内容
*
* @param
*       char * path
*
* @return   char *
*               成功:
*               失败: NULL
*
* @note     分配内存,不释放形参
*
**********************************************************************/
char * get_bin_file (const char * path) {

    if(path == NULL)  {
        IOT_ERR("%s : NULL file path.\n",__FUNCTION__);
        return NULL;
    }
    else{
    }

    unsigned int file_length = get_file_size(path);
    if(file_length == 0) {
        IOT_ERR("%s : NULL file.\n",__FUNCTION__);
        return NULL;
    }
    else{
    }

    FILE * fp = NULL;
    fp = fopen (path,"rb");
    if(fp == NULL) {
        IOT_ERR("%s : NULL file pointer.\n",__FUNCTION__);
        return NULL;
    }
    else{
    }

    char * file_buff = (char *)malloc(file_length);
    if(file_buff == NULL) {
        IOT_ERR("%s : malloc failed .\n",__FUNCTION__);
        return NULL;
    }
    else{
    }

    memset(file_buff, 0 , file_length);
    size_t read_length = fread(file_buff, file_length, 1,fp);
    if(read_length != 1) {
        IOT_WARN("%s : WARN, read file %s length not mathch.\n",__FUNCTION__, path);
    }
    else{
    }

    fclose(fp);

    return file_buff;
}

/*********************************************************************/
/*
*           put_text_file
*               写入文件内容
*
* @param
*       const char * path
*       const char * file_text
*
* @return   unsigned int
*               成功:
*               失败: NULL
*
* @note     不释放内存
*
**********************************************************************/
unsigned int put_text_file(const char * path, const char * file_text) {

    if ( (path == NULL) || (file_text == NULL) ) {
        IOT_ERR("%s : NULL pointer.\n",__FUNCTION__);
        return IOT_FAILED;
    }
    else{
    }

    char * dir_name = get_dir_name(path);
    exec_shell_mkdir_p(dir_name);
    free(dir_name);

    FILE *fp = NULL;
    fp = fopen(path, "w+");

    if(1!=fwrite(file_text, strlen(file_text),1,fp)) {
        IOT_ERR("%s : write file %s failed.\n",__FUNCTION__,path);
        fclose(fp);
        return IOT_FAILED;
    }
    else{
    }

    fclose(fp);

    return IOT_SUCCEESE;

}

/*********************************************************************/
/*
*           put_bin_file
*               写入二进制文件内容
*
* @param
*       const char * path
*       const char * file_text
*       unsigned int len
*
* @return   unsigned int
*               成功:
*               失败: NULL
*
* @note     不释放内存
*
**********************************************************************/
unsigned int put_bin_file(const char * path, const char * bin_text, unsigned int len) {

    if ( (path == NULL) || (bin_text == NULL) ) {
        IOT_ERR("%s : NULL pointer.\n",__FUNCTION__);
        return IOT_FAILED;
    }
    else{
    }

    FILE *fp = NULL;
    fp = fopen(path, "wb");
    if(1!=fwrite(bin_text, len,1,fp)) {
        IOT_ERR("%s : write file %s failed.\n",__FUNCTION__,path);
        fclose(fp);
        return IOT_FAILED;
    }
    else{
    }
    fclose(fp);


    return IOT_SUCCEESE;
}

/*获得系统时间(second)*/
unsigned int get_time_sec() {
    struct timespec time;
    clock_gettime(CLOCK_REALTIME, &time);  //获取相对于1970到现在的秒数
    return(unsigned int) time.tv_sec;
}

/*----------------------------------------------------------------------*/
/*                                                                      */
/*                                bash指令                                */
/*                                                                      */
/*----------------------------------------------------------------------*/

/*********************************************************************/
/*
*           exec_shell_cmd_cp
*              copy file
*
* @param
*           char * d_file
*           char * s_file
*
* @return   unsigned int
*               成功: IOT_SUCCEESE
*               失败: IOT_FAILED
*
* @note
*           每个指令均需启动线进程，效率较低，
*           todo 考虑使用popen代替system
*
**********************************************************************/
unsigned int exec_shell_cmd_cp_rename(char * d_file, char * s_file) {

    /*判定文件合法性*/
    if ( (s_file == NULL) || (d_file == NULL) ) {
        IOT_ERR("%s : NULL pointer.\n",__FUNCTION__);
        return IOT_FAILED;
    }
    else{
    }

    if(0 != access(s_file,R_OK)) {
        IOT_ERR("%s : invalid s_file %s\n",__FUNCTION__,s_file);
        return IOT_FAILED;
    }
    else{
    }

    /*确保目标目录存在*/
    if( IOT_FAILED == check_file_dir(d_file)){
        IOT_WARN("%s : check_file_dir failed.\n",__FUNCTION__);
    }
    else{
    }

    /*生产指令并执行*/
    char * fmt_shell_cmd_cp = "/bin/cp %s %s";
    unsigned int len = strlen(fmt_shell_cmd_cp) + strlen(s_file) \
                       +strlen(d_file)+ 1;

    char * cmd_buffer = (char *)malloc(len);
    if(NULL == cmd_buffer) {
        IOT_ERR("%s : cmd buffer malloc FAILED.\n",__FUNCTION__);
        return IOT_FAILED;
    }
    else{
        sprintf(cmd_buffer, fmt_shell_cmd_cp, s_file, d_file);
        IOT_INFO("cmd : %s\n",cmd_buffer);
        system(cmd_buffer);
        free(cmd_buffer);
    }

    return IOT_SUCCEESE;
}

/*********************************************************************/
/*
*           exec_shell_chmod_run
*              添加运行权限
*
* @param
*           char * file_path
*
* @return   unsigned int
*               成功: IOT_SUCCEESE
*               失败: IOT_FAILED
*
* @note
*           每个指令均需启动线进程，效率较低，
*           todo 考虑使用popen代替system
*
**********************************************************************/
unsigned int exec_shell_chmod_run(char * file_path) {

    /*判定文件合法性*/
    if ( file_path == NULL ) {
        IOT_ERR("%s : NULL pointer.\n",__FUNCTION__);
        return IOT_FAILED;
    }
    else{
    }

    if(0 != access(file_path,W_OK)) {
        IOT_ERR("%s : invalid file %s.\n",__FUNCTION__,file_path);
        return IOT_FAILED;
    }
    else{
    }

    /*生产指令并执行*/
    char * fmt_shell_cmd_chmod = "/bin/chmod a+x %s";
    unsigned int len = strlen(fmt_shell_cmd_chmod) + \
                       strlen(file_path) + 1;
    char * cmd_buffer = (char *)malloc(len);
    if(NULL == cmd_buffer) {
        IOT_ERR("%s : cmd buffer malloc FAILED.\n",__FUNCTION__);
        return IOT_FAILED;
    }
    else{
        sprintf(cmd_buffer, fmt_shell_cmd_chmod, file_path);
        IOT_INFO("cmd : %s\n",cmd_buffer);
        system(cmd_buffer);
        free(cmd_buffer);
    }

    return IOT_SUCCEESE;
}

/*********************************************************************/
/*
*           exec_shell_mkdir_p
*              确认目录存在
*
* @param
*           char * dir_path
*
* @return   unsigned int
*               成功: IOT_SUCCEESE
*               失败: IOT_FAILED
*
* @note
*           每个指令均需启动线进程，效率较低，
*           todo 考虑使用popen代替system
*
**********************************************************************/
unsigned int exec_shell_mkdir_p(char * dir_path){

    /*判定文件合法性*/
    if ( dir_path == NULL ) {
        IOT_ERR("%s : NULL pointer.\n",__FUNCTION__);
        return IOT_FAILED;
    }
    else{
    }

    /*生产指令并执行*/
    unsigned int len = strlen("mkdir -p %s") + strlen(dir_path) + 1;
    char * cmd_buffer = (char *)malloc(len);
    if(NULL == cmd_buffer) {
        IOT_ERR("%s : cmd buffer malloc FAILED.\n",__FUNCTION__);
        return IOT_FAILED;
    }
    else{
        sprintf(cmd_buffer, "mkdir -p %s", dir_path);
        IOT_INFO("%s\n",cmd_buffer);
        system(cmd_buffer);
        free(cmd_buffer);
    }

    return IOT_SUCCEESE;
}

/*********************************************************************/
/*
*           exec_shell_umount
*              卸载文件系统
*
* @param
*           char * fs_path
*
* @return   unsigned int
*               成功: IOT_SUCCEESE
*               失败: IOT_FAILED
*
* @note
*           每个指令均需启动线进程，效率较低，
*           todo 考虑使用popen代替system
*
**********************************************************************/
unsigned int exec_shell_umount(char * fs_path) {

    /*判定文件系统合法性*/
    if ( fs_path == NULL ) {
        IOT_ERR("%s : NULL pointer.\n",__FUNCTION__);
        return IOT_FAILED;
    }
    else{
    }

    /*产生指令并执行*/
    unsigned int len = strlen("umount -fl %s") + strlen(fs_path) + 1;
    char * cmd_buffer = (char *)malloc(len);
    if(NULL == cmd_buffer) {
        IOT_ERR("%s : cmd buffer malloc FAILED.\n",__FUNCTION__);
        return IOT_FAILED;
    }
    else{
        sprintf(cmd_buffer, "umount -fl %s", fs_path);
        IOT_INFO("%s : cmd %s\n",__FUNCTION__,cmd_buffer);
        system(cmd_buffer);
        free(cmd_buffer);
    }

    return IOT_SUCCEESE;
}
