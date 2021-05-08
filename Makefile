default: colorforth

SRC=main.c colorforth.c \
	extensions/os-utils.c \
	extensions/dict-utils.c \
	extensions/io-utils.c
SRC_H=colorforth.h extensions/lib.cf.h

EMBED_LIB=-D__EMBED_LIB
ECHO_COLOR=-D__ECHO_COLOR

EXTRA_CFLAGS=-I. -Iextensions \
	-Wl,--build-id=none -Wl,--gc-sections -Wl,-zcommon-page-size=64 -zmax-page-size=4096 \
	$(EMBED_LIB) $(ECHO_COLOR)



extensions/lib.cf.h: forth/lib.cf
	xxd -i forth/lib.cf extensions/lib.cf.h

colorforth: Makefile $(SRC) $(SRC_H)
	gcc -fPIE -std=c99 -Os -Wall -Werror -Wextra -pedantic \
	-s -Wno-missing-braces -Wno-missing-field-initializers -Wno-unused-parameter \
	$(EXTRA_CFLAGS) \
	-o colorforth $(SRC)

optimize: colorforth
	@#readelf -l colorforth
	@# required: .gnu.version .gnu.version_r
	strip --strip-all colorforth
	strip -R.comment -R.note -R.eh_frame_hdr -R.eh_frame -R.note.ABI-tag -R.jcr \
	colorforth
	objcopy -I elf64-x86-64 -O elf64-x86-64 --section-alignment 512 colorforth colorforth
	@#xcc -std=c99 -Os -Wno-missing-braces -target=XS1-L16A-128-QF124-C10 colorforth.c -o colorforth.xe

dump: colorforth
	 objdump -d -M intel colorforth | less

dumpelf: colorforth
	readelf -a colorforth | less

colorize: colorforth
	cat forth/colorize.cf forth/lib.cf | ./colorforth | less -R

run: colorforth
	@#rlwrap ./colorforth
	@#rlwrap cat lib.cf - | ./colorforth
	cat forth/lib.cf - | ./colorforth
	@#rlwrap xsim --trace colorforth.xe < lib.cf

clean:
	rm -rf colorforth
