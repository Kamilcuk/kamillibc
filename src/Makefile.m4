OBJ_M4=$(wildcard ./*.m4)
OBJ=$(patsubst %.m4,%,$(OBJ_M4))

all: $(OBJ)

%: %.m4
	m4 $< > $*
