include cc.mk
include includes.mk

SUBDIRS := calculate message messenger rpi_sensor_control sensor_poll socket strike types utils  
.PHONY: $(SUBDIRS) clean
.SUFFIXES: .c .o

server: server.o
	$(CC) $(CFLAGS) -pthread \
			-v\
			utils/utils.o \
			calculate/calculate.o\
			message/get_message/get_message.o\
			message/set_message/set_message.o\
			sensor_poll/sensor_poll.o\
			socket/socket.o\
			strike/strike.o\
			types/types.o\
			rpi_sensor_control/rpi_sensor_control.o\
			messenger/messenger.o\
			server.o\
			-L$(HOME)/wsServer/ -l ws\
			-l wiringPi\
			-o server

server.o: server.c $(SUBDIRS)
-include server.d

$(SUBDIRS):
	(cd $@; make)

clean:
	for dir in $(SUBDIRS); do \
		(cd $$dir; make clean); \
	done

	$(RM) server
	$(RM) server.o
	$(RM) server.d
