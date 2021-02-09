# Laboratório de Programação PThreads
___
## Prof. André Leon S. Gradvohl, Dr.
### gradvohl@ft.unicamp.br
___

Para este laboratório, você deverá produzir um relatório (arquivo no formato PDF ou no formato Markdown) com o resultado dos exercícios de programação propostos a seguir.

Os códigos fontes que você criou, além das instruções para compila-los (se puder providenciar um `makefile`, melhor) e executá-los devem estar disponíveis em um repositório Git e o relatório deve ser enviado pelo Moodle. Eventualmente, o relatório também pode estar disponível no repositório Git.

## Exercício
Crie um programa serial e um programa paralelo com PThreads que calcule a operação matricial D = A * B + C, onde todas as matrizes (A, B, C e D) têm dimensões n x n. Prepare-se para gerar gráficos para cada um dos itens a seguir.

1. Calcule o tempo de execução do programa serial para matrizes de tamanho n x n, onde n = 10, 100 e 1000.

2. Calcule o tempo de execução do programa paralelo para matrizes de tamanho n x n, onde n = 10, 100 e 1000, cada uma com 2, 4, 8, 16 e 32 _threads_.

3. Responda as perguntas a seguir no relatório:
   * Há necessidade de sincronização entre as _threads_ para resolver as operações?
   * Qual foi o _speedup_ em relação ao programa serial em cada uma das execuções?
   * Houve algum caso em que não houve _speedup_ em relação ao programa serial? Se houve, qual a razão para isso?
