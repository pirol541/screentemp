compile: screentemp.c
	@printf "\033[1m\033[36m==> Compiling source code...\033[0m\n"
	gcc screentemp.c -lX11 -lXrandr -o screentemp

clean:
	rm -f *.o screentemp

install: compile
	@printf "\033[1m\033[36m==> Installing binary file...\033[0m\n"
	mkdir -p /usr/local/bin
	cp -f screentemp /usr/local/bin
	@printf "\033[1m\033[32m==> Finished.\033[0m\n"
