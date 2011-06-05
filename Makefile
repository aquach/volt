LIB = libvolt

BIN_DIR = Build
SRC_DIR = Volt
OBJ_DIR = Obj
TEST_DIR = Test

LIB := $(OBJ_DIR)/$(LIB).a

LIB_SRCS = $(shell cd $(SRC_DIR) && find . -name '*.cpp')
LIB_OBJS = $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(LIB_SRCS))
LIB_DEPS = $(patsubst %.cpp,$(OBJ_DIR)/%.d,$(LIB_SRCS))

INCLUDE_DIR = -I $(SRC_DIR)
CPPFLAGS = -g -Wall -Wno-reorder $(INCLUDE_DIR)

all: dirs test $(LIB)

dirs: $(BIN_DIR) $(OBJ_DIR)

$(BIN_DIR) $(OBJ_DIR):
	@mkdir -p $@

lib : $(LIB)

$(LIB): $(LIB_OBJS)
	ar rcs $@ $^

$(LIB_DEPS) : $(OBJ_DIR)/%.d : $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) -MM $(CPPFLAGS) $< > $@

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) -c $< -o $@

test : $(LIB)
	cd $(TEST_DIR) && $(MAKE)

.PHONY : all clean dirs

clean:
	rm -rf $(APP) Obj
	cd $(TEST_DIR) && $(MAKE) clean

ifneq "$(MAKECMDGOALS)" "clean"
	-include $(LIB_DEPS)
endif
