#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef void (*print_fn)(void* this);
typedef void (*free_fn)(void* this);
typedef struct {
    print_fn print;
    free_fn free;
} ObjectVTable;

typedef struct {
    ObjectVTable* vtable;
} Object;

void object_print(Object *this) {
    this->vtable->print(this);
}

void object_free(Object *this) {
    this->vtable->free(this);
}

typedef struct {
    Object base;
    int value;
} Int;

ObjectVTable IntVTable;
Int* int_new(int value) {
    Int* i = calloc(1, sizeof(Int));
    i->base.vtable = &IntVTable;
    i->value = value;
    return i;
}

void int_print(Int* this) {
    printf("%d", this->value);
}

ObjectVTable IntVTable = { .print = (print_fn) int_print, .free = free };

typedef struct {
    Object base;
    int value;
} Char;

ObjectVTable CharVTable;
Char* char_new(char value) {
    Char* i = calloc(1, sizeof(Char));
    i->base.vtable = &CharVTable;
    i->value = value;
    return i;
}

void char_print(Char* this) {
    printf("%c", this->value);
}

ObjectVTable CharVTable = { .print = (print_fn) char_print, .free = free };

typedef struct {
    Object base;
    Object** ptr;
    int len;
    int cap;
} Array;

#define MINIMUM_ARRAY_LEN 1024

void array_realloc(Array *array, int cap) {
    array->ptr = realloc(array->ptr, sizeof(Object*) * cap);
    assert(array->ptr && "Failed to reallocate array");
    array->cap = cap;
}

ObjectVTable ArrayVTable;
Array* array_new(int cap) {
    Array* array = calloc(1, sizeof(Array));
    array->base.vtable = &ArrayVTable;
    
    array_realloc(array, cap);
    return array;
}

void array_print(Array* this) {
    printf("[");
    for (int i = 0; i < this->len; i += 1) {
        if (i != 0) printf(", ");
        object_print(this->ptr[i]);
    }
    printf("]");
}

void array_free(Array* this) {
    for (int i = 0; i < this->len; i += 1) {
        object_free(this->ptr[i]);
    }
    free(this);
}

ObjectVTable ArrayVTable = { .print = (print_fn) array_print, .free = (free_fn) array_free };

void array_push(Array* array, Object* obj) {
    if (array->cap < array->len + 1) {
        int new_cap = array->cap;
        while (new_cap < array->len + 1) {
            new_cap = new_cap ? new_cap * 2 : MINIMUM_ARRAY_LEN;
        }
        array_realloc(array, new_cap);
    }

    array->ptr[array->len] = obj;
    array->len += 1;
}

typedef struct {
    Object base;
    char* str;
    int len;
} String;

ObjectVTable StringVTable;
String* string_copy(char* str) {
    String* string = calloc(1, sizeof(String));
    string->base.vtable = &StringVTable;
    
    string->len = strlen(str);
    string->str = malloc(string->len);
    assert(string->str && "Failed to reallocate array");

    strcpy(string->str, str);
    
    return string;
}

void string_print(String* this) {
    printf("%.*s", this->len, this->str);
}

void string_free(String* this) {
    free(this->str);
    free(this);
}

ObjectVTable StringVTable = { .print = (print_fn) string_print, .free = (free_fn) string_free };

typedef void (*speak_fn)(void* this);

typedef struct {
    ObjectVTable v;
    speak_fn speak;
} AnimalVTable;

typedef struct {
    AnimalVTable* vtable;
    String* name;
} Animal;

void animal_speak(Animal* this) {
    this->vtable->speak(this);
}

void animal_print(Animal* this) {
    this->vtable->v.print(this);
}

void animal_free(Animal* this) {
    this->vtable->v.free(this);
}

void animal__print(Animal* this) {
    printf("Animal named: ");
    string_print(this->name);
}

void animal__free(Animal* this) {
    string_free(this->name);
}

typedef struct {
    Animal base;
    int isBigBoy;
} Dog;

AnimalVTable DogVTable;
Dog* dog_new(String* name, int isBigBoy) {
    Dog* dog = calloc(1, sizeof(Dog));
    dog->base.vtable = &DogVTable;
    dog->base.name = name;
    dog->isBigBoy = isBigBoy;
    return dog;
}

void dog_speak(Dog* this) {
    string_print(this->base.name);
    if (this->isBigBoy) {
        printf(" says: 'boof'");
    } else {
        printf(" says: 'woof'");
    }
}

void dog_free(Dog* this) {
    animal__free((Animal*)this);
    free(this);
}
AnimalVTable DogVTable = { .v = { .print = (print_fn) animal__print, .free = (free_fn) dog_free }, .speak = (speak_fn)dog_speak };

typedef struct {
    Animal base;
} Cat;

AnimalVTable CatVTable;
Cat* cat_new(String* name) {
    Cat* cat = calloc(1, sizeof(Cat));
    cat->base.vtable = &CatVTable;
    cat->base.name = name;
    return cat;
}

void cat_speak(Cat* this) {
    string_print(this->base.name);
    printf(" says: 'meow'");
}

void cat_free(Cat* this) {
    animal__free((Animal*)this);
    free(this);
}
AnimalVTable CatVTable = { .v = { .print = (print_fn) animal__print, .free = (free_fn) cat_free }, .speak = (speak_fn) cat_speak };


int main(void) {
    Array* objs = array_new(0);    
    array_push(objs, &string_copy("Hello, World!")->base);
    array_push(objs, &char_new('A')->base);
    array_push(objs, &int_new(69)->base);
    array_push(objs, &array_new(1024)->base);
    Animal* dog = &dog_new(string_copy("Lucy"), 1)->base;
    Animal* cat = &cat_new(string_copy("Bexter"))->base;
    animal_speak(dog);
    printf("\n");
    animal_speak(cat);
    printf("\n");

    
    array_push(objs, (Object*)dog);
    array_push(objs, (Object*)cat);

    array_print(objs);
    printf("\n");

    object_free((Object*)objs);

    return 0;
}