all: capsettest

capsettest: capsettest.c
	gcc $^ -o $@ -lcap 

.PHONY: clean

clean:
	@rm -f capsettest
