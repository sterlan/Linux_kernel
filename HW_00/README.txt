Onomatepwnumo: Lantzos Stergios, AEM: 2789, e-mail: lstergios@uth.gr
Onomatepwnumo: Kwnstantinos Zouroufidis, AEM: 1315, e-mail: zouroufi@uth.gr
Onomatepwnumo: Alexandros Leventis, AEM: 2702, e-mail: lalexandros@uth.gr

Compile find_roots_lib app:
1. Compile to roots.c gia na paraxthei to .o
	gcc -c roots.c -o roots.o:
2. link tou .o me thn statikh vivliothiki 
	ar -rcs libroots.a roots.o
3. compile to find_roots_lib.c:
	gcc -g -Wall find_roots_lib.c libroots.a -o find_roots_lib

Compile tou module:
1. make to Makefile (paragetai to project1-kyber.ko arxeio)
2. egkatastash tou kernel module: sudo insmod project1-kyber.ko (pou paragetai apo to Makefile)

gia energopoihsh tou dikou mas scheduler :
	sudo bash -c 'echo team15_02_89 > /sys/block/sda/queue/scheduler'   

gia epanafora tou mq-deadline: 
	sudo bash -c 'echo mq-deadline > /sys/block/sda/queue/scheduler'

kai sthn sunexeia: sudo rmmod project1-kyber.ko , wste na afairethei to module pou prosthesame
prohgoumenws

Compile tou 3ou merous:
1. make to Makefile
2. sudo insmod sysfs_module.ko
3. ektelesh entolhs: sudo cat /sys/kernel/team15_02_89/find_roots
4. Ektelesh dmesg (h dmesg | tail)
5. sudo rmmod sysfs_module.ko






