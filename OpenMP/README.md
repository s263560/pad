# Diretório OpenMP

Esse diretório contém alguns códigos fonte com exemplos sobre OpenMP, utilizados na disciplina Computação de Alto Desempenho ministrada pelo prof. André Leon Sampaio Gradvohl, Dr. 

Os exemplos foram obtidos do [Tutorial](https://computing.llnl.gov/tutorials/openMP) de Blaise Barney, do Lawrence Livermore National Laboratory disponível em https://computing.llnl.gov/tutorials/openMP

Para compilar um arquivo especifico, basta usar o comando a seguir:
```
gcc -fopenmp fonte.c -o programa.o
```

Onde:
*   ``gcc`` é o compilador para a linguagem C;
*   ``-fopenmp`` é o parâmetro que indica que a biblioteca OpenMP deve ser usada;
*   ``fonte.c`` é o código fonte do programa;
*   ``-o programa.o`` é o nome do programa gerado.

Cuidado, pois alguns programas estão, intencionamente, com erros de compilação ou erros de lógica.

Se restarem dúvidas, entre em contato com o professor responsável no e-mail gradvohl@ft.unicamp.br
