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
    echo "Uso: $0 <axml> <acsv>"
    exit 1
else
    # Guarda os parametros em variaveis mais intuitivas.
    ARQUIVO_XML="$1"
    ARQUIVO_FINAL="$2"
fi

# Verifica se o parametro do arquivo compactado existe e é do tipo esperado,
# termina o script com erro 2 em caso negativo.
if [ ! -f $ARQUIVO_XML ]; then
    echo "Arquivo \"$ARQUIVO_XML\" inexistente."
    exit 2
else
    if [[ $ARQUIVO_XML != *.xml* ]]; then
        echo "Arquivo \"$ARQUIVO_XML\" não é do formato \".xml\" ou \".xml.gz\"."
        exit 2
    fi
    # Se o arquivo estiver compactado, faz a descompressão
    # sem afetar o arquivo original e atualiza a variável
    # para o novo nome correto.
    if [[ $ARQUIVO_XML == *.gz ]]; then
        ARQUIVO_TEMP3=$(mktemp -p . --suffix=.xml.gz)
        cp $ARQUIVO_XML $ARQUIVO_TEMP3
        gzip -d $ARQUIVO_TEMP3
        ARQUIVO_XML="$(basename "$ARQUIVO_TEMP3" .gz)"
        # Atualiza o nome da variável auxiliar
        # para poder deletar ao fim do script.
        ARQUIVO_TEMP3=$ARQUIVO_XML
    fi
fi

ARQUIVO_TEMP1=$(mktemp -p .)
ARQUIVO_TEMP2=$(mktemp -p .)

# Utiliza o xgrep para extrair apenas os campos de interesse.
/home/soft/xgrep/bin/xgrep -t -x "//PMID|//ArticleTitle|//Abstract|//DescriptorName|//QualifierName" $ARQUIVO_XML > $ARQUIVO_TEMP1

# Remove os artigos filtrados que não possuem resumo. Também separa cada artigo
# em um bloco de texto, com cada bloco separado por uma linha em branco.
awk '/<PMID Version/{u=$0; x=0}/<ArticleTitle>/{v=$0}/<Abstract/{x=$0; a++; if (a>1) printf "\n\n"; print u; print v; print $0}/<Descriptor/{if (x) printf " %s,", $0}/<Qualifier/{if (x) printf " %s,", $0}' $ARQUIVO_TEMP1 > $ARQUIVO_TEMP2

# Remove as tags e espaços em branco contiguos de cada artigo. 
# A partir deste ponto, cada artigo será separado em blocos 
# de quatro linhas com as informações desejadas.
awk '/<PMID/{sub(/<PMID Version="/,""); sub(/">/,"");} {gsub(/<[^>]*>/,""); gsub(/[ ]{9,}/,", "); sub(/ $/,""); sub(/^ /,"")} {print $0}' $ARQUIVO_TEMP2 > $ARQUIVO_TEMP1

# Quebra as linhas do artigo em separações pelo caractere "<" para encaixar no
# formato de texto lido pelos arquivos ".csv". Adiciona o cabeçalho no arquivo.
cat $ARQUIVO_TEMP1 | tr '\n' '<' | awk '{if (NR < 2) print "version+pmid<title<abstract<keywords"} {gsub(/<</, "\n")} {print $0}' > $ARQUIVO_TEMP2

# Remove a vírgula do fim da linha de cada artigo e a vírgula seguida de um 
# separador no fim da última linha (resquícios da impressão dos
# "descriptors" e "qualifiers" na primeira chamada do awk).
awk '{sub(/,<$/,"")} {sub(/,$/,"")} {print $0}' $ARQUIVO_TEMP2 > $ARQUIVO_TEMP1

# Remove as vírgulas extras decorrentes da substituição dos nove (ou mais)
# espaços contiguos na segunda chamada do awk. E também os caracteres "<" do
# começo de cada linha.
awk '{sub(/^</,"")} {gsub(/<, /,"<"); gsub(/,</,"<"); gsub(/, ©/," ©")} {print $0}' $ARQUIVO_TEMP1 > $ARQUIVO_TEMP2

# Corrige os separadores dos artigos que não possuem o quarto campo.
awk 'BEGIN {FS="<"; OFS=FS} {if ($4=="") {print $0"<"} else {print $0}}' $ARQUIVO_TEMP2 > $ARQUIVO_FINAL

# Apaga os arquivos auxiliares criados para a execução do script.
rm $ARQUIVO_TEMP1 $ARQUIVO_TEMP2
if [ -e "$ARQUIVO_TEMP3" ]; then
    rm $ARQUIVO_TEMP3
fi
