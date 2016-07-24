cbus_dump: cbus_dump.c
	mkdir -p bin
	gcc -o bin/cbus_dump cbus_dump.c 
clean:
	rm  -rf bin

all: cbus_dump
