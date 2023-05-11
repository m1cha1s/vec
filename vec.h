/*
MIT License

Copyright (c) 2023 m1cha1s

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef VEC_H
#define VEC_H

#ifndef NO_STD
#include <stdlib.h>
#endif

typedef struct {
    void* (*malloc)(unsigned long long);
    void (*free)(void*);
    
    unsigned int capacity; // The bytes available in the current buffer
    unsigned int count; // The bytes used by the current buffer
    unsigned int growth_factor;
    void** data;
} vec;

vec* vec_new(void*(*vec_malloc)(unsigned long long), void(*vec_free)(void*), unsigned int growth_factor) {
    vec* v = (vec *)vec_malloc(sizeof(vec));
    if (!v) 
        return (vec*)0;
    
    v->malloc = vec_malloc;
    v->free = vec_free;
    v->growth_factor = growth_factor;
    v->data = (void**)0;
    v->capacity = 0;
    v->count = 0;
    
    return v;
}

int vec_grow(vec* v) {
    if (v->capacity == 0) {
        v->capacity = 8;
        v->data = (void**)v->malloc(v->capacity*sizeof(void*));
        return 0;
    }

    v->capacity*=v->growth_factor;
    
    void** new_data = (void**)v->malloc(v->capacity*sizeof(void*));
    if (!new_data)
        return -1;

    for (unsigned int i = 0; i < v->count-1; i++)
        new_data[i] = v->data[i];

    v->free(v->data);
    v->data = new_data;

    return 0;
}

int vec_try_shrink(vec* v) {
    if (v->count > v->capacity)
        return -2; // There is no need to shrink the vec

    unsigned int new_capacity = v->capacity/v->growth_factor;
    void** new_data = (void**)v->malloc(new_capacity*sizeof(void*));
    if (!new_data)
        return -1;

    for (int i = 0; i < v->count; i++)
        new_data[i] = v->data[i];

    v->free(v->data);
    v->data = new_data;
    v->capacity = new_capacity;

    return 0;
}

int vec_add(vec* v, void* item) {
    v->count++;
    if (v->capacity < v->count)
        if (vec_grow(v)) {
            v->count--;
            return -1;
        }

    v->data[v->count-1] = item;

    return 0;
}

void* vec_get(vec* v, unsigned int idx) {
    if (idx+1 > v->count)
        return (void*)0;

    return v->data[idx];
}

unsigned int vec_len(vec* v) {
    return v->count;
}

void* vec_remove(vec* v, unsigned int idx) {
    if (idx+1 > v->count)
        return (void*)0;

    void* val = v->data[idx];

    v->count--;

    for (int i = idx; i < v->count; i++)
        v->data[i] = v->data[i+1];

    // This may fail byt we don't care :)
    vec_try_shrink(v);

    return val;
}

void vec_clear(vec* v, void(*item_free)(void*)) {
    for (int i = 0; i < v->count; i++)
        item_free(v->data[i]);
}


void vec_free(vec* v) {
    v->free(v->data);
    v->free(v);
}

void vec_clear_free(vec*v, void(*item_free)(void*)) {
    vec_clear(v, item_free);
    vec_free(v);
}

#ifndef NO_STD

vec* vec_new_default() {
    return vec_new(malloc, free, 8);
}

void vec_clear_default(vec* v) {
    for (int i = 0; i < v->count; i++)
        free(v->data[i]);
}

void vec_clear_free_default(vec*v) {
    vec_clear_default(v);
    vec_free(v);
}

#endif // NO_STD

#endif //VEC_H