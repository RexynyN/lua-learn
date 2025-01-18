--[[
Lua Básico ao Intermediário

Este script explica os principais conceitos da linguagem Lua,
do básico ao intermediário, com exemplos comentados para facilitar o aprendizado.
]]

-- Imprimindo uma mensagem na tela
print("Olá, mundo! Bem-vindo ao tutorial de Lua.")

-- Variáveis e tipos de dados
-- Lua é dinamicamente tipada, então você não precisa declarar o tipo de uma variável
local nome = "Daniel" -- String
local idade = 25        -- Número
local altura = 1.75     -- Número (float)
local ativo = true      -- Booleano

print("Nome:", nome, "Idade:", idade, "Altura:", altura, "Ativo:", ativo)

-- Operadores matemáticos
local soma = 10 + 5
local subtracao = 10 - 5
local multiplicacao = 10 * 5
local divisao = 10 / 5
local modulo = 10 % 3
local potencia = 2 ^ 3

print("Soma:", soma, "Subtração:", subtracao, "Multiplicação:", multiplicacao, "Divisão:", divisao, "Módulo:", modulo, "Potência:", potencia)

-- Estruturas condicionais
local idade = 18
if idade >= 18 then
    print("Você é maior de idade.")
else
    print("Você é menor de idade.")
end

-- Loops
-- While loop
local contador = 1
while contador <= 5 do
    print("Contador:", contador)
    contador = contador + 1
end

-- For loop
for i = 1, 5 do
    print("Iteração:", i)
end

-- Repeat-until loop
contador = 1
repeat
    print("Repeat contador:", contador)
    contador = contador + 1
until contador > 5

-- Funções
local function saudacao(nome)
    return "Olá, " .. nome .. "!"
end

print(saudacao("Lua"))

-- Tabelas (estrutura de dados versátil em Lua)
local pessoa = {
    nome = "Daniel",
    idade = 25,
    altura = 1.75
}

print("Nome da pessoa:", pessoa.nome)
print("Idade da pessoa:", pessoa.idade)

-- Adicionando novos campos na tabela
pessoa.profissao = "Programador"
print("Profissão:", pessoa.profissao)

-- Iterando sobre tabelas
for chave, valor in pairs(pessoa) do
    print(chave .. ":", valor)
end

-- Metatables e Metamethods (nível intermediário)
local mt = {
    __add = function(t1, t2)
        return {x = t1.x + t2.x, y = t1.y + t2.y}
    end
}

local ponto1 = {x = 2, y = 3}
local ponto2 = {x = 4, y = 5}

setmetatable(ponto1, mt)
setmetatable(ponto2, mt)

local ponto3 = ponto1 + ponto2
print("Ponto 3:", "x=" .. ponto3.x, "y=" .. ponto3.y)

-- Manipulação de strings
local texto = "Aprender Lua é divertido!"
print("Tamanho do texto:", #texto)
print("Texto em maiúsculas:", string.upper(texto))
print("Substituir 'Lua' por 'programação':", string.gsub(texto, "Lua", "programação"))

-- Manipulação de arquivos (básico)
local arquivo = io.open("teste.txt", "w")
if arquivo then
    arquivo:write("Este é um arquivo de teste criado em Lua.")
    arquivo:close()
    print("Arquivo criado com sucesso!")
else
    print("Erro ao criar o arquivo.")
end

-- Módulos e bibliotecas
-- Exemplo com a biblioteca matemática
local raiz = math.sqrt(16)
print("Raiz quadrada de 16:", raiz)

-- Pegando argumentos da linha de comando
print("\nArgumentos da linha de comando:")
for i = 0, #arg do
    print("arg[" .. i .. "]:", arg[i])
end

-- Interação com o usuário
print("\nDigite seu nome:")
local usuario_nome = io.read()
print("Olá, " .. usuario_nome .. "! Bem-vindo ao Lua.")

print("Digite sua idade:")
local usuario_idade = io.read("*n")
print("Você tem " .. usuario_idade .. " anos.")

-- Fim do tutorial
print("Parabéns! Você completou o tutorial básico ao intermediário de Lua.")
