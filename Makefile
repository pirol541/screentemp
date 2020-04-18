NAME = screentemp
DEST = /usr/local/bin

compile: ${NAME}.c
	@printf "\033[1m\033[36m==> Compiling source code...\033[0m\n"
	gcc ${NAME}.c -lX11 -lXrandr -o ${NAME}
	@printf "\033[1m\033[32m==> Done.\033[0m\n"

clean:
	rm -f *.o screentemp

install: compile
	@printf "\033[1m\033[36m==> Installing binary file...\033[0m\n"
	mkdir -p ${DEST}
	cp -f ${NAME} ${DEST}
	@printf "\033[1m\033[32m==> Done.\033[0m\n"

uninstall:
	@printf "\033[1m\033[36m==> Uninstalling binary file...\033[0m\n"
	rm -f ${DEST}/${NAME}
	@printf "\033[1m\033[32m==> Done.\033[0m\n"
