# GNU licence - copyleft	  #
# author:		David �epel�k #

.PHONY: clean tests

CFLAGS+=-c -std=gnu11 -Wall -Wextra --pedantic -Werror -ggdb3
LDFLAGS+=-Wall

OBJS=rfidconv.o crc8.o common.o provis.o safeq.o alpus.o
PROG=rfidconv


$(PROG): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c %.h
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -rf -- $(PROG) $(OBJS)

install: $(PROG)
	sudo install $(PROG) /usr/bin

tests: $(PROG)
	cd ../tests; ./run-tests.pl < data.csv
