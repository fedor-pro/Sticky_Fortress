CC = gcc
UNAME_S := $(shell uname -s)

CFLAGS = -O2 -I./src -I./src/include -I/opt/homebrew/include
TARGET = ./apps/sticky_fortress

# Все .c файлы в src/
SRCS = $(wildcard ./src/*.c)
OBJS = $(SRCS:.c=.o)

HEADERS = $(wildcard ./src/include/*.h)

LDFLAGS = -lm -lpthread

ifeq ($(UNAME_S), Darwin)
	CFLAGS += -I/opt/homebrew/include
	LDFLAGS += -L/opt/homebrew/lib -lraylib \
	           -framework CoreVideo -framework IOKit -framework Cocoa \
	           -framework GLUT -framework OpenGL
endif

ifeq ($(UNAME_S), Linux)
	LDFLAGS += -lraylib -lGL -ldl -lrt -lX11
endif

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	mkdir -p $(dir $(TARGET))
	$(CC) -o $@ $^ $(LDFLAGS)

# Компиляция каждого .c в .o
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

run: $(TARGET)
	$(TARGET)