OBJS := socketpair socketpair2

all: $(OBJS)
	@echo $(OBJS)

socketpair: socketpair.c
	gcc $^ -o $@

socketpair2: socketpair2.c
	gcc $^ -o $@

.PHONY: clean

clean:
	@rm -f ${OBJS}
