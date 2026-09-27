#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unicode/ustring.h>
#include <unicode/uchar.h>
#include <unicode/ustdio.h>

#include "utils.h"
#include "mlstrings.h"

// DISCLAIMER: THIS CODE IS AI-GENERATED.  
// There is a first prototype which was human-made on the "proto" folder in this project
// However, for simplicity and time constraints I decided on implement this as quickly as possible.
// This code follows a python-like API for easy string operations, which was based on the human version of the code.

/**
 * @brief Returns the length of the ICU string.
 */
int32_t stringLen(const string src) {
    if (src == NULL) return 0;
    return u_strlen(src);
}

/**
 * @brief Utility to handle ICU errors and exit.
 */
void stringCheckError(UErrorCode status, const char* msg) {
    if (U_FAILURE(status)) {
        fprintf(stderr, "ICU Error [%s]: %s\n", msg, u_errorName(status));
        exit(status);
    }
}

bool stringEquals(const string src1, const string src2){
    return u_strcmp(src1, src2) == 0; 
}

/**
 * @brief Converts a standard UTF-8 char* to an ICU string (UTF-16).
 * Edge cases: Returns NULL if input is NULL. Handles surrogate pairs by doubling capacity if needed.
 */
string charToString(const char* input_utf8) {
    if (input_utf8 == NULL) return NULL;
    UErrorCode status = U_ZERO_ERROR;
    int32_t dest_capacity = (int32_t)strlen(input_utf8) + 1;
    string dest = ALLOC_ARRAY(dest, dest_capacity);
    
    u_strFromUTF8(dest, dest_capacity, NULL, input_utf8, -1, &status);
    if (status == U_BUFFER_OVERFLOW_ERROR) {
        status = U_ZERO_ERROR;
        free(dest);
        dest = ALLOC_ARRAY(dest, dest_capacity * 2);
        u_strFromUTF8(dest, dest_capacity * 2, NULL, input_utf8, -1, &status);
    }
    stringCheckError(status, "charToString conversion");
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

/**
 * @brief Returns a copy of the string in uppercase.
 */
string stringUpper(const string src) {
    UErrorCode status = U_ZERO_ERROR;
    int32_t len = stringLen(src);
    int32_t dest_len = u_strToUpper(NULL, 0, src, len, NULL, &status);
    status = U_ZERO_ERROR;
    string dest = ALLOC_ARRAY(dest, dest_len + 1);
    u_strToUpper(dest, dest_len + 1, src, len, NULL, &status);
    return dest;
}

/**
 * @brief Returns a copy of the string in lowercase.
 */
string stringLower(const string src) {
    UErrorCode status = U_ZERO_ERROR;
    int32_t len = stringLen(src);
    int32_t dest_len = u_strToLower(NULL, 0, src, len, NULL, &status);
    status = U_ZERO_ERROR;
    string dest = ALLOC_ARRAY(dest, dest_len + 1);
    u_strToLower(dest, dest_len + 1, src, len, NULL, &status);
    return dest;
}

/**
 * @brief Capitalizes the first character and lowers the rest.
 * Edge cases: Returns NULL on empty.
 */
string stringCapitalize(const string src) {
    int32_t len = stringLen(src);
    if (len == 0) return charToString("");
    string dest = ALLOC_ARRAY(dest, len + 1);
    
    // First char upper
    UErrorCode status = U_ZERO_ERROR;
    u_strToUpper(dest, 2, src, 1, NULL, &status);
    
    // Rest lower
    if (len > 1) {
        u_strToLower(dest + 1, len, src + 1, len - 1, NULL, &status);
    }
    dest[len] = 0;
    return dest; 
}

/**
 * @brief Swaps case: lower to upper and vice versa.
 */
string stringSwapCase(const string src) {
    int32_t len = stringLen(src);
    string dest = ALLOC_ARRAY(dest, len + 1);
    for (int32_t i = 0; i < len; i++) {
        if (u_islower(src[i])) dest[i] = u_toupper(src[i]);
        else if (u_isupper(src[i])) dest[i] = u_tolower(src[i]);
        else dest[i] = src[i];
    }
    dest[len] = 0;
    return dest;
}

/**
 * @brief Slices the string from start to end with Python logic.
 * Handles negative indices (offsets from end) and clamping.
 * Special case: start=-1, end=-1 reverses the string
 */
string stringSubstring(const string src, int32_t start, int32_t end) {
    int32_t len = stringLen(src);
    if (start == -1 && end == -1) {
        string rev = ALLOC_ARRAY(rev, len + 1);
        for(int32_t i = 0; i < len; i++) rev[len - 1 - i] = src[i];
        rev[len] = 0;
        return rev;
    }

    if (start < 0) start += len;
    if (end < 0) end += len;
    if (start < 0) start = 0; if (start > len) start = len;
    if (end < 0) end = 0; if (end > len) end = len;

    int32_t res_len = (start < end) ? (end - start) : 0;
    string dest = ALLOC_ARRAY(dest, res_len + 1);
    if (res_len > 0) u_strncpy(dest, src + start, res_len);
    dest[res_len] = 0;
    return dest;
}

/**
 * @brief Counts non-overlapping occurrences of substr in src[start:end].
 */
int32_t stringCount(const string src, const string substr, int32_t start, int32_t end) {
    string slice = stringSubstring(src, start, end);
    int32_t sub_len = stringLen(substr);
    int32_t count = 0;
    if (stringLen(slice) > 0 && sub_len > 0) {
        const character* curr = slice;
        while ((curr = u_strstr(curr, substr)) != NULL) {
            count++;
            curr += sub_len;
        }
    }
    free(slice);
    return count;
}

/**
 * @brief Returns the index of the first occurrence of sub. Returns -1 if not found.
 */
int32_t stringFind(const string src, const string sub) {
    string pos = u_strstr(src, sub);
    return (pos == NULL) ? -1 : (int32_t)(pos - src);
}

bool stringIsAlnum(const string src) {
    int32_t len = stringLen(src);
    if (len == 0) return false;
    for (int32_t i = 0; i < len; i++) if (!u_isalnum(src[i])) return false;
    return true;
}

bool stringIsAlpha(const string src) {
    int32_t len = stringLen(src);
    if (len == 0) return false;
    for (int32_t i = 0; i < len; i++) if (!u_isalpha(src[i])) return false;
    return true;
}

bool stringIsAscii(const string src) {
    int32_t len = stringLen(src);
    for (int32_t i = 0; i < len; i++) if (src[i] > 0x7F) return false;
    return true;
}

bool stringIsDigit(const string src) {
    int32_t len = stringLen(src);
    if (len == 0) return false;
    for (int32_t i = 0; i < len; i++) if (!u_isdigit(src[i])) return false;
    return true;
}

bool stringIsNumeric(const string src) {
    int32_t len = stringLen(src);
    if (len == 0) return false;
    for (int32_t i = 0; i < len; i++) if (u_getNumericValue(src[i]) == U_NO_NUMERIC_VALUE) return false;
    return true;
}

bool stringIsSpace(const string src) {
    int32_t len = stringLen(src);
    if (len == 0) return false;
    for (int32_t i = 0; i < len; i++) if (!u_isUWhiteSpace(src[i])) return false;
    return true;
}

bool stringIsLower(const string src) {
    int32_t len = stringLen(src);
    bool has_cased = false;
    for (int32_t i = 0; i < len; i++) {
        if (u_isUAlphabetic(src[i])) {
            if (u_isupper(src[i])) return false;
            if (u_islower(src[i])) has_cased = true;
        }
    }
    return has_cased;
}

bool stringIsUpper(const string src) {
    int32_t len = stringLen(src);
    bool has_cased = false;
    for (int32_t i = 0; i < len; i++) {
        if (u_isUAlphabetic(src[i])) {
            if (u_islower(src[i])) return false;
            if (u_isupper(src[i])) has_cased = true;
        }
    }
    return has_cased;
}

/**
 * @brief Pads string with '0' on the left until it reaches width.
 */
string stringZFill(const string src, int32_t width) {
    int32_t len = stringLen(src);
    if (width <= len) return stringSubstring(src, 0, len);
    string dest = ALLOC_ARRAY(dest, width + 1);
    for (int32_t i = 0; i < width - len; i++) dest[i] = (character)'0';
    u_strncpy(dest + (width - len), src, len);
    dest[width] = 0;
    return dest;
}

/**
 * @brief Replaces 'old_sub' with 'new_sub' up to 'max_replace' times (-1 for all).
 * Edge cases: Calculates dynamic length to prevent buffer overflow.
 */
string stringReplace(const string src, const string old_sub, const string new_sub, int32_t max_replace) {
    int32_t src_len = stringLen(src), old_len = stringLen(old_sub), new_len = stringLen(new_sub);
    if (old_len == 0) return stringSubstring(src, 0, src_len);

    int32_t count = 0;
    const character* tmp = src;
    while ((tmp = u_strstr(tmp, old_sub)) != NULL) {
        count++; tmp += old_len;
        if (max_replace != -1 && count >= max_replace) break;
    }

    int32_t final_len = src_len + count * (new_len - old_len);
    string result = ALLOC_ARRAY(result, final_len + 1);
    character* d = result; const character* s = src;
    for (int32_t i = 0; i < count; i++) {
        const character* next = u_strstr(s, old_sub);
        int32_t gap = (int32_t)(next - s);
        u_strncpy(d, s, gap); d += gap;
        u_strncpy(d, new_sub, new_len); d += new_len;
        s = next + old_len;
    }
    u_strncpy(d, s, src_len - (int32_t)(s - src));
    result[final_len] = 0;
    return result;
}

/**
 * @brief Joins an array of strings with a joiner.
 */
string stringJoin(const string* strings, int num, const string joiner) {
    if (num <= 0) return charToString("");
    int32_t j_len = stringLen(joiner), total = 0;
    for (int i = 0; i < num; i++) total += stringLen(strings[i]);
    total += j_len * (num - 1);

    string res = ALLOC_ARRAY(res, total + 1);
    character* p = res;
    for (int i = 0; i < num; i++) {
        int32_t s_len = stringLen(strings[i]);
        u_strncpy(p, strings[i], s_len); p += s_len;
        if (i < num - 1) { u_strncpy(p, joiner, j_len); p += j_len; }
    }
    *p = 0;
    return res;
}

/**
 * @brief Strips whitespace from both ends.
 */
string stringStrip(const string src) {
    int32_t start = 0, end = stringLen(src) - 1;
    while (start <= end && u_isUWhiteSpace(src[start])) start++;
    while (end >= start && u_isUWhiteSpace(src[end])) end--;
    return stringSubstring(src, start, end + 1);
}

/**
 * @brief Returns a casefolded copy of the string. Casefolding is similar to lowercasing but more aggressive 
 * because it is intended to remove all case distinctions in a string.
 */
string stringCaseFold(const string src) {
    UErrorCode status = U_ZERO_ERROR;
    int32_t len = stringLen(src);
    int32_t dest_len = u_strFoldCase(NULL, 0, src, len, U_FOLD_CASE_DEFAULT, &status);
    status = U_ZERO_ERROR;
    string dest = ALLOC_ARRAY(dest, dest_len + 1);
    u_strFoldCase(dest, dest_len + 1, src, len, U_FOLD_CASE_DEFAULT, &status);
    return dest;
}

/**
 * @brief Returns a copy of the string where words start with an uppercase character and the remaining characters are lowercase.
 */
string stringTitle(const string src) {
    UErrorCode status = U_ZERO_ERROR;
    int32_t len = stringLen(src);
    int32_t dest_len = u_strToTitle(NULL, 0, src, len, NULL, NULL, &status);
    status = U_ZERO_ERROR;
    string dest = ALLOC_ARRAY(dest, dest_len + 1);
    u_strToTitle(dest, dest_len + 1, src, len, NULL, NULL, &status);
    return dest;
}

/**
 * @brief Returns the string left justified in a string of length width. Padding is done using fillchar.
 */
string stringLJust(const string src, int32_t width, character fillchar) {
    int32_t len = stringLen(src);
    if (width <= len) return stringSubstring(src, 0, len);
    string dest = ALLOC_ARRAY(dest, width + 1);
    u_strncpy(dest, src, len);
    for (int32_t i = len; i < width; i++) dest[i] = fillchar;
    dest[width] = 0;
    return dest;
}

/**
 * @brief Returns the string right justified in a string of length width. Padding is done using fillchar.
 */
string stringRJust(const string src, int32_t width, character fillchar) {
    int32_t len = stringLen(src);
    if (width <= len) return stringSubstring(src, 0, len);
    string dest = ALLOC_ARRAY(dest, width + 1);
    int32_t pad = width - len;
    for (int32_t i = 0; i < pad; i++) dest[i] = fillchar;
    u_strncpy(dest + pad, src, len);
    dest[width] = 0;
    return dest;
}

/**
 * @brief Returns the string centered in a string of length width. Padding is done using fillchar.
 */
string stringCenter(const string src, int32_t width, character fillchar) {
    int32_t len = stringLen(src);
    if (width <= len) return stringSubstring(src, 0, len);
    string dest = ALLOC_ARRAY(dest, width + 1);
    int32_t total_pad = width - len;
    int32_t left_pad = total_pad / 2;
    for (int32_t i = 0; i < left_pad; i++) dest[i] = fillchar;
    u_strncpy(dest + left_pad, src, len);
    for (int32_t i = left_pad + len; i < width; i++) dest[i] = fillchar;
    dest[width] = 0;
    return dest;
}

/**
 * @brief Like stringFind, but returns -1 if the substring is not found (Python raises ValueError).
 */
int32_t stringIndex(const string src, const string sub) {
    return stringFind(src, sub);
}

/**
 * @brief Returns the highest index in the string where substring sub is found. Returns -1 on failure.
 */
int32_t stringRFind(const string src, const string sub) {
    int32_t src_len = stringLen(src);
    int32_t sub_len = stringLen(sub);
    if (sub_len == 0) return src_len;
    // ICU doesn't have a direct u_strrstr, so we iterate backwards
    for (int32_t i = src_len - sub_len; i >= 0; i--) {
        if (u_strncmp(src + i, sub, sub_len) == 0) return i;
    }
    return -1;
}

/**
 * @brief Returns a copy of the string with leading characters removed. 
 * If chars is NULL, removes whitespace.
 */
string stringLStrip(const string src, const string chars) {
    int32_t len = stringLen(src);
    int32_t start = 0;
    while (start < len) {
        bool match = (chars == NULL) ? u_isUWhiteSpace(src[start]) : (u_strchr(chars, src[start]) != NULL);
        if (!match) break;
        start++;
    }
    return stringSubstring(src, start, len);
}

/**
 * @brief Returns a copy of the string with trailing characters removed.
 */
string stringRStrip(const string src, const string chars) {
    int32_t len = stringLen(src);
    int32_t end = len - 1;
    while (end >= 0) {
        bool match = (chars == NULL) ? u_isUWhiteSpace(src[end]) : (u_strchr(chars, src[end]) != NULL);
        if (!match) break;
        end--;
    }
    return stringSubstring(src, 0, end + 1);
}

/**
 * @brief Splits the string at the first occurrence of sep, and returns a struct with 3 strings: 
 * the part before the separator, the separator itself, and the part after the separator.
 */
Partition stringPartition(const string src, const string sep) {
    Partition p = {NULL, NULL, NULL};
    int32_t idx = stringFind(src, sep);
    if (idx == -1) {
        p.head = stringSubstring(src, 0, stringLen(src));
        p.sep = charToString("");
        p.tail = charToString("");
    } else {
        p.head = stringSubstring(src, 0, idx);
        p.sep = stringSubstring(src, idx, idx + stringLen(sep));
        p.tail = stringSubstring(src, idx + stringLen(sep), stringLen(src));
    }
    return p;
}

/**
 * @brief Returns a copy of the string with the prefix removed if it exists.
 */
string stringRemovePrefix(const string src, const string prefix) {
    if (stringStartsWith(src, prefix)) {
        return stringSubstring(src, stringLen(prefix), stringLen(src));
    }
    return stringSubstring(src, 0, stringLen(src));
}

/**
 * @brief Returns a copy of the string with the suffix removed if it exists.
 */
string stringRemoveSuffix(const string src, const string suffix) {
    if (stringEndsWith(src, suffix)) {
        return stringSubstring(src, 0, stringLen(src) - stringLen(suffix));
    }
    return stringSubstring(src, 0, stringLen(src));
}

/**
 * @brief Returns true if all characters in the string are decimal characters (Unicode Category Nd).
 */
bool stringIsDecimal(const string src) {
    int32_t len = stringLen(src);
    if (len == 0) return false;
    for (int32_t i = 0; i < len; i++) {
        if (u_charType(src[i]) != U_DECIMAL_DIGIT_NUMBER) return false;
    }
    return true;
}

/**
 * @brief Returns true if all characters in the string are printable or the string is empty.
 */
bool stringIsPrintable(const string src) {
    int32_t len = stringLen(src);
    for (int32_t i = 0; i < len; i++) {
        if (!u_isprint(src[i]) && src[i] != (character)' ') return false;
    }
    return true;
}

/**
 * @brief Returns true if the string is a valid identifier according to the Python language definition.
 */
bool stringIsIdentifier(const string src) {
    int32_t len = stringLen(src);
    if (len == 0) return false;
    if (!u_isIDStart(src[0]) && src[0] != (character)'_') return false;
    for (int32_t i = 1; i < len; i++) {
        if (!u_isIDPart(src[i]) && src[i] != (character)'_') return false;
    }
    return true;
}

/**
 * @brief Returns a copy of the string where all tab characters are replaced by one or more spaces, 
 * depending on the current column and the given tab size.
 */
string stringExpandTabs(const string src, int32_t tabsize) {
    int32_t len = stringLen(src);
    // Preliminary size calculation
    int32_t new_capacity = len * tabsize + 1; 
    string dest = ALLOC_ARRAY(dest, new_capacity);
    int32_t column = 0, dest_idx = 0;

    for (int32_t i = 0; i < len; i++) {
        if (src[i] == (character)'\t') {
            int32_t spaces = tabsize - (column % tabsize);
            for (int32_t j = 0; j < spaces; j++) {
                dest[dest_idx++] = (character)' ';
                column++;
            }
        } else {
            dest[dest_idx++] = src[i];
            if (src[i] == (character)'\n' || src[i] == (character)'\r') column = 0;
            else column++;
        }
    }
    dest[dest_idx] = 0;
    return dest;
}


// /* --- Main Example --- */
// int main() {
//     string s = charToString("  Python ICU string!  ");
//     string stripped = stringStrip(s);
//     string upper = stringUpper(stripped);
//     string replaced = stringReplace(upper, charToString("ICU"), charToString("COTTON"), -1);
//     string zfilled = stringZFill(replaced, 30);

//     UFILE* out = u_finit(stdout, NULL, NULL);
//     u_fprintf(out, "Original: [%S]\n", s);
//     u_fprintf(out, "Stripped: [%S]\n", stripped);
//     u_fprintf(out, "Replace:  [%S]\n", replaced);
//     u_fprintf(out, "ZFill:    [%S]\n", zfilled);
//     u_fprintf(out, "IsDigit:  %d\n", stringIsDigit(zfilled));

//     // Cleanup
//     free(s); free(stripped); free(upper); free(replaced); free(zfilled);
//     u_fclose(out);
//     return 0;
// }