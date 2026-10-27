#include <assert.h>

int ARRAY[] = { 67, 69, 420, 1337 };
int ARRAY_LEN = sizeof(ARRAY)/sizeof(*ARRAY);

// Get operation on an array returns the element indicated by index
int get(int *array, int index) {
    // Arrays are just pointers to a chunk of memory
    // using the bracket syntex means the same as:
    // `array + index * sizeof(*array)`
    return array[index];
}

// Set operation on an array changes the element at the specified index
void set(int *array, int index, int value) {
    // Same as if taking dereferencing the array at the correct memory address
    // and storing value there:
    // `*(array + index * sizeof(*array)) = value`
    array[index] = value;
}

// Returns the index of value or -1 if not found
int find(int *array, int array_len, int value) {
    for (int i = 0; i < array_len; i += 1) {
        if (array[i] == value) return i;
    }
    return -1;
}

int main(void) {
    // Some operations on an array
    int a = get(ARRAY, 0);
    assert(a == 67);
    
    set(ARRAY, 1, 70);
    int b = get(ARRAY, 1);
    assert(b == 70);

    int i = find(ARRAY, ARRAY_LEN, 420);
    assert(i == 2);

    return 0;
}