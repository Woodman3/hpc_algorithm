CPPFLAGS = -std=c++20 -g -Ofast -fopenmp
CXX=g++

run: clean main
	yhrun -n 1 -N 1 -c 16 -p thcp3 ./main

sort:sort.o
	$(CXX) $(CPPFLAGS) -o $@ $^

main:main.o
	$(CXX) $(CPPFLAGS) -o $@ $^

clean:
	rm -f *.o main sort

env:
	export OMP_NUM_THREADS=16
	module load proxy GCC/12.2.0