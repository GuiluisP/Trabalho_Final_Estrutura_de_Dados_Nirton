#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PACIENTES 150
#define MAX_STR 100
#define ARQUIVO_CSV "dados_a.csv"

typedef struct {
    int senha;
    char nome[MAX_STR];
    int idade;
    int gravidade;
} Paciente;

Paciente triagem[MAX_PACIENTES];
int qtd_pacientes = 0;

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void removerNovaLinha(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

int senhaExiste(int senha) {
    for (int i = 0; i < qtd_pacientes; i++) {
        if (triagem[i].senha == senha) {
            return i;
        }
    }
    return -1;
}

void salvarCSV() {
    FILE *arquivo = fopen(ARQUIVO_CSV, "w");
    if (arquivo == NULL) {
        printf("[ERRO] Nao foi possivel salvar os dados no arquivo CSV!\n");
        return;
    }

    fprintf(arquivo, "senha,nome,idade,gravidade\n");

    for (int i = 0; i < qtd_pacientes; i++) {
        fprintf(arquivo, "%d,%s,%d,%d\n", 
                triagem[i].senha, triagem[i].nome, triagem[i].idade, triagem[i].gravidade);
    }

    fclose(arquivo);
    printf("[ARQUIVO] Dados salvos em '%s' com sucesso!\n", ARQUIVO_CSV);
}

void carregarCSV() {
    FILE *arquivo = fopen(ARQUIVO_CSV, "r");
    if (arquivo == NULL) {
        printf("[AVISO] Arquivo '%s' nao encontrado. Iniciando lista vazia.\n", ARQUIVO_CSV);
        return;
    }

    char linha[300];
    if (fgets(linha, sizeof(linha), arquivo) == NULL) {
        fclose(arquivo);
        return;
    }

    qtd_pacientes = 0;
    while (fgets(linha, sizeof(linha), arquivo) != NULL && qtd_pacientes < MAX_PACIENTES) {
        Paciente p;
        
        char *token = strtok(linha, ",");
        if (token != NULL) p.senha = atoi(token);

        token = strtok(NULL, ",");
        if (token != NULL) strcpy(p.nome, token);

        token = strtok(NULL, ",");
        if (token != NULL) p.idade = atoi(token);

        token = strtok(NULL, "\n");
        if (token != NULL) p.gravidade = atoi(token);

        triagem[qtd_pacientes] = p;
        qtd_pacientes++;
    }

    fclose(arquivo);
    printf("[ARQUIVO] %d registros carregados do arquivo CSV!\n", qtd_pacientes);
}

void cadastrarPaciente() {
    if (qtd_pacientes >= MAX_PACIENTES) {
        printf("\n[ERRO] A lista de triagem esta cheia! Limite de %d pacientes atingido.\n", MAX_PACIENTES);
        return;
    }

    Paciente novo;
    int senhaProvisoria;

    printf("\n--- Cadastrar Paciente ---\n");
    printf("Digite a Senha (numerica unica): ");
    scanf("%d", &senhaProvisoria);
    limparBuffer();

    if (senhaExiste(senhaProvisoria) != -1) {
        printf("[ERRO] Ja existe um paciente com a senha %d!\n", senhaProvisoria);
        return;
    }
    novo.senha = senhaProvisoria;

    printf("Nome do Paciente: ");
    fgets(novo.nome, MAX_STR, stdin);
    removerNovaLinha(novo.nome);

    printf("Idade: ");
    scanf("%d", &novo.idade);
    limparBuffer();

    do {
        printf("Gravidade (1 - Emergencia ate 4 - Leve): ");
        scanf("%d", &novo.gravidade);
        limparBuffer();
        if (novo.gravidade < 1 || novo.gravidade > 4) {
            printf("[ERRO] Gravidade invalida! Digite um valor entre 1 e 4.\n");
        }
    } while (novo.gravidade < 1 || novo.gravidade > 4);

    int posicao = 0;
    while (posicao < qtd_pacientes) {
        if (novo.gravidade < triagem[posicao].gravidade) {
            break;
        }
        else if (novo.gravidade == triagem[posicao].gravidade) {
            if (novo.senha < triagem[posicao].senha) {
                break;
            }
        }
        posicao++;
    }

    for (int i = qtd_pacientes; i > posicao; i--) {
        triagem[i] = triagem[i - 1];
    }

    triagem[posicao] = novo;
    qtd_pacientes++;

    printf("\n[SUCESSO] Paciente cadastrado e posicionado na fila!\n");
    salvarCSV();
}

void listarPacientes() {
    if (qtd_pacientes == 0) {
        printf("\nNao ha pacientes na fila de triagem.\n");
        return;
    }

    printf("\n--- Fila de Triagem (Ordem de Prioridade) ---\n");
    printf("-------------------------------------------------------\n");
    for (int i = 0; i < qtd_pacientes; i++) {
        printf("Posicao: %d. Fila | Senha: %d | Gravidade: %d | Nome: %s | Idade: %d anos\n",
               i + 1, triagem[i].senha, triagem[i].gravidade, triagem[i].nome, triagem[i].idade);
    }
    printf("-------------------------------------------------------\n");
    printf("Total de pacientes aguardando: %d\n", qtd_pacientes);
}

void buscarPaciente() {
    if (qtd_pacientes == 0) {
        printf("\nNao ha pacientes na triagem para buscar.\n");
        return;
    }

    int senha;
    printf("\nDigite a senha do paciente que deseja buscar: ");
    scanf("%d", &senha);
    limparBuffer();

    int indice = senhaExiste(senha);

    if (indice != -1) {
        printf("\n[PACIENTE ENCONTRADO] (Posicao na fila: %d.):\n", indice + 1);
        printf("  Senha: %d\n", triagem[indice].senha);
        printf("  Nome: %s\n", triagem[indice].nome);
        printf("  Idade: %d anos\n", triagem[indice].idade);
        printf("  Gravidade: %d\n", triagem[indice].gravidade);
    } else {
        printf("\n[ERRO] Paciente com a senha %d nao foi encontrado.\n", senha);
    }
}

void editarPaciente() {
    if (qtd_pacientes == 0) {
        printf("\nNao ha pacientes para editar.\n");
        return;
    }

    int senha;
    printf("\nDigite a senha do paciente que deseja editar: ");
    scanf("%d", &senha);
    limparBuffer();

    int indice = senhaExiste(senha);

    if (indice != -1) {
        Paciente editado = triagem[indice];

        printf("\n--- Editando Paciente (Senha: %d) ---\n", editado.senha);
        printf("Novo Nome: ");
        fgets(editado.nome, MAX_STR, stdin);
        removerNovaLinha(editado.nome);

        printf("Nova Idade: ");
        scanf("%d", &editado.idade);
        limparBuffer();

        do {
            printf("Nova Gravidade (1 a 4): ");
            scanf("%d", &editado.gravidade);
            limparBuffer();
            if (editado.gravidade < 1 || editado.gravidade > 4) {
                printf("[ERRO] Gravidade invalida!\n");
            }
        } while (editado.gravidade < 1 || editado.gravidade > 4);

        for (int i = indice; i < qtd_pacientes - 1; i++) {
            triagem[i] = triagem[i + 1];
        }
        qtd_pacientes--;

        int novaPos = 0;
        while (novaPos < qtd_pacientes) {
            if (editado.gravidade < triagem[novaPos].gravidade) break;
            else if (editado.gravidade == triagem[novaPos].gravidade) {
                if (editado.senha < triagem[novaPos].senha) break;
            }
            novaPos++;
        }

        for (int i = qtd_pacientes; i > novaPos; i--) {
            triagem[i] = triagem[i - 1];
        }
        triagem[novaPos] = editado;
        qtd_pacientes++;

        printf("\n[SUCESSO] Dados atualizados e fila reordenada!\n");
        salvarCSV();
    } else {
        printf("\n[ERRO] Paciente com a senha %d nao encontrado.\n", senha);
    }
}

void excluirPaciente() {
    if (qtd_pacientes == 0) {
        printf("\nNao ha pacientes na fila para remocao.\n");
        return;
    }

    int senha;
    printf("\nDigite a senha do paciente que deseja dar alta/remover: ");
    scanf("%d", &senha);
    limparBuffer();

    int indice = senhaExiste(senha);

    if (indice != -1) {
        for (int i = indice; i < qtd_pacientes - 1; i++) {
            triagem[i] = triagem[i + 1];
        }
        qtd_pacientes--;
        printf("\n[SUCESSO] Paciente removido com sucesso!\n");
        salvarCSV();
    } else {
        printf("\n[ERRO] Paciente com a senha %d nao encontrado.\n", senha);
    }
}

void exibirEstatisticas() {
    if (qtd_pacientes == 0) {
        printf("\nSem dados estatisticos. Fila vazia.\n");
        return;
    }
    int g1 = 0, g2 = 0, g3 = 0, g4 = 0;
    for (int i = 0; i < qtd_pacientes; i++) {
        if (triagem[i].gravidade == 1) g1++;
        if (triagem[i].gravidade == 2) g2++;
        if (triagem[i].gravidade == 3) g3++;
        if (triagem[i].gravidade == 4) g4++;
    }
    printf("\n=== ESTATISTICAS DA FILA ===\n");
    printf("  Total de Pacientes: %d\n", qtd_pacientes);
    printf("  Gravidade 1 (Criticos): %d\n", g1);
    printf("  Gravidade 2 (Urgentes): %d\n", g2);
    printf("  Gravidade 3 (Medios): %d\n", g3);
    printf("  Gravidade 4 (Leves): %d\n", g4);
}

int main() {
    carregarCSV();

    int opcao;

    do {
        printf("\n========= TRIAGEM HOSPITALAR =========");
        printf("\n1. Cadastrar Paciente (Fila Ordenada)");
        printf("\n2. Listar Fila de Espera");
        printf("\n3. Buscar Paciente por Senha");
        printf("\n4. Editar Dados do Paciente");
        printf("\n5. Remover/Dar Alta a Paciente");
        printf("\n6. Visualizar Painel de Estatisticas");
        printf("\n0. Sair do Sistema");
        printf("\n======================================");
        printf("\nEscolha uma opcao: ");
        
        if (scanf("%d", &opcao) != 1) {
            printf("\n[ERRO] Opcao invalida! Digite um numero.\n");
            limparBuffer();
            opcao = -1;
            continue;
        }
        limparBuffer();

        switch (opcao) {
            case 1: cadastrarPaciente(); break;
            case 2: listarPacientes(); break;
            case 3: buscarPaciente(); break;
            case 4: editarPaciente(); break;
            case 5: excluirPaciente(); break;
            case 6: exibirEstatisticas(); break;
            case 0: printf("\nEncerrando o sistema. Ate logo!\n"); break;
            default: printf("\nOpcao invalida! Tente novamente.\n");
        }
    } while (opcao != 0);

    return 0;
}
