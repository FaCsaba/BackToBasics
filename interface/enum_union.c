#include <assert.h>
#include <stdio.h>

typedef enum {
    ScratcherTypeCat,
    ScratcherTypeDog,
    ScratcherTypeCow,
} ScratcherType;


typedef struct {
    ScratcherType type;
    union {
        struct { char* cat_name; } cat;
        struct { char* dog_name; } dog;
        struct { char* cow_name; } cow;
    } as;
} Scratcher;

void scratch(Scratcher* scr) {
    switch (scr->type) {
    case ScratcherTypeCat: {
        printf("Scratched a cat named: %s\n", scr->as.cat.cat_name);
        return;
    }
    case ScratcherTypeDog: {
        printf("Scratched a dog named: %s\n", scr->as.dog.dog_name);
        return;
    }
    case ScratcherTypeCow: {
        printf("Scratched a cow named: %s\n", scr->as.cow.cow_name);
        return;
    }
    default: assert(0 && "scratch doesn't cover enum cases");
    }
}

int main(void) {
    Scratcher dg_scr = (Scratcher){.type = ScratcherTypeDog, .as = { .dog = { "Bruno" } } };
    Scratcher ct_scr = (Scratcher){ .type = ScratcherTypeCat, .as = { .cat = { "Milan" } } };

    scratch(&dg_scr);
    scratch(&ct_scr);
    return 0;
}