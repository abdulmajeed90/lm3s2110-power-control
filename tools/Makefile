# All of develop tools run in x86
TOOL_CC=gcc
objs=$(ls ./*.c)

all:$(objs)
	echo $(objs)

%.bin:%.c
	$(TOOL_CC) -Wall -o $@ $<

%.bin:%.S
	$(TOOL_CC) -Wall -o $@ $<

.PHONY:clean
clean:
	rm -f *.bin
