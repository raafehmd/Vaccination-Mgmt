# Vaccination Records Management System

A C program for managing vaccination records using **Bloom Filters** and **Skip Lists**, featuring fast probabilistic checks and efficient ordered storage.

## Features ✨

-  📂 Load vaccination records from file
-  🔍 Check vaccination status with Bloom Filter acceleration
-  📝 Insert new vaccination records
-  📋 List all vaccinated citizens for a specific virus
-  🧪 Synthetic data generation script included
-  🚀 Optimized with:
   -  **Bloom Filters** for O(1) probabilistic membership checks
   -  **Skip Lists** for O(log n) search/insert operations

## Requirements 📦

-  GCC compiler
-  GNU Make
-  Bash shell (for data generation)

## Installation & Usage 🛠️

1. **Generate Test Data**

   ```
   make generate  # Creates inputRecords.txt with 1000 sample records
   ```

2. **Build Program**

   ```
   make              # compiles vaccinationManager executable
   ```

3. **Run Program**

   ```
   make run          # launches interactive mode
   ```

4. **Interactive Commands**
   ```
   > check <citizen_id> <virus_name>   # check vaccination status
   > list <virus_name>                 # list all vaccinated for virus
   > exit                              # quit program
   ```

## Data Structures Overview 🧠

### Bloom Filter

-  Space-efficient probabilistic structure
-  Fast "possibly in set" checks
-  Configured with 2 hash functions

### Skip List

-  Ordered hierarchical linked list
-  Average O(log n) search/insert
-  Maintains sorted citizen IDs

## File Structure 📁

```
vaccination-mgmt/
├── src/
│   ├── main.c
│   ├── bloom_filter.[ch]
│   └── skip_list.[ch]
├── Makefile
├── generate_data.sh
└── inputRecords.txt
```
