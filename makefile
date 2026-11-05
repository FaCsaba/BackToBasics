all: out/array out/dynamic_array out/singly_linked_list out/stack

.PHONY: test clean
test: out/array out/dynamic_array out/singly_linked_list out/stack
	out/array
	out/dynamic_array
	out/singly_linked_list
	out/stack

clean:
	rm -f out/array
	rm -f out/dynamic_array
	rm -f out/singly_linked_list
	rm -f out/stack

out:
	mkdir -p out

out/array: out
	cc -o out/array datastructures_and_algorithms/array/array.c

out/dynamic_array: out
	cc -o out/dynamic_array datastructures_and_algorithms/array/dynamic_array.c

out/singly_linked_list: out
	cc -o out/singly_linked_list datastructures_and_algorithms/linked_list/singly_linked_list.c

out/stack: out
	cc -o out/stack datastructures_and_algorithms/stack/stack.c
