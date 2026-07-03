#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100
#define ARQUIVO_CSV "dados_b.csv"

typedef struct {
    int id;
    char remetente[100];
    char assunto[100];
    char data[30];
} Email;

typedef struct {
    Email emails[MAX];
    int topo;
} Pilha;

void inicializar(Pilha *p) {
    p->topo = -1;
}

int vazia(Pilha *p) {
    return p->topo == -1;
}

int cheia(Pilha *p) {
    return p->topo == MAX - 1;
}

int idExiste(Pilha *p, int id) {
    for(int i = 0; i <= p->topo; i++) {
        if(p->emails[i].id == id)
            return 1;
    }
    return 0;
}

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// ==========================================
// FUNCOES DE ARQUIVO (CSV)
// ==========================================

void salvarCSV(Pilha *p) {
    FILE *arquivo = fopen(ARQUIVO_CSV, "w");
    if (arquivo == NULL) {
        printf("[ERRO] Nao foi possivel salvar os dados no arquivo CSV!\n");
        return;
    }

    fprintf(arquivo, "id,remetente,assunto,data\n");

    // Salva a partir da base (0) ate o topo para manter a ordem correta na pilha ao recarregar
    for (int i = 0; i <= p->topo; i++) {
        fprintf(arquivo, "%d,%s,%s,%s\n", 
                p->emails[i].id, p->emails[i].remetente, p->emails[i].assunto, p->emails[i].data);
    }

    fclose(arquivo);
    printf("[ARQUIVO] Dados salvos em '%s' com sucesso!\n", ARQUIVO_CSV);
}

void carregarCSV(Pilha *p) {
    FILE *arquivo = fopen(ARQUIVO_CSV, "r");
    if (arquivo == NULL) {
        printf("[AVISO] Arquivo '%s' nao encontrado. Iniciando pilha vazia.\n", ARQUIVO_CSV);
        return;
    }

    char linha[350];
    if (fgets(linha, sizeof(linha), arquivo) == NULL) {
        fclose(arquivo);
        return;
    }

    p->topo = -1;
    while (fgets(linha, sizeof(linha), arquivo) != NULL && p->topo < MAX - 1) {
        Email e;
        
        char *token = strtok(linha, ",");
        if (token != NULL) e.id = atoi(token);

        token = strtok(NULL, ",");
        if (token != NULL) strcpy(e.remetente, token);

        token = strtok(NULL, ",");
        if (token != NULL) strcpy(e.assunto, token);

        token = strtok(NULL, "\n");
        if (token != NULL) strcpy(e.data, token);

        p->topo++;
        p->emails[p->topo] = e;
    }

    fclose(arquivo);
    printf("[ARQUIVO] %d registros carregados do arquivo CSV!\n", p->topo + 1);
}

// ==========================================
// OPERACOES DA PILHA
// ==========================================

void receberEmail(Pilha *p) {
    if(cheia(p)) {
        printf("\n[ERRO] Pilha cheia! Limite de %d emails atingido.\n", MAX);
        return;
    }
    
    Email e;
    printf("\n--- Receber Email (Empilhar) ---\n");
    printf("ID: ");
    if (scanf("%d", &e.id) != 1) {
        printf("[ERRO] ID invalido!\n");
        limparBuffer();
        return;
    }
    getchar();
    
    if(idExiste(p, e.id)) {
        printf("[ERRO] ID ja existe!\n");
        return;
    }
    
    printf("Remetente: ");
    fgets(e.remetente, 100, stdin);
    e.remetente[strcspn(e.remetente, "\n")] = '\0';
    
    printf("Assunto: ");
    fgets(e.assunto, 100, stdin);
    e.assunto[strcspn(e.assunto, "\n")] = '\0';
    
    printf("Data de recebimento: ");
    fgets(e.data, 30, stdin);
    e.data[strcspn(e.data, "\n")] = '\0';
    
    p->topo++;
    p->emails[p->topo] = e;
    printf("\n[SUCESSO] Email recebido com sucesso!\n");
    salvarCSV(p);
}

void lerEmail(Pilha *p) {
    if(vazia(p)) {
        printf("\n[AVISO] Nenhum email para ler (Pilha vazia).\n");
        return;
    }
    
    Email e = p->emails[p->topo];
    printf("\n--- Lendo Ultimo Email Recebido (Desempilhar) ---\n");
    printf("ID: %d\n", e.id);
    printf("Remetente: %s\n", e.remetente);
    printf("Assunto: %s\n", e.assunto);
    printf("Data: %s\n", e.data);
    p->topo--;
    
    printf("\n[SUCESSO] Email lido e removido da caixa de nao lidos.\n");
    salvarCSV(p);
}

void ultimoEmail(Pilha *p) {
    if(vazia(p)) {
        printf("\n[AVISO] Nenhum email na bandeja.\n");
        return;
    }
    
    Email e = p->emails[p->topo];
    printf("\n--- Ultimo Email Recebido (Topo da Pilha) ---\n");
    printf("ID: %d\n", e.id);
    printf("Remetente: %s\n", e.remetente);
    printf("Assunto: %s\n", e.assunto);
    printf("Data: %s\n", e.data);
}

void listarEmails(Pilha *p) {
    if(vazia(p)) {
        printf("\n[AVISO] Nenhum email na bandeja.\n");
        return;
    }
    
    printf("\n===== CAIXA DE ENTRADA (Ordem de Leitura) =====\n");
    printf("-------------------------------------------------------\n");
    for(int i = p->topo; i >= 0; i--) {
        printf("Posicao: %d. da Fila | ID: %d | Remetente: %s | Assunto: %s | Data: %s\n", 
               (p->topo - i) + 1, p->emails[i].id, p->emails[i].remetente, p->emails[i].assunto, p->emails[i].data);
    }
    printf("-------------------------------------------------------\n");
    printf("Total de emails nao lidos: %d\n", p->topo + 1);
}

void marcarComoLido(Pilha *p) {
    if(vazia(p)) {
        printf("\n[AVISO] Nao ha emails para remover (Pilha vazia).\n");
        return;
    }
    
    int id, encontrado = 0;
    printf("\nDigite o ID do email que deseja marcar como lido: ");
    if (scanf("%d", &id) != 1) {
        printf("[ERRO] Entrada invalida!\n");
        limparBuffer();
        return;
    }
    getchar();
    
    Pilha aux;
    inicializar(&aux);
    
    while(!vazia(p)) {
        Email e = p->emails[p->topo];
        p->topo--;
        if(e.id == id) {
            encontrado = 1;
            break;
        }
        aux.topo++;
        aux.emails[aux.topo] = e;
    }
    
    while(!vazia(&aux)) {
        p->topo++;
        p->emails[p->topo] = aux.emails[aux.topo];
        aux.topo--;
    }
    
    if(encontrado) {
        printf("\n[SUCESSO] Email com ID %d removido do meio da pilha!\n", id);
        salvarCSV(p);
    } else {
        printf("\n[ERRO] ID nao encontrado na pilha.\n");
    }
}

// ==========================================
// FUNCAO PRINCIPAL / MENU
// ==========================================
int main() {
    Pilha caixa;
    inicializar(&caixa);
    carregarCSV(&caixa);
    
    int op;
    do {
        printf("\n========== BANDEJA DE E-MAILS ==========\n");
        printf("1 - Receber email (Empilhar)\n");
        printf("2 - Ler proximo email (Desempilhar)\n");
        printf("3 - Mostrar ultimo email (Topo)\n");
        printf("4 - Listar todos os emails\n");
        printf("5 - Marcar email especifico como lido (Remover do meio)\n");
        printf("0 - Sair\n");
        printf("========================================");
        printf("\nOpcao: ");
        
        if (scanf("%d", &op) != 1) {
            printf("\n[ERRO] Opcao invalida! Digite um numero.\n");
            limparBuffer();
            op = -1;
            continue;
        }
        getchar();
        
        switch(op) {
            case 1: receberEmail(&caixa); break;
            case 2: lerEmail(&caixa); break;
            case 3: ultimoEmail(&caixa); break;
            case 4: listarEmails(&caixa); break;
            case 5: marcarComoLido(&caixa); break;
            case 0: printf("\nPrograma encerrado.\n"); break;
            default: printf("\nOpcao invalida. Tente novamente.\n");
        }
    } while(op != 0);
    
    return 0;
}
