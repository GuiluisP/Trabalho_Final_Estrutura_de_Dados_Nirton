#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100
#define ARQUIVO_CSV "dados_c.csv"

typedef struct {
    int id;
    char arquivo[100];
    int paginas;
    char tipo;
    int ativo;
} Trabalho;

Trabalho normal[MAX], prior[MAX];
int iniN = 0, fimN = 0, qN = 0;
int iniP = 0, fimP = 0, qP = 0;

int cheiaN() { return qN == MAX; }
int vaziaN() { return qN == 0; }
int cheiaP() { return qP == MAX; }
int vaziaP() { return qP == 0; }

void enfileiraNormal(Trabalho t) {
    if (cheiaN()) { puts("[ERRO] Fila normal cheia."); return; }
    normal[fimN] = t; 
    fimN = (fimN + 1) % MAX; 
    qN++;
}

void enfileiraPrior(Trabalho t) {
    if (cheiaP()) { puts("[ERRO] Fila prioritaria cheia."); return; }
    prior[fimP] = t; 
    fimP = (fimP + 1) % MAX; 
    qP++;
}

int desenfileiraPrior(Trabalho *t) {
    if (vaziaP()) return 0;
    *t = prior[iniP]; 
    iniP = (iniP + 1) % MAX; 
    qP--; 
    return 1;
}

int desenfileiraNormal(Trabalho *t) {
    if (vaziaN()) return 0;
    *t = normal[iniN]; 
    iniN = (iniN + 1) % MAX; 
    qN--; 
    return 1;
}

int idExiste(int id) {
    for (int i = 0, p = iniP; i < qP; i++, p = (p + 1) % MAX) {
        if (prior[p].id == id) return 1;
    }
    for (int i = 0, p = iniN; i < qN; i++, p = (p + 1) % MAX) {
        if (normal[p].id == id) return 1;
    }
    return 0;
}

void salvar() {
    FILE *f = fopen(ARQUIVO_CSV, "w");
    if (!f) {
        puts("[ERRO] Nao foi possivel salvar o arquivo CSV.");
        return;
    }
    fprintf(f, "fila,id,arquivo,paginas,tipo\n");
    for (int i = 0, p = iniP; i < qP; i++, p = (p + 1) % MAX)
        fprintf(f, "P,%d,%s,%d,%c\n", prior[p].id, prior[p].arquivo, prior[p].paginas, prior[p].tipo);
    for (int i = 0, p = iniN; i < qN; i++, p = (p + 1) % MAX)
        fprintf(f, "N,%d,%s,%d,%c\n", normal[p].id, normal[p].arquivo, normal[p].paginas, normal[p].tipo);
    fclose(f);
}

void carregar() {
    FILE *f = fopen(ARQUIVO_CSV, "r");
    if (!f) {
        printf("[AVISO] Arquivo '%s' nao encontrado. Iniciando filas vazias.\n", ARQUIVO_CSV);
        return;
    }
    char linha[256];
    fgets(linha, 256, f);
    while (fgets(linha, 256, f)) {
        Trabalho t; 
        char fila;
        if (sscanf(linha, "%c,%d,%99[^,],%d,%c", &fila, &t.id, t.arquivo, &t.paginas, &t.tipo) == 5) {
            if (fila == 'P') enfileiraPrior(t); else enfileiraNormal(t);
        }
    }
    fclose(f);
    printf("[ARQUIVO] Registros carregados com sucesso!\n");
}

void cadastrar() {
    Trabalho t;
    printf("\n--- Cadastrar Trabalho de Impressao ---\n");
    printf("ID: "); 
    scanf("%d", &t.id);
    
    if (idExiste(t.id)) {
        puts("[ERRO] ID ja existe em uma das filas!");
        return;
    }

    printf("Arquivo: "); 
    scanf(" %99[^\n]", t.arquivo);
    printf("Paginas: "); 
    scanf("%d", &t.paginas);
    
    do {
        printf("Tipo (N - Normal / P - Prioritario): "); 
        scanf(" %c", &t.tipo);
        if (t.tipo >= 'a' && t.tipo <= 'z') t.tipo -= 32;
    } while (t.tipo != 'N' && t.tipo != 'P');
    
    if (t.tipo == 'P') enfileiraPrior(t); else enfileiraNormal(t);
    
    puts("[SUCESSO] Trabalho adicionado a fila!");
    salvar();
}

void listar() {
    puts("\n===== FILAS DE IMPRESSAO =====");
    puts("--- Prioritarios (Fila Circular) ---");
    if (vaziaP()) puts("Nenhum trabalho prioritario.");
    for (int i = 0, p = iniP; i < qP; i++, p = (p + 1) % MAX)
        printf("Posicao: %d | ID: %d | Arquivo: %s | %d paginas\n", i + 1, prior[p].id, prior[p].arquivo, prior[p].paginas);
    
    puts("\n--- Normais (Fila Simples) ---");
    if (vaziaN()) puts("Nenhum trabalho normal.");
    for (int i = 0, p = iniN; i < qN; i++, p = (p + 1) % MAX)
        printf("Posicao: %d | ID: %d | Arquivo: %s | %d paginas\n", i + 1, normal[p].id, normal[p].arquivo, normal[p].paginas);
    puts("==============================");
}

void buscar() {
    int id; 
    printf("\nDigite o ID para busca: "); 
    scanf("%d", &id);
    
    for (int i = 0, p = iniP; i < qP; i++, p = (p + 1) % MAX) {
        if (prior[p].id == id) {
            printf("[ENCONTRADO] ID %d esta na fila PRIORITARIA. Arquivo: %s\n", id, prior[p].arquivo); 
            return;
        }
    }
    for (int i = 0, p = iniN; i < qN; i++, p = (p + 1) % MAX) {
        if (normal[p].id == id) {
            printf("[ENCONTRADO] ID %d esta na fila NORMAL. Arquivo: %s\n", id, normal[p].arquivo); 
            return;
        }
    }
    puts("[AVISO] ID nao encontrado em nenhuma das filas.");
}

void cancelar() {
    int id; 
    printf("\nDigite o ID do trabalho a cancelar: "); 
    scanf("%d", &id);
    
    Trabalho aux[MAX]; 
    int c = 0; 
    Trabalho t; 
    int ok = 0;
    
    while (desenfileiraPrior(&t)) { 
        if (t.id == id) { ok = 1; continue; } 
        aux[c++] = t; 
    }
    for (int i = 0; i < c; i++) enfileiraPrior(aux[i]); 
    
    c = 0;
    while (desenfileiraNormal(&t)) { 
        if (t.id == id) { ok = 1; continue; } 
        aux[c++] = t; 
    }
    for (int i = 0; i < c; i++) enfileiraNormal(aux[i]);
    
    if (ok) {
        puts("[SUCESSO] Trabalho cancelado e removido da fila.");
        salvar();
    } else {
        puts("[ERRO] ID nao encontrado.");
    }
}

void processar() {
    Trabalho t;
    printf("\n--- Processando Proximo Trabalho ---\n");
    if (desenfileiraPrior(&t) || desenfileiraNormal(&t)) {
        printf("[IMPRIMINDO] %s (%d paginas) - Tipo: %c\n", t.arquivo, t.paginas, t.tipo);
        salvar();
    } else {
        puts("[AVISO] Nenhum trabalho na fila de impressao.");
    }
}

int main() {
    carregar();
    int op;
    do {
        printf("\n========== GERENCIADOR DE IMPRESSAO ==========\n");
        printf("1 - Enviar para impressao (Cadastrar)\n");
        printf("2 - Executar impressao (Processar)\n");
        printf("3 - Buscar trabalho por ID\n");
        printf("4 - Listar filas de espera\n");
        printf("5 - Cancelar trabalho (Remover da fila)\n");
        printf("0 - Sair\n");
        printf("==============================================");
        printf("\nOpcao: ");
        
        if (scanf("%d", &op) != 1) {
            puts("\n[ERRO] Opcao invalida! Digite um numero.");
            int ch; while ((ch = getchar()) != '\n' && ch != EOF);
            op = -1;
            continue;
        }
        
        switch(op) {
            case 1: cadastrar(); break;
            case 2: processar(); break;
            case 3: buscar(); break;
            case 4: listar(); break;
            case 5: cancelar(); break;
            case 0: salvar(); printf("\nSistema encerrado.\n"); break;
            default: puts("\nOpcao invalida. Tente novamente.");
        }
    } while (op != 0);
    return 0;
}
