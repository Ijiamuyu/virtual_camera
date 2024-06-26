KERN_DIR = /home/jiamuyu/workspace/Project/stm32mp157/linux

all:
	make -C $(KERN_DIR) M=`pwd` modules

clean:
	make -C $(KERN_DIR) M=`pwd` modules clean
	rm -rf modules.order

vsif-objs = sif/vsif.o
vsensor-objs = sensor/vsensor.o

# obj-m    += $(vsif-objs)
obj-m   += $(vsensor-objs)
