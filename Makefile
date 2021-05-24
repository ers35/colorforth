unix:
	make -f build/Makefile

dos:
	make -f build/Makefile.dos patch4dos
	make -f build/Makefile.dos

clean:
	rm -f colorforth

mrproper: clean
	rm -f conf.h
