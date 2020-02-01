
#include "log_manage.h"
#include "../path_manage/path_manage.h"

DAEMON_LOG_CTRL __daemon_log_ctrl;

/*----------------------------------------------------------------------*/
/*                                                                      */
/*                              日志操作函数                              */
/*                                                                      */
/*----------------------------------------------------------------------*/
unsigned int ini_daemon_log_ctrl(){

    __daemon_log_ctrl.log_file_lifespan         = LOG_FILE_LIFESPAN;
    __daemon_log_ctrl.log_file_switch_period    = LOG_FILE_SWITCH_PERIOD;
    __daemon_log_ctrl.log_file_clear_period     = LOG_FILE_CLEAR_PERIOD;

    __daemon_log_ctrl.last_clear_time           = 0;
    __daemon_log_ctrl.last_switch_time          = 0;

    clear_daemon_log_file();
    switch_daemon_log_file();

    return IOT_SUCCEESE;
}

unsigned int set_daemon_log_ctrl(unsigned int param_cnt,unsigned int * param){

    if(param_cnt != LOG_FILE_PARAM_NUM){
        IOT_ERR("%s : illegal param num\n",__FUNCTION__);
        return IOT_FAILED;
    }
    else{}

    if(param == NULL){
        IOT_ERR("%s : NULL param\n",__FUNCTION__);
        return IOT_FAILED;
    }
    else{}

    if((param[0]>=MIN_LOG_FILE_LIFESPAN)&&(param[0]<=MAX_LOG_FILE_LIFESPAN)){
        IOT_INFO("%s : log_file_lifespan set form %d to %d\n",\
                 __FUNCTION__,__daemon_log_ctrl.log_file_lifespan,param[0]);
        __daemon_log_ctrl.log_file_lifespan = param[0];
    }
    else{
        IOT_INFO("%s : illegal log_file_lifespan setting\n",__FUNCTION__);
    }

    if((param[1]>=MIN_LOG_FILE_SWITCH_PERIOD)&&(param[1]<=MAX_LOG_FILE_SWITCH_PERIOD)){
        IOT_INFO("%s : log_file_switch_period set form %d to %d\n",\
                 __FUNCTION__,__daemon_log_ctrl.log_file_switch_period,param[1]);
        __daemon_log_ctrl.log_file_switch_period = param[1];
    }
    else{
        IOT_INFO("%s : illegal log_file_switch_period setting\n",__FUNCTION__);
    }

    if((param[2]>=MIN_LOG_FILE_CLEAR_PERIOD)&&(param[2]<=MAX_LOG_FILE_CLEAR_PERIOD)){
        IOT_INFO("%s : log_file_clear_period set form %d to %d\n",\
                 __FUNCTION__,__daemon_log_ctrl.log_file_clear_period,param[2]);
        __daemon_log_ctrl.log_file_clear_period = param[2];
    }
    else{
        IOT_INFO("%s : illegal log_file_clear_period setting\n",__FUNCTION__);
    }

    return IOT_SUCCEESE;
}

unsigned int get_daemon_log_ctrl(unsigned int param_buffer_len,unsigned int * param){

    if(param_buffer_len < LOG_FILE_PARAM_NUM){
        IOT_ERR("%s : param buffer len too short\n",__FUNCTION__);
        return IOT_FAILED;
    }
    else{}

    if(param == NULL){
        IOT_ERR("%s : NULL param buffer\n",__FUNCTION__);
        return IOT_FAILED;
    }
    else{}

    memset(param, 0 ,sizeof(unsigned int) * param_buffer_len);
    param[0] = __daemon_log_ctrl.log_file_lifespan;
    param[1] = __daemon_log_ctrl.log_file_switch_period;
    param[2] = __daemon_log_ctrl.log_file_clear_period;

    return LOG_FILE_PARAM_NUM;
}


/*********************************************************************/
/*
*           check_daemon_log_name
*               根据时间判定是否需要清除log文件
*
* @param
*           char * log_name 文件名
*
* @return   unsigned int
*               需要清理         :  IOT_SUCCEESE
*               不需要清理    :  IOT_SOFT_FAIL
*               失败           :  IOT_FAILED
*
* @note     log name example : iotdaemon_2019-07-09_10:22:29.log
*
**********************************************************************/
unsigned int check_daemon_log_name(char * log_name) {

    /*get time*/
    time_t now;
    time(&now);
    struct tm * timenow = localtime(&now);   //do not free!

    unsigned int year  = timenow->tm_year + 1900;
    unsigned int month = timenow->tm_mon  + 1;
    unsigned int day   = timenow->tm_mday;

    /*check name ptr*/
    if(log_name == NULL) {
        IOT_ERR("%s : NULL pointer.\n",__FUNCTION__);
        return IOT_FAILED;
    }
    else{
    }

    /*check file head*/
    if(strncmp ("iot_daemon",log_name,strlen("iot_daemon")) == 0){
    }
    else{
        IOT_INFO("%s : prefix not match, %s is not a log file.\n",__FUNCTION__,log_name);
        return IOT_SUCCEESE;
    }

    /*check file extensions*/
    char * ptr_log = strrchr(log_name,'.');
    if(ptr_log == NULL){
        return IOT_SUCCEESE;
    }
    if(strcmp(".log",ptr_log) == 0){
    }
    else{
        IOT_INFO("%s : file extensions not match, %s is not a log file.\n",__FUNCTION__,log_name);
        return IOT_SUCCEESE;
    }

    /*get log file name year*/
    char * ptr_1st_underline = strchr(log_name+strlen("iot_daemon"),'_');
    if(ptr_1st_underline == NULL) {
        IOT_INFO("%s : time info not found, %s is not a log file.\n",__FUNCTION__,log_name);
        return IOT_SUCCEESE;
    }
    else{

    }

    /*year*/
    unsigned int log_year = atoi(ptr_1st_underline + 1);
    if( (log_year == 0) || (log_year > 2100) ) {
        IOT_INFO("%s : year info not found, %s is not a log file.\n",\
                 __FUNCTION__,log_name);
        return IOT_SUCCEESE;
    }
    else{
    }
    /*month*/
    unsigned int log_month = atoi(ptr_1st_underline + 6);
    if( (log_year == 0) || (log_month > 12) ) {
        IOT_INFO("%s : month info not found, %s is not a log file.\n",\
                 __FUNCTION__,log_name);
        return IOT_SUCCEESE;
    }
    else{
    }
    /*day*/
    unsigned int log_day = atoi(ptr_1st_underline + 9);
    if( (log_year == 0) || (log_month > 31) ) {
        IOT_INFO("%s : dat info not found, %s is not a log file.\n",\
                 __FUNCTION__,log_name);
        return IOT_SUCCEESE;
    }
    else{
    }

    /*check data interval*/
    unsigned int total_day = year*365+(month-1)*31+day;
    unsigned int log_total_dat = log_year*365+(log_month-1)*31+log_day;

    if( (log_total_dat + __daemon_log_ctrl.log_file_lifespan) < total_day) {
        IOT_INFO("%s : %s will be cleaned.\n",__FUNCTION__,log_name);
        return IOT_SUCCEESE ;/*应当清理*/
    }
    else{
        IOT_INFO("%s : %s will be reserved.\n",__FUNCTION__,log_name);
        return IOT_SOFT_FAIL;
    }
}

/*********************************************************************/
/*
*           clear_daemon_log_file
*               清除过期log文件
*
* @param
*
*
* @return   unsigned int
*               成功  :  清理数量
*               失败   :  IOT_FAILED
*
* @note     log name example : iotdaemon_2019-07-09_10:22:29.log
*
**********************************************************************/
unsigned int clear_daemon_log_file() {

    __daemon_log_ctrl.last_clear_time = get_time_sec();

    /*get dir path*/
    char * str_log_dir = get_path(NUM_DAEMON_LOG_DIR,0);
    if(str_log_dir == NULL) {
        IOT_ERR("%s : get str_log_dir failed.\n",__FUNCTION__);
        return IOT_FAILED;
    }
    else{
        IOT_INFO("%s : log dir path %s\n",__FUNCTION__,str_log_dir);
    }

    /*open dir*/
    DIR * dp = opendir(str_log_dir);
    if(dp == NULL) {
        IOT_ERR("%s : load dir %s failed.\n",__FUNCTION__,str_log_dir);
        free(str_log_dir);
        return IOT_FAILED;
    }
    else{
        IOT_INFO("%s : load dir %s OK.\n",__FUNCTION__,str_log_dir);
    }

    /*change to dir*/
    if( 0 != chdir(str_log_dir)) {
        IOT_ERR("%s : change to dir %s failed.\n",__FUNCTION__,str_log_dir);
        free(str_log_dir);
        return IOT_FAILED;
    }
    else{
        IOT_INFO("%s : change to dir %s OK.\n",__FUNCTION__,str_log_dir);
    }

    /*check log file*/
    IOT_DBG("%s : begin log file clear check.\n",__FUNCTION__);

    unsigned int log_file_clear_num = 0;

    struct dirent * entry; /*do not free!*/
    struct stat statbuf;
    while ( (entry = readdir(dp)) != NULL) {
        lstat(entry->d_name,&statbuf); /*get file info*/
        if(S_ISREG(statbuf.st_mode)) {  /*is regular file?*/
            IOT_INFO("%s : checking file %s\n",__FUNCTION__,entry->d_name);
            if( IOT_SUCCEESE == check_daemon_log_name(entry->d_name)){
                IOT_INFO("%s : log file will be cleared.\n",__FUNCTION__);

                if(unlink(entry->d_name)<0){
                    IOT_ERR("%s : clear file %s failed.\n",__FUNCTION__,entry->d_name);
                }
                else{
                    log_file_clear_num ++;
                    IOT_DBG("%s : clear file %s OK.\n",__FUNCTION__,entry->d_name);
                }
            }
            else{
                IOT_INFO("%s : log file will be preserved.\n",__FUNCTION__);
            }
        }
        else{}
    }

    closedir(dp);
    free(str_log_dir);

    IOT_MSG("%s : log file clear OK, %d file cleared.\n",__FUNCTION__, log_file_clear_num);

    return log_file_clear_num;
}

/*********************************************************************/
/*
*           switch_daemon_log_file
*               切换日志文件
*
* @param
*
* @return   unsigned int
*               成功       :  IOT_SUCCEESE
*               失败       :  IOT_FAILED
*
* @note     log name example : iotdaemon_2019-07-09_10:22:29.log
*
**********************************************************************/
unsigned int switch_daemon_log_file(){

    __daemon_log_ctrl.last_switch_time = get_time_sec();
#ifdef LOG_FILE
    /*获得日志文件名称*/
    char * log_file = gen_log_file_name();
    if(log_file == NULL){
        return IOT_FAILED;
    }
    else{}

    /*确认路径存在*/
    if(IOT_FAILED == check_file_dir(log_file)) {
        IOT_ERR("%s : check_file_dir %s FAILED.\n",__FUNCTION__,log_file);
        return IOT_FAILED;
    }
    else{}

    /*打开并切换日志*/
    int fd_daemon_log = open(log_file,O_RDWR| O_CREAT |O_DSYNC | O_APPEND, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH); //  O_TRUNC
    if(fd_daemon_log == -1) {
        IOT_ERR("%s : daemon log file open FAILED.\n",__FUNCTION__);
        free(log_file);
        return IOT_FAILED;
    }
    else{
        IOT_MSG("%s : switch log file to %s\n", __FUNCTION__, log_file);
        free(log_file);
        __daemon_log_ctrl.last_switch_time = get_time_sec();

        dup2(fd_daemon_log,1);
        dup2(fd_daemon_log,2);
    }
#endif

    return IOT_SUCCEESE;

}

unsigned int close_daemon_log_file(){
    close(1);
    close(2);
    return IOT_SUCCEESE;
}

unsigned int check_daemon_switch_time(){
    unsigned int time       = get_time_sec();
    unsigned int interval   = time - __daemon_log_ctrl.last_switch_time;
    unsigned int period     = __daemon_log_ctrl.log_file_switch_period*3600*24; /*day -> sec*/
    if(interval > period){
        IOT_INFO("%s : interval %d > period %d, begin switch...\n",\
                 __FUNCTION__, interval, period);
        return IOT_SUCCEESE;
    }
    else{
        IOT_INFO("%s : interval %d <= period %d, skip switch...\n",\
                 __FUNCTION__, interval, period);
        return IOT_FAILED;
    }
}

unsigned int check_daemon_clear_time(){
    unsigned int time       = get_time_sec();
    unsigned int interval   = time - __daemon_log_ctrl.last_clear_time;
    unsigned int period     = __daemon_log_ctrl.log_file_clear_period*3600*24; /*day -> sec*/
    if(interval > period){
        IOT_INFO("%s : interval %d > period %d, begin clear...\n",\
                 __FUNCTION__, interval, period);
        return IOT_SUCCEESE;
    }
    else{
        IOT_INFO("%s : interval %d <= period %d, skip clear...\n",\
                 __FUNCTION__, interval, period);
        return IOT_FAILED;
    }
}

unsigned int dump_daemon_log_ctrl(DAEMON_LOG_CTRL * ctrl){
    printf("dump_daemon_log_ctrl...\n");
    if(ctrl!=NULL){
        printf("    log_file_lifespan       : %d day.\n",ctrl->log_file_lifespan);
        printf("    log_file_switch_period  : %d day.\n",ctrl->log_file_switch_period);
        printf("    log_file_clear_period   : %d day.\n",ctrl->log_file_clear_period);
    }
    else{
        printf("    NULL\n");
    }
    return IOT_SUCCEESE;
}
