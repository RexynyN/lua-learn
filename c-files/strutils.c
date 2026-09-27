#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


char** stringSplit(char* s, char* separator) {

}

char** stringSplitChar(char* s, char separator) {
    size_t len = strlen(s) + 1; // For the \0 

    for(size_t i = 0; i < len; i++) {
        if(s[i] == separator) {
            
        }
    }

}


char* stringTrim(char* s) {
    while (isspace((unsigned char)*s)) {
        s++; // Trim leading whitespace
    }

    // If the string is all whitespace, s will point to the null terminator
    if (*s == '\0')
        return s;

    // Trim trailing whitespace
    char* end = s + strlen(s) - 1;
    while (end >= s && isspace((unsigned char)*end)) {
        *end = '\0'; // Overwrite with null terminator
        end--;
    }

    return s; // String is modified in-place
}


// int main() {
    // // Must use a mutable character array, not a string literal (which is read-only)
    // char str1[] = "   Hello, World!   ";
    // char str2[] = "\t  Another string with tabs and newlines\n";
    // char str3[] = "     "; // String with only spaces

    // printf("Original 1: '%s'\n", str1);
    // printf("Trimmed 1 : '%s'\n", stringTrim(str1));
    
    // printf("\nOriginal 2: '%s'\n", str2);
    // printf("Trimmed 2 : '%s'\n", stringTrim(str2));

    // printf("\nOriginal 3: '%s'\n", str3);
    // printf("Trimmed 3 : '%s'\n", stringTrim(str3));
// }