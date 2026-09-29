#include <lua.h>
#include <lauxlib.h>
#include <stdlib.h>
#include <stdio.h>

int process_homogenous_matrix(lua_State *L) {
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