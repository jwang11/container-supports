OBJS := setns clonens unsharens

all: $(OBJS)
	@echo $(OBJS)

setns: setns.c
	gcc $^ -o $@

clonens: clonens.c
	gcc $^ -o $@

unsharens: unsharens.c
	gcc $^ -o $@

.PHONY: clean

clean:
	@rm -f ${OBJS}
