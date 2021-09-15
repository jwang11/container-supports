OBJS := ban ban2

all: $(OBJS)
	@echo $(OBJS)

ban: ban.c
	gcc $^ -o $@ -lseccomp

ban2: ban2.c
	gcc $^ -o $@ -lseccomp

.PHONY: clean

clean:
	@rm -f ${OBJS}
