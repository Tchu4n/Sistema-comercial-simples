/*
 * BIBLIOTECAS (LIBRARIES)
 * Importamos ferramentas prontas da linguagem C.
 * stdio.h  -> Para entrada e saída (printf, scanf, fgets).
 * stdlib.h -> Para comandos do sistema (como limpar a tela com system("clear")).
 * string.h -> Para manipular textos (comparar strings, copiar strings).
 * time.h   -> Para capturar a data e hora do computador.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*
 * CONSTANTES E CORES
 * O #define cria "apelidos". Sempre que o compilador ver "COR_VERDE", 
 * ele substitui pelo código ANSI correspondente. Isso facilita a leitura.
 */
#define COR_RESET   "\033[0m"
#define COR_VERDE   "\033[1;32m"
#define COR_VERMELHO "\033[1;31m"
#define COR_CIANO   "\033[1;36m"
#define COR_AMARELO "\033[1;33m"
#define COR_ROXO    "\033[1;35m"
#define COR_BRANCO  "\033[1;37m"
#define COR_AZUL    "\033[1;34m"

// Define limites máximos para os vetores (arrays) para evitar uso infinito de memória.
#define MAX_ITENS 50
#define MAX_ITENS_POR_VENDA 10

/*
 * ESTRUTURAS (STRUCTS)
 * Structs são "moldes" que agrupam diferentes tipos de dados em um único pacote.
 */

// Molde para cadastrar uma substância no estoque.
typedef struct {
    int id;           // Código de identificação (número inteiro)
    char nome[30];    // Texto com até 29 caracteres + o caractere nulo '\0'
    float preco;      // Número decimal (com vírgula)
    int estoque;      // Quantidade disponível (número inteiro)
} Produto;

// Molde para registrar uma substância específica dentro de uma venda maior.
typedef struct {
    int id_produto;   // Qual produto foi vendido
    int quantidade;   // Quanto desse produto foi vendido
    float subtotal;   // Preço unitário * quantidade
} ItemVenda;

// Molde para a venda completa (a "nota fiscal").
typedef struct {
    int id_venda;                           // Número da nota
    ItemVenda itens[MAX_ITENS_POR_VENDA];   // Lista de até 10 itens diferentes
    int qtd_itens;                          // Quantos itens diferentes entraram nessa venda
    float valor_total;                      // Soma de todos os subtotais
    char data_hora[25];                     // Texto para guardar o momento exato da venda
} Venda;

/*
 * VARIÁVEIS GLOBAIS (BANCO DE DADOS EM MEMÓRIA)
 * Ficam fora das funções para que TODAS as funções possam ler e alterar seus valores.
 * Se o programa fechar, esses dados são perdidos (pois estão na memória RAM).
 */
Produto estoque[MAX_ITENS];         // Vetor que guarda todos os produtos cadastrados
Venda historico_vendas[MAX_ITENS];  // Vetor que guarda todas as vendas finalizadas

int qtd_produtos = 0; // Contador de quantos produtos existem no vetor 'estoque'
int qtd_vendas = 0;   // Contador de quantas vendas existem no vetor 'historico_vendas'

/*
 * PROTÓTIPOS DE FUNÇÕES
 * Avisam ao compilador (o tradutor do C) que essas funções existem e 
 * serão detalhadas mais para baixo no código. Sem isso, o C lê de cima para baixo
 * e daria erro ao tentar usar uma função antes de ela ser declarada.
 */
void limpar_tela();
void limpar_buffer();
void inicializar_dados();
void tela_abertura();
void exibir_cabecalho(const char *titulo);
void menu_principal();
void cadastrar_substancia();
void mostrar_estoque();
void realizar_venda();
void mostrar_lucro_e_alertas();
void salvar_relatorio_txt();
void gerar_relatorio(FILE *arquivo);

/*
 * FUNÇÃO PRINCIPAL (MAIN)
 * Ponto de partida obrigatório de qualquer programa em C.
 */
int main() {
    inicializar_dados(); // Popula o sistema com 3 itens pré-cadastrados
    tela_abertura();     // Exibe a arte inicial em binário
    menu_principal();    // Inicia o loop infinito do menu
    return 0;            // Retorna 0 ao sistema operacional, indicando sucesso
}

/*
 * FUNÇÃO: limpar_tela
 * Executa um comando no terminal do sistema operacional para apagar os textos antigos.
 */
void limpar_tela() {
    #ifdef _WIN32      // Se o sistema operacional for Windows
        system("cls"); // Usa o comando cls
    #else              // Se for Linux ou Mac (Unix)
        system("clear"); // Usa o comando clear
    #endif
}

/*
 * FUNÇÃO: limpar_buffer
 * Quando usamos o scanf(), ele deixa um "Enter" (\n) perdido na memória (buffer do teclado).
 * Se não limparmos esse Enter, o próximo fgets() vai ler o Enter vazio e pular a entrada de dados.
 * Essa função consome todos os caracteres restantes até achar o '\n'.
 */
void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/*
 * FUNÇÃO: inicializar_dados
 * Preenche as primeiras posições do vetor 'estoque' para facilitar os testes 
 * sem precisar cadastrar tudo do zero toda vez que rodar o programa.
 */
void inicializar_dados() {
    estoque[0] = (Produto){1, "Maconha", 10.0, 150};
    estoque[1] = (Produto){2, "Cocaina", 50.0, 80};
    estoque[2] = (Produto){3, "Crack", 5.0, 200};
    qtd_produtos = 3; // Atualiza o contador informando que já temos 3 itens
}

/*
 * FUNÇÃO: tela_abertura
 * Apenas imprime as cores e os caracteres 1 e 0 para formar o desenho.
 */
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
    getchar(); // Pausa o programa até o usuário apertar Enter
}

/*
 * FUNÇÃO: exibir_cabecalho
 * Uma função auxiliar que recebe um texto (o título da tela) e padroniza a interface.
 * Evita repetir os comandos de printf da borda em cada tela do sistema.
 */
void exibir_cabecalho(const char *titulo) {
    limpar_tela();
    printf(COR_ROXO "=========================================================\n" COR_RESET);
    printf(COR_CIANO "                 BOCA DO PICA PAU BIRUTA                 \n" COR_RESET);
    printf(COR_ROXO "=========================================================\n" COR_RESET);
    printf(COR_BRANCO " TELA ATUAL: %s\n" COR_RESET, titulo);
    printf(COR_ROXO "---------------------------------------------------------\n" COR_RESET);
}

/*
 * FUNÇÃO: menu_principal
 * Mantém o programa rodando através de um laço "do-while".
 * Ele lê uma opção, entra na função correta (via switch) e repete até o usuário digitar 0.
 */
void menu_principal() {
    int opcao;
    do {
        exibir_cabecalho("PAINEL PRINCIPAL");
        printf(COR_CIANO "  [1]" COR_BRANCO " Vender Substância\n" COR_RESET);
        printf(COR_CIANO "  [2]" COR_BRANCO " Ver Estoque Atual\n" COR_RESET);
        printf(COR_CIANO "  [3]" COR_BRANCO " Adicionar Nova Substância\n" COR_RESET);
        printf(COR_CIANO "  [4]" COR_BRANCO " Ver Relatório de Vendas e Alertas\n" COR_RESET);
        printf(COR_CIANO "  [5]" COR_BRANCO " Gerar Relatório TXT\n" COR_RESET);
        printf(COR_VERMELHO "  [0]" COR_BRANCO " Sair do Sistema\n" COR_RESET);
        printf(COR_ROXO "---------------------------------------------------------\n" COR_RESET);
        printf(" Escolha o que quer fazer: ");
        
        // Lê a opção do teclado. Se o usuário digitar letras no lugar de números,
        // o scanf falha (retorna != 1) e forçamos opcao = -1 para cair no 'default'.
        if (scanf("%d", &opcao) != 1) {
            opcao = -1;
        }
        limpar_buffer(); // Limpa o "Enter" que ficou no teclado após o scanf.

        switch (opcao) {
            case 1: realizar_venda(); break;
            case 2: mostrar_estoque(); break;
            case 3: cadastrar_substancia(); break;
            case 4: mostrar_lucro_e_alertas(); break;
            case 5:
                salvar_relatorio_txt();
                printf("\nAperte Enter para voltar.");
                getchar();
                break;
            case 0: 
                limpar_tela();
                printf(COR_VERMELHO "\n Fechando sistema...\n\n" COR_RESET);
                break;
            default:
                printf(COR_VERMELHO "\n Opção errada. Aperte Enter para tentar de novo.\n" COR_RESET);
                getchar();
        }
    } while (opcao != 0); // Continua girando no menu enquanto a opção não for 0 (Sair).
}

/*
 * FUNÇÃO: cadastrar_substancia
 * Cria uma nova "Produto" temporária, preenche com os dados digitados
 * e depois salva essa estrutura dentro do vetor global 'estoque'.
 */
void cadastrar_substancia() {
    exibir_cabecalho("CADASTRO DE PRODUTO");
    
    // Trava de segurança: impede cadastro se o limite do vetor for atingido.
    if (qtd_produtos >= MAX_ITENS) {
        printf(COR_VERMELHO " Memória cheia. Não dá para cadastrar mais nada.\n" COR_RESET);
        printf("\nAperte Enter para voltar.");
        getchar();
        return; // Retorna ao menu imediatamente, cancelando o resto da função.
    }

    Produto p; // Variável temporária para segurar os dados digitados
    
    printf(" Nome da Substância (ou digite '0' para cancelar): ");
    fgets(p.nome, sizeof(p.nome), stdin); // Lê o texto incluindo espaços.
    p.nome[strcspn(p.nome, "\n")] = 0;    // Remove a quebra de linha '\n' que o fgets captura no final.

    // Compara o que foi digitado com "0". Se for igual a 0, cancela a operação.
    if (strcmp(p.nome, "0") == 0) {
        printf(COR_AMARELO "\n Cadastro cancelado com sucesso.\n" COR_RESET);
        printf("\nAperte Enter para voltar ao menu.");
        getchar();
        return;
    }

    p.id = qtd_produtos + 1; // O ID é gerado automaticamente baseado na quantidade atual.
    
    printf(" Preço de Venda (R$): ");
    scanf("%f", &p.preco);
    
    printf(" Quantidade que está entrando: ");
    scanf("%d", &p.estoque);
    limpar_buffer(); // Sempre após um scanf, limpamos o buffer antes do final da função.

    estoque[qtd_produtos] = p; // Copia os dados da variável 'p' para dentro do vetor principal.
    qtd_produtos++;            // Incrementa o contador para saber que existe +1 produto registrado.

    printf(COR_VERDE "\n Substância adicionada ao estoque!\n" COR_RESET);
    printf("\nAperte Enter para voltar.");
    getchar();
}

/*
 * FUNÇÃO: mostrar_estoque
 * Navega pelo vetor 'estoque' do começo (0) até o número atual de itens (qtd_produtos)
 * e imprime as informações organizadas na tela usando formatação do printf.
 */
void mostrar_estoque() {
    exibir_cabecalho("ESTOQUE DISPONÍVEL");
    
    // %-5s significa: imprima um texto (string - s) e preencha com espaços à direita até dar 5 caracteres.
    // Isso é útil para alinhar as colunas da tabela no terminal.
    printf(COR_BRANCO "%-5s %-25s %-15s %-10s\n", "ID", "Substância", "Preço (R$)", "Quantidade" COR_RESET);
    printf(COR_ROXO "---------------------------------------------------------\n" COR_RESET);
    
    for (int i = 0; i < qtd_produtos; i++) {
        // %02d imprime um inteiro com 2 casas (ex: 1 vira 01, 5 vira 05).
        // %.2f imprime um decimal com apenas 2 casas após a vírgula.
        printf(" [%02d] %-25s R$ %-13.2f %-10d\n", estoque[i].id, estoque[i].nome, estoque[i].preco, estoque[i].estoque);
    }
    printf(COR_ROXO "---------------------------------------------------------\n" COR_RESET);
    printf("\nAperte Enter para voltar.");
    getchar();
}

/*
 * FUNÇÃO: realizar_venda
 * Permite adicionar vários itens à mesma venda (o "carrinho" de compras),
 * deduz as quantidades do estoque e, no final, salva um registro da venda inteira.
 */
void realizar_venda() {
    // Validações iniciais para garantir que a venda pode ocorrer
    if (qtd_produtos == 0) {
        exibir_cabecalho("REGISTRAR VENDA");
        printf(COR_VERMELHO " Não há nenhuma substância cadastrada no estoque.\n" COR_RESET);
        printf("\nAperte Enter para voltar.");
        getchar();
        return;
    }

    if (qtd_vendas >= MAX_ITENS) {
        exibir_cabecalho("REGISTRAR VENDA");
        printf(COR_VERMELHO " Histórico de vendas cheio. Não é possível registrar novas vendas.\n" COR_RESET);
        printf("\nAperte Enter para voltar.");
        getchar();
        return;
    }

    // Prepara o "molde" da nova venda temporária (o carrinho de compras)
    Venda nova_venda;
    nova_venda.id_venda = qtd_vendas + 1;
    nova_venda.qtd_itens = 0;
    nova_venda.valor_total = 0.0;

    int continuar = 1; // Variável para controlar o laço de repetição abaixo. 1 = sim, 0 = não.

    // Este laço executa enquanto o operador quiser continuar adicionando mais produtos à mesma nota.
    do {
        exibir_cabecalho("REGISTRAR VENDA - ADICIONAR SUBSTÂNCIA");
        printf(COR_BRANCO " O que tem na casa:\n" COR_RESET);
        for (int i = 0; i < qtd_produtos; i++) {
            printf("   ID: %d | %-15s | Preço: R$ %.2f | No Estoque: %d\n", estoque[i].id, estoque[i].nome, estoque[i].preco, estoque[i].estoque);
        }
        printf(COR_ROXO "---------------------------------------------------------\n" COR_RESET);
        
        int id_busca;
        printf(" Digite o ID da substância vendida: ");
        scanf("%d", &id_busca);
        limpar_buffer();

        int index = -1; // Posição do produto encontrado no vetor. -1 indica que não foi achado ainda.
        
        // Busca o produto correspondente ao ID digitado
        for (int i = 0; i < qtd_produtos; i++) {
            if (estoque[i].id == id_busca) {
                index = i; // Encontrou! Salva a posição.
                break;     // Interrompe o loop de busca, pois já achou.
            }
        }

        if (index == -1) {
            printf(COR_VERMELHO "\n Substância não encontrada.\n" COR_RESET);
            printf("\nAperte Enter para tentar adicionar outro ID.");
            getchar();
            continue; // Pula o resto deste bloco e recomeça o "do-while"
        }

        int qtd_venda;
        // Laço específico para validar a quantidade digitada
        do {
            printf(" Quantidade de %s (ou 0 para desistir desse item): ", estoque[index].nome);
            scanf("%d", &qtd_venda);
            limpar_buffer();

            if (qtd_venda == 0) {
                break; // Se digitou zero, sai deste laço interno de quantidade.
            }

            if (qtd_venda < 0) {
                printf(COR_VERMELHO " Quantidade inválida. Digite um valor maior que zero.\n\n" COR_RESET);
                qtd_venda = estoque[index].estoque + 1; // Força uma condição inválida para repetir o laço
                continue;
            }

            if (qtd_venda > estoque[index].estoque) {
                printf(COR_VERMELHO " Não há quantidade suficiente. Estoque atual: %d unidades.\n" COR_RESET, estoque[index].estoque);
                printf(" Insira um valor válido.\n\n");
            }
        } while (qtd_venda > estoque[index].estoque); // Repete se pedir mais do que existe no estoque.

        // Se confirmou uma quantidade maior que zero
        if (qtd_venda > 0) {
            estoque[index].estoque -= qtd_venda; // Subtrai o item do estoque físico

            ItemVenda iv; // Cria um molde do item da venda
            iv.id_produto = estoque[index].id;
            iv.quantidade = qtd_venda;
            iv.subtotal = qtd_venda * estoque[index].preco; // Multiplica qtd pelo preço unitário

            // Coloca o item na lista de itens da venda (o carrinho)
            nova_venda.itens[nova_venda.qtd_itens] = iv;
            nova_venda.qtd_itens++; // Incrementa quantos itens diferentes o carrinho possui
            nova_venda.valor_total += iv.subtotal; // Acumula o valor na conta final

            printf(COR_VERDE "\n -> %d unidade(s) de %s adicionada(s) à lista da venda!\n" COR_RESET, qtd_venda, estoque[index].nome);
        }

        // Verifica se alcançou o limite físico do carrinho (10 tipos diferentes por nota)
        if (nova_venda.qtd_itens >= MAX_ITENS_POR_VENDA) {
            printf(COR_AMARELO "\n Limite máximo de %d tipos de substâncias por venda atingido.\n" COR_RESET, MAX_ITENS_POR_VENDA);
            break; // Força a saída do laço principal, encerrando a venda compulsoriamente.
        }

        printf("\n Quer colocar mais alguma substância nessa mesma venda? (1 - Sim / 0 - Fechar Venda): ");
        if (scanf("%d", &continuar) != 1) {
            continuar = 0; // Se digitar lixo, entende como fechar a venda (0)
        }
        limpar_buffer();

    } while (continuar == 1);

    // Finaliza e salva o registro, desde que o carrinho não esteja vazio
    if (nova_venda.qtd_itens > 0) {
        // Bloco responsável por pegar a data/hora do sistema operacional
        time_t t = time(NULL);                      // Pega o número bruto de tempo do sistema
        struct tm *tm_info = localtime(&t);         // Converte para dia, mês, ano, hora, etc.
        strftime(nova_venda.data_hora, sizeof(nova_venda.data_hora), "%d/%m/%Y %H:%M:%S", tm_info); // Formata numa string bonitinha.

        // Salva a nota temporária no banco de dados definitivo (vetor global)
        historico_vendas[qtd_vendas] = nova_venda;
        qtd_vendas++;

        // Exibe o "Recibo"
        exibir_cabecalho("VENDA CONCLUÍDA");
        printf(COR_VERDE " VENDA REGISTRADA COM SUCESSO!\n" COR_RESET);
        printf(COR_BRANCO " Resumo da Nota:\n" COR_RESET);
        
        // Percorre todos os itens dessa nota específica para mostrar
        for(int i = 0; i < nova_venda.qtd_itens; i++) {
            int p_id = nova_venda.itens[i].id_produto;
            char nome_prod[30] = "Desconhecido";
            
            // Busca o nome do produto no vetor principal pelo ID salvo no carrinho
            for(int j = 0; j < qtd_produtos; j++) {
                if(estoque[j].id == p_id) {
                    strcpy(nome_prod, estoque[j].nome); // Copia o texto para nome_prod
                    break;
                }
            }
            printf("   - %d x %s | Subtotal: R$ %.2f\n", nova_venda.itens[i].quantidade, nome_prod, nova_venda.itens[i].subtotal);
        }
        printf(COR_ROXO "---------------------------------------------------------\n" COR_RESET);
        printf(" Valor Total Pago: " COR_VERDE "R$ %.2f\n" COR_RESET, nova_venda.valor_total);
    } else {
        // Entra aqui caso o usuário tenha entrado na tela de venda e depois cancelado a digitação
        printf(COR_VERMELHO "\n Nenhum item foi adicionado. Venda descartada.\n" COR_RESET);
    }

    printf("\nAperte Enter para voltar ao painel principal.");
    getchar();
}

/*
 * FUNÇÃO: mostrar_lucro_e_alertas
 * Passa por todas as vendas salvas para somar o dinheiro e gerar relatórios,
 * e passa pelo estoque para verificar avisos de itens críticos (< 10 unidades).
 */
void mostrar_lucro_e_alertas() {
    exibir_cabecalho("RELATÓRIO GERAL E DETALHADO DE VENDAS");
    float faturamento_bruto = 0; // Variável acumuladora
    
    // Soma todos os totais de todas as vendas do histórico
    for (int i = 0; i < qtd_vendas; i++) {
        faturamento_bruto += historico_vendas[i].valor_total; // Acumulação
    }

    // Parte 1: Resumo Matemático
    printf(COR_BRANCO " Resultados Gerais:\n" COR_RESET);
    printf(" Total de atendimentos realizados: %d\n", qtd_vendas);
    printf(" Faturamento bruto total: " COR_VERDE "R$ %.2f\n" COR_RESET, faturamento_bruto);
    printf(COR_ROXO "---------------------------------------------------------\n" COR_RESET);
    
    // Parte 2: Relatório Detalhado Venda por Venda
    printf(COR_BRANCO " HISTÓRICO DETALHADO DE ATENDIMENTOS:\n" COR_RESET);
    if (qtd_vendas == 0) {
        printf(COR_AMARELO "   Nenhuma venda realizada até o momento.\n" COR_RESET);
    } else {
        // Laço externo: Passa por cada Venda
        for (int i = 0; i < qtd_vendas; i++) {
            printf(COR_AMARELO "   [Venda #%02d] - Data/Hora: %s - Valor Pago: R$ %.2f\n" COR_RESET, 
                   historico_vendas[i].id_venda, 
                   historico_vendas[i].data_hora, 
                   historico_vendas[i].valor_total);
                   
            // Laço interno: Passa por cada Produto dentro daquela venda específica
            for (int j = 0; j < historico_vendas[i].qtd_itens; j++) {
                int p_id = historico_vendas[i].itens[j].id_produto;
                char nome_prod[30] = "Desconhecido";
                
                // Pega a descrição real do produto
                for (int k = 0; k < qtd_produtos; k++) {
                    if (estoque[k].id == p_id) {
                        strcpy(nome_prod, estoque[k].nome);
                        break;
                    }
                }
                
                // Exibe em formato de "bala" (bullet point)
                printf("     • %d x %s (Subtotal: R$ %.2f)\n", 
                       historico_vendas[i].itens[j].quantidade, 
                       nome_prod, 
                       historico_vendas[i].itens[j].subtotal);
            }
            printf("\n"); // Pula linha entre as notas
        }
    }
    printf(COR_ROXO "---------------------------------------------------------\n" COR_RESET);
    
    // Parte 3: Checagem Crítica de Estoque
    printf(COR_BRANCO " Substâncias acabando (menos de 10 unidades):\n" COR_RESET);
    int alertas = 0; // Se terminar em 0, significa que nenhum produto acionou o alerta
    for (int i = 0; i < qtd_produtos; i++) {
        if (estoque[i].estoque < 10) {
            printf(COR_VERMELHO "   -> %s está com estoque baixo: %d unidades\n" COR_RESET, estoque[i].nome, estoque[i].estoque);
            alertas++;
        }
    }
    if (alertas == 0) {
        printf(COR_VERDE "   Tudo abastecido. Nenhuma substância acabando.\n" COR_RESET);
    }
    
    printf(COR_ROXO "---------------------------------------------------------\n" COR_RESET);
    printf("\nAperte Enter para voltar.");
    getchar();
}
/*
 * FUNÇÃO: gerar_relatorio
 * Recebe um ponteiro para um arquivo aberto para escrita e escreve dentro dele
 * o mesmo conteúdo do relatório detalhado que mostramos na tela, mas formatado para um arquivo
 */
void gerar_relatorio(FILE *arquivo) {
    float faturamento_bruto = 0;

    // Calcula o faturamento bruto somando o valor total de todas as vendas
    for (int i = 0; i < qtd_vendas; i++) {
        faturamento_bruto += historico_vendas[i].valor_total;
    }

    fprintf(arquivo, "RELATORIO GERAL E DETALHADO DE VENDAS\n");
    fprintf(arquivo, "=====================================\n\n");

    fprintf(arquivo, "Resultados Gerais:\n");
    fprintf(arquivo, "Total de atendimentos realizados: %d\n", qtd_vendas);
    fprintf(arquivo, "Faturamento bruto total: R$ %.2f\n\n", faturamento_bruto);

    fprintf(arquivo, "HISTORICO DETALHADO DE ATENDIMENTOS:\n");

    // Se não houver vendas, já informa isso no relatório.
    if (qtd_vendas == 0) {
        fprintf(arquivo, "Nenhuma venda realizada ate o momento.\n");
    } else {
        // Laço externo para cada venda
        for (int i = 0; i < qtd_vendas; i++) {

            fprintf(arquivo,
                "\n[Venda #%02d] - Data/Hora: %s - Valor Pago: R$ %.2f\n",
                historico_vendas[i].id_venda,
                historico_vendas[i].data_hora,
                historico_vendas[i].valor_total);

            for (int j = 0; j < historico_vendas[i].qtd_itens; j++) {

                int p_id = historico_vendas[i].itens[j].id_produto;
                char nome_prod[30] = "Desconhecido";

                // Busca o nome do produto no estoque para colocar no relatório
                for (int k = 0; k < qtd_produtos; k++) {
                    if (estoque[k].id == p_id) {
                        strcpy(nome_prod, estoque[k].nome);
                        break;
                    }
                }

                // Escreve a linha do item vendido no relatório, com quantidade, nome e subtotal.
                fprintf(arquivo,
                    "   - %d x %s (Subtotal: R$ %.2f)\n",
                    historico_vendas[i].itens[j].quantidade,
                    nome_prod,
                    historico_vendas[i].itens[j].subtotal);
            }
        }
    }

    fprintf(arquivo, "\n\nALERTAS DE ESTOQUE:\n");

    int alertas = 0;

    // Passa por todo o estoque para verificar quais produtos estão com quantidade menor que 10 e escreve no relatório.
    for (int i = 0; i < qtd_produtos; i++) {
        if (estoque[i].estoque < 10) {
            fprintf(arquivo,
                "-> %s esta com estoque baixo: %d unidades\n",
                estoque[i].nome,
                estoque[i].estoque);

            alertas++;
        }
    }

    // Se nenhum produto acionou o alerta, já informa isso no relatório.
    if (alertas == 0) {
        fprintf(arquivo, "Tudo abastecido. Nenhum alerta.\n");
    }
}
/*
 * FUNÇÃO: salvar_relatorio_txt
 * Gera um nome de arquivo único baseado na data e hora atual, cria um arquivo de texto,
 * chama a função gerar_relatorio para escrever o conteúdo dentro do arquivo, e depois fecha o arquivo.
 */
void salvar_relatorio_txt() {
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);

    // Verificação de segurança para garantir que a data e hora foram obtidas corretamente
    if (tm_info == NULL) {
        printf("Erro ao obter data e hora.\n");
        return;
    }

    char nome_arquivo[100];
    char data_relatorio[50];

    // Gera um nome de arquivo no formato "relatorio_DD-MM-AAAA_HH-MM-SS.txt"
    strftime(nome_arquivo, sizeof(nome_arquivo),
             "relatorio_%d-%m-%Y_%H-%M-%S.txt",
             tm_info);
    strftime(data_relatorio, sizeof(data_relatorio),
             "%d/%m/%Y %H:%M:%S",
             tm_info);
    FILE *arquivo = fopen(nome_arquivo, "w");

    // Verificação de segurança para garantir que o arquivo foi criado com sucesso
    if (arquivo == NULL) {
        printf("Erro ao criar o arquivo.\n");
        return;
    }

    // Escreve o cabeçalho do relatório, incluindo a data e hora de geração
    fprintf(arquivo, "========================================\n");
    fprintf(arquivo, " RELATORIO DE VENDAS E ESTOQUE\n");
    fprintf(arquivo, " Gerado em: %s\n", data_relatorio);
    fprintf(arquivo, "========================================\n\n");

    // Chama a função que escreve o conteúdo completo do relatório dentro do arquivo aberto.
    gerar_relatorio(arquivo);
    
    // Depois de terminar de escrever tudo, fecha o arquivo para garantir que os dados sejam salvos corretamente.
    fclose(arquivo);

    // Informa o usuário que o relatório foi salvo e mostra o nome do arquivo gerado.
    printf(COR_VERDE "\nRelatorio salvo com sucesso!\n" COR_RESET);
    printf("Arquivo: %s\n", nome_arquivo);
}