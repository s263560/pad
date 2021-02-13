# Arquivo Makefile para compilar os programas que exemplificam
# o uso de threads.
#
# Esse arquivo Makefile foi criado para trabalhar tanto em Sistemas AIX
# quanto Ubuntu e Fedora.
#
# Criado por Prof. Andre' Leon S. Gradvohl, Dr.
# e-mail: gradvohl@ft.unicamp.br
# Ultima versao: Seg 25 Mar 18:42:22 CET 2019
#
# Determinando o ambiente onde estamos
CC=gcc
DIRETIVA_OPENMP=-fopenmp
DIRETIVAS=-Wall -lm $(DIRETIVA_OPENMP)

#Arquivos
FONTES=omp_barrier.c omp_critical.c omp_hello.c omp_prod_vet.c  omp_sections.c
OBJETOS=$(FONTES:.c=.o)

#Dependencias de Compilacao
all: $(OBJETOS)

omp_barrier.o: omp_barrier.c
	@echo "Compilando arquivo: " $<
	$(CC) $(DIRETIVAS) $< -o $@ 
	@echo "Arquivo " $@ "gerado"

omp_critical.o: omp_critical.c
	@echo "Compilando arquivo: " $<
	$(CC) $(DIRETIVAS) $< -o $@
	@echo "Arquivo " $@ "gerado"

omp_hello.o: omp_hello.c
	@echo "Compilando arquivo: " $<
	$(CC) $(DIRETIVAS) $< -o $@
	@echo "Arquivo " $@ "gerado"

omp_prod_vet.o: omp_prod_vet.c
	@echo "Compilando arquivo: " $<
	$(CC) $(DIRETIVAS) $< -o $@
	@echo "Arquivo " $@ "gerado"

omp_sections.o: omp_sections.c  
	@echo "Compilando arquivo: " $<
	$(CC) $(DIRETIVAS) $< -o $@
	@echo "Arquivo " $@ "gerado"

#Limpeza
clean:
	rm -f $(OBJETOS)
