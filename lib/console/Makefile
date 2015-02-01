#####################################################################
#                                    
#
#	File  Name   : Makefile
#	Description  : General Makefile Version
#		       
#	Modification : 2015/02/01  16:42 Oliver Create the file
#####################################################################
CC      = $(CROSS)gcc
CFLAGS += -lpthread -Wall -O2

SRCDIR	= ./
SRCS 	= $(SRCDIR)console.c \
 	  $(SRCDIR)console_test.c 


EXEC	= console

OBJS	= $(SRCS:.c=.o)

all: $(EXEC)

$(EXEC):$(OBJS)  #complie appcation
	$(CC) $(OBJS) -o $(EXEC) $(CFLAGS)
	@echo "-- Build Target Appcation ($(EXEC)) Successful --"

clean:
	@echo ">>>>>>>>>>>> make clean <<<<<<<<<<<<<<"
	@find . -type f	\(	-name 'core'				\
					-o	-name '*.bak'				\
					-o	-name '*~'					\
					-o	-name '*.o'					\
					-o	-name '.depend'				\
					\)	-print | xargs rm -f
	@rm -rf $(LIB) $(EXEC)
