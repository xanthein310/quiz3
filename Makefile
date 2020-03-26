all: xorlist linklist

.PHONY: all clean

clean:
	rm -f xorlist linklist

xorlist: xorlist.c
	gcc -o xorlist -std=gnu11 xorlist.c
linklist: linklist.c
	gcc -o linklist -std=gnu11 linklist.c
