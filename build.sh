#!/usr/bin/env bash
echo "Starting build..."

# Usa o primeiro parâmetro passado ($1) ou "lua_test.c" como fallback
file="${1:-lua_test.c}"

gcc -O2 -Wall -fPIC -shared -o test.so "$file" $(pkg-config --cflags lua5.4) $(pkg-config --libs lua5.4)

echo "Build Done!"