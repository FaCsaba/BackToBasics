all: out/array out/dynamic_array

.PHONY: test clean
test: clean out/array out/dynamic_array
	out/array
	out/dynamic_array

clean:
	rm -f out/array
	rm -f out/dynamic_array

out:
	mkdir out

out/array: out
	cc -o out/array datastructures_and_algorithms/array/array.c

out/dynamic_array: out
	cc -o out/dynamic_array datastructures_and_algorithms/array/dynamic_array.c
