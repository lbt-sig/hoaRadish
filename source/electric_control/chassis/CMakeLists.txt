cmake_minimum_required(VERSION 3.10)

DEVICE=STM32F767ZIT

ASM_SOURCES =  \
./libs/STLib/startup_stm32f40_41xxx.s

# link script
LDSCRIPT = STM32G0B1RBTx_FLASH.ld

# cpu
CPU = -mcpu=cortex-m0plus

# C defines
C_DEFS =  \
-DUSE_HAL_DRIVER \
-DSTM32F40_41xx

TARGET = app

# debug build?
# DEBUG = 1
# optimization
OPT = -O2

BUILD_DIR = build

#######################################
# binaries
#######################################
PREFIX = arm-none-eabi-
# The gcc compiler bin path can be either defined in make command via GCC_PATH variable (> make GCC_PATH=xxx)
# either it can be added to the PATH environment variable.
ifdef GCC_PATH
CC = $(GCC_PATH)/$(PREFIX)gcc
AS = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(GCC_PATH)/$(PREFIX)objcopy
SZ = $(GCC_PATH)/$(PREFIX)size
else
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
endif
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S
 
#######################################
# CFLAGS
#######################################

# fpu
# NONE for Cortex-M0/M0+/M3

# float-abi

# mcu
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# macros for gcc
# AS defines
ASM_DEFS = 

# AS includes
ASM_INCLUDES = 

IF (WIN32)
    MESSAGE(STATUS "Windows.\n")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /W4")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")

ELSEIF (LINUX_x86)
    add_definitions(-DTZUNIX)
    MESSAGE(STATUS "UNIX-like.\n")
#编译器unix
    set (CMAKE_C_COMPILER "gcc")
    set (CMAKE_CXX_COMPILER "g++")
# 32、64位编译选项 unix
    set(PROGRAM_BITS -m64)
#警告级别
    set(WARN_LEVEL -Wall)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${PROGRAM_BITS} ${WARN_LEVEL}") 
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${PROGRAM_BITS} ${WARN_LEVEL}")
    MESSAGE(STATUS "program_bits ${PROGRAM_BITS}")
ENDIF ()


# about source file defiine.
set(3DART_HOME ./3dparty)
set(BSP_HOME ./Bsp)
set(COMMON_HOME ./common)
set(LIBS_HOME ./libs)

# if use keil . noraml not use this.
set(PROJECT_HOME ./project)

set(USER_HOME ./User)

C_SOURCES =$(shell find . -name "*.c")
C_INCLUDES=$(addprefix -I, sort $(dir $(shell find . -name "*.h")))


# compile gcc flags
ASMFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif


# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"

echo_file:
	@echo $(C_SOURCES)


# libraries
LIBS = -lc -lm -lnosys 
LIBDIR = 
LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

# default action: build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin


#######################################
# build the application
#######################################
# list of objects

OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@
	
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@	
	
$(BUILD_DIR):
	mkdir $@		

#######################################
# clean up
#######################################
clean:
	-rm -fR $(BUILD_DIR)


install:$(BUILD_DIR)/$(TARGET).hex
	$(GEN_INSTALL_JLINK) > ./Jlink/install.jlink
	$(JLINK) ./Jlink/install.jlink

#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)

# *** EOF ***

