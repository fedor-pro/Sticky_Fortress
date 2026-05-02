CC = gcc
CFLAGS = -O2 -I./modules
LDFLAGS = -g -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
TARGET = ./apps/sticky_fortress

HEADERS = $(wildcard ./modules/*.h)

$(TARGET): main.c $(HEADERS)
	$(CC) $(CFLAGS) -o $(TARGET) main.c $(LDFLAGS)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	$(TARGET)