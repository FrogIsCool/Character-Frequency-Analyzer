#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FILE_NAME 100
#define NUM_CHARS 26
#define NUM_CHARS_DISPLAY 20

int lettercmp(const void *pa, const void *pb) {
	const int *a = (const int *)pa;
	const int *b = (const int *)pb;
	return b[1] - a[1];
}

/* Given a text file, counts the occurrences of each letter and
*  prints the frequencies of each.
*/
int main(int argc, char *argv[])
{	
	char fileName[MAX_FILE_NAME] = "";
	FILE *fp = NULL;
	int i = 0;

	// Choose file to read from argv or ask user for path.
	if(argc == 2) {
		memcpy(fileName, argv[1], MAX_FILE_NAME);
	} else if(argc != 1) {
		printf("Too many arguments\nTakes either a path to a file or no arguments");
		exit(1);
	} else {
		printf("Enter path to file: ");
		fgets(fileName, MAX_FILE_NAME, stdin);
	}
	
	// Open file at fileName and point fp to it.
	fileName[strcspn(fileName, "\n")] = '\0';
	if((fp = fopen(fileName, "r")) == NULL) {
		printf("File not found\n");
		exit(2);
	}
	
	// Indexes of charCount map to the offset ASCII values for each letter.
	// 'a' maps to charCount[0], 'b' to charCount[1], etc...
	// The first value of each inner array is the ASCII code of the letter at that index.
	// The second value of each inner array is the number of occurrences.
	int charCount[NUM_CHARS][2];
	memset(charCount, 0, sizeof(charCount));
	// Set first value of the inner arrays to the ASCII code of the related letter at each index.
	for(i = 0; i < NUM_CHARS; i++) {charCount[i][0] = i + 'a';}
	// numLetters tracks the total number of letters within the file for calculating percentages.
	int numLetters = 0;
	// Get letters from file and count the occurrences of each.
	// Uppercase letters count as occurrences of their lowercase counterparts.
	char c;
	while((c = fgetc(fp)) != EOF) {
		switch (c) {
			case 'a'...'z':
				numLetters++;
				charCount[c - 'a'][1]++;
				break;
			case 'A'...'Z':
				numLetters++;
				charCount[tolower(c) - 'a'][1]++;
				break;
			default:
				// Non letters handling
				break;
		}
	}
	
	// Done with file, close it.
	fclose(fp);

	// Calculate the occurrence percentage of each letter within the file.
	float percentages[NUM_CHARS] = {0.0};
	for(i = 0; i < NUM_CHARS; i++) {
		percentages[i] = (float)charCount[i][1] / numLetters;
	}

	// Print out NUM_CHARS_DISPLAY most frequent letters.
	qsort(charCount, NUM_CHARS, sizeof(charCount[0]), lettercmp);
	printf("Letter       Frequency                        Letter       Frequency\n");
	for(i = 0; i < NUM_CHARS; i++) {
		printf("%6c%15.1f%%", charCount[i][0], percentages[charCount[i][0] - 'a'] * 100);
		if(++i < NUM_CHARS) {printf("%30c%15.1f%%", charCount[i][0], percentages[charCount[i][0] - 'a'] * 100);}
		printf("\n");
	}

	return EXIT_SUCCESS;
}

