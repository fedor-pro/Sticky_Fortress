CC = gcc
CFLAGS = -Wall -O2
LDFLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
TARGET = ./apps/sticky_fortress

all: $(TARGET)

$(TARGET): main.c
	$(CC) $(CFLAGS) -o $(TARGET) main.c $(LDFLAGS)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)
