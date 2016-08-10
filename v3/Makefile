KERNEL_DIR:=/lib/modules/$(shell uname -r)/build
obj-m:=yy_diag.o
default:
	$(MAKE) -C $(KERNEL_DIR) M=$(shell pwd) modules
clean:
	rm -rf .*.cmd *.mod.c *.o -r .tmp *.ko *.order *.symvers

