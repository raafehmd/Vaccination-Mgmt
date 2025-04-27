# setting compiler to gcc
CC = gcc

# naming final executable
TARGET = vaccinationManager

# listing all source (.c) files
SRCS = src/main.c src/bloom_filter.c src/skip_list.c

# converting source (.c) files to object (.o) files
OBJS = $(SRCS:.c=.o)

# this is the default build rule when the make command
# is run with no arguments
# requires TARGET to be built first
all: $(TARGET)

# executable linking rule
# this rule links all object files into the final executable using gcc
# gcc -o vaccinationManager main.o bloom_filter.o skip_list.o
$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS)

# defining rule for building a .o file
# automatic variables $< and $@ dynamically reference
# the target (.o) and dependency (.c) files respectively
%.o: %.c
	$(CC) -c $< -o $@

# path to generate_data.sh file
DATAGEN = D:\repo\Vaccination-mgmt\generate_data.sh

# command to run generate_data.sh
generate:
	chmod +x generate_data.sh
	$(DATAGEN)

# command to run the executable with input file
run: all
	./$(TARGET) inputRecords.txt

# clean command to delete all compiled files
clean:
	rm -f $(OBJS) $(TARGET)

# the phony command tells make that these targets do not produce actual files
.PHONY: all clean generate run