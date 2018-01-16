all: libfreebindfree.so

lib%.so: %.c
	gcc48 -shared -o $@ $< -fPIC
