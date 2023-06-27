include ../cc.mk

.PHONY: clean

utils.o: utils.c utils.h
-include utils.d # ignores if not found

clean: 
	$(RM) utils.o
	$(RM) utils.d
