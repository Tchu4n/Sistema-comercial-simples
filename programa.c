#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define COR_RESET   "\033[0m"
#define COR_VERDE   "\033[1;32m"
#define COR_VERMELHO "\033[1;31m"
#define COR_CIANO   "\033[1;36m"
#define COR_AMARELO "\033[1;33m"
#define COR_ROXO    "\033[1;35m"
#define COR_BRANCO  "\033[1;37m"
#define COR_AZUL    "\033[1;34m"

#define MAX_ITENS 50
#define MAX_ITENS_POR_VENDA 10

typedef struct {
    int id;
    char nome[30];
    float preco;
    int estoque;
} Produto;

typedef struct {
    int id_produto;
    int quantidade;
    float subtotal;
} ItemVenda;

typedef struct {
    int id_venda;
    ItemVenda itens[MAX_ITENS_POR_VENDA];
    int qtd_itens;
    float valor_total;
    char data_hora[25];
} Venda;

Produto estoque[MAX_ITENS];
Venda historico_vendas[MAX_ITENS];

int qtd_produtos = 0;
int qtd_vendas = 0;

void limpar_tela();
void limpar_buffer();
int ler_inteiro_seguro();
float ler_float_seguro();
void inicializar_dados();
void tela_abertura();
void exibir_cabecalho(const char *titulo);
void menu_principal();
void cadastrar_substancia();
void mostrar_estoque();
void realizar_venda();
void mostrar_lucro_e_alertas();
void remover_substancia();
void editar_preco_substancia();

int main() {
    inicializar_dados();
    tela_abertura();
    menu_principal();
    return 0;
}

void limpar_tela() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int ler_inteiro_seguro() {
    int valor;
    while (scanf("%d", &valor) != 1) {
        limpar_buffer();
        printf(COR_VERMELHO " [ERRO] Entrada invalida. Digite apenas numeros inteiros: " COR_RESET);
    }
    limpar_buffer();
    return valor;
}

float ler_float_seguro() {
    float valor;
    while (scanf("%f", &valor) != 1) {
        limpar_buffer();
        printf(COR_VERMELHO " [ERRO] Entrada invalida. Digite um numero decimal valido (ex: 10.50): " COR_RESET);
    }
    limpar_buffer();
    return valor;
}

void inicializar_dados() {
    estoque[0] = (Produto){1, "Maconha", 10.0, 150};
    estoque[1] = (Produto){2, "Cocaina", 50.0, 80};
    estoque[2] = (Produto){3, "Crack", 5.0, 200};
    qtd_produtos = 3;
}

void tela_abertura() {
    limpar_tela();
    printf(COR_VERMELHO "       11100                              00111\n");
    printf(COR_VERMELHO "       111000                            000111\n");
    printf(COR_VERMELHO "        11100000000000000000000000000000000111\n");
    printf(COR_VERMELHO "         111000000000000000000000000000000011\n");
    printf(COR_VERMELHO "        111000" COR_BRANCO "11111111" COR_VERMELHO "000000" COR_BRANCO "11111111" COR_VERMELHO "000111\n");
    printf(COR_VERMELHO "       11100" COR_BRANCO "111" COR_VERDE "000000" COR_BRANCO "11" COR_VERMELHO "0000" COR_BRANCO "111" COR_VERDE "000000" COR_BRANCO "11" COR_VERMELHO "00111\n");
    printf(COR_VERMELHO "       11100" COR_BRANCO "11" COR_VERDE "00111100" COR_BRANCO "11" COR_VERMELHO "00" COR_BRANCO "11" COR_VERDE "00111100" COR_BRANCO "11" COR_VERMELHO "00111\n");
    printf(COR_VERMELHO "       11100" COR_BRANCO "11" COR_VERDE "00111100" COR_BRANCO "11" COR_VERMELHO "00" COR_BRANCO "11" COR_VERDE "00111100" COR_BRANCO "11" COR_VERMELHO "00111\n");
    printf(COR_VERMELHO "        11100" COR_BRANCO "111" COR_VERDE "000000" COR_BRANCO "11" COR_VERMELHO "0000" COR_BRANCO "111" COR_VERDE "000000" COR_BRANCO "11" COR_VERMELHO "00111\n");
    printf(COR_VERMELHO "         111000" COR_BRANCO "11111111" COR_VERMELHO "0000" COR_BRANCO "11111111" COR_VERMELHO "000111\n");
    printf(COR_AMARELO  "           1110000000000000000000000000000000000000000111\n");
    printf(COR_AMARELO  "         111000" COR_BRANCO "111" COR_AMARELO "0" COR_BRANCO "111" COR_AMARELO "0" COR_BRANCO "111" COR_AMARELO "0" COR_BRANCO "111" COR_AMARELO "0" COR_BRANCO "111" COR_AMARELO "0" COR_BRANCO "111" COR_AMARELO "0000000000000111\n");
    printf(COR_AMARELO  "        111000" COR_BRANCO "1000" COR_AMARELO "1" COR_BRANCO "1000" COR_AMARELO "1" COR_BRANCO "1000" COR_AMARELO "1" COR_BRANCO "1000" COR_AMARELO "1" COR_BRANCO "1000" COR_AMARELO "1" COR_BRANCO "1000" COR_AMARELO "00000000000111\n");
    printf(COR_AMARELO  "       11100000" COR_BRANCO "11" COR_AMARELO "000" COR_BRANCO "11" COR_AMARELO "000" COR_BRANCO "11" COR_AMARELO "000" COR_BRANCO "11" COR_AMARELO "000" COR_BRANCO "11" COR_AMARELO "0000000000000111\n");
    printf(COR_AMARELO  "        1110000000000000000000000000000000000111\n");
    printf(COR_BRANCO   "            1111111111111111111111111111111\n");
    printf(COR_BRANCO   "          111000000000000000000000000000000111\n");
    printf(COR_AZUL     "         1110000000000000000000000000000000000111\n");
    printf(COR_AZUL     "        11100000000000000000000000000000000000000111\n" COR_RESET);
    printf(COR_ROXO "\n=========================================================\n");
    printf(COR_CIANO "                 BOCA DO PICA PAU BIRUTA                 \n");
    printf(COR_ROXO "=========================================================\n" COR_RESET);
    printf(COR_BRANCO "\n Pressione Enter para iniciar o sistema..." COR_RESET);
    getchar();
}

void exibir_cabecalho(const char *titulo) {
    limpar_tela();
    printf(COR_ROXO "=========================================================\n" COR_RESET);
    printf(COR_CIANO "                 BOCA DO PICA PAU BIRUTA                 \n" COR_RESET);
    printf(COR_ROXO "=========================================================\n" COR_RESET);
    printf(COR_BRANCO " TELA ATUAL: %s\n" COR_RESET, titulo);
    printf(COR_ROXO "---------------------------------------------------------\n" COR_RESET);
}

void menu_principal() {
    int opcao;
    do {
        exibir_cabecalho("PAINEL PRINCIPAL");
        printf(COR_CIANO "  [1]" COR_BRANCO " Vender Substancia\n" COR_RESET);
        printf(COR_CIANO "  [2]" COR_BRANCO " Ver Estoque Atual\n" COR_RESET);
        printf(COR_CIANO "  [3]" COR_BRANCO " Adicionar nova substancia ao estoque\n" COR_RESET);
        printf(COR_CIANO "  [4]" COR_BRANCO " Remover substancia do estoque\n" COR_RESET);
        printf(COR_CIANO "  [5]" COR_BRANCO " Editar preco de substancia\n" COR_RESET);
        printf(COR_CIANO "  [6]" COR_BRANCO " Ver Relatorio de Vendas e Alertas\n" COR_RESET);
        printf(COR_VERMELHO "  [0]" COR_BRANCO " Sair do Sistema\n" COR_RESET);
        printf(COR_ROXO "---------------------------------------------------------\n" COR_RESET);
        printf(" Escolha o que quer fazer: ");
        
        opcao = ler_inteiro_seguro();

        switch (opcao) {
            case 1: realizar_venda(); break;
            case 2: mostrar_estoque(); break;
            case 3: cadastrar_substancia(); break;
            case 4: remover_substancia(); break;
            case 5: editar_preco_substancia(); break;
            case 6: mostrar_lucro_e_alertas(); break;
            case 0: 
                limpar_tela();
                printf(COR_VERMELHO "\n Fechando sistema...\n\n" COR_RESET);
                break;
            default:
                printf(COR_VERMELHO "\n Opcao errada. Aperte Enter para tentar de novo.\n" COR_RESET);
                getchar();
        }
    } while (opcao != 0);
}

void cadastrar_substancia() {
    exibir_cabecalho("CADASTRO DE PRODUTO");
    
    if (qtd_produtos >= MAX_ITENS) {
        printf(COR_VERMELHO " Memoria cheia. Nao da para cadastrar mais nada.\n" COR_RESET);
        printf("\nAperte Enter para voltar.");
        getchar();
        return;
    }

    Produto p;
    
    printf(" Nome da Substancia (ou digite '0' para cancelar): ");
    fgets(p.nome, sizeof(p.nome), stdin);
    p.nome[strcspn(p.nome, "\n")] = 0;

    if (strcmp(p.nome, "0") == 0) {
        printf(COR_AMARELO "\n Cadastro cancelado com sucesso.\n" COR_RESET);
        printf("\nAperte Enter para voltar ao menu.");
        getchar();
        return;
    }

    int max_id = 0;
    for (int i = 0; i < qtd_produtos; i++) {
        if (estoque[i].id > max_id) {
            max_id = estoque[i].id;
        }
    }
    p.id = max_id + 1;
    
    printf(" Preco de Venda (R$): ");
    p.preco = ler_float_seguro();
    
    printf(" Quantidade que esta entrando: ");
    p.estoque = ler_inteiro_seguro();

    estoque[qtd_produtos] = p;
    qtd_produtos++;

    printf(COR_VERDE "\n Substancia adicionada ao estoque!\n" COR_RESET);
    printf("\nAperte Enter para voltar.");
    getchar();
}

void mostrar_estoque() {
    exibir_cabecalho("ESTOQUE DISPONIVEL");
    
    printf(COR_BRANCO "%-5s %-25s %-15s %-10s\n", "ID", "Substancia", "Preco (R$)", "Quantidade" COR_RESET);
    printf(COR_ROXO "---------------------------------------------------------\n" COR_RESET);
    
    for (int i = 0; i < qtd_produtos; i++) {
        printf(" [%02d] %-25s R$ %-13.2f %-10d\n", estoque[i].id, estoque[i].nome, estoque[i].preco, estoque[i].estoque);
    }
    printf(COR_ROXO "---------------------------------------------------------\n" COR_RESET);
    printf("\nAperte Enter para voltar.");
    getchar();
}

void remover_substancia() {
    if (qtd_produtos == 0) {
        exibir_cabecalho("REMOVER SUBSTANCIA");
        printf(COR_VERMELHO " Nao ha nenhuma substancia cadastrada no estoque.\n" COR_RESET);
        printf("\nAperte Enter para voltar.");
        getchar();
        return;
    }

    exibir_cabecalho("REMOVER SUBSTANCIA");
    printf(COR_BRANCO " O que tem na casa:\n" COR_RESET);
    for (int i = 0; i < qtd_produtos; i++) {
        printf("   ID: %d | %-15s | Preco: R$ %.2f | No Estoque: %d\n", estoque[i].id, estoque[i].nome, estoque[i].preco, estoque[i].estoque);
    }
    printf(COR_ROXO "---------------------------------------------------------\n" COR_RESET);
    
    printf(" Digite o ID da substancia que deseja remover (ou 0 para cancelar): ");
    int id_busca = ler_inteiro_seguro();

    if (id_busca == 0) {
        printf(COR_AMARELO "\n Operacao cancelada.\n" COR_RESET);
        printf("\nAperte Enter para voltar.");
        getchar();
        return;
    }

    int index = -1;
    for (int i = 0; i < qtd_produtos; i++) {
        if (estoque[i].id == id_busca) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf(COR_VERMELHO "\n Substancia nao encontrada.\n" COR_RESET);
        printf("\nAperte Enter para voltar.");
        getchar();
        return;
    }

    for (int i = index; i < qtd_produtos - 1; i++) {
        estoque[i] = estoque[i + 1];
    }
    
    qtd_produtos--;

    printf(COR_VERDE "\n Substancia removida com sucesso!\n" COR_RESET);
    printf("\nAperte Enter para voltar.");
    getchar();
}

void editar_preco_substancia() {
    if (qtd_produtos == 0) {
        exibir_cabecalho("EDITAR PRECO");
        printf(COR_VERMELHO " Nao ha nenhuma substancia cadastrada no estoque.\n" COR_RESET);
        printf("\nAperte Enter para voltar.");
        getchar();
        return;
    }

    exibir_cabecalho("EDITAR PRECO");
    printf(COR_BRANCO " O que tem na casa:\n" COR_RESET);
    for (int i = 0; i < qtd_produtos; i++) {
        printf("   ID: %d | %-15s | Preco: R$ %.2f | No Estoque: %d\n", estoque[i].id, estoque[i].nome, estoque[i].preco, estoque[i].estoque);
    }
    printf(COR_ROXO "---------------------------------------------------------\n" COR_RESET);

    printf(" Digite o ID da substancia que deseja editar (ou 0 para cancelar): ");
    int id_busca = ler_inteiro_seguro();

    if (id_busca == 0) {
        printf(COR_AMARELO "\n Operacao cancelada.\n" COR_RESET);
        printf("\nAperte Enter para voltar.");
        getchar();
        return;
    }

    int index = -1;
    for (int i = 0; i < qtd_produtos; i++) {
        if (estoque[i].id == id_busca) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf(COR_VERMELHO "\n Substancia nao encontrada.\n" COR_RESET);
        printf("\nAperte Enter para voltar.");
        getchar();
        return;
    }

    printf(" Preco atual: R$ %.2f\n", estoque[index].preco);
    printf(" Digite o novo preco (R$): ");
    float novo_preco = ler_float_seguro();

    if (novo_preco < 0) {
        printf(COR_VERMELHO "\n Preco invalido. Operacao cancelada.\n" COR_RESET);
    } else {
        estoque[index].preco = novo_preco;
        printf(COR_VERDE "\n Preco atualizado com sucesso!\n" COR_RESET);
    }

    printf("\nAperte Enter para voltar.");
    getchar();
}

void realizar_venda() {
    if (qtd_produtos == 0) {
        exibir_cabecalho("REGISTRAR VENDA");
        printf(COR_VERMELHO " Nao ha nenhuma substancia cadastrada no estoque.\n" COR_RESET);
        printf("\nAperte Enter para voltar.");
        getchar();
        return;
    }

    if (qtd_vendas >= MAX_ITENS) {
        exibir_cabecalho("REGISTRAR VENDA");
        printf(COR_VERMELHO " Historico de vendas cheio. Nao e possivel registrar novas vendas.\n" COR_RESET);
        printf("\nAperte Enter para voltar.");
        getchar();
        return;
    }

    Venda nova_venda;
    nova_venda.id_venda = qtd_vendas + 1;
    nova_venda.qtd_itens = 0;
    nova_venda.valor_total = 0.0;

    int continuar = 1;

    do {
        exibir_cabecalho("REGISTRAR VENDA - ADICIONAR SUBSTANCIA");
        printf(COR_BRANCO " O que tem na casa:\n" COR_RESET);
        for (int i = 0; i < qtd_produtos; i++) {
            printf("   ID: %d | %-15s | Preco: R$ %.2f | No Estoque: %d\n", estoque[i].id, estoque[i].nome, estoque[i].preco, estoque[i].estoque);
        }
        printf(COR_ROXO "---------------------------------------------------------\n" COR_RESET);
        
        int id_busca;
        printf(" Digite o ID da substancia vendida: ");
        id_busca = ler_inteiro_seguro();

        int index = -1;
        
        for (int i = 0; i < qtd_produtos; i++) {
            if (estoque[i].id == id_busca) {
                index = i;
                break;
            }
        }

        if (index == -1) {
            printf(COR_VERMELHO "\n Substancia nao encontrada.\n" COR_RESET);
            printf("\nAperte Enter para tentar adicionar outro ID.");
            getchar();
            continue;
        }

        int qtd_venda;
        do {
            printf(" Quantidade de %s (ou 0 para desistir desse item): ", estoque[index].nome);
            qtd_venda = ler_inteiro_seguro();

            if (qtd_venda == 0) {
                break;
            }

            if (qtd_venda < 0) {
                printf(COR_VERMELHO " Quantidade invalida. Digite um valor maior que zero.\n\n" COR_RESET);
                qtd_venda = estoque[index].estoque + 1;
                continue;
            }

            if (qtd_venda > estoque[index].estoque) {
                printf(COR_VERMELHO " Nao ha quantidade suficiente. Estoque atual: %d unidades.\n" COR_RESET, estoque[index].estoque);
                printf(" Insira um valor valido.\n\n");
            }
        } while (qtd_venda > estoque[index].estoque);

        if (qtd_venda > 0) {
            estoque[index].estoque -= qtd_venda;

            ItemVenda iv;
            iv.id_produto = estoque[index].id;
            iv.quantidade = qtd_venda;
            iv.subtotal = qtd_venda * estoque[index].preco;

            nova_venda.itens[nova_venda.qtd_itens] = iv;
            nova_venda.qtd_itens++;
            nova_venda.valor_total += iv.subtotal;

            printf(COR_VERDE "\n -> %d unidade(s) de %s adicionada(s) a lista da venda!\n" COR_RESET, qtd_venda, estoque[index].nome);
        }

        if (nova_venda.qtd_itens >= MAX_ITENS_POR_VENDA) {
            printf(COR_AMARELO "\n Limite maximo de %d tipos de substancias por venda atingido.\n" COR_RESET, MAX_ITENS_POR_VENDA);
            break;
        }

        printf("\n Quer colocar mais alguma substancia nessa mesma venda? (1 - Sim / 0 - Fechar Venda): ");
        continuar = ler_inteiro_seguro();

    } while (continuar == 1);

    if (nova_venda.qtd_itens > 0) {
        time_t t = time(NULL);
        struct tm *tm_info = localtime(&t);
        strftime(nova_venda.data_hora, sizeof(nova_venda.data_hora), "%d/%m/%Y %H:%M:%S", tm_info);

        historico_vendas[qtd_vendas] = nova_venda;
        qtd_vendas++;

        exibir_cabecalho("VENDA CONCLUIDA");
        printf(COR_VERDE " VENDA REGISTRADA COM SUCESSO!\n" COR_RESET);
        printf(COR_BRANCO " Resumo da Nota:\n" COR_RESET);
        
        for(int i = 0; i < nova_venda.qtd_itens; i++) {
            int p_id = nova_venda.itens[i].id_produto;
            char nome_prod[30] = "Desconhecido";
            
            for(int j = 0; j < qtd_produtos; j++) {
                if(estoque[j].id == p_id) {
                    strcpy(nome_prod, estoque[j].nome);
                    break;
                }
            }
            printf("   - %d x %s | Subtotal: R$ %.2f\n", nova_venda.itens[i].quantidade, nome_prod, nova_venda.itens[i].subtotal);
        }
        printf(COR_ROXO "---------------------------------------------------------\n" COR_RESET);
        printf(" Valor Total Pago: " COR_VERDE "R$ %.2f\n" COR_RESET, nova_venda.valor_total);
    } else {
        printf(COR_VERMELHO "\n Nenhum item foi adicionado. Venda descartada.\n" COR_RESET);
    }

    printf("\nAperte Enter para voltar ao painel principal.");
    getchar();
}

void mostrar_lucro_e_alertas() {
    exibir_cabecalho("RELATORIO GERAL E DETALHADO DE VENDAS");
    float faturamento_bruto = 0;
    
    for (int i = 0; i < qtd_vendas; i++) {
        faturamento_bruto += historico_vendas[i].valor_total;
    }

    printf(COR_BRANCO " Resultados Gerais:\n" COR_RESET);
    printf(" Total de atendimentos realizados: %d\n", qtd_vendas);
    printf(" Faturamento bruto total: " COR_VERDE "R$ %.2f\n" COR_RESET, faturamento_bruto);
    printf(COR_ROXO "---------------------------------------------------------\n" COR_RESET);
    
    printf(COR_BRANCO " HISTORICO DETALHADO DE ATENDIMENTOS:\n" COR_RESET);
    if (qtd_vendas == 0) {
        printf(COR_AMARELO "   Nenhuma venda realizada ate o momento.\n" COR_RESET);
    } else {
        for (int i = 0; i < qtd_vendas; i++) {
            printf(COR_AMARELO "   [Venda #%02d] - Data/Hora: %s - Valor Pago: R$ %.2f\n" COR_RESET, 
                   historico_vendas[i].id_venda, 
                   historico_vendas[i].data_hora, 
                   historico_vendas[i].valor_total);
                   
            for (int j = 0; j < historico_vendas[i].qtd_itens; j++) {
                int p_id = historico_vendas[i].itens[j].id_produto;
                char nome_prod[30] = "Desconhecido";
                
                for (int k = 0; k < qtd_produtos; k++) {
                    if (estoque[k].id == p_id) {
                        strcpy(nome_prod, estoque[k].nome);
                        break;
                    }
                }
                
                printf("     * %d x %s (Subtotal: R$ %.2f)\n", 
                       historico_vendas[i].itens[j].quantidade, 
                       nome_prod, 
                       historico_vendas[i].itens[j].subtotal);
            }
            printf("\n");
        }
    }
    printf(COR_ROXO "---------------------------------------------------------\n" COR_RESET);
    
    printf(COR_BRANCO " Substancias acabando (menos de 10 unidades):\n" COR_RESET);
    int alertas = 0;
    for (int i = 0; i < qtd_produtos; i++) {
        if (estoque[i].estoque < 10) {
            printf(COR_VERMELHO "   -> %s esta com estoque baixo: %d unidades\n" COR_RESET, estoque[i].nome, estoque[i].estoque);
            alertas++;
        }
    }
    if (alertas == 0) {
        printf(COR_VERDE "   Tudo abastecido. Nenhuma substancia acabando.\n" COR_RESET);
    }
    
    printf(COR_ROXO "---------------------------------------------------------\n" COR_RESET);
    printf("\nAperte Enter para voltar.");
    getchar();
}

```
