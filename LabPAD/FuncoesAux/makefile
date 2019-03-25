# Arquivo Makefile para compilar os programas auxiliares para 
# os projetos.
#
# Criado por Prof. Andre' Leon S. Gradvohl, Dr.
# e-mail: gradvohl@ft.unicamp.br
# Ultima versao: Seg 25 Mar 20:05:14 CET 2019

CC=gcc
DIRETIVA_PTHREAD= -pthread
DIRETIVA_OPENMP=-fopenmp
DIRETIVAS= -Wall -lm $(DIRETIVA_PTHREAD) $(DIRETIVA_OPENMP)

#Arquivos
FONTES_AUX_1= generateRandomMatrix.c MatrixIO.c
FONTES=$(FONTES_AUX_1) MatrixOperationsOMP.c
OBJETOS=$(FONTES:.c=.o)

#Dependencias de Compilacao
all: $(OBJETOS)

generateRandomMatrix.o: generateRandomMatrix.c MatrixIO.c MatrixIO.h
	@echo "Compilando arquivo: " $<
	$(CC)  MatrixIO.c generateRandomMatrix.c -o $@ -Wall
	@echo "Arquivo " $@ "gerado"

MatrixOperationsOMP.o: MatrixOperationsOMP.c MatrixOperationsOMP.h MatrixIO.c MatrixIO.h
	@echo "Compilando arquivo: " $<
	$(CC)  MatrixIO.c MatrixOperationsOMP.c -o $@ $(DIRETIVAS)
	@echo "Arquivo " $@ "gerado"

#Limpeza
clean:
	rm -f $(OBJETOS)
