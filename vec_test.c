#include <stdio.h>

#include "vec.h"
#include "alloc/alloc.h"

int main() {
    vec* v = vec_new_default();

    for (int i = 0; i < 10; i ++) {
        int* nya = (int*)std_alloc.malloc(sizeof(int), v->vec_alloc.meta);
        *nya = 69 + i;

        vec_add(v, nya);
    }

    printf("Vec len: %d\n", vec_len(v));

    for (int i = 0 ; i < vec_len(v); i ++) {
        int* nya = (int*)vec_get(v, i);
        printf("Value at %p is %d\n", nya, *nya);
    }

    vec_clear_free_default(v);
    return 0;
}