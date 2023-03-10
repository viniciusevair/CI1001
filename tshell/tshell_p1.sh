#!/bin/bash

###############################################
#  Script feito por Vinicius Evair da Silva   #
# para a disciplina de Programacao 1 (CI1001) #
###############################################

# Verifica se a quantidade minima de parametros foram passados.
if [ "$#" -le 2 ]; then
    # Se menos que o necessário foi passado,
    # explica o uso correto e termina o script com erro 1
    echo "Numero incorreto de parâmetros"
    echo "Uso: $0 <dpm> <ds> <t1> <t2> <t3>"
    echo "<t2> e <t3> são opcionais"
    exit 1
else
    # Guarda os parametros em variaveis mais intuitivas.
    DIRETORIO_PUBMED="$1"
    DIRETORIO_SAIDA="$2"
    T1="$3"
fi

# Verifica a existencia dos parametros opcionais e também
# as guarda em variaveis com nomes mais intuitivos.
if [ ! -z "$4" ]; then
    T2="$4"
fi
if [ ! -z "$5" ]; then
    T3="$5"
fi

# Verifica se o parametro do diretorio de origem passado 
# eh um diretorio existente, termina o script com erro 2 se nao for.
if [ ! -d "$DIRETORIO_PUBMED" ]; then
    echo "Parâmetro \"$DIRETORIO_PUBMED\" não existe ou não é um diretório"
    exit 2
fi

# Verifica se o parametro do diretorio de destino passado
# eh um diretorio existente, cria o diretorio caso nao exista ainda.
if [ ! -d "$DIRETORIO_SAIDA" ]; then
    mkdir -p "$DIRETORIO_SAIDA"
fi

# Para cada arquivo .csv no diretorio de origem,
# busca pelas linhas que contenham os parametros T1, T2 e T3.
# Cria um arquivo no diretorio de destino com o mesmo nome do arquivo original,
# contendo apenas as linhas com todos os parametros.
for ARQUIVO in $(find "$DIRETORIO_PUBMED" -name "*.csv"); do
    # A flag -w foi adicionada ao grep para garantir a pesquisa correta de
    # termos muito pequenos que poderiam estar contidos no meio de outras
    # palavras. Consequentemente perdemos o plural dos termos pesquisados, mas
    # assim garantimos um resultado mais preciso.
    cat "$ARQUIVO" | grep -iw "$T1" | grep -iw "$T2" | grep -iw "$T3" > "${DIRETORIO_SAIDA}/"$(basename "$ARQUIVO")
    if [ ! -s "${DIRETORIO_SAIDA}/"$(basename "$ARQUIVO") ]; then
        rm "${DIRETORIO_SAIDA}/"$(basename "$ARQUIVO")
    fi
done
