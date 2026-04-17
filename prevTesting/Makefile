CC = gcc
CFLAGS = -O2 -I./modules
LDFLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
TARGET = ./apps/sticky_fortress

HEADERS = $(wildcard ./modules/*.h)

all: $(TARGET)

$(TARGET): main.c $(HEADERS)
	$(CC) $(CFLAGS) -o $(TARGET) main.c $(LDFLAGS)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run