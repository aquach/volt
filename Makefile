all: Volt Build/game Build/editor

Volt:
	@mkdir -p Build && cd Build && cmake ..
	@cd Build && $(MAKE)

Build/game:
	@rm Build/game && ln -s Game/game Build/game

Build/editor:
	@rm Build/editor && ln -s Editor/editor Build/editor
	
.PHONY: all Volt
