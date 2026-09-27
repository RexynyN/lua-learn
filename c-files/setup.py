from setuptools import setup, Extension

# Define a extensão
module = Extension(
    'fastmath',              # Nome do módulo no Python (import fastmath)
    sources=['fastmath.c']   # Arquivos fonte
    # include_dirs=['/usr/local/include'] # Opcional: headers externos
    # libraries=['m']                     # Opcional: libs externas (ex: math lib 'm' no linux)
)

setup(
    name='fastmath',
    version='1.0',
    description='Exemplo de extensao C para calculos estatisticos',
    ext_modules=[module]
)




str().count()