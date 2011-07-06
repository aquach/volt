all: Volt

Volt:
	@mkdir -p Build && cd Build && ../Util/cmake_with_tools.sh ..
	@cd Build && $(MAKE)
	
.PHONY: all Volt
