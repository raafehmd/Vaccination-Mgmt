#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "skip_list.c"

// Sample vaccine data
const char *first_names[] = {"John", "Emma", "Michael", "Sophia", "William"};
const char *last_names[] = {"Smith", "Johnson", "Brown", "Lee", "Garcia"};
const char *countries[] = {"US", "UK", "Canada", "Australia", "Japan"};
const char *viruses[] = {"COVID-19", "Flu", "Measles"};

// Generate random test data
void generate_test_record(char *citizen_id, char *first_name, char *last_name,
                          char *country, int *age, char *virus_name)
{
   sprintf(citizen_id, "%d", 10000 + rand() % 90000);
   strcpy(first_name, first_names[rand() % 5]);
   strcpy(last_name, last_names[rand() % 5]);
   strcpy(country, countries[rand() % 5]);
   *age = 18 + rand() % 60;
   strcpy(virus_name, viruses[rand() % 3]);
}

int main()
{
   // Initialize random seed
   srand(time(NULL));

   // Create a skip list for COVID-19 vaccinations (max 4 levels)
   SkipList *covid_list = list_create(4);

   printf("=== Testing Skip List Print Function ===\n\n");

   // Insert 10 random vaccination records
   for (int i = 0; i < 10; i++)
   {
      char citizen_id[10], first_name[20], last_name[20], country[10], virus[20];
      int age;

      generate_test_record(citizen_id, first_name, last_name, country, &age, virus);

      // Insert with 75% vaccination rate
      if (rand() % 4 != 0)
      { // 75% vaccinated
         char date[20];
         sprintf(date, "2023-%02d-%02d", 1 + rand() % 12, 1 + rand() % 28);
         list_insert(covid_list, citizen_id, first_name, last_name,
                     country, age, virus, "YES", date);
      }
      else
      {
         list_insert(covid_list, citizen_id, first_name, last_name,
                     country, age, virus, "NO", NULL);
      }
   }

   // Print the entire skip list structure
   printf("=== Skip List Structure ===\n");
   list_print(covid_list);

   // Print all vaccinated citizens in order
   printf("\n=== Vaccinated Citizens (Sorted by ID) ===\n");
   Node *node = covid_list->head->next[0];
   while (node != NULL)
   {
      if (strcmp(node->vaccinated, "YES") == 0)
      {
         printf("%s %s %s (%s, %d) - %s on %s\n",
                node->citizen_id, node->first_name, node->last_name,
                node->country, node->age, node->virus_name, node->date);
      }
      node = node->next[0];
   }

   // Clean up
   list_delete(covid_list);
   return 0;
}