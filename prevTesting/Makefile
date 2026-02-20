CC = gcc
CFLAGS = -Wall -O2
LDFLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
TARGET = bouncing_balls

all: $(TARGET)

$(TARGET): main.c
	$(CC) $(CFLAGS) -o $(TARGET) main.c $(LDFLAGS)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)
