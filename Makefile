myls: myls.c
	gcc -o myls myls.c

clean: clean_all

clean_all:
	rm -f myls

