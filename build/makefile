### Target Platform ####################
TARGET = zxn

### Application Name ###################
APPNAME = pwd

### SOurce Directories #################
SRC_DIR = ../src
INC_DIR = ../inc
BUILD_DIR = .

### Source Files #######################
SRCS = $(SRC_DIR)/main.c
# SRCS += $(SRC_DIR)/zxn_utils.c

### Compiler Flags #####################
CFLAGS = -compiler=sdcc --vc -clib=sdcc_iy -SO3 --opt-code-size -I$(INC_DIR) -pragma-include:$(INC_DIR)/zpragma.inc

# select CRT
CFLAGS += -startup=30

# tradeoff speed vs. quality
CFLAGS += --max-allocs-per-node200000

# verbose output
# CFLAGS += -v

# create list files
# CFLAGS += --list

# create debug code
# CFLAGS += -D__DEBUG__

### Linker Flags #######################
LDFLAGS = -subtype=dot -Cz"--clean" -create-app -o $(BUILD_DIR)/$(APPNAME) 

# create map file
# LDFLAGS += -m  

# create symbol files
# LDFLAGS += -s  

### Compiler Command ###################
CC = zcc +$(TARGET) $(CFLAGS) $(SRCS) $(LDFLAGS)

### Build target #######################
all:
	$(CC)

### Clean Build Files ##################
clean:
	@rm -f $(BUILD_DIR)/$(APPNAME)
	@rm -f $(BUILD_DIR)/$(APPNAME).lis
	@rm -f $(BUILD_DIR)/$(APPNAME).map
	@rm -f $(BUILD_DIR)/$(APPNAME).sym
	@rm -f $(SRC_DIR)/*.lis
	@rm -f $(SRC_DIR)/*.sym
