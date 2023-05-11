# VEC
This is a small stb style dynamic array library by m1cha1s.

## Usage
Copy the [vec.h](vec.h) into your project. The api is:

### Error codes
Error codes:
- -1 => Alloc failed
- -2 => Operation not necessary

### Methods always available

#### `vec* vec_new(alloc vec_alloc, unsigned int growth_factor);`
Create new vec with custom allocator and growth factor

#### `int vec_grow(vec* v);`
Grow the vec.

#### `int vec_try_shrink(vec* v);`
Try to shrink the vec

#### `int vec_add(vec* v, void* item);`
Add item to vec

#### `void* vec_get(vec* v, unsigned int idx);`
Get item from vec

#### `unsigned int vec_len(vec* v);`
Get vec lenght

#### `void* vec_remove(vec* v, unsigned int idx);`
Remove item from vec

#### `void vec_clear(vec* v, alloc item_alloc);`
Free items with custom allocator

#### `void vec_free(vec* v);`
Free the vec itself(not its items)

#### `void vec_clear_free(vec*v, alloc item_alloc);`
Free everything(also items inside)


### Methods NOT available with NO_STD definition defined

#### `vec* vec_new_default();`
Same as vec_new but uses malloc and free from stdlib

#### `void vec_clear_default(vec* v);`
Same as vec_clear but uses and free from stdlib

#### `void vec_clear_free_default(vec*v);`
Same as vec_clear_free but uses free from stdlib
