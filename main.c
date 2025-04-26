/*
   This is the main.c file that acts as the entry point for this application
   and defines its logic.
*/

// including relevant libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bloom_filter.h"
#include "skip_list.h"

#define MAX_VIRUSES 50   // max number of viruses that this program can handle
#define BLOOM_SIZE 10000 // setting the size of the bloom filter

// defining the structure for a virus
typedef struct
{
   char *name;
   BloomFilter *bloom;
   SkipList *skip_list;
} Virus;

Virus viruses[MAX_VIRUSES]; // initializing global array viruses[] to contain all viruses
int virus_count = 0;        // initializing global var virus count to zero

/*
   function declarations
*/
Virus *create_virus(const char *name); // function to create a new virus
Virus *find_virus(const char *name);   // function to find an existing virus
void process_record(char *citizen_id, char *first_name, char *last_name, char *country,
                    int age, char *virus_name, char *vaccinated, char *date); // function to process a new vaccination record
void load_records(const char *filename);                                      // function to load vaccination records from a file
void check_vaccination_status(char *citizen_id, const char *virus_name);      // function to check vaccination status
void list_vaccinated(const char *virus_name);                                 // function to list all vaccination records for a given virus
void run();                                                                   // function to enable user interaction

// driver function
int main(int argc, char *argv[])
{
   // check if user provided input file as an argument
   if (argc != 2)
   {
      printf("Usage: %s <input_file>\n", argv[0]);
      return 1;
   }

   // if input file provided, load all records from input file
   load_records(argv[1]);

   // start user interaction
   run();

   // cleaning up allocated memory
   for (int i = 0; i < virus_count; i++)
   {
      free(viruses[i].name);
      bloom_delete(viruses[i].bloom);
      list_delete(viruses[i].skip_list);
   }

   return 0;
}

// implementing create_virus(...) to create a new virus
Virus *create_virus(const char *name)
{
   // if we can't add more viruses...
   if (virus_count >= MAX_VIRUSES)
   {
      return NULL;
   }

   // if we can...
   Virus *virus = &viruses[virus_count++];  // create new Virus
   virus->name = strdup(name);              // set virus's name
   virus->bloom = bloom_create(BLOOM_SIZE); // create a new bloom filter for the virus
   virus->skip_list = list_create(5);       // create a new skip list for the virus

   return virus;
}

// implementing find_virus(...) to find an existing virus
Virus *find_virus(const char *name)
{
   for (int i = 0; i < virus_count; i++)
   {
      if (strcmp(viruses[i].name, name) == 0)
         return &viruses[i];
   }

   return NULL;
}

// implementing process_record(...) to process a vaccination record from start to finish
void process_record(char *citizen_id, char *first_name, char *last_name,
                    char *country, int age, char *virus_name, char *vaccinated,
                    char *date)
{
   Virus *virus = find_virus(virus_name);

   // create virus if it does not already exist
   if (virus == NULL)
   {
      virus = create_virus(virus_name);
   }

   // if virus is not created for some reason...
   if (!virus)
   {
      printf("Error encountered with virus %s", virus_name);
      return;
   }

   // if citizen is vaccinated, add citizen to the virus's bloom filter and skip list
   if (strcmp(vaccinated, "YES") == 0)
   {
      bloom_insert(virus->bloom, citizen_id);
      list_insert(virus->skip_list, citizen_id, first_name, last_name,
                  country, age, virus_name, vaccinated, date);
   }
}

// implementing load_records(...) to read records from file
void load_records(const char *filename)
{
   FILE *file = fopen(filename, "r");

   // if file was not opened successfully
   if (!file)
   {
      printf("Error opening file");
      return;
   }

   char line[256]; // setting a 256 character limit for a line

   while (fgets(line, sizeof(line), file))
   {
      // initializing strings and int to read data into
      char citizen_id[50], first_name[50], last_name[50], country[50];
      char virus_name[50], vaccinated[10], date[20] = "";
      int age;

      // sscanf(...) parses one line (record) according to given string
      // into the data variables and returns number of successfully assigned variables
      int fields = sscanf(line, "%s %s %s %s %d %s %s %s",
                          citizen_id, first_name, last_name, country,
                          &age, virus_name, vaccinated, date);

      // check if the format of the record is valid (at least 7 fields)
      if (fields >= 7)
      {
         process_record(citizen_id, first_name, last_name, country,
                        age, virus_name, vaccinated,
                        (fields == 8) ? date : NULL);
      }
      else
      {
         printf("Record format is invalid");
         return;
      }
   }

   fclose(file); // close file
}

// implementing check_vaccination_status(...) to check if a citizen is vaccinated for the given virus
void check_vaccination_status(char *citizen_id, const char *virus_name)
{
   // loop through all viruses
   for (int i = 0; i < virus_count; i++)
   {
      // if given virus name matches any of the existing viruses...
      if (strcmp(viruses[i].name, virus_name) == 0)
      {
         // if the citizen ID is found in the bloom filter of the virus
         if (bloom_check(viruses[i].bloom, citizen_id))
         {
            Node *node = list_search(viruses[i].skip_list, citizen_id);
            if (node)
            {
               printf("%s %s %s %s %d %s %s %s\n",
                      node->citizen_id, node->first_name, node->last_name,
                      node->country, node->age, node->virus_name,
                      node->vaccinated, node->date ? node->date : "");
            }
            else
            {
               printf("MAYBE (False positive from Bloom Filter)\n");
            }
         }
         else
         {
            printf("NOT VACCINATED\n");
         }
         return;
      }
   }
   printf("Virus not found\n"); // if virus was not found
}

// implementing list_vaccinated(...) to display records of all citizens that are
// vaccinated for the given virus
void list_vaccinated(const char *virus_name)
{
   // loop through all the viruses
   for (int i = 0; i < virus_count; i++)
   {
      // if given virus exists
      if (strcmp(viruses[i].name, virus_name) == 0)
      {
         // start at head node of the skip list and move to the first node at level 0
         Node *node = viruses[i].skip_list->head->next[0];

         // continue as long as there is a node
         while (node)
         {
            // print the records
            printf("%s %s %s %s %d %s %s %s\n",
                   node->citizen_id, node->first_name, node->last_name,
                   node->country, node->age, node->virus_name,
                   node->vaccinated, node->date ? node->date : "");

            // move to the next node
            node = node->next[0];
         }
         return;
      }
   }
   printf("Virus not found\n"); // if virus not found
}

void run()
{
   printf("\nVaccination Records Management System\n");
   printf("\nCommands:\n");
   printf("\tcheck <citizen_id> <virus>\n");
   printf("\tlist <virus>\n");
   printf("\texit\n");

   char command[20], arg1[50], arg2[50];

   // keep running until user exits
   while (1)
   {
      printf("\n> ");

      //
      if (scanf("%s", command) != 1)
      {
         break;
      }

      // if user typed "check" as the command...
      if (strcmp(command, "check") == 0)
      {
         scanf("%s %s", arg1, arg2);           // read the two arguments citizen_id and virus_name
         check_vaccination_status(arg1, arg2); // call function to check vaccination status
      }
      // if user typed "list" as the command...
      else if (strcmp(command, "list") == 0)
      {
         scanf("%s", arg1);     // read the argument virus_name into arg1
         list_vaccinated(arg1); // call function to list all records of respective virus
      }
      // if user types "exit" as the command...
      else if (strcmp(command, "exit") == 0)
      {
         break;
      }
      // if user types something other than the listed commands...
      else
      {
         printf("Unknown command\n");
      }
   }
}