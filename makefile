CPPFLAGS=-std=c++11 -O

go: luts sim
	./sim

luts: make_luts
	./make_luts >luts.h

make_luts: make_luts.cpp

sim:	sim.cpp

clean:
	rm -f *~ luts.h make_luts sim
