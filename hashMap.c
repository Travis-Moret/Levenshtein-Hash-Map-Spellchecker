/*
 * CS 261 Data Structures
 * Assignment 5
 * Name: Travis Moret
 * Date: 03/06/2020
 */

#include "hashMap.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

/*This Function was provided by the class*/
int hashFunction1(const char* key)
{
    int r = 0;
    for (int i = 0; key[i] != '\0'; i++)
    {
        r += key[i];
    }
    return r;
}



/*This Function was provided by the class*/
int hashFunction2(const char* key)
{
    int r = 0;
    for (int i = 0; key[i] != '\0'; i++)
    {
        r += (i + 1) * key[i];
    }
    return r;
}


/*This Function was provided by the class*/
/*
 * Creates a new hash table link with a copy of the key string.
 * @param key Key string to copy in the link.
 * @param value Value to set in the link.
 * @param next Pointer to set as the link's next.
 * @return Hash table link allocated on the heap.
 */
HashLink* hashLinkNew(const char* key, int value, HashLink* next)
{
    HashLink* link = malloc(sizeof(HashLink));
    link->key = malloc(sizeof(char) * (strlen(key) + 1));
    strcpy(link->key, key);
    link->value = value;
    link->next = next;
    return link;
}


/*This Function was provided by the class*/
/*
 * Free the allocated memory for a hash table link created with hashLinkNew.
 * @param link
 */
static void hashLinkDelete(HashLink* link)
{
    free(link->key);
    free(link);
}


/*This Function was provided by the class*/
/*
 * Initializes a hash table map, allocating memory for a link pointer table with
 * the given number of buckets.
 * @param map
 * @param capacity The number of table buckets.
 */
void hashMapInit(HashMap* map, int capacity)
{
    map->capacity = capacity;
    map->size = 0;
    map->table = malloc(sizeof(HashLink*) * capacity);
    for (int i = 0; i < capacity; i++)
    {
        map->table[i] = NULL;
    }
}


/*This function was written by Travis Moret*/
/*
 * Removes all links in the map and frees all allocated memory. You can use
 * hashLinkDelete to free the links.
 * @param map
 */
void hashMapCleanUp(HashMap* map)
{
    /*Assert hash map is initialized */
    assert(map != 0); 

    /*Cycle through the hash map, and for each value in the hash table, delete each of it's links.*/
    for(int i = 0; i < map->capacity; i++)
    {
        /*Make a link for each table value*/
        struct HashLink* temp = map->table[i]; 

        /*Cycle through all links and delete each*/
        while(temp != 0)
        {
            struct HashLink* deleteLink = temp; 
            temp = temp->next; 
            hashLinkDelete(deleteLink); 
        }
        free(temp); 
    }
    /*Free the table and set it's size to 0*/
    free(map->table); 
    map->size = 0; 
}



/*This Function was provided by the class*/
/*
 * Creates a hash table map, allocating memory for a link pointer table with
 * the given number of buckets.
 * @param capacity The number of buckets.
 * @return The allocated map.
 */
HashMap* hashMapNew(int capacity)
{
    HashMap* map = malloc(sizeof(HashMap));
    hashMapInit(map, capacity);
    return map;
}



/*This Function was provided by the class*/
/*
 * Removes all links in the map and frees all allocated memory, including the
 * map itself.
 * @param map
 */
void hashMapDelete(HashMap* map)
{
    hashMapCleanUp(map);
    free(map);
}


/*This function was written by Travis Moret*/
/*
 * Returns a pointer to the value of the link with the given key and skip traversing as well. Returns NULL if no link with that key is in the table.
 * Use HASH_FUNCTION(key) and the map's capacity to find the index of the
 * correct linked list bucket. Also make sure to search the entire list.
 * 
 * @param map
 * @param key
 * @return Link value or NULL if no matching link.
 */
int* hashMapGet(HashMap* map, const char* key)
{
    /*Determine that the map has been initialized*/
    assert(map != 0); 

    /*Create an index using the Hash function and key*/
    int index = HASH_FUNCTION(key) % map->capacity; 

    /*Create a link to store the value at the index*/
    struct HashLink* temp = map->table[index]; 

    /*While the link has a value, compare the key of the index and the input key, and return the value if they are the same. Then do so for consecutinve links. If the value is not found return NULL. */
    while(temp != 0)
    {
        if(strcmp(temp->key,key) == 0)
        {
            return &temp->value;  
        }
        temp = temp->next;
    }
    return NULL;
}



/*This function was written by Travis Moret*/
/*
 * Resizes the hash table to have a number of buckets equal to the given 
 * capacity (double of the old capacity). After allocating the new table, all of the links need to rehashed into it because the capacity has changed.
 * 
 * Remember to free the old table and any old links if you use hashMapPut to rehash them.
 * 
 * @param map
 * @param capacity The new number of buckets.
 */
void resizeTable(HashMap* map, int capacity)
{
    /*Determine map has been initialized*/
    assert(map != 0); 

    /*Determine Cpacity is doubled*/
    assert(capacity == map->capacity * 2); 

    /*Create a variable that stores the old capacity*/
    int oldCap = hashMapCapacity(map); 

    /*Create a new table with new capacity to store temporary values*/
    struct HashMap* newTable = hashMapNew(capacity); 
     
    /*Iterate through the links and add them to the new table*/
    for(int i = 0; i < oldCap; i++)
    {
        struct HashLink* current = map->table[i]; 

        while(current != 0) //add the values traversed to the table
        { 
            hashMapPut(newTable, current->key, current->value); 
            current = current->next; 
        }
    }
    
    /*Destroy the old table*/
    hashMapCleanUp(map);

    /*Re-initialize the table value*/
    map->size = newTable->size;
    map->table = newTable->table;
    map->capacity = newTable->capacity;

    /*Free the new table*/
    newTable->table = NULL;
    free(newTable);
}



/*This function was written by Travis Moret*/
/*
 * Updates the given key-value pair in the hash table. If a link with the given key already exists, this will just update the value and skip traversing. Otherwise, it will create a new link with the given key and value and add it to the table bucket's linked list. You can use hashLinkNew to create the link.
 * 
 * Use HASH_FUNCTION(key) and the map's capacity to find the index of the
 * correct linked list bucket.
 * 
 * @param map
 * @param key
 * @param value
 */
void hashMapPut(HashMap* map, const char* key, int value)
{
    /*Assert map is initiated*/
    assert(map != 0); 

    /*Create an index with the hash function and key*/
    int index = HASH_FUNCTION(key) % map->capacity; 

    /*Create a link to iterate with*/
    struct HashLink* current = map->table[index]; 

    /*Iterate through the keys. If the key already exists, update the value and keep traversing*/
    while(current != 0)
    {
        if(strcmp(current->key,key) == 0)
        {
            current->value = value; 
            return; 
        }
        current = current->next; 
    }

    /*Create a new link with the value and add it to the table*/
    struct HashLink* newLink = hashLinkNew(key,value,map->table[index]); 

    map->table[index] = newLink; //set the index to the new link value
    map->size++; //increase the map size

    /*Resize the table if the table load is too high*/
    if(hashMapTableLoad(map) >= MAX_TABLE_LOAD)
    {
        resizeTable(map, 2 * map->capacity); 
    }
}



/*This function was written by Travis Moret*/
/**
 * Removes and frees the link with the given key from the table. If no such link
 * exists, this does nothing. Remember to search the entire linked list at the
 * bucket. You can use hashLinkDelete to free the link.
 * @param map
 * @param key
 */
void hashMapRemove(HashMap* map, const char* key)
{
    /*Determine the map is initialized and key has a value*/ 
    assert(map != 0); 
    assert(key != 0); 

    /*If the hash map doesn't contain the key, print a message*/
    if(!hashMapContainsKey(map,key))
    {
        printf("Value not found in hash map"); 
        return; 
    }

    /*Create an index with the hash function and key*/
    int index = HASH_FUNCTION(key) % map->capacity;

    /*Adjust the index if needed*/
    if(index < 0)
    {
        index += map->capacity; 
    }

    /*Create a link containing an table index*/
    struct HashLink* current = map->table[index]; 
    struct HashLink* prev = NULL; 

    /*If empty, exit the function*/
    if(current == 0)
    {
        return; 
    }

    /*While there are values, iterate through each and compare them to the key. If a match is found, delete the link and adjust links.*/
    while(current != 0)
    {
        if(strcmp(current->key, key) == 0)
        {
            if(prev == 0)
            {
                map->table[index] = current->next; 
            }
            else
            {
                prev->next = current->next; 
            }
            hashLinkDelete(current); 
            current = 0; 
            map->size--; 
            return; 
        }
        prev = current; 
        current = current->next; 
    }
}



/*This function was written by Travis Moret*/
/*
 * Returns 1 if a link with the given key is in the table and 0 otherwise.
 * 
 * Use HASH_FUNCTION(key) and the map's capacity to find the index of the
 * correct linked list bucket. Also make sure to search the entire list.
 * 
 * @param map
 * @param key
 * @return 1 if the key is found, 0 otherwise.
 */
int hashMapContainsKey(HashMap* map, const char* key)
{
    assert(map != 0); 
    assert(key != 0); 

    int index = HASH_FUNCTION(key) % map->capacity; 

    if(index < 0)
    {
        index += map->capacity; 
    }

    struct HashLink* current = map->table[index]; 

    while(current != 0)
    {
        if(strcmp(current->key, key) == 0)
        {
            return 1; 
        }
        current = current->next; 
    }
    return 0;
}



/*This function was written by Travis Moret*/
/**
 * Returns the number of links in the table.
 * @param map
 * @return Number of links in the table.
 */
int hashMapSize(HashMap* map)
{
    assert(map != 0); 

    return map->size;
}


/*This function was written by Travis Moret*/
/*
 * Returns the number of buckets in the table.
 * @param map
 * @return Number of buckets in the table.
 */
int hashMapCapacity(HashMap* map)
{
    assert(map != 0); 

    return map->capacity;
}


/*This function was written by Travis Moret*/
/*
 * Returns the number of table buckets without any links.
 * @param map
 * @return Number of empty buckets.
 */
int hashMapEmptyBuckets(HashMap* map)
{
    assert(map != 0); 

    int count  = 0; 

    /*Loop through each index and count empty buckets in the table*/
    for(int i = 0; i < map->capacity; i++)
    {
        if(map->table[i] == 0)
        {
            count++; 
        }
    }
    return count;
}



/*This function was written by Travis Moret*/
/*
 * Returns the ratio of (number of links) / (number of buckets) in the table.
 * Remember that the buckets are linked lists, so this ratio tells you nothing about the number of empty buckets. Remember also that the load is a floating point number, so don't do integer division.
 * @param map
 * @return Table load.
 */
float hashMapTableLoad(HashMap* map)
{
    assert(map != 0); 

    /*Determines a ratio of the table*/
    return (float)map->size / (float)map->capacity;
}



/*This function was written by Travis Moret*/
/**
 * Prints all the links in each of the buckets in the table.
 * @param map
 */
void hashMapPrint(HashMap* map)
{
    assert(map != 0); 

    printf("\n"); 

    /*Iterate through each bucket and print it's key and value pairs*/
    for(int i = 0; i < map->capacity; i++)
    {
        struct HashLink* current = map->table[i]; 

        printf("The value in bucket %d is: ", i); 

        while(current != NULL)
        {
            printf("key: %s Value: %d", current->key, current->value); 
            current = current->next; 
        }
        printf("\n"); 
    }
}

