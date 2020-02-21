# linux-lkm
A Loadable Kernel Module in Linux

lab2test is the user space code, lab2 is a LKM.

with following commands;
sudo insmod lab2.ko mknod /dev/lab2 c 244 0 we are creating this device. The output of userspace, according to parameters, will be written in that device, for other users to read.
