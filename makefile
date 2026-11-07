all: out/array out/dynamic_array out/singly_linked_list out/stack out/enum_union out/vtable out/runtime_vtable

.PHONY: test clean
test: out/array out/dynamic_array out/singly_linked_list out/stack out/enum_union out/vtable out/runtime_vtable
	out/array
	out/dynamic_array
	out/singly_linked_list
	out/stack
	out/enum_union
	out/vtable
	out/runtime_vtable

clean:
	rm -rf out

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

out/enum_union: out
	cc -o out/enum_union interface/enum_union.c

out/vtable: out
	cc -o out/vtable interface/vtable.c

out/runtime_vtable: out
	cc -o out/runtime_vtable interface/runtime_vtable.c
