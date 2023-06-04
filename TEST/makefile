# hello: main.c
# 	gcc -o hello main.c


# GCC = gcc
# TARGET = build
# OBJ = main.o

# GCCFLAG = -c -Wall

# $(TARGET): $(OBJ)
# 	$(GCC) -o $@ $^

# %.o: %.c
# 	$(GCC) $(GCCFLAG) $< -o $@


GCC = gcc 
TARGET = build 
SRC = $(wildcard *.c)
OBJ = $(patsubst %.c, %.o, $(SRC))

GCCFLAG = -c -Wall

$(TARGET): $(OBJ)
	$(GCC) -o $@ $^

%.o: %.c
	$(GCC) $(GCCFLAG) $< -o $@

.PHONY:clean
clean:
	rm -rf *.o $(TARGET)
