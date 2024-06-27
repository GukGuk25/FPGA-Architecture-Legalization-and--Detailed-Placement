CC = g++
CFLAGS = -Wall -g
CLEANC=rm -rf *.o
TARGET = legalizer

all: $(TARGET)

$(TARGET): $(TARGET).cpp
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).cpp

clean:
	$(CLEANC) $(TARGET)