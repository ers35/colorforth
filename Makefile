default: colorforth

lib.fs.h: lib.fs
	xxd -i lib.fs lib.fs.h

colorforth: Makefile colorforth.c lib.fs.h
	gcc -fPIE -std=c99 -Os -Wall -Wextra -pedantic \
	-s -Wl,--build-id=none -Wl,--gc-sections -Wl,-zcommon-page-size=64 -zmax-page-size=4096 \
	-Wl,--no-ld-generated-unwind-info \
	-Wno-missing-braces -Wno-missing-field-initializers -Wno-unused-parameter -Wno-array-bounds \
	-o colorforth colorforth.c
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
	lua colorize.lua < lib.fs | less -R

run: colorforth
	@#rlwrap ./colorforth
	@#rlwrap cat lib.fs - | ./colorforth
	cat lib.fs - | ./colorforth
	@#rlwrap xsim --trace colorforth.xe < lib.fs

editor: colorforth
	stty raw
	cat lib.fs editor.fs - | ./colorforth
	stty -raw

clean:
	rm -rf colorforth
