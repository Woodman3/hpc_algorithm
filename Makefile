CPPFLAGS = -std=c++20 -g
CXX=g++

run: clean main
	./main 

sort:sort.o
	$(CXX) $(CPPFLAGS) -o $@ $^

main:main.o
	$(CXX) $(CPPFLAGS) -o $@ $^

clean:
	rm -f *.o main sort