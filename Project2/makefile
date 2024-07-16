G = gcc -g
O = -o
names = main plane collectingworker splittingworker distributionworker families soldier containers timer

files:
	$(G) main.c $(O) main
	$(G) plane.c $(O) plane
	$(G) collectingworker.c $(O) collectingworker
	$(G) splittingworker.c $(O) splittingworker
	$(G) distributionworker.c $(O) distributionworker
	$(G) families.c $(O) families
	$(G) soldier.c $(O) soldier
	$(G) containers.c $(O) containers
	$(G) timer.c $(O) timer

clean:
	rm -f $(names)