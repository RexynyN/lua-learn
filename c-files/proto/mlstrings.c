#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unicode/ustring.h>
#include <unicode/uchar.h>
#include <unicode/ustdio.h>

#include "utils.h"
#include "mlstrings.h"

// 'casefold', 'center', 'count', 'encode', 
// 'expandtabs', 'format', 'format_map', 'index', 
// 'ljust', 'lstrip', 'maketrans', 'partition', 'removeprefix', 
// 'removesuffix', 'replace', 'rfind', 'rindex', 'rjust', 'rpartition', 
// 'rsplit', 'rstrip', 'swapcase', 
// 'title', 'translate', 'zfill'


void stringCheckError(UErrorCode status, const char* msg) {
    if (U_FAILURE(status)) {
        fprintf(stderr, "Erro ICU [%s]: %s\n", msg, u_errorName(status));
        exit(status);
    }
}

static int32_t _get_len(const string src) {
    return (src == NULL) ? 0 : stringLen(src);
}

bool stringIsAlnum(const string src) {
    int32_t len = _get_len(src);
    if (len == 0) return false;
    for (int32_t i = 0; i < len; i++) {
        if (!u_isalnum(src[i])) return false;
    }
    return true;
}

bool stringIsAlpha(const string src) {
    int32_t len = _get_len(src);
    if (len == 0) return false;
    for (int32_t i = 0; i < len; i++) {
        if (!u_isalpha(src[i])) return false;
    }
    return true;
}

bool stringIsAscii(const string src) {
    int32_t len = _get_len(src);
    if (len == 0) return true; // No Python, string vazia é ASCII
    for (int32_t i = 0; i < len; i++) {
        if (src[i] > 0x7F) return false;
    }
    return true;
}

bool stringIsDecimal(const string src) {
    int32_t len = _get_len(src);
    if (len == 0) return false;
    for (int32_t i = 0; i < len; i++) {
        if (u_charType(src[i]) != U_DECIMAL_DIGIT_NUMBER) return false;
    }
    return true;
}

bool stringIsIdentifier(const string src) {
    int32_t len = _get_len(src);
    if (len == 0) return false;
    // First character: Letter or Underline
    if (!u_isIDStart(src[0]) && src[0] != (character)'_') return false;
    // Rest: Letter, Number or Underline
    for (int32_t i = 1; i < len; i++) {
        if (!u_isIDPart(src[i]) && src[i] != (character)'_') return false;
    }
    return true;
}

bool stringIsLower(const string src) {
    int32_t len = _get_len(src);
    bool has_cased = false;
    if (len == 0) return false;
    for (int32_t i = 0; i < len; i++) {
        if (u_isUAlphabetic(src[i])) { // Verifica se é uma letra
            if (u_islower(src[i])) has_cased = true;
            else if (u_isupper(src[i]) || u_istitle(src[i])) return false;
        }
    }
    return has_cased;
}

bool stringIsNumeric(const string src) {
    int32_t len = stringLen(src);
    if (len == 0) return false;
    for (int32_t i = 0; i < len; i++) {
        if (u_getNumericValue(src[i]) == U_NO_NUMERIC_VALUE) return false;
    }
    return true;
}

bool stringIsPrintable(const string src) {
    int32_t len = _get_len(src);
    if (len == 0) return true;
    for (int32_t i = 0; i < len; i++) {
        if (!u_isprint(src[i]) && src[i] != (character)' ') return false;
    }
    return true;
}

bool stringIsSpace(const string src) {
    int32_t len = _get_len(src);
    if (len == 0) return false;
    for (int32_t i = 0; i < len; i++) {
        if (!u_isUWhiteSpace(src[i])) return false;
    }
    return true;
}

bool stringIsTitle(const string src) {
    int32_t len = _get_len(src);
    if (len == 0) return false;
    bool cased = false;
    bool previous_is_cased = false;

    for (int32_t i = 0; i < len; i++) {
        character c = src[i];
        if (u_isupper(c) || u_istitle(c)) {
            if (previous_is_cased) return false;
            previous_is_cased = true;
            cased = true;
        } else if (u_islower(c)) {
            if (!previous_is_cased) return false;
            previous_is_cased = true;
            cased = true;
        } else {
            previous_is_cased = false;
        }
    }
    return cased;
}

bool stringIsUpper(const string src) {
    int32_t len = _get_len(src);
    bool has_cased = false;
    if (len == 0) return false;
    for (int32_t i = 0; i < len; i++) {
        if (u_isUAlphabetic(src[i])) {
            if (u_isupper(src[i])) has_cased = true;
            else if (u_islower(src[i]) || u_istitle(src[i])) return false;
        }
    }
    return has_cased;
}

string stringUpper(const string src) {
    UErrorCode status = U_ZERO_ERROR;
    int32_t src_len = stringLen(src);
    int32_t dest_len = u_strToUpper(NULL, 0, src, src_len, NULL, &status);
    status = U_ZERO_ERROR;
    
    string dest = (string)malloc((dest_len + 1) * sizeof(character));
    u_strToUpper(dest, dest_len + 1, src, src_len, NULL, &status);
    return dest;
}

string stringLower(const string src) {
    UErrorCode status = U_ZERO_ERROR;
    int32_t src_len = stringLen(src);    
    int32_t dest_len = u_strToLower(NULL, 0, src, src_len, NULL, &status);
    status = U_ZERO_ERROR;
    
    string dest = (string)malloc((dest_len + 1) * sizeof(character));
    u_strToLower(dest, dest_len + 1, src, src_len, NULL, &status);
    return dest;
}

bool stringStartsWith(const string src, const string prefix) {
    int32_t prefix_len = stringLen(prefix);
    if (prefix_len > stringLen(src)) return false;
    return u_strncmp(src, prefix, prefix_len) == 0;
}

bool stringEndsWith(const string src, const string suffix) {
    int32_t src_len = stringLen(src);
    int32_t suffix_len = stringLen(suffix);
    if (suffix_len > src_len) return false;
    return u_strncmp(src + (src_len - suffix_len), suffix, suffix_len) == 0;
}

bool stringEquals(const string src1, const string src2){
    return u_strcmp(src1, src2) == 0; 
}


string stringStrip(const string src) {
    int32_t len = stringLen(src);
    if (len == 0) {
        string empty = (string)malloc(sizeof(character));
        empty[0] = 0;
        return empty;
    }

    int32_t start = 0;
    int32_t end = len - 1;

    while (start <= end && stringIsWhitespace(src[start])) start++;
    while (end >= start && stringIsWhitespace(src[end])) end--;

    int32_t new_len = (start > end) ? 0 : (end - start + 1);
    string dest = (string)malloc((new_len + 1) * sizeof(character));
    
    if (new_len > 0) {
        u_strncpy(dest, src + start, new_len);
    }

    dest[new_len] = 0;
    return dest;
}

string stringCapitalize(const string src) {
    int32_t len = stringLen(src);
    if (len == 0) return NULL;

    string dest = (string)malloc((len + 1) * sizeof(character));
    
    // First letter is upper
    character first[] = { src[0], 0 };
    string first_upper = stringUpper(first);
    dest[0] = first_upper[0];
    free(first_upper);
    
    // Rest is lower
    if (len > 1) {
        character rest[len];
        u_strncpy(rest, src + 1, len - 1);
        rest[len - 1] = 0;
        string rest_lower = stringLower(rest);
        u_strncpy(dest + 1, rest_lower, len - 1);
        free(rest_lower);
    }
    
    dest[len] = 0;
    return dest; 
}

SplittedString stringSplit(const string src, const character sep) {
    SplittedString ss = { .strings = NULL, .num = 0 };
    size_t len = stringLen(src);
    if (len == 0) return ss;

    /* Conta separadores para saber quantos tokens teremos */
    size_t sepCount = 0;
    for (size_t i = 0; i < len; i++)
        if (src[i] == sep) sepCount++;

    size_t tokens = sepCount + 1;
    string *arr = malloc(tokens * sizeof(string));
    if (!arr) { perror("malloc"); exit(EXIT_FAILURE); }

    size_t start = 0;
    size_t arrCounter = 0;
    for (size_t i = 0; i <= len; i++) { /* <= len para forçar o flush do último token */
        if (i == len || src[i] == sep) {
            size_t toklen = i - start;
            string tok = (string) malloc((toklen + 1) * sizeof(character));
            if (!tok) { perror("malloc"); exit(EXIT_FAILURE); }
            for (size_t j = 0; j < toklen; j++) tok[j] = src[start + j];
            tok[toklen] = 0;
            arr[arrCounter++] = tok;
            start = i + 1;
        }
    }

    ss.strings = arr;
    ss.num = (int)arrCounter;
    return ss;
}


SplittedString stringSplitStr(const string src, const string sep) {
    SplittedString ss = { .strings = NULL, .num = 0 };
    size_t len = stringLen(src);
    size_t sepLen = stringLen(sep);
    if (len == 0 || sepLen == 0) return ss;

    // Count separators
    size_t sepCount = 0;
    for (size_t i = 0; i < len; i++) {
        if (src[i] == sep[0]) {
            bool found = true;
            for (size_t j = 0; j < sepLen && i + j < len; j++) {
                if (src[i + j] != sep[j]) {
                    found = false; 
                    break; 
                }
            }            
            if (found) {
                sepCount++;
            }
        }
    }
        
    size_t tokens = sepCount + 1;
    string *arr = malloc(tokens * sizeof(string));
    if (!arr) { perror("malloc"); exit(EXIT_FAILURE); }

    size_t start = 0;
    size_t arrCounter = 0;
    for (size_t i = 0; i <= len; i++) { 
        bool isSeparator = false;
        if (i + sepLen <= len) {
            isSeparator = true;
            for (size_t j = 0; j < sepLen; j++) {
                if (src[i + j] != sep[j]) {
                    isSeparator = false;
                    break;
                }
            }
        }
        
        if (i == len || isSeparator) {
            size_t toklen = i - start;
            string tok = (string) malloc((toklen + 1) * sizeof(character));
            if (!tok) { perror("malloc"); exit(EXIT_FAILURE); }
            for (size_t j = 0; j < toklen; j++) tok[j] = src[start + j];
            tok[toklen] = 0;
            arr[arrCounter++] = tok;
            start = i + sepLen;
            i += sepLen - 1;
        }
    }

    ss.strings = arr;
    ss.num = (int)arrCounter;
    return ss;
}

string stringJoin(const string* strings, size_t len, string joiner) {
    size_t stringLeng = 0; 
    for (size_t i = 0; i < len; i++)
        stringLeng += stringLen(strings[i]);
        
    size_t joinLen = stringLen(joiner);
    stringLeng += (joinLen * (len - 1)) + 1; // For the null terminator
    string joinedString = ALLOC_ARRAY(joinedString, stringLeng);
    size_t strPointer = 0; 
    for (size_t i = 0; i < len-1; i++) {
        for (size_t j = 0; j < stringLen(strings[i]); j++) {
            joinedString[strPointer] = strings[i][j];
            strPointer++;
        }

        for (size_t j = 0; j < joinLen; j++) {
            joinedString[strPointer] = joiner[j];
            strPointer++;
        }
    }
    
    // Last one can't have the joiner at the end
    for (size_t j = 0; j < stringLen(strings[len-1]); j++) {
        joinedString[strPointer] = strings[len-1][j];
        strPointer++;
    }

    joinedString[stringLeng] = 0;
    return joinedString;
}

string charToString(const char* input_utf8) {
    UErrorCode status = U_ZERO_ERROR;
    
    int32_t dest_capacity = strlen(input_utf8) + 1;
    string dest = (string)malloc(dest_capacity * sizeof(character));
    
    u_strFromUTF8(dest, dest_capacity, NULL, input_utf8, -1, &status);
    stringCheckError(status, "Conversão inicial");

    return dest;
}

SplittedString stringSplitLines(const string src) {
    string newline = charToString("\n");
    SplittedString result = stringSplitStr(src, newline);
    free(newline);
    return result;
}

static string _reverse_string(const string src) {
    size_t len = stringLen(src); 
    string new = ALLOC_ARRAY(new, len + 1);
    len = len - 1; 
    for(size_t  i = 0; i < stringLen(src); i++) {
        new[len - i] = src[i];
    }

    new[len + 1] = 0; 
    return new;
}

static string _reversed_substring(const string src, size_t start, size_t end) {
    size_t len = stringLen(src); 

    start = len - start;
    end = len - end;

    string new = ALLOC_ARRAY(new, (end - start) + 2);
    len = len - 1; 
    for(size_t  i = 0; i < stringLen(src); i++) {
        new[len - i] = src[i];
    }
}

string stringSubstring(const string src, int start, int end) {
    if (start == -1 && end == -1) 
        return _reverse_string(src);

    size_t len = stringLen(src) -1;
    if(start < 0) {
        start = (len - abs(start)) + 1; 
        end = len - abs(end); 
        printf("Start: %d \nEnd: %d", start, end);
    }

    if(end == -1) 
        end = len;

    if(end > stringLen(src)) {
        printf("stringSubstring(): ERROR: 'end' index is higher than string length");
        return NULL; 
    }

    if(start > end) {
        printf("stringSubstring(): ERROR: 'start' index is higher than 'end'");
        return NULL; 
    }

    string new = ALLOC_ARRAY(new, (end - start) + 2); // Null terminator + index offset
    size_t strPointer = 0; 
    for(size_t i = start; i <= end; i++) {
        new[strPointer] = src[i];
        strPointer++;
    }
    
    new[(end - start) + 1] = 0; 
    return new; 
}

int stringCount(const string src, const string substr, int start, int end) {
    size_t len = stringLen(src);
    size_t subLen = stringLen(substr);

    if (len == 0 || subLen == 0) 
        return 0;

    // Count separators
    size_t sepCount = 0;
    for (size_t i = 0; i < len; i++) {
        if (src[i] == substr[0]) { // Found first character in substr
            bool found = true;
            for (size_t j = 0; j < subLen && i + j < len; j++) {
                // Found a character that is not in the substr 
                // or the string ended in the middle of the substr (is not the entire substr)
                if (src[i + j] != substr[j] || (j < subLen - 1 && i + j == len - 1)) {
                    found = false; 
                    break; 
                }
            }            

            if (found) {
                sepCount++;
                i += subLen - 1;
            }
        }
    }

    return sepCount;
}

int32_t stringFind(const string src, const string sub) {
    string pos = u_strstr(src, sub);
    if (pos == NULL) return -1;
    return (int32_t)(pos - src);
}



bool stringIsDigit(const string src) {
    int32_t len = stringLen(src);
    if (len == 0) return false;

    for (int32_t i = 0; i < len; i++) {
        if (!u_isdigit(src[i])) return false;
    }
    return true;
}


#define chToStr(src) charToString(src)
#define toString(src) charToString(src)
#define str(src) charToString(src)
#define string(src) charToString(src)
#define stringJoinChar(strings, len, joiner) stringJoin(strings, len, charToString(joiner))
#define stringIsWhitespace(string) u_isUWhiteSpace(string)


// gcc utils.c cottonstring.c -licuuc -licuio -o cottonml && ./cottonml
int main() {
    const char* raw_text = "  Olá Mundo! Γειά σου κόσμε  ";

    // Renomeamos a variável da ICU para 'u_src' (Unicode Source)
    string u_src = charToString(raw_text);

    // Agora use 'u_src' nos testes
    string lower = stringLower(u_src);
    string stripped = stringStrip(u_src);

    string cap = stringCapitalize(u_src);

    // Saída usando stdout da ICU
    UFILE* out = u_finit(stdout, NULL, NULL);
    u_fprintf(out, "Original: [%S]\n", u_src);
    u_fprintf(out, "Lower:    [%S]\n", lower);
    u_fprintf(out, "Strip:    [%S]\n", stripped);
    u_fprintf(out, "Capitalize:    [%S]\n", cap);
    // u_fprintf(out, "Find 'Mundo': %d\n", stringFind(src, stripped + 4)); // Exemplo simples de find

    u_fprintf(out, "Splitted: \n");
    SplittedString split = stringSplit(stripped, ' ');
    for(int i = 0; i < split.num; i++) {
        u_fprintf(out, "\t[%S]\n", split.strings[i]);
    }

    string joined = stringJoinChar(split.strings, split.num, " - ");
    u_fprintf(out, "\t[%S]\n", joined);

    // Mundo! Γειά 
    string substring = stringSubstring(stripped, -53, 0);
    u_fprintf(out, "\tSubstring: [%S]\n", substring);

    size_t counter = stringCount(str("asdiubngbrenoaiodnbrenobrenoasfbregahg"), str("breno"), -5, -2);
    printf("\tCounter: [%zu]\n", counter);

    // Limpeza
    free(u_src);
    free(lower);
    free(stripped);
    u_fclose(out);

    return 0;
}