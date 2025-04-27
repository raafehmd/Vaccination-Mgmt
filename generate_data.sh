#!/bin/bash

# defining sample data
FIRST_NAMES=("John" "Mary" "Michael" "William" "David" "Susan" "Paul")
LAST_NAMES=("Smith" "Johnson" "Williams" "Jones" "Davis" "Wilson" "Miller")
COUNTRIES=("USA" "UK" "Canada" "Australia" "Spain" "Japan" "China" "India" "Ethiopia")
VIRUSES=("COVID-19" "Influenza" "Measles" "Polio" "Rabies" "Cholera" "Malaria" "Yellofever" "HepatitisB")

# number of records to generate
NUM_RECORDS=500

# output file
OUTPUT_FILE="inputRecords.txt"

# emptying the output file before writing new data into it
> "$OUTPUT_FILE"

for ((i=1; i<=NUM_RECORDS; i++)); do
    # generating a random four-digit citizen ID between 1000 & 9999
    # using bash's in-built 'RANDOM'
    citizen_id=$((1000 + RANDOM % 9000))
    
    # selecting random names and country from pre-defined data
    first_name=${FIRST_NAMES[$RANDOM % ${#FIRST_NAMES[@]}]}
    last_name=${LAST_NAMES[$RANDOM % ${#LAST_NAMES[@]}]}
    country=${COUNTRIES[$RANDOM % ${#COUNTRIES[@]}]}
    
    # generating a random age between 1 and 100
    age=$((1 + RANDOM % 100))
    
    # selecting a random virus from pre-defined data
    virus=${VIRUSES[$RANDOM % ${#VIRUSES[@]}]}
    
    # setting a 70% chance of being vaccinated
    if (( RANDOM % 100 < 70 )); then
        vaccinated="YES"
        # generating a random date in YYYY-MM-DD format
        year=$((2005 + RANDOM % 14)) # random year between 2005 and 2024
        month=$((1 + RANDOM % 12))
        # generating a random day between 1 and 28, avoiding February 29 issue
        day=$((1 + RANDOM % 28))
        # formatting date correctly
        date=$(printf "%04d-%02d-%02d" $year $month $day)
        printf "%d %s %s %s %d %s %s %s\n" "$citizen_id" "$first_name" "$last_name" "$country" "$age" "$virus" "$vaccinated" "$date" >> "$OUTPUT_FILE"
    else
        vaccinated="NO"
        # if not vaccinated, a record without the date is written to the file
        printf "%d %s %s %s %d %s %s\n" "$citizen_id" "$first_name" "$last_name" "$country" "$age" "$virus" "$vaccinated" >> "$OUTPUT_FILE"
    fi
done

# message upon success
echo "Generated $NUM_RECORDS records in $OUTPUT_FILE"