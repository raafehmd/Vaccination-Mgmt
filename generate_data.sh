#!/bin/bash

# Define sample data
FIRST_NAMES=("John" "Mary" "Robert" "Jennifer" "Michael" "Linda" "William" "Elizabeth" "David" "Susan")
LAST_NAMES=("Smith" "Johnson" "Williams" "Brown" "Jones" "Miller" "Davis" "Garcia" "Rodriguez" "Wilson")
COUNTRIES=("USA" "UK" "Canada" "Australia" "Germany" "France" "Italy" "Spain" "Japan" "China")
VIRUSES=("COVID-19" "Influenza" "HepatitisB" "Measles" "Polio" "Rabies" "YellowFever" "HPV" "Cholera" "Malaria")

# Number of records to generate
NUM_RECORDS=1000

# Output file
OUTPUT_FILE="inputRecords.txt"

# Clear the output file
> "$OUTPUT_FILE"

for ((i=1; i<=NUM_RECORDS; i++)); do
    # Generate random citizen ID
    citizen_id=$((10000 + RANDOM % 90000))
    
    # Select random names and country
    first_name=${FIRST_NAMES[$RANDOM % ${#FIRST_NAMES[@]}]}
    last_name=${LAST_NAMES[$RANDOM % ${#LAST_NAMES[@]}]}
    country=${COUNTRIES[$RANDOM % ${#COUNTRIES[@]}]}
    
    # Random age between 1 and 100
    age=$((1 + RANDOM % 100))
    
    # Select random virus
    virus=${VIRUSES[$RANDOM % ${#VIRUSES[@]}]}
    
    # 70% chance of being vaccinated
    if (( RANDOM % 100 < 70 )); then
        vaccinated="YES"
        # Generate random date in YYYY-MM-DD format
        year=$((2010 + RANDOM % 14))
        month=$((1 + RANDOM % 12))
        day=$((1 + RANDOM % 28))
        date=$(printf "%04d-%02d-%02d" $year $month $day)
        printf "%d %s %s %s %d %s %s %s\n" "$citizen_id" "$first_name" "$last_name" "$country" "$age" "$virus" "$vaccinated" "$date" >> "$OUTPUT_FILE"
    else
        vaccinated="NO"
        printf "%d %s %s %s %d %s %s\n" "$citizen_id" "$first_name" "$last_name" "$country" "$age" "$virus" "$vaccinated" >> "$OUTPUT_FILE"
    fi
done

echo "Generated $NUM_RECORDS records in $OUTPUT_FILE"