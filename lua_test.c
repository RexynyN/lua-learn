#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include <stdio.h>
// gcc test_lua.c -o main $(pkg-config --cflags lua5.4) $(pkg-config --libs lua5.4)

// gcc -O2 -Wall -fPIC -shared -o test.so lua_test.c $(pkg-config --cflags lua5.4) $(pkg-config --libs lua5.4)
int add(lua_State *L) {
    double n1 = lua_tonumber(L, 1);
    double n2 = lua_tonumber(L, 2);

    lua_pushnumber(L, n1 + n2);
    return 1;
}

static const luaL_Reg function_list[] = {
    {"c_add",   add},
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