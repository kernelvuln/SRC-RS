#
# Sample Linux Kernel Module Makefile
#

# module name
obj-m = mod_param.o
# module src
#sample-y = base_sample.o

PWD :=$(shell pwd)
KDIR :=/lib/modules/$(shell uname -r)/build

all:
	make -C $(KDIR) M=$(PWD) modules
clean:
	make -C $(KDIR) M=$(PWD) clean
