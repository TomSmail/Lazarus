CC = gcc
CFLAGS  ?= -std=c11 -g\
	-D_POSIX_SOURCE -D_DEFAULT_SOURCE\
	-Wall -Werror -Wextra -Wstrict-prototypes -pedantic\
	-MD
