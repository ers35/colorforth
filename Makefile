default: Makefile colorforth.c
	gcc -Wall -Wextra -std=c99 -m64 -Os -s \
	-Wno-missing-braces -Wno-missing-field-initializers -Wno-unused-parameter \
	-o colorforth colorforth.c
	@#xcc -std=c99 -Os -Wno-missing-braces -target=XS1-L16A-128-QF124-C10 colorforth.c -o colorforth.xe
run: default
	@#rlwrap ./colorforth
	rlwrap cat lib.fs - | ./colorforth
	@#./colorforth < test.fs
	@#rlwrap xsim --trace colorforth.xe < lib.fs
clean:
	rm -rf colorforth
