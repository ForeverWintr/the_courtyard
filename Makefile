CC=g++
CFLAGS=
TARGET=courtyard
HEADERS=courtyard.h

$(TARGET): $(TARGET).cc $(HEADERS)
	$(CC) -o $(TARGET) $(TARGET).cc $(CFLAGS)

clean:
	rm -f *.o *~
