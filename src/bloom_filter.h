#ifndef BLOOM_FILTER_H
#define BLOOM_FILTER_H

#include <stdbool.h>

// defining bloom filter structure
typedef struct
{
   unsigned char *bits; // the bit array
   unsigned int size;   // the size of the array
} BloomFilter;

/*
function prototypes
*/
BloomFilter *bloom_create(unsigned int size);               // function to create a new bloom filter
void bloom_delete(BloomFilter *filter);                     // function to delete an existing bloom filter
void bloom_insert(BloomFilter *filter, const char *record); // function to insert record into filter
bool bloom_check(BloomFilter *filter, const char *record);  // function to check if record exists in filter
unsigned int hash_1(const char *str, unsigned int size);    // first hash function
unsigned int hash_2(const char *str, unsigned int size);    // second hash function
unsigned int hash_3(const char *str, unsigned int size);    // third hash function

#endif