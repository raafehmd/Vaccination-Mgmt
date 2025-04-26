/*
This is the bloom_filter.c file that implements the Bloom Filter structure for our
vaccination records management system.
*/

// importing relevant libraries
#include <stdlib.h>
#include <string.h>
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
BloomFilter *bloom_create(unsigned int size);             // function to create a new bloom filter
void bloom_delete(BloomFilter *filter);                   // function to delete an existing bloom filter
void bloom_insert(BloomFilter *filter, const char *item); // function to insert element into filter
bool bloom_check(BloomFilter *filter, const char *item);  // function to check if element exists in filter
unsigned int hash_1(const char *str, unsigned int size);  // first hash function
unsigned int hash_2(const char *str, unsigned int size);  // second hash function
unsigned int hash_3(const char *str, unsigned int size);  // third hash function

// implementing bloom_create to create a new bloom filter structure
// unsigned is used to ensure there are no negative values and that
// the full range of positive values is available to us
BloomFilter *bloom_create(unsigned int size)
{
   BloomFilter *filter = malloc(sizeof(BloomFilter));
   filter->size = size;
   filter->bits = calloc((size + 7) / 8, sizeof(unsigned char));
   return filter;
}

void bloom_destroy(BloomFilter *filter)
{
   free(filter->bits);
   free(filter);
}

void bloom_insert(BloomFilter *filter, const char *item)
{
   unsigned int h1 = hash_1(item, filter->size);
   unsigned int h2 = hash_2(item, filter->size);

   filter->bits[h1 / 8] |= (1 << (h1 % 8));
   filter->bits[h2 / 8] |= (1 << (h2 % 8));
}

bool bloom_check(BloomFilter *filter, const char *item)
{
   unsigned int h1 = hash_1(item, filter->size);
   unsigned int h2 = hash_2(item, filter->size);

   return (filter->bits[h1 / 8] & (1 << (h1 % 8))) &&
          (filter->bits[h2 / 8] & (1 << (h2 % 8)));
}

unsigned int hash_1(const char *str, unsigned int size)
{
   unsigned int hash = 5381;
   int c;

   while ((c = *str++))
      hash = ((hash << 5) + hash) + c;

   return hash % size;
}

unsigned int hash_2(const char *str, unsigned int size)
{
   unsigned int hash = 0;
   int c;

   while ((c = *str++))
      hash = c + (hash << 6) + (hash << 16) - hash;

   return hash % size;
}