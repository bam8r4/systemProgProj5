all: mast work

mast: master.c
	gcc master.c -o mast

work: worker.c
	gcc worker.c -o work

clean:
	rm mast
	rm work

