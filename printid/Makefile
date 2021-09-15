OBJS := printid setpgid

all: $(OBJS)
	@echo $(OBJS)

printid: printid.c
	gcc $^ -o $@

setpgid: setpgid.c
	gcc $^ -o $@

.PHONY: clean

clean:
	@rm -f ${OBJS}
