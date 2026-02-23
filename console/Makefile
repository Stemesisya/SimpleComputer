SOURCES= ${shell find ./ -name '*.c'}
OBJECTS= $(addprefix .build/, $(SOURCES:.c=.o))

BIN_NAME= console.bin

LIB = ../mySimpleComputer/libMySimpleComputer.a
LIB2 = ../myTerm/libMyTerm.a

COMPILE= gcc -g -c -I ../
LINK= gcc -g -Wall -I ../ 

all: ${BIN_NAME}

${BIN_NAME}: ${OBJECTS} ${LIB} ${LIB2}
	${LINK} $^ -o ${BIN_NAME}

.build/%.o: %.c
	@mkdir -p $(@D)
	${COMPILE} $< -o $@

run:
	./${BIN_NAME}

clean:
	find -name '${BIN_NAME}' -delete
