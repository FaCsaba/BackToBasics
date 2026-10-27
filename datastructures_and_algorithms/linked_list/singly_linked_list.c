#include <assert.h>
#include <stdlib.h>

struct list_node_int {
    int value;
    struct list_node_int* next;
};

struct list_int {
    struct list_node_int* first;
};

struct list_node_int* list_node_create(int value) {
    struct list_node_int* list = malloc(sizeof(struct list_node_int));
    assert(list && "Failed to allocate enough memory during list_node_create");
    *list = (struct list_node_int) { .value = value, .next = 0 };
    return list;
}

void list_node_free(struct list_node_int* node) {
    assert(node && "Tried to free nullptr list_node");
    free(node);
}

void list_free(struct list_int* list) {
    assert(list && "List must be set");
    struct list_node_int* node = list->first;
    while (node) {
        struct list_node_int* free_node = node;
        node = node->next;
        list_node_free(free_node);
    }
    list->first = 0;
}

void list_add(struct list_int* list, int value) {
    assert(list && "List must be set");
    // Find the last node_list_item
    struct list_node_int* node = list->first;
    while (node && node->next) node = node->next;
    if (node) node->next = list_node_create(value);
    else list->first = list_node_create(value);
}

void list_remove(struct list_int* list, int index) {
    assert(list && "List must be set");
    struct list_node_int* prev = 0;
    struct list_node_int* node = list->first;
    int i = 0;
    while (i < index && node) {
        prev = node;
        node = node->next;
        i += 1;
    }
    assert(i == index && "Tried to access index out of bounds");
    if (prev == 0) list->first = node->next;
    else prev->next = node->next;
    if (node) list_node_free(node);
}

int list_len(struct list_int list) {
    int i = 0;
    struct list_node_int* node = list.first;
    while (node) {
        node = node->next;
        i += 1;
    }
    return i;
}

int main(void) {
    struct list_int list = {0};
    list_add(&list, 69);
    list_add(&list, 67);
    list_add(&list, 420);
    assert(list_len(list) == 3);

    list_remove(&list, 1);
    assert(list_len(list) == 2);
    list_free(&list);

    return 0;
}