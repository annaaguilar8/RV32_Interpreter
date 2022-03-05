//Anna Aguilar 80669897
//Lab 1 Tokenizer

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* Return true (non-zero) if c is the delimiter character
   previously chosen by the user.
   Zero terminators are not printable (therefore false) */
bool delim_character(char c, char delim){
   // checking if char is delim and not null character (case to look for end chars when going char by char)
   if (c == delim && c != '\0'){
        return true;
   }
   return false;
}

/* Return true (non-zero) if c is a non-delimiter
   character.
   Zero terminators are not printable (therefore false) */
bool non_delim_character(char c, char delim){
    if (c != delim && c != '\0'){
        return true;
    }
    return false;
}

/* Returns a pointer to the first character of the next
   word starting with a non-delimiter character. Return a zero pointer if
   str does not contain any words.*/
char *word_start(char* str,char delim){
    //variable to iterate through string
    int i = 0;
   

    //Search through string, if non-delimieter char is found, return its address
    while(delim_character(str[i], delim)){
      i++;
    }
    //str[0] -> *str
    //&str[i] -> str + i
    return str + i;
}

/* Returns a pointer to the first delimiter character of the zero
   terminated string*/
char *end_word(char* str,char delim){
   //using word start here to help us with count tokens method
   //finding the start of the word to help is iterate using out non delim char method
   str = word_start(str, delim);
   int i = 0;
   if (*str == '\0'){
      return str;
   }

   //Search through word from start of word
   //if delimieter char is found, 
   while(non_delim_character(str[i], delim)){
      i++;
   }
   //return the address of the charater before
   return str + i;
}

/* Counts the number of words or tokens*/
int count_tokens(char* str,char delim){
    int numTokens = 0;

   //starting from the start or end of previous word
   //end word method will already find the start of the word, so by recalling end word method, it will iterate through each word
    while(str != end_word(str, delim)){
      str = end_word(str, delim);
      numTokens++;
   }
   return numTokens;
}

/* Returns a freshly allocated new zero-terminated string
   containing <len> chars from <inStr> */
char *copy_str(char *inStr, short len, char delim){
   //allocating correct amount of memory
   //len + 1 for zero-terminator at end of string (/0)
   char *inStrCopy = malloc(sizeof(char) * (len + 1)); 
   int i = 0;
   //copy all contents of input to new copy
   while(i <= len){
      if(inStr[i] != delim){
        inStrCopy[i] = inStr[i];
        i++;
      }else{
         i++;
      }  
   }
   
   return inStrCopy;
}

/* Returns a freshly allocated zero-terminated vector of freshly allocated
   delimiter-separated tokens from zero-terminated str.
   For example, tokenize("hello world string"), with a character delimiter
   of a space " " would result in:
     tokens[0] = "hello"
     tokens[1] = "world"
     tokens[2] = "string"
     tokens[3] = 0
*/
char** tokenize(char* str, char delim){
   //first find number of words and allocate memory for all tokens using a double pointer / array of pointers
   int numWords = count_tokens(str, delim);
   char **tokensArray = malloc(sizeof(char *) * (numWords + 1)); 

   //find initial start and end to first word
   char* startStr = word_start(str, delim);
   char* endStr = end_word(str, delim);

   //loop to fill in tokens array
   int i;

   for(i=0; i < numWords; i++){
      //find length by subtracting pointers
      short wordLen = endStr - startStr;
      //fill tokens with copy of input words
      tokensArray[i] = copy_str(startStr, wordLen, delim);
      //initialize new start and end to next word
      startStr = word_start(endStr, delim);
      endStr = end_word(startStr, delim);
   }
   //set last index of array to null to help us print in our print method
   tokensArray[i] = NULL;
   return tokensArray;
}

void print_all_tokens(char** tokens){
   int i = 0;
   //loop to print all tokens from users input
   while(tokens[i] != NULL){
      printf("tokens[%d]: %s\n", i, tokens[i]);
      i++;
   }
}
