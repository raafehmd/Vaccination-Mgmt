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
BloomFilter *bloom_create(unsigned int size);               // function to create a new bloom filter
void bloom_delete(BloomFilter *filter);                     // function to delete an existing bloom filter
void bloom_insert(BloomFilter *filter, const char *record); // function to insert record into filter
bool bloom_check(BloomFilter *filter, const char *record);  // function to check if record exists in filter
unsigned int hash_1(const char *str, unsigned int size);    // first hash function
unsigned int hash_2(const char *str, unsigned int size);    // second hash function
unsigned int hash_3(const char *str, unsigned int size);    // third hash function

// implementing bloom_create(...) to create a new bloom filter structure
// unsigned is used to ensure there are no negative values and that
// the full range of positive values is available to us
BloomFilter *bloom_create(unsigned int size)
{
   BloomFilter *filter = malloc(sizeof(BloomFilter));

   // checking if memory was allocated successfully
   if (filter == NULL)
   {
      printf("Error while creating bloom filter");
      return -1;
   }

   filter->size = size;

   // using calloc() instead of malloc() to ensure all bits are set to 0 initially
   // (size + 7) rounds up and ensures we always have enough bytes to store the bits
   filter->bits = calloc((size + 7) / 8, sizeof(unsigned char));

   return filter;
}

// implementing bloom_delete(...) to delete the filter and free any allocated memory
void bloom_delete(BloomFilter *filter)
{
   free(filter->bits);
   free(filter);
}

// implementing bloom_insert(...) to add a new record to the bit array
void bloom_insert(BloomFilter *filter, const char *record)
{
   // running the hash functions on the new record to get bit positions
   unsigned int h1 = hash_1(record, filter->size);
   unsigned int h2 = hash_2(record, filter->size);
   unsigned int h3 = hash_3(record, filter->size);

   /*
   the code below sets the bits in the respective hash positions to 1
   (h / 8) tells us which byte our hash position belongs to
   (h % 8) tells us which bit to set within that byte
   */

   // setting the first hash position to 1
   filter->bits[h1 / 8] |= (1 << (h1 % 8));

   // setting second hash position to 1
   filter->bits[h2 / 8] |= (1 << (h2 % 8));

   // setting third hash position to 1
   filter->bits[h3 / 8] |= (1 << (h3 % 8));
}

// implementing bloom_check(...) to check if record exists
bool bloom_check(BloomFilter *filter, const char *record)
{
   // running the hash functions on the new record to get bit positions
   unsigned int h1 = hash_1(record, filter->size);
   unsigned int h2 = hash_2(record, filter->size);
   unsigned int h3 = hash_3(record, filter->size);

   // performing bitwise & operation to check if the bit at the respective hash position is set
   // then performing && to check if all the hash bits are set
   return ((filter->bits[h1 / 8] & (1 << (h1 % 8))) &&
           (filter->bits[h2 / 8] & (1 << (h2 % 8))) &&
           (filter->bits[h3 / 8] & (1 << (h3 % 8))));
}

// implementing the first hash function
unsigned int hash_1(const char *str, unsigned int size)
{
   unsigned int hash = 5381; // initial seed value
   int c;                    // int to store ascii value of each character of the record

   // continues until end of file is encountered
   // c stores ascii value of current character
   while ((c = *str++))
   {
      hash = ((hash << 5) + hash) + c; // same as (hash * 33 + c), faster with bit operations
   }

   // finally, we perform the (% size) operation to ensure the hash is within bounds
   return hash % size;
}

// implementing the second hash function
unsigned int hash_2(const char *str, unsigned int size)
{
   unsigned int hash = 0; // initial seed value
   int c;                 // int to store ascii value of current character

   while ((c = *str++))
   {
      hash = c + (hash << 6) + (hash << 16) - hash; // same as (c + hash * 64 + hash * 65536 - hash)
   }
   return hash % size; // ensuring hash value is within bounds
}

// implementing the third hash function
unsigned int hash_3(const char *str, unsigned int size)
{
   unsigned int hash = 0; // initial seed value
   int c;                 // int to store ascii value of current character

   while ((c = *str++))
   {
      hash = (hash * 31) + c; // multiplying with a prime number
   }

   return hash % size; // ensuring hash value is within bounds
}