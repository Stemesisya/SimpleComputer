COMPILE= gcc -g -c
LINK= gcc -g -Wall

DEPENDENCIES= ${shell find ./ -name '*.c'}
OBJECTS= $(DEPENDENCIES:.c=.o)
BIN_NAME= console.bin

compile: ${OBJECTS}
	${LINK} $^ -o ${BIN_NAME}

%.o: %.c
	${COMPILE} $< -o $@

format:
	find . -type f -name *.[ch] | xargs clang-format --style GNU -i --verbose

all: format compile run

run:
	./${BIN_NAME}

clean:
	rm *.o
	rm ${BIN_NAME}
