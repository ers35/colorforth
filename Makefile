default: colorforth

colorforth: Makefile colorforth.c
	gcc -std=c99 -m64 -Os -s -Wall -Wextra -pedantic \
	-Wno-missing-braces -Wno-missing-field-initializers -Wno-unused-parameter \
	-o colorforth colorforth.c
	@#xcc -std=c99 -Os -Wno-missing-braces -target=XS1-L16A-128-QF124-C10 colorforth.c -o colorforth.xe

dump: colorforth
	 objdump -d -M intel colorforth | less

colorize: colorforth
	lua colorize.lua < lib.fs | less -R

run: colorforth
	@#rlwrap ./colorforth
	@#rlwrap cat lib.fs - | ./colorforth
	./colorforth < lib.fs
	@#rlwrap xsim --trace colorforth.xe < lib.fs

editor: colorforth
	cat lib.fs editor.fs | ./colorforth

clean:
	rm -rf colorforth
