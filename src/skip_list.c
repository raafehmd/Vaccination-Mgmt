/*
This is the skip_list.c file that implements the Skip List structure for our
vaccination records management system.
*/

// importing relevant libraries
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "skip_list.h"

// implementing list_create(...) to create a new skip list
SkipList *list_create(int max_level)
{
   SkipList *list = malloc(sizeof(SkipList));

   // checking if memory was allocated successfully
   if (list == NULL)
   {
      printf("Error while creating skip list");
      return NULL; // exit with failure
   }

   list->max_level = max_level; // setting max_level of skip list
   list->level = 0;             // initializing current highest level to zero

   Node *head = malloc(sizeof(Node));

   // checking if memory was allocated successfully
   if (head == NULL)
   {
      printf("Error while creating head node");
      return NULL; // exit with failure
   }

   head->citizen_id = NULL;                               // setting dummy head-node key to NULL
   head->next = malloc(sizeof(Node *) * (max_level + 1)); // allocating memory for max_level + 1 Node pointers

   // initializing all next pointers to NULL
   for (int i = 0; i <= max_level; i++)
   {
      head->next[i] = NULL;
   }

   list->head = head; // setting the head of the list to the dummy head we just created
   srand(time(NULL)); // seeding random number generator
   return list;
}

// implementing list_delete(...) to destroy a skip_list after use
void list_delete(SkipList *list)
{
   /*
      for destroying the list, since the base level contains all nodes, we
      iterate through every node at the base level and free all allocated memory
   */
   Node *current = list->head->next[0]; // setting current node to the first node (after head) of the base level
   while (current != NULL)
   {
      Node *next_node = current->next[0];

      // freeing all allocated memory
      free(current->citizen_id);
      free(current->first_name);
      free(current->last_name);
      free(current->country);
      free(current->virus_name);
      free(current->vaccinated);
      if (current->date)
         free(current->date);
      free(current->next);
      free(current);

      current = next_node; // iterating to the next node
   }

   // free all remaining memory
   free(list->head->next);
   free(list->head);
   free(list);
}

// implementing random_level(...) to generate a random level
int random_level(int max_level)
{
   int level = 0;

   // incrementing level a random number of times using the rand() function
   while (rand() % 2 && level < max_level)
   {
      level++;
   }

   return level;
}

// implementing list_insert(...) to add a new record (Node)  to the skip list
Node *list_insert(SkipList *list, char *citizen_id, char *first_name,
                  char *last_name, char *country, int age,
                  char *virus_name, char *vaccinated, char *date)
{
   Node *update[list->max_level + 1]; // initializing a Node array of size max_level + 1 to keep track of linking
   Node *current = list->head;        // starting at the head node of the list

   // a for-loop to find where to insert the new node at each level
   for (int i = list->level; i >= 0; i--)
   {
      // moving forward as long as there is a node ahead and its ID is smaller
      while (current->next[i] != NULL &&
             strcmp(current->next[i]->citizen_id, citizen_id) < 0)
      {
         current = current->next[i]; // moving to next node
      }
      /*
         After the for-loop ends, 'current' points to the Node that should exist before
         the new node, i.e., our new node is to be inserted right after the current node
      */
      update[i] = current;
   }

   /*
      After finding the correct position, it could be that a node with the same ID already
      exists. Therefore, we check if the next node has the same ID as our new node.
   */

   // move to next node
   current = current->next[0];

   if (current != NULL && strcmp(current->citizen_id, citizen_id) == 0)
   {
      return NULL; // found duplicate, exit with failure
   }

   // get a random level for the new node
   int new_level = random_level(list->max_level);

   // if the random level for the new node is higher than the
   // current highest level of the list, update the list level
   if (new_level > list->level)
   {
      // update the head nodes on each new level
      for (int i = list->level + 1; i <= new_level; i++)
      {
         update[i] = list->head;
      }

      list->level = new_level; // set new list level
   }

   // allocating memory for the new node and filling in given data
   Node *new_node = malloc(sizeof(Node));
   new_node->citizen_id = strdup(citizen_id);
   new_node->first_name = strdup(first_name);
   new_node->last_name = strdup(last_name);
   new_node->country = strdup(country);
   new_node->age = age;
   new_node->virus_name = strdup(virus_name);
   new_node->vaccinated = strdup(vaccinated);
   new_node->date = date ? strdup(date) : NULL;

   // allocating space for next nodes according to the new level
   new_node->next = malloc(sizeof(Node *) * (new_level + 1));

   // checking for memory allocation errors
   if (new_node->next == NULL)
   {
      printf("Error while allocating memory");
      return NULL; // exit with failure
   }

   // linking the new node to each level using the update[] array
   for (int i = 0; i <= new_level; i++)
   {
      new_node->next[i] = update[i]->next[i];
      update[i]->next[i] = new_node;
   }

   return new_node;
}

// implementing list_search(...) to search for records in the skip list
Node *list_search(SkipList *list, char *citizen_id)
{
   Node *current = list->head; // start at the head of the list

   // go through the levels starting from the highest level
   for (int i = list->level; i >= 0; i--)
   {
      // continue as long as there is a node ahead and its ID is less than the current node
      while (current->next[i] != NULL &&
             strcmp(current->next[i]->citizen_id, citizen_id) < 0)
      {
         current = current->next[i]; // move to next node
      }
   }

   // move to the next node (because we stop at the node that has ID strictly less than the next node)
   current = current->next[0];

   // check if the ID matches
   if (current != NULL && strcmp(current->citizen_id, citizen_id) == 0)
   {
      return current; // return current node if ID matches
   }

   return NULL; // return NULL if ID does not match
}

void list_print(SkipList *list)
{
   printf("Skip List (%d levels):\n\n", list->level);

   // to print top-down starting from the highest level
   for (int i = list->level; i >= 0; i--)
   {
      Node *current = list->head->next[i]; // start at the first node at level i
      printf("Level %d: ", i);
      // continue while there is a node at position 'current'
      while (current != NULL)
      {
         printf("%s -> ", current->citizen_id);
         current = current->next[i]; // move to next node
      }
      printf("NULL\n"); // point to NULL after last node
   }

   // to print starting from level 0
   /*
   for (int i = 0; i <= list->level; i++)
   {
      Node *node = list->head->next[i];
      printf("Level %d: ", i);
      while (node != NULL)
      {
         printf("%s -> ", node->citizen_id);
         node = node->next[i];
      }
      printf("NULL\n");
   }
   */
}