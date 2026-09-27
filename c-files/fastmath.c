#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <math.h>

// Lógica C pura (sem saber que Python existe) para manter o código limpo
double calculate_mean(double sum, Py_ssize_t count) {
    if (count == 0) return 0.0;
    return sum / count;
}

// A função "wrapper" que o Python vai chamar
static PyObject* fastmath_analyze(PyObject* self, PyObject* args) {
    PyObject *py_list;
    
    // 1. Parsing: "O" significa que esperamos um Objeto Python genérico (a lista)
    if (!PyArg_ParseTuple(args, "O", &py_list)) {
        return NULL; // Erro de parsing já define a exceção
    }

    // 2. Validação de Tipo
    if (!PyList_Check(py_list)) {
        PyErr_SetString(PyExc_TypeError, "O argumento deve ser uma lista.");
        return NULL;
    }

    Py_ssize_t size = PyList_Size(py_list);
    double sum = 0.0;
    double sq_sum = 0.0;
    
    // 3. Iteração na Lista (Crítico: Performance e Referências)
    for (Py_ssize_t i = 0; i < size; i++) {
        // PyList_GetItem retorna uma "Borrowed Reference" (não precisa de decref)
        PyObject *item = PyList_GetItem(py_list, i);
        
        if (!PyNumber_Check(item)) {
            PyErr_SetString(PyExc_TypeError, "A lista deve conter apenas números.");
            return NULL;
        }

        // Converte Python Object -> C Double
        double val = PyFloat_AsDouble(item);
        if (PyErr_Occurred()) return NULL; // Checa se a conversão falhou

        sum += val;
        sq_sum += (val * val);
    }

    double mean = calculate_mean(sum, size);
    double variance = (size > 1) ? (sq_sum / size) - (mean * mean) : 0.0;
    double std_dev = sqrt(variance > 0 ? variance : 0.0);

    // 4. Construindo o Retorno (C -> Python Dict)
    return Py_BuildValue("{s:d, s:d, s:d}", 
                        "count", (double)size,
                        "sum", sum,
                        "std_dev", std_dev);

}

// 5. Tabela de Métodos do Módulo
static PyMethodDef FastMathMethods[] = {
    { "analyze", fastmath_analyze, METH_VARARGS, "Analisa uma lista de numeros e retorna estatisticas." },
    { NULL, NULL, 0, NULL } // Sentinela de fim
};

// 6. Definição do Módulo
static struct PyModuleDef fastmathmodule = {
    PyModuleDef_HEAD_INIT,
    "fastmath",   // Nome do módulo
    "Modulo de exemplo escrito em C", // Docstring
    -1, // Estado global (-1 mantém estado no global vars, ok para módulos simples)
    FastMathMethods
};

// 7. Função de Inicialização (O que o Python chama ao fazer 'import')
PyMODINIT_FUNC PyInit_fastmath(void) {
    return PyModule_Create(&fastmathmodule);
}


// Fread
#include <stdio.h>

typedef struct {
    int id;
    char nome[50];
} Registro;

int main() {
    FILE *arquivo;
    Registro reg;
    size_t lidos;

    arquivo = fopen("dados.bin", "rb"); // Abre arquivo binário para leitura
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        return 1;
    }

    // Lê um registro do arquivo
    lidos = fread(&reg, sizeof(Registro), 1, arquivo);

    if (lidos == 1) {
        printf("ID: %d, Nome: %s\n", reg.id, reg.nome);
    } else {
        printf("Não foi possível ler o registro.\n");
    }

    fclose(arquivo);
    return 0;
}
