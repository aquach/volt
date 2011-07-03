all: Build/Makefile Volt Build/game Build/editor

Build/Makefile: CMakeLists.txt
	@mkdir -p Build && cd Build && cmake ..
	
Volt: Build/Makefile
	@cd Build && $(MAKE)

Build/game:
	@rm Build/game && ln -s Game/game Build/game

Build/editor:
	@rm Build/editor && ln -s Editor/editor Build/editor
	
.PHONY: all Volt
