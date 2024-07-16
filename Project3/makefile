G = gcc -g
O = -o
names = main liquidline pillline manager opengl
thread_args = -lpthread -lrt

files:
	$(G) manager.c $(O) manager %(thread_args)
	$(G) main.c $(O) main %(thread_args)
	$(G) liquidline.c $(O) liquidline %(thread_args)
	$(G) pillline.c $(O) pillline %(thread_args)
	$(G) timer.c $(O) timer %(thread_args)
	$(G) opengl.c $(O) opengl %(thread_args)

clean:
	rm -f $(names)