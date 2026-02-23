SOURCES= ${shell find ./ -name '*.c'}
OBJECTS= $(addprefix .build/, $(SOURCES:.c=.o))

LIB_NAME= libMyTerm.a

COMPILE= gcc -I ../ -g -c
LINK= ar rcs

all: ${LIB_NAME}

${LIB_NAME}: ${OBJECTS}
	${LINK} ${LIB_NAME} $^

.build/%.o: %.c
	@mkdir -p $(@D)
	${COMPILE} $< -o $@

clean:
	find -name '${LIB_NAME}' -delete
