#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>

#include "linearalg.h"
#include "dataset.h"
#include "utils.h"

#define FREAD_BUFFER 65536

void printDataset(Dataset *dataset) {
    printf("Dataset Features (%zu rows, %zu columns):\n", dataset->features.rows, dataset->features.columns);
    for (size_t i = 0; i < dataset->features.rows; i++) {
        for (size_t j = 0; j < dataset->features.columns; j++) {
            printf("%.2f ", dataset->features.data[i][j]);
        }
        printf("\n");
    }
    
    printf("Dataset Targets (%zu elements):\n", dataset->targets.len);
    for (size_t i = 0; i < dataset->targets.len; i++) {
        printf("%.2f ", dataset->targets.data[i]);
    }
    printf("\n");
}

// Gets the line count of a file and the FILE* pointer set to the start
size_t countFileLines(FILE* file) {
    fseek(file, 0, SEEK_SET); // Set to the start of the file
    char *stringBuffer = ALLOC_ARRAY(stringBuffer, FREAD_BUFFER);
    int counter = 0; 

    for(;;) {
        size_t nBytes = fread(stringBuffer, sizeof(char), FREAD_BUFFER, file);
        if (ferror(file)) 
            return -1; 

        for(size_t i = 0; i < nBytes; i++) {
            if(stringBuffer[i] == '\n')
                counter++; 
        }

        if(feof(file))
            break;
    }

    fseek(file, 0, SEEK_SET); // Set to the start of the file
    return counter;
}


// Gets the column count of a CSV and returns the FILE* pointer set to the start
size_t csvCountColumns(FILE* csvFile, char sep) {
    char *stringBuffer = ALLOC_ARRAY(stringBuffer, FREAD_BUFFER);
    int scanned = fread(stringBuffer, sizeof(char), FREAD_BUFFER, csvFile);
    size_t cols = 0; 
    for (size_t i = 0; i < scanned; i++) {
        if (stringBuffer[i] == sep)
            cols++;

        if (stringBuffer[i] == '\n') {
            for(;;) { // Line ends, but the last item may or may not have the separator, which misses 1 column
                if (stringBuffer[i] == sep) 
                    break; 
                
                if (!isspace(stringBuffer[i]) && stringBuffer[i] != sep){
                    cols++;
                    break;
                }
                i--;
            } 
            break;
        }
    }

    free(stringBuffer);
    fseek(csvFile, 0, SEEK_SET); // Set to the start of the file
    return cols;
}

CSVFile createCSV(char* path, char separator, int hasHeader) {
    CSVFile csv; 
    csv.filename = path;     
    csv.hasHeader = hasHeader;
    csv.separator = separator;

    FILE* csvfile = fopen(path, "r");
    if (csvfile == NULL) {
        printf("ERROR: The CSV path %s could not be opened! Check if the file exists.", csv.filename);
        exit(1);
    }

    size_t lines = countFileLines(csvfile);
    csv.rows = hasHeader ? lines - 1 : lines;
    csv.columns = csvCountColumns(csvfile, csv.separator);
    fclose(csvfile);
    return csv;
}



char* readFileToString(char* path){
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        printf("ERROR: The file path %s could not be opened! Check if the file exists.", path);
        exit(1);
    }

    // Get the file length in bytes
    fseek(file, 0, SEEK_END);
    size_t fileLength = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Create the buffer and the string where the file will sit
    char *entireFuckingFile = malloc(st_mult((fileLength + 1), sizeof(char))); // +1 for the null terminator "\0"
    char *stringBuffer = ALLOC_ARRAY(stringBuffer, FREAD_BUFFER);

    int entireIdx = 0, scanned = 0;
    for(;;) {
        scanned = fread(stringBuffer, sizeof(char), FREAD_BUFFER, file);
        
        // Does the csv string treatment
        for (size_t i = 0; i < scanned; i++) {
            entireFuckingFile[i] = stringBuffer[i];
            entireIdx++; 
        }

        if(feof(file))
            break; 
    }

    fclose(file);
    free(stringBuffer);
    entireFuckingFile[entireIdx] = '\0';
    return entireFuckingFile;  
}

Vector parseCsvLine(char* line, char sep, size_t size) {
    size_t lineLen = strlen(line) + 1, linePointer = 0, doubleP =0;
    char *newLine = ALLOC_ARRAY(newLine, lineLen);
    double* new_data = ALLOC_ARRAY(new_data, size);
    
    for (size_t i = 0; i < lineLen; i++){
        if(line[i] == sep || line[i] == '\0'){
            newLine[linePointer] = '\0';
            new_data[doubleP] = strtod(newLine, NULL);

            linePointer = 0; 
            doubleP = doubleP == size - 1 ? doubleP : doubleP + 1; 
            continue; 
        }

        newLine[linePointer] = line[i];
        linePointer++;
    }

    free(newLine);
    return _vector_from_array(new_data, size);
}

static void skipCsvHeader(FILE* csv, char* stringBuffer) {
    bool hitNewLine = false; 

    while(!hitNewLine) {
        int scanned = fread(stringBuffer, sizeof(char), FREAD_BUFFER, csv);
        for (size_t i = 0; i < scanned; i++) {
            if (stringBuffer[i] == '\n') {
                fseek(csv, i + 1, SEEK_SET);
                hitNewLine = true;
                break;
            }
        }
    }
} 

// Read all the contents of .csv file and converts then to double-filled Matrix
Matrix readCsvToMatrix(CSVFile csv){
    FILE *csvFile = fopen(csv.filename, "r");
    if (csvFile == NULL) {
        printf("ERROR: The CSV path %s could not be opened! Check if the file exists.", csv.filename);
        exit(1);
    }

    char *stringBuffer = ALLOC_ARRAY(stringBuffer, FREAD_BUFFER);
    char *lineBuilder = ALLOC_ARRAY(stringBuffer, FREAD_BUFFER + 1); // +1 for possible null terminator
    int scanned = 0, bufPointer = 0;

    if(csv.hasHeader) {
        skipCsvHeader(csvFile, stringBuffer);
    }

    Vector* vecs = malloc(st_mult(sizeof(Vector), csv.rows));
    size_t idxVecs = 0; 
    for(;;) {
        scanned = fread(stringBuffer, sizeof(char), FREAD_BUFFER, csvFile);
        // Does the csv string treatment
        for (size_t i = 0; i < scanned; i++) {
            lineBuilder[bufPointer] = stringBuffer[i];
            bufPointer++;
            if (stringBuffer[i] == '\n') {
                lineBuilder[bufPointer] = '\0';
                
                // It completes a line, and parses it
                Vector v = parseCsvLine(lineBuilder, csv.separator, csv.columns);
                vecs[idxVecs] = v;
                idxVecs++;
                bufPointer = 0;
            }
        }

        if(feof(csvFile))
            break; 
    }


    fclose(csvFile);
    free(stringBuffer);
    free(lineBuilder);
    return _matrix_from_vectors(vecs, csv.rows, csv.columns);   
}

// Read all the contents of .csv file and converts to a dataset 
// @param targetCol  is the 0-indexed columns of label/regression metric of a model
Dataset readCsvToDataset(CSVFile csv, int targetCol){
    if (targetCol + 1 > csv.columns) {
        printf("readCsvToDataset(): The target column is out of range\n");
        exit(1);
    }

    Matrix mat = readCsvToMatrix(csv); 
    Vector vec = matrixGetColumn(&mat, targetCol);

    double** matNew = ALLOC_ARRAY(matNew, mat.rows);
    for(size_t i = 0; i < mat.rows; i++) {
        matNew[i] = (double*) malloc(sizeof(double) * (mat.columns - 1));
        size_t newIdx = 0;
        for(size_t j = 0; j < mat.columns; j++) {
            if (j == targetCol) 
                continue;
            matNew[i][newIdx] = mat.data[i][j]; 
            newIdx++;
        }
        free(mat.data[i]);
    }


    free(mat.data);
    mat.data = matNew;
    mat.columns = mat.columns - 1;

    Dataset data; 
    data.targets = vec; 
    data.features = mat;

    return data; 
}

#include "knn.h"


#include <stdio.h>
#include <wchar.h>
#include <wctype.h>
#include <locale.h>

void to_lowercase_unicode(wchar_t* str) {
    // Set the locale to the user's default, which includes Unicode information
    // If the specific locale is known (e.g., "en_US.UTF-8" or "C.UTF-8"), it can be used instead
    setlocale(LC_ALL, ""); 

    if (str == NULL) {
        return;
    }

    for (int i = 0; str[i] != L'\0'; ++i) {
        str[i] = towlower(str[i]);
    }
}


int main(){
    // printf(readFileToString("Iris_only_numbers_no_header.csv"));
    // CSVFile irisDataset = createCSV("Iris_hot_encoded.csv", ',', 1);

    // Matrix matri = readCsvToMatrix(irisDataset);
    // print_matrix(matri);

    // Dataset m = readCsvToDataset(irisDataset, 1);

    // // printDataset(&m);
    // KNN knnModel = createKNNModel(m, 7, KNN_DIST_MINKOWSKI);

    // // 6.3,3.4,5.6,2.4,2
    // double d_data[] = {6.3,3.4,5.6,2.4};
    // Vector new = createVector(d_data);

    // printf("Class: %f\n", knnClassify(&knnModel, &new));



    // Use wide string literal with L prefix
    wchar_t unicode_string[] = L"HELLO WORLD! ÄÖÜ"; 

    wprintf(L"Original string: %ls\n", unicode_string);

    to_lowercase_unicode(unicode_string);

    wprintf(L"Lowercase string: %ls\n", unicode_string);



    return 0;
}