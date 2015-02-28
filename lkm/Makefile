#
# Sample Linux Kernel Module Makefile
#

obj-m = main.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd)
clean:
	rm -rf *.o *.ko *.mod.c modules.order Module.symvers
