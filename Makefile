#Define Project Name (output filenames are based on this)
PRONAME = heating

#Define toolchain executeables
AR = xtensa-lx106-elf-ar
CC = xtensa-lx106-elf-cc
NM = xtensa-lx106-elf-nm
CPP = xtensa-lx106-elf-cpp
OBJCOPY = xtensa-lx106-elf-objcopy

#Define file paths
SRCDIR=./src
OBJDIR := .obj
BINDIR=./bin
INCLUDES=-I./include -I../include -I../ribanesp8266/include -I../include/espressif -I../include/lwip -I../include/lwip/ipv4 -I../include/lwip/ipv6 -I../extra_include -I../include/freertos/
LDDIR=../ld
LIBS=-lfreertos -lgcc -lribanesp8266 -lhal -lphy -lpp -lnet80211 -lwpa -lmain -lfreertos -llwip -ludhcp

#Define each object to be created
OBJS := $(addprefix $(OBJDIR)/,user.o)

#Define compiler and linker flags
CFLAGS= -Os -DICACHE_FLASH -mlongcalls $(INCLUDES)
LDFLAGS= -Wl,-EL \
	-L../lib \
	-nostdlib \
    -T$(LD_FILE) \
	-Wl,--no-check-sections	\
    -u call_user_start	\
	-Wl,-static \
	-Wl,--start-group \
	$(LIBS) $(BINDIR)/$(PRONAME).a \
	-Wl,--end-group \

ifeq ($(FLAVOR),debug)
    LDFLAGS += -g -O2
endif

ifeq ($(FLAVOR),release)
    LDFLAGS += -g -O0
endif
LD_FILE = $(LDDIR)/eagle.app.v6.ld

#This is the main target to create the library
all: $(OBJS) $(PRONAME)

#Rule to build each object file from its source file
$(OBJDIR)/%.o : $(SRCDIR)/%.c
	$(COMPILE.c) $(OUTPUT_OPTION) $<

#Rule to link libraries to binary elf file
$(PRONAME) :
	mkdir -p $(BINDIR)
	$(AR) ru $(BINDIR)/$(PRONAME).a $(OBJDIR)/user.o
	$(CC) -o $(BINDIR)/$(PRONAME) $(LDFLAGS)
	esptool.py elf2image $(BINDIR)/$(PRONAME)

#Rule to create obect directory if not exist and build objects
$(OBJS): | $(OBJDIR)

#Rule to create object directory
$(OBJDIR):
	mkdir -p $(OBJDIR)

#$(BINDIR)/%.bin: $(IMAGEODIR)/%.out
#	@mkdir -p $(BINDIR)
#	$(OBJCOPY) -O binary $< $@
#	esptool.py elf2image $<

flash:
	esptool.py --port /dev/ttyS4 write_flash 0x00000 $(BINDIR)/$(PRONAME)-0x00000.bin 0x40000 $(BINDIR)/$(PRONAME)-0x40000.bin

.PHONY : clean

#Rule to clean (remove) object directory
clean:
	-rm -rf $(OBJDIR) 
	-rm -rf $(BINDIR)

