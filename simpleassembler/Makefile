ARGS=

SOURCES= ${shell find ./ -name '*.c'}
OBJECTS= $(addprefix .build/, $(SOURCES:.c=.o))

LIBS= ${shell find ../ -name '*.a'}

BIN_NAME= .bin/sat.bin

LIBS = ../mySimpleComputer/.bin/libMySimpleComputer.a


COMPILE= gcc -g -c -Wall -Werror -Wextra -I ../
LINK= gcc -g -Wall -Werror -Wextra -I ../ 

all: ${BIN_NAME}

${BIN_NAME}: ${OBJECTS} ${LIBS}
	@mkdir -p $(@D)
	${LINK} $^ -o ${BIN_NAME}

.build/%.o: %.c
	@mkdir -p $(@D)
	${COMPILE} $< -o $@

run:
	./${BIN_NAME} ${ARGS}

clean:
	-@rm -r .bin/
	-@rm -r .build/