obj-m += kp_ex2.o
#startstop-objs := start.o stop.o 

KDIR = /usr/src/linux-headers-$(shell uname -r)

all:
	make -C $(KDIR) M=$(PWD) modules
clean:
	make -C $(KDIR) M=$(PWD) clean

