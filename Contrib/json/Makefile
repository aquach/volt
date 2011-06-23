LIB = libjson

SRC_DIR = .
OBJ_DIR = Obj

LIB := $(OBJ_DIR)/$(LIB).a

LIB_SRCS = $(shell cd $(SRC_DIR) && find . -name '*.cpp')
LIB_OBJS = $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(LIB_SRCS))
LIB_DEPS = $(patsubst %.cpp,$(OBJ_DIR)/%.d,$(LIB_SRCS))

INCLUDE_DIR = -I$(SRC_DIR)/../

CPPFLAGS = -g -Wall -Wno-reorder $(INCLUDE_DIR)

all: dirs $(LIB)

dirs: $(OBJ_DIR)

$(OBJ_DIR):
	@mkdir -p $@

lib : $(LIB)

$(LIB): $(LIB_OBJS)
	ar rcs $@ $^

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) -c $< -o $@

.PHONY : all clean dirs

clean:
	rm -rf $(LIB) Obj
