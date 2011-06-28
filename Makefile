LIB = libvolt

BIN_DIR = Build
SRC_DIR = Volt
OBJ_DIR = Obj
TEST_DIR = Test
APP_DIR = Game
CONTRIB_DIR = Contrib

LIB := $(OBJ_DIR)/$(LIB).a

JSON_DIR = $(CONTRIB_DIR)/json
LIB_JSON = $(JSON_DIR)/Obj/libjson.a

LIB_SRCS = $(shell cd $(SRC_DIR) && find . -name '*.cpp')
LIB_OBJS = $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(LIB_SRCS))
LIB_DEPS = $(patsubst %.cpp,$(OBJ_DIR)/%.d,$(LIB_SRCS))

INCLUDE_DIR = -I $(SRC_DIR)
INCLUDE_DIR += -I $(CONTRIB_DIR)
INCLUDE_DIR += -I $(CONTRIB_DIR)/Box2D_v2.1.2/Box2D

CPPFLAGS = -g -Wall -Wno-reorder $(INCLUDE_DIR)

all: dirs test app $(LIB)

install-deps:
	sudo apt-get install libsdl1.2-dev libsdl-image1.2-dev libglew1.5-dev \
	                     libopenal-dev libogg-dev libvorbis-dev

dirs: $(BIN_DIR) $(OBJ_DIR)

$(BIN_DIR) $(OBJ_DIR):
	@mkdir -p $@

lib : $(LIB)

$(LIB): $(LIB_OBJS) $(LIB_JSON)
	ar rcs $@ $^

# Because python is easier than shell seds.
$(LIB_DEPS) : $(OBJ_DIR)/%.d : $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) -MM $(CPPFLAGS) $< > $@
	@./Util/fix_depends.py $@

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) -c $< -o $@

test : $(LIB)
	cd $(TEST_DIR) && $(MAKE)

app : $(LIB)
	cd $(APP_DIR) && $(MAKE)

$(LIB_JSON):
	cd $(JSON_DIR) && $(MAKE)

.PHONY : all clean dirs depends

clean:
	rm -rf $(APP) Obj
	cd $(TEST_DIR) && $(MAKE) clean
	cd $(APP_DIR) && $(MAKE) clean
	cd $(JSON_DIR) && $(MAKE) clean

ifneq "$(MAKECMDGOALS)" "clean"
-include $(LIB_DEPS)
endif
