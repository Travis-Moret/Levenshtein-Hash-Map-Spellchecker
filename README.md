# Levenshtein-Hash-Map-Spellchecker

A Spellchecker build in C which Implements a Hash Map and Levenshtein's Distance Formula to suggest possible suggestions to misspelled words

Creator: Travis Moret

Date: 3/24/2020

Class: CS 261 Data Structures

School: Oregon State University

This program is a project I created for class. I have permission to use this project for my portfolio, as it was designated as a portfolio project for the class.

The program is built in C. I imported a Dictionary of words into a Hash Map that I built. The user types a word into the command line, and the program checks if the word is spelled correctly by implementing the words from the dictionary in the hash map. If the word is not in the Hash Map, and is spelled incorrectly, the program will use the Levenshtein Distance Formula to determine five words that are close to what the user typed, and will suggest those five words to the user. 


                                                          Instructions
                                                            
                                                       Running the program
                                                       
                                       To compile the program, use the terminal command: 
                                                        
                                                        make spellChecker
                                                        
                                               To run the program, use the command: 
                                                          
                                                         ./spellChecker
                                                           
