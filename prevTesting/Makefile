CC = gcc # Compiler
CFLAGS = -I./src/include -I./src # C compilation flags
LDFLAGS = -lm -lpthread # Multi OS linker flags

MAKEFLAGS += --no-print-directory

UNAME_S := $(shell uname -s) # Operation system

TARGET = ./apps/sticky_fortress # Final programm

OBJ_DIR = ./bin
SRCS = $(wildcard ./src/run/*.c ./src/*.c) # Collecting .c files
OBJS = $(SRCS:.c=.o) 

HEADERS = $(wildcard ./src/include/*.h) # Headers

ifeq ($(strip $(UNAME_S)), Linux) # Linux linker flags
	LDFLAGS += -lraylib -lGL -ldl -lrt -lX11 -lm -lpthread
endif

.PHONY: all clean run crun # Reserve this names for directives
.IGNORE: crun # If error, compilation results will be clean anyway
 
all: $(TARGET)

$(TARGET): $(OBJS) # Create folder for app and compile it
	mkdir -p $(dir $(TARGET))
	$(CC) -o $@ $^ $(LDFLAGS)
# gcc ./apps/sticky_fortress [OBJS] [LDFLAGS]

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@
# gcc [CFLAGS] [.c for compile] [target .o]

clean: # Clear compilation results
	rm -f $(OBJS) 
	rm -f $(TARGET)

run: $(TARGET) # Run this program
	$(TARGET)

crun: $(TARGET) # Run this program and clean compilation results
	@./$(TARGET)
	@$(MAKE) clean