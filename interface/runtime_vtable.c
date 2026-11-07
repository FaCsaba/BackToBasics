#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct object {
    void* base;

    void (*print)(void* base);
    void (*free)(void* base);
};

void object_print(struct object obj) {
    if (obj.base == NULL || obj.print == NULL) return;
    obj.print(obj.base);
}

void object_free(struct object obj) {
    if (obj.base == NULL || obj.free == NULL) return;
    obj.free(obj.base);
}

struct string {
    char* ptr;
    int len;
    int cap;
};

struct string string_copy(char* str) {
    int str_len = strlen(str);
    char *ptr = malloc(str_len);
    strcpy(ptr, str);
    return (struct string){ .ptr = ptr, .len = str_len, .cap = str_len };
}

void string_print(struct string* print) {
    printf("\"%s\"", print->ptr);
}

void string_free(struct string* string) {
    if (string->ptr == NULL) return;
    free(string->ptr);
}

struct object string_to_object(struct string* string) {
    return (struct object){ .base = string, .print = (void (*)(void*)) string_print, .free = (void (*)(void*)) string_free };
}

struct character {
    struct string name;
    int health;
    int height;
    struct object metadata;
};

struct character character_new(char* name, int health, int height) {
    return (struct character){ .name = string_copy(name), .health = health, .height = height, .metadata = {0} };
}

void character_print(struct character* character) {
    printf("Character{ name: ");
    string_print(&character->name);
    printf(", health: %d, height: %d, metadata: ", character->health, character->height);
    object_print(character->metadata);
    printf(" }");
}

void character_free(struct character* character) {
    string_free(&character->name);
    object_free(character->metadata);
}

struct object character_to_object(struct character* character) {
    return (struct object){ .base = character, .print = (void (*)(void*)) character_print, .free = (void (*)(void*)) character_free };
}

void intiger_print(int* v) {
    printf("%d", *v);
}

struct object intiger_to_object(int *v) {
    return (struct object){ .base = v, .print = (void (*)(void*))intiger_print, .free = NULL};
}


// Let's play with some objects
int main(void) {
    // We create some objects
    struct string enemy_name = string_copy("Ugly enemy");
    struct character player = character_new("Mr. World Hello", 69, 420);
    struct string player_meta = string_copy("Best Game Everrrr");
    player.metadata = string_to_object(&player_meta);
    int v = 69;

    #define my_objects_len 3
    struct object my_objects[my_objects_len] = {
        string_to_object(&enemy_name),
        intiger_to_object(&v),
        character_to_object(&player),
    };


    for(int i = 0; i < my_objects_len; i += 1) {
        object_print(my_objects[i]);
        printf("\n");
    }


    for(int i = 0; i < my_objects_len; i += 1) {
        object_free(my_objects[i]);
    }
}

