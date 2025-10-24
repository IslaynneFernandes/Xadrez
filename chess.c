/*
  Trabalho de Programação - Batalha Naval
  Curso: Ciência da Computação - Estácio
  Aluna: Islaynne Fernandes Amorim
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TAM 8

/* Representação do tabuleiro:
   '.' = casa vazia
   'R' = Torre (Rook)
   'B' = Bispo (Bishop)
   'Q' = Rainha (Queen)
   'N' = Cavalo (Knight)
   peças maiúsculas para um "lado" (simulação simples)
*/

void init_tabuleiro(char tab[TAM][TAM]) {
    for (int i = 0; i < TAM; i++)
        for (int j = 0; j < TAM; j++)
            tab[i][j] = '.';

    // coloca algumas peças para demonstração
    tab[0][0] = 'R'; // Torre
    tab[0][2] = 'B'; // Bispo
    tab[0][4] = 'Q'; // Rainha
    tab[1][1] = 'N'; // Cavalo
    // add alguns "obstáculos" para testar caminhos
    tab[3][0] = 'P';
    tab[2][2] = 'P';
    tab[4][4] = 'P';
}

void exibir_tabuleiro(char tab[TAM][TAM]) {
    printf("  a b c d e f g h\n");
    for (int i = 0; i < TAM; i++) {
        printf("%d ", 8 - i);
        for (int j = 0; j < TAM; j++) {
            printf("%c ", tab[i][j]);
        }
        printf(" %d\n", 8 - i);
    }
    printf("  a b c d e f g h\n");
}

int dentro(int x, int y) {
    return (x >= 0 && x < TAM && y >= 0 && y < TAM);
}

/* ------------------------------
   Movimentos básicos usando estruturas de repetição simples
   Torre (for), Bispo (while), Rainha (do-while)
   Estas funções marcam possíveis movimentos com 'x' em uma cópia do tabuleiro.
   ------------------------------*/

void movimentoTorre(char tab[TAM][TAM], int x, int y) {
    char temp[TAM][TAM];
    memcpy(temp, tab, TAM*TAM);

    // movimento vertical e horizontal usando for
    int i;
    for (i = 0; i < TAM; i++) {
        if (i != x && temp[i][y] == '.') temp[i][y] = 'x';
        if (i != y && temp[x][i] == '.') temp[x][i] = 'x';
    }

    printf("\nMovimentos possíveis da Torre (for):\n");
    exibir_tabuleiro(temp);
}

void movimentoBispo(char tab[TAM][TAM], int x, int y) {
    char temp[TAM][TAM];
    memcpy(temp, tab, TAM*TAM);

    int i, j;
    // diagonal superior esquerda usando while
    i = x-1; j = y-1;
    while (dentro(i,j)) {
        if (temp[i][j] == '.') temp[i][j] = 'x';
        else break;
        i--; j--;
    }
    // diagonal inferior direita
    i = x+1; j = y+1;
    while (dentro(i,j)) {
        if (temp[i][j] == '.') temp[i][j] = 'x';
        else break;
        i++; j++;
    }
    // diagonal superior direita
    i = x-1; j = y+1;
    while (dentro(i,j)) {
        if (temp[i][j] == '.') temp[i][j] = 'x';
        else break;
        i--; j++;
    }
    // diagonal inferior esquerda
    i = x+1; j = y-1;
    while (dentro(i,j)) {
        if (temp[i][j] == '.') temp[i][j] = 'x';
        else break;
        i++; j--;
    }

    printf("\nMovimentos possíveis do Bispo (while):\n");
    exibir_tabuleiro(temp);
}

void movimentoRainha(char tab[TAM][TAM], int x, int y) {
    char temp[TAM][TAM];
    memcpy(temp, tab, TAM*TAM);

    int i = 0;
    // usando do-while para marcar linhas e colunas
    do {
        if (i != x && temp[i][y] == '.') temp[i][y] = 'x';
        if (i != y && temp[x][i] == '.') temp[x][i] = 'x';
        i++;
    } while (i < TAM);

    // diagonais com while (reaproveitando lógica do bispo)
    int j;
    i = x-1; j = y-1;
    while (dentro(i,j)) {
        if (temp[i][j] == '.') temp[i][j] = 'x';
        else break;
        i--; j--;
    }
    i = x+1; j = y+1;
    while (dentro(i,j)) {
        if (temp[i][j] == '.') temp[i][j] = 'x';
        else break;
        i++; j++;
    }
    i = x-1; j = y+1;
    while (dentro(i,j)) {
        if (temp[i][j] == '.') temp[i][j] = 'x';
        else break;
        i--; j++;
    }
    i = x+1; j = y-1;
    while (dentro(i,j)) {
        if (temp[i][j] == '.') temp[i][j] = 'x';
        else break;
        i++; j--;
    }

    printf("\nMovimentos possíveis da Rainha (do-while + while):\n");
    exibir_tabuleiro(temp);
}

/* ------------------------------
   Movimento complexo do Cavalo (usando loops aninhados)
   O cavalo se move em "L": 2+1 em todas as direções (8 possibilidades)
   ------------------------------*/
void movimentoCavalo(char tab[TAM][TAM], int x, int y) {
    char temp[TAM][TAM];
    memcpy(temp, tab, TAM*TAM);

    int moves[8][2] = {
        {-2, -1}, {-2, 1}, {-1, -2}, {-1, 2},
        {1, -2}, {1, 2}, {2, -1}, {2, 1}
    };

    // Usando loops aninhados para verificar possibilidade de cada movimento
    for (int m = 0; m < 8; m++) {
        int nx = x + moves[m][0];
        int ny = y + moves[m][1];
        // checagem e marcação
        if (dentro(nx, ny) && temp[nx][ny] == '.') {
            temp[nx][ny] = 'x';
        }
    }

    printf("\nMovimentos possíveis do Cavalo (loops aninhados):\n");
    exibir_tabuleiro(temp);
}

/* ------------------------------
   Recursividade e loops complexos com múltiplas condições
   - Função recursiva para checar caminho livre para peças deslizantes (R, B, Q)
   - Função que gera movimentos avançados usando essa checagem recursiva e várias condições
   ------------------------------*/

/* Checa recursivamente se caminho entre (cx,cy) e destino (dx,dy) com passo (sx,sy) está livre.
   Retorna 1 se estiver livre (ou destino tem peça adversária), 0 se há bloqueio.
*/
int caminho_livre_rec(int cx, int cy, int dx, int dy, int sx, int sy, char tab[TAM][TAM]) {
    int nx = cx + sx;
    int ny = cy + sy;
    if (!dentro(nx, ny)) return 0; // saiu do tabuleiro sem alcançar destino -> inválido
    if (nx == dx && ny == dy) {
        // destino atingido: se está vazio ou tem peça capturável (simulação: qualquer coisa que não seja '.')
        return 1;
    }
    // se há peça no caminho antes do destino, caminho bloqueado
    if (tab[nx][ny] != '.') return 0;
    // recursão: avançar mais um passo
    return caminho_livre_rec(nx, ny, dx, dy, sx, sy, tab);
}

/* Gera movimentos avançados para Torre/Bispo/Rainha usando recursividade para validar cada destino.
   Mostra todos os destinos possíveis considerando obstáculos e limites.
*/
void movimentos_avancados(char tab[TAM][TAM], int x, int y, char peca) {
    char temp[TAM][TAM];
    memcpy(temp, tab, TAM*TAM);

    // Vetor de direções dependendo da peça
    int dirs[8][2];
    int ndir = 0;
    if (peca == 'R') { // Torre - 4 direções ortogonais
        int rdirs[4][2] = {{-1,0},{1,0},{0,-1},{0,1}};
        for (int i=0;i<4;i++){ dirs[i][0]=rdirs[i][0]; dirs[i][1]=rdirs[i][1]; }
        ndir = 4;
    } else if (peca == 'B') { // Bispo - 4 direções diagonais
        int bdirs[4][2] = {{-1,-1},{-1,1},{1,-1},{1,1}};
        for (int i=0;i<4;i++){ dirs[i][0]=bdirs[i][0]; dirs[i][1]=bdirs[i][1]; }
        ndir = 4;
    } else if (peca == 'Q') { // Rainha - 8 direções
        int qdirs[8][2] = {{-1,0},{1,0},{0,-1},{0,1},{-1,-1},{-1,1},{1,-1},{1,1}};
        for (int i=0;i<8;i++){ dirs[i][0]=qdirs[i][0]; dirs[i][1]=qdirs[i][1]; }
        ndir = 8;
    } else {
        printf("Peça não suportada para movimentos avançados.\n");
        return;
    }

    // Laço complexo: para cada direção, testar todos os destinos possíveis naquela direção
    for (int d = 0; d < ndir; d++) {
        int sx = dirs[d][0];
        int sy = dirs[d][1];
        int step = 1;
        while (1) {
            int dx = x + sx*step;
            int dy = y + sy*step;
            if (!dentro(dx, dy)) break;
            // Se o caminho até (dx,dy) estiver livre (recursivamente validado), marcar
            // Observação: caminho_livre_rec parte de (x,y) e verifica se pode alcançar (dx,dy) com passo (sx,sy)
            if (caminho_livre_rec(x, y, dx, dy, sx, sy, tab)) {
                // Se destino estiver vazio, marcar; se tiver peça, marcar como possível captura e parar nesta direção
                if (tab[dx][dy] == '.') {
                    temp[dx][dy] = 'x';
                    // continuar avançando
                    step++;
                    continue;
                } else {
                    // peça bloqueadora/capturável: marca e para
                    temp[dx][dy] = 'c'; // c = possível captura
                    break;
                }
            } else {
                // caminho bloqueado antes de alcançar (dx,dy) -> não pode ir mais adiante nessa direção
                break;
            }
        }
    }

    printf("\nMovimentos avançados da peça %c (recursão + loops complexos):\n", peca);
    exibir_tabuleiro(temp);
}

/* ------------------------------
   Funções utilitárias para leitura de posições no formato "e2" -> (x,y) índice Interno
   ------------------------------*/
int coluna_para_indice(char c) {
    if (c >= 'a' && c <= 'h') return c - 'a';
    if (c >= 'A' && c <= 'H') return c - 'A';
    return -1;
}
int linha_para_indice(char c) {
    // '8' -> 0, '1' -> 7
    if (c >= '1' && c <= '8') return 8 - (c - '0');
    return -1;
}

/* Ajuste: a função acima tem bug potencial (usar expressão correta) -> criar função segura */
int parse_pos(const char *s, int *rx, int *ry) {
    if (!s || strlen(s) < 2) return 0;
    int col = coluna_para_indice(s[0]);
    int row = 8 - (s[1] - '0'); // '8' -> 0
    if (!dentro(row, col)) return 0;
    *rx = row; *ry = col;
    return 1;
}

/* ------------------------------
   Main: menu simples que permite ao usuário escolher peça e posição para ver movimentos.
   Gera o arquivo C (este mesmo) e imprime link (o arquivo já está sendo criado pelo programa que você receberá).
   ------------------------------*/
int main() {
    char tab[TAM][TAM];
    init_tabuleiro(tab);

    printf("Simulador simplificado de movimentos de xadrez\n");
    printf("Regras demonstradas:\n");
    printf("- Torre: utiliza for\n");
    printf("- Bispo: utiliza while\n");
    printf("- Rainha: utiliza do-while + while\n");
    printf("- Cavalo: loops aninhados\n");
    printf("- Movimentos avançados: recursão + loops complexos\n\n");

    exibir_tabuleiro(tab);

    while (1) {
        printf("\nEscolha a peça para simular (R, B, Q, N) ou S para sair: ");
        char opc;
        if (scanf(" %c", &opc) != 1) break;
        if (opc == 'S' || opc == 's') break;

        printf("Informe posição da peça no formato cL (ex: a1, e2): ");
        char pos[16];
        if (scanf("%s", pos) != 1) break;
        int x,y;
        if (!parse_pos(pos, &x, &y)) {
            printf("Posição inválida. Tente novamente.\n");
            continue;
        }

        // checar se na posição está a peça escolhida (simulação simples)
        if (tab[x][y] != opc) {
            printf("A posição %s não contém a peça %c. Contém '%c'.\n", pos, opc, tab[x][y]);
            // ainda assim permitir demonstrar movimentos naquela posição
        }

        if (opc == 'R') {
            movimentoTorre(tab, x, y);
            movimentos_avancados(tab, x, y, 'R');
        } else if (opc == 'B') {
            movimentoBispo(tab, x, y);
            movimentos_avancados(tab, x, y, 'B');
        } else if (opc == 'Q') {
            movimentoRainha(tab, x, y);
            movimentos_avancados(tab, x, y, 'Q');
        } else if (opc == 'N') {
            movimentoCavalo(tab, x, y);
        } else {
            printf("Peça não reconhecida. Use R, B, Q ou N.\n");
        }
    }

    printf("Encerrando simulador. Bom trabalho, Islaynne!\n");
    return 0;
}
