CPPFLAGS = -std=c++20
CXX=g++

run:sort
	./$^

sort:sort.o
	$(CXX) $(CPPFLAGS) -o $@ $^

