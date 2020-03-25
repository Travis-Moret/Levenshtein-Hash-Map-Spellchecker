#include "hashMap.h"
#include <assert.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


/*This function was provided by the class*/
/*
 * Allocates a string for the next word in the file and returns it. This string
 * is null terminated. Returns NULL after reaching the end of the file.
 * @param file
 * @return Allocated string or NULL.
 */
char* nextWord(FILE* file)
{
    int maxLength = 16;
    int length = 0;
    char* word = malloc(sizeof(char) * maxLength);
    while (1)
    {
        char c = fgetc(file);
        if ((c >= '0' && c <= '9') ||
            (c >= 'A' && c <= 'Z') ||
            (c >= 'a' && c <= 'z') ||
            c == '\'')
        {
            if (length + 1 >= maxLength)
            {
                maxLength *= 2;
                word = realloc(word, maxLength);
            }
            word[length] = c;
            length++;
        }
        else if (length > 0 || c == EOF)
        {
            break;
        }
    }
    if (length == 0)
    {
        free(word);
        return NULL;
    }
    word[length] = '\0';
    return word;
}



/*This function was written by Travis Moret*/
/*
 * Loads the contents of the file into the hash map.
 * @param file
 * @param map
 */
void loadDictionary(FILE* file, HashMap* map)
{
    /*Read each word in the file into a variable*/
    char* word = nextWord(file); 

    /*place each value in the file into a hash map*/
    while(word != 0)
    {
        hashMapPut(map, word, 1); 
        free(word); 
        word = nextWord(file); 
    }
}



/*Calculates Levenshtein Distance between two strings in a dictionary
 * Accessed 3/9/2020
 * from source: 
 * https://en.wikibooks.org/wiki/Algorithm_Implementation/Strings/  Levenshtein_distance#C
*/
#define MIN3(a, b, c) ((a) < (b) ? ((a) < (c) ? (a) : (c)) : ((b) < (c) ? (b) : (c)))

int levenshtein(char *s1, char *s2) {
    unsigned int x, y, s1len, s2len;
    s1len = strlen(s1);
    s2len = strlen(s2);
    unsigned int matrix[s2len+1][s1len+1];
    matrix[0][0] = 0;
    for (x = 1; x <= s2len; x++)
        matrix[x][0] = matrix[x-1][0] + 1;
    for (y = 1; y <= s1len; y++)
        matrix[0][y] = matrix[0][y-1] + 1;
    for (x = 1; x <= s2len; x++)
        for (y = 1; y <= s1len; y++)
            matrix[x][y] = MIN3(matrix[x-1][y] + 1, matrix[x][y-1] + 1, matrix[x-1][y-1] + (s1[y-1] == s2[x-1] ? 0 : 1));

    return(matrix[s2len][s1len]);
}



/*This function was written by Travis Moret*/
/*
 * Checks the spelling of the word provded by the user. If the word is spelled incorrectly, print the 5 closest words as determined by a metric like the Levenshtein distance.
 * Otherwise, indicate that the provded word is spelled correctly. Use dictionary.txt to create the dictionary.
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, const char** argv)
{
    /*Create a new hash map with capacity*/
    HashMap* map = hashMapNew(1000);

    /*Read in dictionary file and calculate time to download*/
    FILE* file = fopen("dictionary.txt", "r");
    clock_t timer = clock();
    loadDictionary(file, map);
    timer = clock() - timer;
    printf("Dictionary loaded in %f seconds\n", (float)timer / (float)CLOCKS_PER_SEC);
    fclose(file);

    char inputBuffer[256];
    int quit = 0;

    /*Loop as long as the user does not type quit*/
    while (!quit)
    {
        printf("Enter a word or \"quit\" to quit: ");
        
        /*Adapted for use from source:
        https://stackoverflow.com/questions/3756308/how-to-read-only-the-first-word-from-each-line
        Only reads in first word up to a space from input buffer*/
        scanf("%s%*[^\n]",inputBuffer);

        /*converts all characters in input buffer to lowercase*/
        for(int i = 0; i < strlen(inputBuffer); i++)
        {
            inputBuffer[i] = tolower(inputBuffer[i]);   
        }

        /*If use types quit, program exits*/ 
        if(strcmp(inputBuffer, "quit") == 0)
        {
            quit = 1;
            break; 
        }

        char* matches[5]; 

        /*Allocates space for each match of an incorrect spelled word*/
        for (int i = 0; i < 5; i++)
        {
            matches[i] = malloc(sizeof(char) * 257);

            assert(matches[i] != 0);
        } 

        int matchFound = 0; 

        /*Levenshtein Distances array*/
        int distances[5] = {0,0,0,0,0}; 

        /*Search through hash map for the user input string*/
        for(int i = 0; i < map->capacity; i++)
        {
            HashLink* current = map->table[i]; 

            while(current != 0)
            {
                /*If string is found, print message and exit loop*/
                if(strcmp(inputBuffer, current->key) == 0)
                {
                    printf("\n"); 
                    printf("The inputted word %s is spelled correctly \n", inputBuffer); 
                    printf("\n"); 
                    matchFound = 1; 
                }
                /*If string not found in loop calculate Levenshtein distances*/
                else
                { 
                    /*Call levenshtein function to store distance in a variable*/
                    int distance = levenshtein(inputBuffer,current->key); 

                    /*Store distance in an appropriate spot in the distances array*/ 
                    for(int k = 0; k < 5; k++)
                    {
                        if((distances[k] == 0) || (distances[k] > distance))
                        {
                            distances[k] = distance; 
                            strcpy(matches[k], current->key); 
                            break;  
                        }
                    }
                }
                current = current->next; 
            }
        }

        /*If not match is found, print incorrect spelling and display 5 levenshtein matches*/ 
        if(matchFound == 0)
        {
            printf("\n"); 
            printf("The inputted word %s is spelled incorrectly \n", inputBuffer);
            printf("\n");

            for(int i = 0; i < 5; i++)
            {
               printf("Did you mean %s? \n", matches[i]); 
            }  
            printf("\n");
        }

        /*Free the matches*/
        for(int i = 0; i < 5; i++)
        {
            free(matches[i]); 
        }

    } 
    /*Print when exiting*/
    printf("\n"); 
    printf("Exiting the program"); 
    printf("\n"); 

    /*Destroy the Hash Map*/
    hashMapDelete(map);
    return 0;
}
