all: out/array out/dynamic_array out/singly_linked_list

.PHONY: test clean
test: out/array out/dynamic_array out/singly_linked_list
	out/array
	out/dynamic_array
	out/singly_linked_list

clean:
	rm -f out/array
	rm -f out/dynamic_array
	rm -f out/singly_linked_list

out:
	mkdir -p out

out/array: out
	cc -o out/array datastructures_and_algorithms/array/array.c

out/dynamic_array: out
	cc -o out/dynamic_array datastructures_and_algorithms/array/dynamic_array.c

out/singly_linked_list: out
	cc -g -o out/singly_linked_list datastructures_and_algorithms/linked_list/singly_linked_list.c