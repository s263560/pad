#!/bin/bash
# Esse script serve para descompactar os arquivos e preparar o ambiente para os exercicios sobre PThreads.
# Autor: Prof. Andre Leon S. Gradvohl
# Ultima versao: Sab 30 Jan 2016 10:45:43 BRST
#
GRUPO=pthreads
ARQUIVO=${GRUPO}.tar

echo "Descompactando arquivo: ${ARQUIVO}" 
tar xf pthreads.tar

if [ $? -eq 0 ]; then
        echo "Arquivo (${ARQUIVO}) descompactado sem problemas";
        echo "Agora entre no diretorio ${GRUPO} e leia o arquivo LEIAME"
        echo -e "Use:\n\t cd ${GRUPO} ; cat LEIAME"
        exit 0;
else
        echo "Problemas na descompactacao do arquivo (${ARQUIVO}).";
        echo "Chame o professor.";
        exit 1;
fi
