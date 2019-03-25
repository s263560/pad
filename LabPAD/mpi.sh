#!/bin/bash
# Esse script serve para descompactar os arquivos e preparar o ambiente para os exercicios sobre MPI.
# Autor: Prof. Andre Leon S. Gradvohl
# Last version: Seg 02 Mai 2016 09:23:15 BRT
#
GRUPO=mpi
ARQUIVO=${GRUPO}.tar

echo "Descompactando arquivo: ${ARQUIVO}"
/bin/tar xf ${ARQUIVO}

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
