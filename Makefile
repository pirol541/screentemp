# ================  CONFIGURATION  ========================
# Program name
NAME = screentemp
# Installation path
DEST = /usr/local/bin
# Compiler and linker
CC = gcc
# Compiler settings
CFLAGS =
# Linker settings
LDFLAGS = -lX11 -lXrandr
# =========================================================


all: clean compile link

clean:
	@printf "\033[1m\033[36m==> Cleaning...\033[0m\n"
	rm -f *.o ${NAME}

compile: ${NAME}.c
	@printf "\033[1m\033[36m==> Compiling source file to object file...\033[0m\n"
	${CC} ${NAME}.c -c ${CFLAGS} -o ${NAME}.o
	@printf "\033[1m\033[32m==> Done.\033[0m\n"

link: compile ${NAME}.o
	@printf "\033[1m\033[36m==> Linking object file with libs...\033[0m\n"
	${CC} ${NAME}.o ${LDFLAGS} -o ${NAME}
	@printf "\033[1m\033[32m==> Done.\033[0m\n"

install: link
	@printf "\033[1m\033[36m==> Installing binary file...\033[0m\n"
	mkdir -p ${DEST}
	cp -f ${NAME} ${DEST}
	@printf "\033[1m\033[32m==> Done.\033[0m\n"

uninstall:
	@printf "\033[1m\033[36m==> Uninstalling binary file...\033[0m\n"
	rm -f ${DEST}/${NAME}
	@printf "\033[1m\033[32m==> Done.\033[0m\n"
