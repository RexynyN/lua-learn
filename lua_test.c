#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

// gcc test_lua.c -o main $(pkg-config --cflags lua5.4) $(pkg-config --libs lua5.4)

// gcc -O2 -Wall -fPIC -shared -o test.so lua_test.c $(pkg-config --cflags lua5.4) $(pkg-config --libs lua5.4)
int add(lua_State *L) {
    double n1 = lua_tonumber(L, 1);
    double n2 = lua_tonumber(L, 2);

    lua_pushnumber(L, n1 + n2);
    return 1;
}

int parse_array_double(lua_State *L) {
    // 1. Garante que o parâmetro recebido seja uma tabela
    luaL_checktype(L, 1, LUA_TTABLE);

    // 2. Obtém o número de linhas (M)
    size_t rows = lua_rawlen(L, 1);
    if (rows == 0) {
        return luaL_error(L, "The matrix cannot be empty");
    }

    // 3. Obtém a 1ª linha para definir o número de colunas esperado (N)
    lua_rawgeti(L, 1, 1);
    if (!lua_istable(L, -1)) {
        lua_pop(L, 1);
        return luaL_error(L, "Row 1 is not a table!");
    }
    
    size_t expected_cols = lua_rawlen(L, -1);
    lua_pop(L, 1); // Remove a 1ª linha da pilha

    if (expected_cols == 0) {
        return luaL_error(L, "The matrix lines cannot be empty!");
    }

    printf("[DEBUG] Total de linhas detectadas: %zu\n", rows);
    printf("[DEBUG] Colunas esperadas por linha: %zu\n", expected_cols);

    // 4. Aloca memória contígua para a matriz (M x N)
    double *matrix = (double *)malloc(rows * expected_cols * sizeof(double));
    if (!matrix) {
        return luaL_error(L, "Memory allocation failure");
    }

    // 5. Percorre e valida cada linha
    for (size_t r = 1; r <= rows; r++) {
        lua_rawgeti(L, 1, r); // Empilha a tabela da linha 'r'
        
        if (!lua_istable(L, -1)) {
            free(matrix);
            return luaL_error(L, "Row %d is not a table!", (int)r);
        }

        size_t current_cols = lua_rawlen(L, -1);
        printf("[DEBUG] Lendo Linha %zu (%zu colunas encontadas)\n", r, current_cols);

        if (current_cols != expected_cols) {
            free(matrix);
            return luaL_error(L, 
                "Irregular matrix: row %d has %d columns, but expected %d columns", 
                (int)r, (int)current_cols, (int)expected_cols);
        }

        // Lê os elementos da linha
        for (size_t c = 1; c <= expected_cols; c++) {
            lua_rawgeti(L, -1, c); // Empilha o elemento [r][c]

            if (!lua_isnumber(L, -1)) {
                free(matrix);
                return luaL_error(L, "Element [%d][%d] is NOT a number.", (int)r, (int)c);
            }

            double val = lua_tonumber(L, -1);
            size_t idx = (r - 1) * expected_cols + (c - 1);
            matrix[idx] = val;

            printf("  [DEBUG] Matriz[%zu][%zu] (índice flat %zu) = %f\n", r, c, idx, val);

            lua_pop(L, 1); // Remove o número da pilha
        }

        lua_pop(L, 1); // Remove a tabela da linha da pilha
    }

    printf("[SUCCESS] Matriz %dx%d carregada perfeitamente na memória contígua!\n", (int)rows, (int)expected_cols);

    free(matrix);
    return 0;
}

int parse_array_string(lua_State *L) {
    // 1. Garante que o argumento principal seja uma tabela (matriz)
    luaL_checktype(L, 1, LUA_TTABLE);

    size_t rows = lua_rawlen(L, 1);
    if (rows == 0) {
        return luaL_error(L, "A matriz de strings não pode estar vazia.");
    }

    // 2. Obtém a 1ª linha para determinar a quantidade esperada de colunas
    lua_rawgeti(L, 1, 1);
    if (!lua_istable(L, -1)) {
        lua_pop(L, 1);
        return luaL_error(L, "A linha 1 precisa ser uma tabela.");
    }

    size_t expected_cols = lua_rawlen(L, -1);
    lua_pop(L, 1); // Remove a linha 1 da pilha

    if (expected_cols == 0) {
        return luaL_error(L, "As linhas da matriz não podem estar vazias.");
    }

    printf("[DEBUG] Matriz detectada: %zu linhas x %zu colunas\n", rows, expected_cols);

    // 3. Aloca um array de ponteiros para char (char**) na memória contígua
    // Cada elemento guardará o endereço de uma string alocada dinamicamente
    char **string_matrix = (char **)malloc(rows * expected_cols * sizeof(char *));
    if (!string_matrix) {
        return luaL_error(L, "Falha na alocação de memória.");
    }

    // 4. Percorre as linhas e colunas
    for (size_t r = 1; r <= rows; r++) {
        lua_rawgeti(L, 1, r); // Empilha a tabela da linha 'r'

        if (!lua_istable(L, -1)) {
            // Limpa a memória alocada até o momento em caso de erro
            for (size_t i = 0; i < (r - 1) * expected_cols; i++) free(string_matrix[i]);
            free(string_matrix);
            return luaL_error(L, "Linha %d não é uma tabela.", (int)r);
        }

        size_t current_cols = lua_rawlen(L, -1);
        if (current_cols != expected_cols) {
            for (size_t i = 0; i < (r - 1) * expected_cols; i++) free(string_matrix[i]);
            free(string_matrix);
            return luaL_error(L, "Matriz irregular na linha %d.", (int)r);
        }

        for (size_t c = 1; c <= expected_cols; c++) {
            lua_rawgeti(L, -1, c); // Pile up the string on index [r][c]

            // Checks if the element is a string (or is convertable to string)
            if (!lua_isstring(L, -1)) {
                for (size_t i = 0; i < (r - 1) * expected_cols + (c - 1); i++) free(string_matrix[i]);
                free(string_matrix);
                return luaL_error(L, "Elemento [%d][%d] não é uma string.", (int)r, (int)c);
            }
            
            // lua_tolstring brings the pointer of the string and its total byte size
            size_t bytes_len;
            const char *lua_str = lua_tolstring(L, -1, &bytes_len);
            
            // Clones the string to have full control on C's memory
            size_t idx = (r - 1) * expected_cols + (c - 1);
            string_matrix[idx] = strdup(lua_str);

            printf("  [DEBUG] Matriz[%zu][%zu] = \"%s\" (%zu bytes na memória)\n", r, c, string_matrix[idx], bytes_len);

            lua_pop(L, 1); // Remove string from stack
        }

        lua_pop(L, 1); // Remove table from stack
    }

    printf("[SUCCESS] Matriz de strings UTF-8 carregada com sucesso!\n");

    // Frees the allocated memory (each individual string and the full array)
    for (size_t i = 0; i < rows * expected_cols; i++) {
        free(string_matrix[i]);
    }
    free(string_matrix);

    return 0;
}


static const luaL_Reg function_list[] = {
    {"c_add",   add},
    {"c_parse_array_double", parse_array_double},
    {"c_parse_array_string", parse_array_string},
    {NULL, NULL}
};

LUAMOD_API int luaopen_test(lua_State *L) {
    luaL_newlib(L, function_list);
    return 1;
}

// int main(void) {
//     lua_State *L = luaL_newstate();
//     luaL_openlibs(L);

//     lua_register(L, "add", add);

//     luaL_dofile(L, "test.lua");

//     lua_getglobal(L, "x");

//     printf("%lf\n", lua_tonumber(L, -1));

//     lua_close(L);
//     return 0;
// }