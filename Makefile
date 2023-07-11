##
# Project ASMUlator
#
# @file
# @version 0.1

CC=gcc
#FLAGS=-ggdb -lpthread -lm -lunicorn
FLAGS=-lm -lunicorn 

all: build strip

build: createBin createShellcode
	$(CC) -I ./inc/ ./src/*.c $(FLAGS) -o ./bin/asmulator

createBin:
	mkdir -p ./bin/

strip: createBin createShellcode
	strip ./bin/asmulator

createShellcode:
# sets EAX: 0xdeadbeef
# executes command touch flag
# echo -n -e 'sheltouch flag\xb8\xef\xbe\xad\xde\xba\xca\xfe\x00\x00' > /tmp/shellcode.raw
# echo -n -e 'shel\xb8\xef\xbe\xad\xde\xb9\x00\x02\x00\x00' > /tmp/shellcode.raw
# See the emulate function header for a protocl breakdown
	echo -n -e "shel\xde\x00\xa0\xe3\xad\x10\xa0\xe3\xbe\x20\xa0\xe3\xef\x30\xa0\xe3\x7f\x45\x4c\x46\x90\x90\x90\xa0" > /tmp/shellcode.raw

clean:
	rm -rf ./bin/
