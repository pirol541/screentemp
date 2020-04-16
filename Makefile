compile: screentemp.c
	@printf "\033[1m\033[36m==> Compiling source code...\033[0m\n"
	gcc screentemp.c -lX11 -lXrandr -o ~/.local/bin/screentemp

