all: Build/Makefile Volt

Build/Makefile: CMakeLists.txt
	mkdir -p Build && cd Build && cmake ..
	
Volt: Build/Makefile
	cd Build && $(MAKE)
	
.PHONY: all
