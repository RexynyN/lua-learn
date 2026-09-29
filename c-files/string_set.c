#include <stdlib.h>
#include <stdio.h> 
#include <string.h>

#include "string_set.h"
#include "utils.h"

static int __set_str_cmp(const void *a, const void *b) {
    const char *str_a = *(const char **)a;
    const char *str_b = *(const char **)b;
    return strcmp(str_a, str_b);
}

// Retorna o índice de uma string dentro do Set ou (size_t)-1 se não encontrada
static size_t __set_val_index(SetString* set, const char* val) {
    if (set->sorted) {
        char** item = (char**) bsearch(&val, set->data, set->size, sizeof(char*), __set_str_cmp);
        if (item) 
            return (size_t)(item - set->data);
        return (size_t)-1;
    }

    for (size_t i = 0; i < set->size; i++) {
        if (strcmp(set->data[i], val) == 0) 
            return i;
    }

    return (size_t)-1; 
}

// Expande o vetor de ponteiros
static void __set_expd_arr(SetString* set) {
    size_t newSize = set->capacity == 0 ? 4 : set->capacity + (set->capacity / 2);
    char** newPtr = (char**) realloc(set->data, newSize * sizeof(char*));

    if (!newPtr) {
        perror("ERROR: Realloc did not return a valid memory address in SetString expand array");
        return;
    }

    set->data = newPtr;
    set->capacity = newSize;
}

static size_t __set_add_val(SetString* set, const char* val) {
    if (set->size + 1 > set->capacity) {
        __set_expd_arr(set);
    }

    size_t idx = set->size;
    set->data[idx] = strdup(val); // Duplica a string para gestão própria de memória
    set->size += 1;

    // Se a nova string for lexicograficamente menor que a anterior, perde a ordenação
    if (set->size > 1 && set->sorted && strcmp(set->data[idx - 1], set->data[idx]) > 0) {
        set->sorted = 0;
    }

    return idx;
}

SetString* setStringNewFull(char** data, size_t capacity, size_t size, int sorted) {
    SetString* set = (SetString*) malloc(sizeof(SetString));
    if (!set) return NULL;
    
    set->data = data; 
    set->capacity = capacity; 
    set->size = size;
    set->sorted = sorted; 

    return set; 
}

SetString* setStringNew(size_t capacity) {
    SetString* set = (SetString*) malloc(sizeof(SetString));
    if (!set) return NULL;

    set->data = capacity > 0 ? (char**) malloc(sizeof(char*) * capacity) : NULL;
    set->capacity = capacity; 
    set->size = 0;
    set->sorted = 1; 

    return set; 
}

void setStringFree(SetString* set) {
    if (!set) return;
    if (set->data) {
        for (size_t i = 0; i < set->size; i++) {
            free(set->data[i]);
        }
        free(set->data);
    }
    free(set);
}

size_t setStringAdd(SetString* set, const char* val) {
    size_t idx = __set_val_index(set, val);
    if (idx == (size_t)-1) {
        return __set_add_val(set, val);
    }

    return idx; 
}

void setStringAddArray(SetString* set, const char** arr, size_t size) {
    for (size_t i = 0; i < size; i++) {
        setStringAdd(set, arr[i]);
    }
    setStringSort(set);
}

void setStringSort(SetString* set) { 
    if (!set || set->sorted)
        return;

    qsort(set->data, set->size, sizeof(char*), __set_str_cmp);
    set->sorted = 1; 
}

static int checkSets(SetString* setA, SetString* setB) {
    if (!setA || !setB) return 0;

    if (!setA->sorted) {
        qsort(setA->data, setA->size, sizeof(char*), __set_str_cmp);
        setA->sorted = 1;
    }

    if (!setB->sorted) {
        qsort(setB->data, setB->size, sizeof(char*), __set_str_cmp);
        setB->sorted = 1;
    }

    return 1; 
}

SetString* setStringIntersection(SetString* setA, SetString* setB) {
    if (!checkSets(setA, setB))
        return NULL; 

    size_t maxSize = (setA->size < setB->size) ? setA->size : setB->size;
    if (maxSize == 0) {
        return setStringNewFull(NULL, 0, 0, 1);
    }

    char** newData = (char**) malloc(maxSize * sizeof(char*));
    if (!newData) return NULL;

    size_t i = 0, j = 0, pointer = 0;
    while (i < setA->size && j < setB->size) {
        int cmp = strcmp(setA->data[i], setB->data[j]);
        if (cmp == 0) {
            newData[pointer++] = strdup(setA->data[i]);
            i++;
            j++;
        } else if (cmp < 0) {
            i++;
        } else {
            j++;
        }
    }

    if (pointer == 0) {
        free(newData);
        newData = NULL;
    } else if (pointer < maxSize) {
        char** temp = (char**) realloc(newData, pointer * sizeof(char*));
        if (temp) {
            newData = temp;
        }
    }

    return setStringNewFull(newData, pointer, pointer, 1);
}

SetString* setStringUnion(SetString* setA, SetString* setB) {
    if (!checkSets(setA, setB))
        return NULL;

    size_t maxSize = setA->size + setB->size;
    if (maxSize == 0) {
        return setStringNewFull(NULL, 0, 0, 1);
    }

    char** newData = (char**) malloc(maxSize * sizeof(char*));
    if (!newData) return NULL;

    size_t i = 0, j = 0, pointer = 0;

    while (i < setA->size && j < setB->size) {
        int cmp = strcmp(setA->data[i], setB->data[j]);
        if (cmp < 0) {
            newData[pointer++] = strdup(setA->data[i++]);
        } else if (cmp > 0) {
            newData[pointer++] = strdup(setB->data[j++]);
        } else {
            newData[pointer++] = strdup(setA->data[i]);
            i++;
            j++;
        }
    }

    while (i < setA->size) newData[pointer++] = strdup(setA->data[i++]);
    while (j < setB->size) newData[pointer++] = strdup(setB->data[j++]);

    if (pointer == 0) {
        free(newData);
        newData = NULL;
    } else if (pointer < maxSize) {
        char** temp = (char**) realloc(newData, pointer * sizeof(char*));
        if (temp) {
            newData = temp;
        }
    }

    return setStringNewFull(newData, pointer, pointer, 1);
}

SetString* setStringDifference(SetString* setA, SetString* setB) {
    if (!checkSets(setA, setB))
        return NULL; 

    if (setA->size == 0) {
        return setStringNewFull(NULL, 0, 0, 1);
    }

    char** newData = (char**) malloc(setA->size * sizeof(char*));
    if (!newData) return NULL;

    size_t i = 0, j = 0, pointer = 0;
    while (i < setA->size && j < setB->size) {
        int cmp = strcmp(setA->data[i], setB->data[j]);
        if (cmp < 0) {
            newData[pointer++] = strdup(setA->data[i++]);
        } else if (cmp == 0) {
            i++;
            j++;
        } else {
            j++;
        }
    }
    
    while (i < setA->size) {
        newData[pointer++] = strdup(setA->data[i++]);
    }

    if (pointer == 0) {
        free(newData);
        newData = NULL;
    } else if (pointer < setA->size) {
        char** temp = (char**) realloc(newData, pointer * sizeof(char*));
        if (temp) {
            newData = temp;
        }
    }

    return setStringNewFull(newData, pointer, pointer, 1);
}

SetString* setSimmStringDifference(SetString* setA, SetString* setB) {
    if (!checkSets(setA, setB)) 
        return NULL; 

    size_t maxSize = setA->size + setB->size;
    if (maxSize == 0) {
        return setStringNewFull(NULL, 0, 0, 1);
    }

    char** newData = (char**) malloc(maxSize * sizeof(char*));
    if (!newData) return NULL;

    size_t i = 0, j = 0, pointer = 0;

    while (i < setA->size && j < setB->size) {
        int cmp = strcmp(setA->data[i], setB->data[j]);
        if (cmp < 0) {
            newData[pointer++] = strdup(setA->data[i++]);
        } else if (cmp > 0) {
            newData[pointer++] = strdup(setB->data[j++]);
        } else {
            i++;
            j++;
        }
    }

    while (i < setA->size) newData[pointer++] = strdup(setA->data[i++]);
    while (j < setB->size) newData[pointer++] = strdup(setB->data[j++]);

    if (pointer == 0) {
        free(newData);
        newData = NULL;
    } else if (pointer < maxSize) {
        char** temp = (char**) realloc(newData, pointer * sizeof(char*));
        if (temp) {
            newData = temp;
        }
    }

    return setStringNewFull(newData, pointer, pointer, 1);
}


int* setStringIntLabels(SetString* set, char** strLabels, int* labelLen) {
    if(!set->sorted) {
        setStringSort(set);
    }

    int* intLabels = ALLOC_ARRAY(intLabels, set->size);
    for(size_t i = 0; i < set->size; i++) {
        intLabels[i] = i; 
    }

    if(strLabels == NULL)
        return intLabels; 
        
    char** new = ALLOC_ARRAY(new, set->size);
    for(size_t i = 0; i < set->size; i++) {
        new[i] = strdup(set->data[i]); 
    }

    if(labelLen != NULL) { 
        *(labelLen) = set->size;
    } 
    strLabels = new;
    return intLabels;
}