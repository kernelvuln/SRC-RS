#
# Sample Linux Kernel Module Makefile
#

obj-m += base_sample.o
PWD :=$(shell pwd)
KDIR :=/lib/modules/$(shell uname -r)/build

all:
	make -C $(KDIR) M=$(PWD) modules
clean:
	make -C $(KDIR) M=$(PWD) clean
