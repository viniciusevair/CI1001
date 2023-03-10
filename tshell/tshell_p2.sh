#!/bin/bash

###############################################
#  Script feito por Vinicius Evair da Silva   #
# para a disciplina de Programacao 1 (CI1001) #
###############################################

# Verifica se a quantidade minima de parametros foram passados.
if [ "$#" -le 1 ]; then
    # Se menos que o necessário foi passado,
    # explica o uso correto e termina o script com erro 1
    echo "Numero incorreto de parâmetros"
    echo "Uso: $0 <dcsv> <dsin>"
    exit 1
else
    # Guarda os parametros em variaveis mais intuitivas.
    DIRETORIO_ARTIGOS="$1"
    DIRETORIO_PROTEINAS="$2"
fi

# Verifica se o diretorio de artigos passado 
# eh um diretorio existente, termina o script com erro 2 se nao for.
if [ ! -d $DIRETORIO_ARTIGOS ]; then
    echo "Diretorio de artigos inexistente"
    exit 2
fi

# Verifica se o diretorio de fatores de transcrição
# eh um diretorio existente, termina o script com erro 2 se nao for.
if [ ! -d $DIRETORIO_PROTEINAS ]; then
    echo "Diretorio de fatores de transcrição inexistente"
    exit 2
fi

# Para cada fator de transcrição, busca em todos os arquivos ".csv"
# presentes no diretorio os sinônimos do fator e conta suas ocorrencias. 
# Imprime na tela a soma de todas as ocorrências 
# junto do nome presente no título do arquivo ".txt".
for PROTEINA in $(find "$DIRETORIO_PROTEINAS" -name "*.txt" | sort); do
    echo "$(basename "$PROTEINA" .txt):$(grep -i -w -F -f "$PROTEINA" "${DIRETORIO_ARTIGOS}/"*.csv | wc -l)"
done
