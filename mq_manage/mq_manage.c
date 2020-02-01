
#include "mq_manage.h"

int __mq_heartbeat_id = -1;
int __mq_daemon_ctrl_id = -1;

/*----------------------------------------------------------------------*/
/*                                                                      */
/*                              MQ management                           */
/*                                                                      */
/*----------------------------------------------------------------------*/

/*********************************************************************/
/*
*       ini_mq
*           初始化消息队列
*
* @param
*
* @return
*
* @note
*
**********************************************************************/
unsigned int ini_mq() {

    unsigned int index = 0;
    unsigned mq_id = 0;
    for(index = 0; index < IOT_MQ_NUM; index ++){
        mq_id = index + IOT_MQ_BASE_ID;
        clear_mq(mq_id);
    }

    return create_daemon_mq();
}

/*********************************************************************/
/*
*       clear_mq
*           先连接队列,再清除消息队列
*
* @param
*           unsigned int mq_key
*
* @return
*
* @note
*
**********************************************************************/
unsigned int clear_mq(unsigned int mq_key) {
    int mq_id = msgget ((key_t)mq_key,0666 | IPC_CREAT);
    if(mq_id == -1) {
    }
    else{
        msgctl (mq_id, IPC_RMID,0);
    }
    return IOT_SUCCEESE;
}

/*********************************************************************/
/*
*       create_daemon_mq
*           申请消息队列
*
* @param
*
* @return
*
* @note
*
**********************************************************************/
unsigned int create_daemon_mq (){

    unsigned int flag = IOT_SUCCEESE;
    /*heart beat*/
    __mq_heartbeat_id = msgget ((key_t)DAEMON_HEARTBEAT_MQ, 0666 | IPC_CREAT);
    if(__mq_heartbeat_id == -1) {
        IOT_ERR("%s : create __mq_heartbeat_id FAILED.\n",__FUNCTION__);
        flag = IOT_FAILED;
    }
    else{
    }

    /*daemon ctrl*/
    __mq_daemon_ctrl_id = msgget ((key_t)DAEMON_CTRL_MQ, 0666 | IPC_CREAT | IPC_NOWAIT);
    if(__mq_daemon_ctrl_id == -1) {
        IOT_ERR("%s : create __mq_daemon_ctrl_id FAILED.\n",__FUNCTION__);
        flag = IOT_FAILED;
    }
    else{
    }

    return flag;
}

/*********************************************************************/
/*
*           release_daemon_mq
*           关闭已经连接的消息队列
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
unsigned int release_daemon_mq() {
    unsigned int flag = IOT_SUCCEESE;

    if(msgctl(__mq_heartbeat_id,IPC_RMID, 0) == -1) {
        IOT_ERR("%s : release mq_clct_task_id FAILED.\n",__FUNCTION__);
        flag =  IOT_FAILED;
    }
    else{
    }

    if(msgctl(__mq_daemon_ctrl_id,IPC_RMID, 0) == -1) {
        IOT_ERR("%s : release mq_daemon_ota_id FAILED.\n",__FUNCTION__);
        flag = IOT_FAILED;
    }
    else{
    }

    return flag;
}

/*----------------------------------------------------------------------*/
/*                                                                      */
/*                              MSG_HEARTBEAT                           */
/*                                                                      */
/*----------------------------------------------------------------------*/
MSG_HEARTBEAT * create_heartbeat_msg(){

    MSG_HEARTBEAT * msg = (MSG_HEARTBEAT *)malloc(sizeof(MSG_HEARTBEAT));
    if(msg == NULL) {
        IOT_ERR("%s : MSG_HEARTBEAT malloc failed.\n",__FUNCTION__);
        return NULL;
    }
    else{
        memset(msg , 0, sizeof(MSG_HEARTBEAT));
    }

    return msg;
}

unsigned int destroy_heartbeat_msg(MSG_HEARTBEAT ** msg) {

    if((* msg) == NULL) {
        return IOT_SUCCEESE;
    }
    else{
    }

    free((* msg));

    (* msg) = NULL;

    return IOT_SUCCEESE;

}

unsigned int dump_heartbeat_msg(MSG_HEARTBEAT * msg){

    printf("dumping MSG_HEARTBEAT msg ...\n");
    if(msg == NULL) {
        printf("    NULL pointer\n");
        return IOT_SUCCEESE;
    }
    else{
    }

    printf("    msg type : %d\n",(unsigned int)msg->msg_type);
    printf("    msg IOT_pid : %d\n",msg->IOT_pid);
    printf("    msg IOT_state : %d\n",msg->IOT_state);
    printf("    msg IOT_ctrl : %d\n",msg->IOT_ctrl);
    printf("    msg IOT_OTA_flag : %d\n",msg->IOT_OTA_flag);

    return IOT_SUCCEESE;
}

MSG_HEARTBEAT * get_heartbeat_msg(unsigned int type) {

    MSG_HEARTBEAT msg;
    MSG_HEARTBEAT * pmsg = NULL;

    if(__mq_heartbeat_id == -1) {
        IOT_ERR("%s : msg queue __mq_heartbeat_id not correct opened.\n",__FUNCTION__);
        return NULL;
    }
    else{
    }

    int msgrcv_flag = msgrcv (__mq_heartbeat_id, \
                              (void *) &msg, sizeof(MSG_HEARTBEAT) - sizeof(long int), type, IPC_NOWAIT);
    if(msgrcv_flag == -1) {
    }
    else{
        pmsg = (MSG_HEARTBEAT *)malloc(sizeof(MSG_HEARTBEAT));
        if(pmsg == NULL) {
            IOT_ERR("%s : MSG_HEARTBEAT malloc failed.\n",__FUNCTION__);
            return NULL;
        }
        else{
            memcpy(pmsg, &msg , sizeof(MSG_HEARTBEAT));
        }
    }

    return pmsg;
}

unsigned int put_heartbeat_msg(unsigned int type, MSG_HEARTBEAT * msg) {

    if( msg == NULL ) {
        IOT_ERR("%s : NULL pointer.\n",__FUNCTION__);
        return IOT_FAILED;
    }
    else{
    }

    if(__mq_heartbeat_id == -1) {
        IOT_ERR("%s : msg queue __mq_heartbeat_id not correct opened.\n",__FUNCTION__);
        return IOT_FAILED;
    }
    else{
    }

    msg->msg_type = type;

    int msgsnd_flag = msgsnd (__mq_heartbeat_id, (void *) msg, \
                              sizeof(MSG_HEARTBEAT) - sizeof(long int), IPC_NOWAIT);
    if(msgsnd_flag == -1) {
        IOT_WARN("%s : __mq_heartbeat_id msgsnd FAILED.\n",__FUNCTION__);
        return IOT_FAILED;
    }
    else{
    }

    return IOT_SUCCEESE;
}

/*通过心跳消息向主程序发送指令*/
unsigned int heartbeat_msg_ctrl(unsigned int iot_ctrl) {

    MSG_HEARTBEAT msg ;
    memset(&msg, 0, sizeof(MSG_HEARTBEAT));

    msg.IOT_ctrl = iot_ctrl;

    if(IOT_SUCCEESE != put_heartbeat_msg(MSG_HEARTBEAT_TYPE_RSPD,&msg)) {
        IOT_ERR("%s : heartbeat_msg_feed_back FAILED.\n",__FUNCTION__);
        return IOT_FAILED;
    }
    else{
    }

    return IOT_SUCCEESE;

}

/*----------------------------------------------------------------------*/
/*                                                                      */
/*                              MSG_DAEMON_CTRL                         */
/*                                                                      */
/*----------------------------------------------------------------------*/
MSG_DAEMON_CTRL * create_daemon_ctrl_msg(){

    MSG_DAEMON_CTRL * msg = (MSG_DAEMON_CTRL *)malloc(sizeof(MSG_DAEMON_CTRL));
    if(msg == NULL) {
        IOT_ERR("%s : MSG_DAEMON_CTRL malloc failed.\n",__FUNCTION__);
        return NULL;
    }
    else{
        memset(msg , 0, sizeof(MSG_DAEMON_CTRL));
    }

    return msg;
}

unsigned int destroy_daemon_ctrl_msg(MSG_DAEMON_CTRL ** msg) {
    if((* msg) == NULL) {
        return IOT_SUCCEESE;
    }
    else{
    }
    free((* msg));
    (* msg) = NULL;
    return IOT_SUCCEESE;
}

MSG_DAEMON_CTRL * get_daemon_ctrl_msg(unsigned int type){

    MSG_DAEMON_CTRL msg;
    MSG_DAEMON_CTRL * pmsg = NULL;

    if(__mq_daemon_ctrl_id == -1) {
        IOT_ERR("%s : msg queue __mq_daemon_ctrl_id not correct opened.\n",__FUNCTION__);
        return NULL;
    }
    else{
    }

    int msgrcv_flag = msgrcv (__mq_daemon_ctrl_id, \
                              (void *) &msg, sizeof(MSG_DAEMON_CTRL) - sizeof(long int), type, IPC_NOWAIT);
    if(msgrcv_flag == -1) {
    }
    else{
        pmsg = (MSG_DAEMON_CTRL *)malloc(sizeof(MSG_DAEMON_CTRL));
        if(pmsg == NULL) {
            IOT_ERR("%s : MSG_DAEMON_CTRL malloc failed.\n",__FUNCTION__);
            return NULL;
        }
        else{
            memcpy(pmsg, &msg , sizeof(MSG_DAEMON_CTRL));
        }
    }
    return pmsg;
}


unsigned int put_daemon_ctrl_msg(unsigned int type, MSG_DAEMON_CTRL * msg) {

    if( msg == NULL ) {
        IOT_ERR("%s : NULL pointer.\n",__FUNCTION__);
        return IOT_FAILED;
    }
    else{
    }

    if(__mq_daemon_ctrl_id == -1) {
        IOT_ERR("%s : msg queue __mq_daemon_ctrl_id not correct opened.\n",__FUNCTION__);
        return IOT_FAILED;
    }
    else{
    }

    msg->msg_type = type;

    int msgsnd_flag = msgsnd (__mq_daemon_ctrl_id, (void *) msg, \
                              sizeof(MSG_DAEMON_CTRL) - sizeof(long int), IPC_NOWAIT);
    if(msgsnd_flag == -1) {
        IOT_WARN("%s : __mq_daemon_ctrl_id msgsnd FAILED.\n",__FUNCTION__);
        return IOT_FAILED;
    }
    else{
    }

    return IOT_SUCCEESE;
}



unsigned int dump_daemon_ctrl_msg(MSG_DAEMON_CTRL * msg){
    printf("dumping MSG_DAEMON_CTRL msg ...\n");
    if(msg == NULL) {
        printf("    NULL pointer\n");
        return IOT_SUCCEESE;
    }
    else{
    }

    printf("    msg type    : %d\n",(unsigned int)msg->msg_type);
    printf("    ctrl type   : %d\n",(unsigned int)msg->iot_daemon_ctrl);
    printf("    param type  : %d\n", (unsigned int)msg->iot_daemon_param_type);
    printf("    param num   : %d\n", (unsigned int)msg->iot_daemon_param_num);
    printf("        index   param\n");
    unsigned int index = 0;
    for (index = 0; index < IOT_DAEMON_PARAM_NUM; index ++){
        printf("        %d  %d\n",index, (unsigned int)msg->iot_daemon_param[index]);
    }
    return IOT_SUCCEESE;
}
