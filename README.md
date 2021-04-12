# CG

Exercícios resolvidos no âmbito da UC de Computação Gráfica, no ano letivo 2020/21. 

## Como correr os modelos

Nota: substituir as variáveis entre chavetas pelos valores apropriados.

```
> cd Aula[X]
> mkdir build
> cd build
> cmake .. -DTOOLKITS_FOLDER:STRING=[caminho absoluto para a pasta dos toolkits] -A Win32
> cmake --build .
> ./Debug/[nome do executável gerado]
```
