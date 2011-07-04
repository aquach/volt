all: Volt

Volt:
	@mkdir -p Build && cd Build && cmake ..
	@cd Build && $(MAKE)
	
.PHONY: all Volt
