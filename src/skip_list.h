#ifndef SKIP_LIST_H
#define SKIP_LIST_H

// defining the Node structure for our skip list
typedef struct Node
{
   // data fields for a citizen's vaccination record
   char *citizen_id; // key for sorting
   char *first_name;
   char *last_name;
   char *country;
   int age;
   char *virus_name;
   char *vaccinated;
   char *date;
   struct Node **next;
} Node;

// defining the structure for our SkipList
typedef struct
{
   int max_level; // maximum number of levels allowed
   int level;     // current highest level
   Node *head;    // a pointer to the head node
} SkipList;

/*
function prototypes
*/
SkipList *list_create(int max_level); // function to create a new skip list
void list_delete(SkipList *list);     // function to delete an existing skip list
Node *list_insert(SkipList *list, char *citizen_id, char *first_name,
                  char *last_name, char *country, int age,
                  char *virus_name, char *vaccinated, char *date); // function to insert a new node (record)
Node *list_search(SkipList *list, char *citizen_id);               // function to search through the skip list
void list_print(SkipList *list);                                   // function to print the skip list
int random_level(int max_level);                                   // function to get a random level to start search

#endif