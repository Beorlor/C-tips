# C Hashtable Implementation

This project is a custom implementation of a `Hashtable` in C. It primarily uses separate chaining to handle collisions but does not implement load factor management or rehashing. The project includes a `static library`, a `dynamic library`, and a `test program` to demonstrate the library's capabilities.

## Features

- **Separate Chaining for Collision Handling**: Efficiently manages collisions by linking entries with the same hash index.
- **Customizable Hash Function**: Users can define their own hash function for the hash table.
- **Cleanup Function Support**: Optional user-defined cleanup function for complex data types, with default `free` behavior.
- **Test Program**: Reads a text file to initialize the hash table and then tests the table by generating random words.

## Building the Project

Run `make` to create a static library (`libhashtable.a`), a dynamic library (`libhashtable.so`), and the test program (`test`).

```bash
make
```

## Usage

### Test Program

The test program initializes the hash table with the content of a text file (specified as the first argument), displays the hash table, generates a number of random words (specified as the second argument), checks if they are in the hash table, and reports how many of the generated words were found in the hash table.

```bash
./test <wordlist_filename> <num_guesses>
```

### Creating a Hash Table

To create a hash table, pass your own hash function to `hash_table_create`. If your objects are more complex than a basic data type, you can use your own cleanup function for objects. Passing `NULL` for the cleanup function will result in the default `free` behavior.

Example:
```c
hash_table *table = hash_table_create(tablesize, custom_hash_function, custom_cleanup_function);
```

## Hashtable API

- `hash_table_create(size, hashFunction, cleanupfunction)`: Create a new hash table.
- `hash_table_insert(table, key, obj)`: Insert a new entry into the hash table.
- `hash_table_lookup(table, key)`: Look up an entry in the hash table.
- `hash_table_delete(table, key)`: Delete an entry from the hash table.
- `hash_table_destroy(table)`: Clean up and free the hash table.
- `hash_table_print(table)`: Print the content of the hash table.

## About Hash Tables

Hash tables are a fundamental data structure in computer science, used for efficiently storing and retrieving data. This implementation handles collisions using separate chaining. In languages like C++, structures such as `std::unordered_map` and `std::unordered_set` utilize similar concepts with additional features like load factor management and rehashing.
