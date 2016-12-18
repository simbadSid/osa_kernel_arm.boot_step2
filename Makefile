##----------------------------------
## Laptop
##----------------------------------
#QEMU=qemu-system-arm
#TOOLCHAIN=/usr/local/bin/gcc-arm-none-eabi-4_8-2014q3/bin/



##----------------------------------
## Mandelbrot
##----------------------------------
#QEMU=qemu-arm
#TOOLCHAIN=


##----------------------------------
## Lqptop ulysse
##----------------------------------
#QEMU=qemu-system-arm
#TOOLCHAIN=/home/coutaudu/Desktop/gcc-arm-none-eabi-5_4-2016q3/bin/


##----------------------------------
## VM ubuntu
##----------------------------------
QEMU=qemu-system-arm
TOOLCHAIN=
import:
	rm -r -f *;\
	scp -r sidlakhr@mandelbrot.e.ujf-grenoble.fr:/home/s/sidlakhr/Desktop/osa_kernel_arm.boot_step2/* .

export:
	ssh sidlakhr@mandelbrot.e.ujf-grenoble.fr 'rm -r -f /home/s/sidlakhr/Desktop/osa_kernel_arm.boot_step2/*';\
	scp -r * sidlakhr@mandelbrot.e.ujf-grenoble.fr:/home/s/sidlakhr/Desktop/osa_kernel_arm.boot_step2/




# Choose your emulated board
BOARD_VERSATILEPB=n
BOARD_VEXPRESS=y

# Set to F to do not arm the timer initially.  Set to a T otherwise 
CONFIG_TEST_TIMER=T

# Say yes ('y') at first, using the linux terminal as a serial line.
# Then say no ('n') to discover the use of telnet connections as serial lines. 
CONFIG_LOCAL_ECHO=y

# Say yes ('y') at first, using a polling technique for the UARTs
# But be prepared to hear your fan ramp up...
# So ultimately, say n, to use interrupts 
CONFIG_POLLING=n

# This turns on minimal statistics on the malloc/free subsystem
CONFIG_SPACE_STATS=y

# This turns on minimal testing of the malloc/free subsystem,
# only when polling is on. (CONFIG_POLLING=y)
CONFIG_TEST_MALLOC=n

# Turn it on if you want to be able to debug with GDB.
# Otherwise, debug symbols are not available and the code is 
# compiled with optimization turned on.
CONFIG_DEBUG=y

####################################################################
# OPTIONS THAT YOU CAN CHANGE ARE ALL ABOVE THIS LINE.
#
# Below this line, you need to go change the OBJS variable 
# to add or remove object files from the generated kernel.
####################################################################

# Add the platform-independent code, which is your kernel.
OBJS = build/kmain.o build/kprintf.o build/kmem.o build/kirqPendingList.o

# Add the necessary support for arithmetic operations.
# The function kprintf uses integer division and modulo.
# You may need to add more support from the libeaabi.

OBJS += libaeabi/uidivmod.o  libaeabi/idiv.o  libaeabi/uldivmod.o

####################################################################
# BELOW THIS LINE -- YOU SHOULD NOT HAVE TO MODIFY ANYTHING
####################################################################

GCC=$(TOOLCHAIN)arm-none-eabi-gcc
LD=$(TOOLCHAIN)arm-none-eabi-ld
AS=$(TOOLCHAIN)arm-none-eabi-gcc
OBJCOPY=$(TOOLCHAIN)arm-none-eabi-objcopy
GDB=$(TOOLCHAIN)arm-none-eabi-gdb

ifeq ($(BOARD_VERSATILEPB),y)
  BOARD=versatilepb
  CPU=arm926ej-s
  QEMU_BOARD=versatileab
  QEMU_OPTIONS=  -m 64M 
  CONFIG_BOARD=$(BOARD)
  LDSCRIPT=ldscript.versatile
  OBJS+= build/pl011.o build/startup.o build/pl190_c.o build/pl190_s.o
endif

ifeq ($(BOARD_VEXPRESS),y)
  BOARD=vexpress-a9
  CPU=cortex-a9
  #QEMU_BOARD=xilinx-zynq-a9
  QEMU_BOARD=vexpress-a9
  QEMU_OPTIONS= -smp cpus=1 -nographic -m 128M 
  CONFIG_BOARD=vexpress_a9
  LDSCRIPT=ldscript.vexpress
  OBJS+= build/pl011.o build/startup.o build/gic_s.o build/gic_c.o build/gid.o build/user.o build/timer.o
endif

ifeq ($(CONFIG_DEBUG),y)
  ASFLAG=-g -DCONFIG_DEBUG
  CFLAGS=-g -DCONFIG_DEBUG
  LDFLAGS=-g
else
  ASFLAG=-g
  CFLAGS=-O3
  LDFLAGS=
endif

ifeq ($(CONFIG_TEST_TIMER),T)
	CFLAGS+= -DCONFIG_TEST_TIMER
endif
# Flags for compiling assembly language.
# Note that we ask GCC to do it after applying the C preprocessor (cpp).
ASFLAGS+= -mcpu=$(CPU) -c -x assembler-with-cpp -D$(CONFIG_BOARD)

# Flags for compiling C code. 
# Notice that we do not include default/standard libraries, start files, and builtin functions,
# this means that the resulting code will contain our code and our code only.
# Note that we give the specific cpu of the target board, so that GCC can generate the most adequate code.
CFLAGS+= -c -mcpu=$(CPU) -D$(CONFIG_BOARD) -fpic -std=gnu99 -nostdlib -nostartfiles  -nodefaultlibs -fno-builtin


ifeq ($(CONFIG_LOCAL_ECHO),y)
  CFLAGS+= -DLOCAL_ECHO
  SERIAL_LINES=-serial mon:stdio
else
  SERIAL_LINES=-serial telnet:localhost:5555,server -serial telnet:localhost:6666,server -monitor stdio
endif

ifeq ($(CONFIG_POLLING),y)
  CFLAGS+= -DCONFIG_POLLING
endif

ifeq ($(CONFIG_TEST_MALLOC),y) 
  CFLAGS += -DCONFIG_TEST_MALLOC
endif

ifeq ($(CONFIG_SPACE_STATS),y) 
  CFLAGS += -DCONFIG_SPACE_STATS
endif

all: dirs libaeabi/libaeabi.a $(OBJS)
	$(LD) $(LDFLAGS) -T $(LDSCRIPT) -o $(BOARD).elf $(OBJS)
	$(OBJCOPY) -O binary $(BOARD).elf $(BOARD).bin

libaeabi/libaeabi.a:  
	(cd libaeabi; make)
	
dirs:
	mkdir -p build
	
clean: 
	(cd libaeabi; make clean)
	rm -rf build
	rm -f *.o 
	rm -f *.elf
	rm -f *.bin

#
# Platform-dependent code
#
build/startup.o: startup.s Makefile
	$(AS) $(ASFLAGS) startup.s -o build/startup.o

build/gic_s.o: gic.s Makefile
	$(AS) $(ASFLAGS) gic.s -o build/gic_s.o

build/gic_c.o: gic.c Makefile
	$(GCC) $(CFLAGS) gic.c -o build/gic_c.o

build/gid.o: gid.c Makefile
	$(GCC) $(CFLAGS) gid.c -o build/gid.o

build/pl011.o: pl011.c Makefile
	$(GCC) $(CFLAGS) pl011.c -o build/pl011.o

build/pl190_c.o: pl190.c Makefile
	$(GCC) $(CFLAGS) pl190.c -o build/pl190_c.o

build/pl190_s.o: pl190.s Makefile
	$(AS) $(ASFLAGS) pl190.s -o build/pl190_s.o

build/timer.o: timer.c Makefile
	$(GCC) $(CFLAGS) $^ -o $@


#
# Platform-independent code
#
build/kprintf.o: kprintf.c Makefile
	$(GCC) $(CFLAGS) kprintf.c -o build/kprintf.o

build/kmem.o: kmem.c Makefile
	$(GCC) $(CFLAGS) kmem.c -o build/kmem.o

build/kirqPendingList.o: kirqPendingList.c Makefile
	$(GCC) $(CFLAGS) -o $@ $^

build/kmain.o: kmain.c Makefile
	$(GCC) $(CFLAGS) kmain.c -o build/kmain.o

#
# User code
#
build/user.o: user.c Makefile
	$(GCC) $(CFLAGS) user.c -o build/user.o

run: all
	$(QEMU) -M $(QEMU_BOARD) -kernel $(BOARD).bin $(SERIAL_LINES) $(QEMU_OPTIONS) 

gdb: all
	$(QEMU) -M $(QEMU_BOARD) -kernel $(BOARD).bin $(SERIAL_LINES) $(QEMU_OPTIONS) -s -S

gdb_local: 
	$(GDB) $(BOARD).elf
kill:
	pkill qemu-system-arm

