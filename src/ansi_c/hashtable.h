#ifndef HASHTABLE_H
#define HASHTABLE_H

/* A generic open-addressing hash table. */

#include <stddef.h>

struct hashtable_t;
typedef struct hashtable_t hashtable_t;

typedef int (*hashtable_cmp_fn)(void *a, void *b);
typedef int (*hashtable_hash_fn)(void *p);
typedef void (*hashtable_destroy_fn)(void *p);

hashtable_t *hashtable_create(size_t min_cap, hashtable_hash_fn hash_key,
			      hashtable_cmp_fn cmp_key,
			      hashtable_destroy_fn destroy_key,
			      hashtable_destroy_fn destroy_val);
void hashtable_destroy(hashtable_t *ht);

size_t hashtable_min_cap(hashtable_t *ht);
int hashtable_set_min_cap(hashtable_t *ht, size_t new_min_cap);
size_t hashtable_cap(hashtable_t *ht);
size_t hashtable_len(hashtable_t *ht);

int hashtable_clear(hashtable_t *ht);
int hashtable_contains(hashtable_t *ht, void *key);
void *hashtable_get(hashtable_t *ht, void *key);
int hashtable_remove(hashtable_t *ht, void *key);
int hashtable_set(hashtable_t *ht, void *key, void *value);

#endif /* HASHTABLE_H */
