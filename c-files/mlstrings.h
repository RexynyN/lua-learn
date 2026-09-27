#ifndef MLSTRINGS_H
#define MLSTRINGS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unicode/ustring.h>
#include <unicode/uchar.h>
#include <unicode/ustdio.h>

typedef UChar character;
typedef UChar* string;

typedef struct {
    string* strings;
    int num;
} SplittedString;

typedef struct {
    string head; 
    string sep; 
    string tail; 
} Partition;

typedef struct StringVector {
    string* data; 
    size_t len;
} StringVector; 

typedef struct StringMatrix {
    string** data; 
    size_t rows; 
    size_t columns; 
} StringMatrix; 


#ifndef ALLOC_ARRAY
    #define ALLOC_ARRAY(x, alloc) malloc((*(x)) * alloc)
#endif

int32_t stringLen(const string src);
void stringCheckError(UErrorCode status, const char* msg);
bool stringEquals(const string src1, const string src2);
string charToString(const char* input_utf8);
bool stringStartsWith(const string src, const string prefix);
bool stringEndsWith(const string src, const string suffix);
string stringUpper(const string src);
string stringLower(const string src);
string stringCapitalize(const string src);
string stringSwapCase(const string src);
string stringSubstring(const string src, int32_t start, int32_t end);
int32_t stringCount(const string src, const string substr, int32_t start, int32_t end);
int32_t stringFind(const string src, const string sub);
bool stringIsAlnum(const string src);
bool stringIsAlpha(const string src);
bool stringIsAscii(const string src);
bool stringIsDigit(const string src);
bool stringIsNumeric(const string src);
bool stringIsSpace(const string src);
bool stringIsLower(const string src);
bool stringIsUpper(const string src);
string stringZFill(const string src, int32_t width);
string stringReplace(const string src, const string old_sub, const string new_sub, int32_t max_replace);
string stringJoin(const string* strings, int num, const string joiner);
string stringStrip(const string src);
string stringCaseFold(const string src);
string stringTitle(const string src);
string stringLJust(const string src, int32_t width, UChar fillchar);
string stringRJust(const string src, int32_t width, UChar fillchar);
string stringCenter(const string src, int32_t width, UChar fillchar);
int32_t stringIndex(const string src, const string sub);
int32_t stringRFind(const string src, const string sub);
string stringLStrip(const string src, const string chars);
string stringRStrip(const string src, const string chars);
Partition stringPartition(const string src, const string sep);
string stringRemovePrefix(const string src, const string prefix);
string stringRemoveSuffix(const string src, const string suffix);
bool stringIsDecimal(const string src);
bool stringIsPrintable(const string src);
bool stringIsIdentifier(const string src);
string stringExpandTabs(const string src, int32_t tabsize);


#define chToStr(src) charToString(src)
#define toString(src) charToString(src)
#define str(src) charToString(src)
#define string(src) charToString(src)
#define stringIsWhitespace(string) u_isUWhiteSpace(string)

#endif