#include <assert.h>
#include <stdlib.h>

struct array_int {
    int* ptr;
    int  len;
    int  cap;
};

#define ARRAY_INIT_LEN 8

void array_resize(struct array_int* array, int cap) {
    array->ptr = realloc(array->ptr, cap);
    assert(array->ptr != 0 && "Failed to allocate enough memory during array_resize");
    array->cap = cap;
}

void array_add(struct array_int* array, int value) {
    while (array->cap < array->len + 1) {
        int cap = cap == 0 ? ARRAY_INIT_LEN : cap * 2;
        array_resize(array, cap);
    }

    array->ptr[array->len] = value;
    array->len += 1;
}

void array_remove(struct array_int* array, int index) {
    assert(index < array->len && "Tried to remove index out of bounds");

    array->len = array->len - 1;

    for (int i = index; i < array->len; i += 1) {
        array->ptr[i] = array->ptr[i+1];
    }
}

int array_get(struct array_int array, int index) {
    assert(index < array.len && "Tried to access index out of bounds");
    return array.ptr[index];
}

void array_set(struct array_int* array, int index, int value) {
    assert(index < array->len && "Tried to access index out of bounds");
    array->ptr[index] = value;
}

int main(void) {
    struct array_int array = {0};
    array_add(&array, 69);
    array_add(&array, 67);
    array_add(&array, 420);
    assert(array.len == 3);

    array_remove(&array, 1);
    assert(array.len == 2);
    assert(array_get(array, 1) == 420);
}