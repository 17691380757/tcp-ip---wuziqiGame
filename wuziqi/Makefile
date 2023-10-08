# 给系统预定义变量进行赋值
CC := g++
CFLAGS := -c -g -Wall
RM := rm -rf

# 自定义变量
SRC_DIR := ./src1/
OBJ_DIR := ./obj1/
BIN_DIR := ./bin/
SRCS := kehu.cpp fuwuqi.cpp
OBJS := $(patsubst %.cpp, %.o, $(SRCS))
TARGETS := $(patsubst %.cpp, %, $(SRCS))
BINARIES := $(addprefix $(BIN_DIR), $(TARGETS))

# 将以上变量应用于整个工程
export CC CFLAGS RM OBJS TARGETS BINARIES

# Makefile来分别管理src和obj目录下的makefile
all: $(TARGETS)

$(TARGETS): $(OBJS)
	$(CC) $(addprefix $(OBJ_DIR), $@.o) -o $(BIN_DIR)$@

$(OBJS): $(addprefix $(SRC_DIR), $(SRCS))
	$(CC) $(CFLAGS) $< -o $(OBJ_DIR)$@

.PHONY: clean
clean:
	$(RM) $(OBJ_DIR)*.o
	$(RM) $(BIN_DIR)*
