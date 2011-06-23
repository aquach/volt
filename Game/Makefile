APP = game

BIN_DIR = ../Build
SRC_DIR = .
OBJ_DIR = Obj
CONTRIB_DIR = ../Contrib

LIBVOLT = ../Obj/libvolt.a

APP := $(BIN_DIR)/$(APP)

APP_SRCS = $(shell cd $(SRC_DIR) && find . -name '*.cpp')

INCLUDE_DIR = -I $(SRC_DIR)
INCLUDE_DIR += -I ../
INCLUDE_DIR += -I ../Volt
INCLUDE_DIR += -I $(CONTRIB_DIR)/Box2D_v2.1.2/Box2D
INCLUDE_DIR += -I $(CONTRIB_DIR)

CPPFLAGS = -g -Wall -Wno-reorder $(INCLUDE_DIR)
LIBS = -lSDL -lSDL_image -lGL -lGLU -lGLEW -lopenal -logg -lvorbisfile

STATIC_LIBS = $(CONTRIB_DIR)/Box2D_v2.1.2/Box2D/Box2D/libBox2D.a
STATIC_LIBS += $(CONTRIB_DIR)/json/Obj/libjson.a

APP_OBJS = $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(APP_SRCS))
APP_DEPS = $(patsubst %.cpp,$(OBJ_DIR)/%.d,$(APP_SRCS))

all: dirs $(APP)

dirs: $(BIN_DIR) $(OBJ_DIR)

$(BIN_DIR) $(OBJ_DIR):
	@mkdir -p $@

$(APP): $(APP_OBJS) $(LIBVOLT)
	$(CXX) $(CPPFLAGS) -o $(APP) $^ $(LIBVOLT) $(LIBS) $(STATIC_LIBS)

$(APP_DEPS) : $(OBJ_DIR)/%.d : $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) -MM $(CPPFLAGS) $< > $@

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) -c $< -o $@

.PHONY : all clean dirs

clean:
	rm -rf $(APP) Obj

ifneq "$(MAKECMDGOALS)" "clean"
	-include $(APP_DEPS)
endif
