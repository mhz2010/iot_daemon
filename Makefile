

###############################################################################

# msg queue base key
BASE_MQ := 0x1230
IOT_MQ_BASE_ID := 0x1230
export CFLAGS += -D BASE_MQ=$(BASE_MQ)
export CFLAGS += -D IOT_MQ_BASE_ID=$(IOT_MQ_BASE_ID)
###############################################################################
PARM_CLCT_ROOT=\"/home/iot_daemon\"
PARM_DAEMON_ROOT=\"/home/iot_daemon\"

export CFLAGS += -D CLCT_ROOT=$(PARM_CLCT_ROOT)
export CFLAGS += -D DAEMON_ROOT=$(PARM_DAEMON_ROOT)

export CFLAGS += -D IOT_DAEMON  #enable daemon msg
export CFLAGS += -D LOG_FILE
###############################################################################

LDFLAGS := -lrt -lm

objs += ./daemon/daemon.o
objs += ./firmware/firmware.o
objs += ./heartbeat/heartbeat.o
objs += ./log_manage/log_manage.o
objs += ./mq_manage/mq_manage.o
objs += ./OTA/OTA.o
objs += ./path_manage/path_manage.o
objs += ./watchdog/watchdog.o

objs += IoT_Daemon.o

deamon : $(objs) 
	$(CC)  -o IoT_Daemon $(objs) $(LDFLAGS)

clean : 
	-rm IoT_Daemon $(objs) 

