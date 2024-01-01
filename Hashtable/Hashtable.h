#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

typedef uint64_t (*hashFunction)(const char*, size_t);
typedef struct _hash_table hash_table;

typedef struct entry {
    char *key;
    void *object;
    struct entry *next;
} entry;

typedef struct _hash_table {
    uint32_t size;
    hashFunction hash; //*hash
    entry **elements;
} hash_table;

hash_table *hash_table_create(uint32_t size, hashFunction *hf);
void hash_table_destroy(hash_table *ht);
void hash_table_print(hash_table *ht);
bool hash_table_insert(hash_table *ht, const char *key, void *obj);
void *hash_table_lookup(hash_table *ht, const char *key);
void *hash_table_delete(hash_table *ht, const char *key);

#endif /* HASHTABLE_H */
