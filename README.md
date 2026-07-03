# Trabalho_Final_Estrutura_de_Dados_Nirton
Trabalho final da matéria de estrutura de dados.

## LINK DO VIDEO NO YOUTUBE

## INTEGRANTES
* Guilherme Luis Paixão Costa
* Guilherme Freire 
* Glebson Santana
* Landidaracy Mota Azevedo

## TURMA

Turma C

## TEMAS ESCOLHIDOS

*Tema Trabalho A, Triagem Hospitalar / Tema Trabalho B, Bandeja de e-mails / Tema Trabalho C, Fila de impressão.

## EXPLICAÇÃO DO PROGRAMA
* O programa do trabalho A simula uma lista de pacientes em uma triagem hospitalar utilizando uma estrutura de vetor de structs em C. Toda vez que um paciente é cadastrado, o sistema valida se a senha é única e se a gravidade está entre 1 e 4 (onde 1 é a maior prioridade).

A lista é mantida automaticamente ordenada por gravidade no momento da inserção. Em casos de empate na gravidade, o sistema utiliza a menor senha (ordem de chegada) como critério de desempate. O programa gerencia de forma completa os estados de estrutura cheia e estrutura vazia, além de ler e salvar os registros automaticamente em um arquivo CSV (triagem.csv), garantindo que os dados não sejam perdidos ao fechar o sistema.

* O programa do trabalho B simula o gerenciamento de uma caixa de e-mails não lidos utilizando uma estrutura de Pilha Estática (LIFO - Last In, First Out). Toda vez que um novo e-mail é recebido, ele é empilhado no topo, tornando-se o primeiro disponível para leitura. O sistema valida rigorosamente se o ID informado é único antes de consolidar a operação.

* O programa do trabalho C gerencia os trabalhos enviados a uma impressora dividindo-os em duas políticas de atendimento: uma Fila Normal Simples e uma Fila Prioritária Circular. Cada documento cadastrado possui identificação própria, número de páginas e seu respectivo tipo ('N' ou 'P').

## INSTRUÇÃO DE COMPILAÇÃO

## OBSERVAÇÕES

*Os codigos criam arquivos csv ao serem executados pela primeira vez
