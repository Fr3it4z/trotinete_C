#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>  // Para Sleep() no Windows
#else
#include <unistd.h>   // Para sleep() no Linux/macOS
#endif

typedef struct {
    int nivelBateria;
    int numeroTrotinete;
    bool estadoEstacionamento;
} Trotinete;

void registar_log_trotinete(int numero, const char *evento) {
    FILE *log_file = fopen("logs.txt", "a");
    if (log_file == NULL) {
        perror("Erro ao abrir o ficheiro de log");
        return;
    }

    time_t agora = time(NULL);
    struct tm *tempo = localtime(&agora);
    char data_hora[64];
    strftime(data_hora, sizeof(data_hora), "%d-%m-%Y %H:%M:%S", tempo);

    fprintf(log_file, "%s - Trotinete %d - %s\n", data_hora, numero, evento);
    fclose(log_file);
}
void simular_descarregamento(Trotinete *t, int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        if (!t[i].estadoEstacionamento && t[i].nivelBateria > 0) {
            int antes = t[i].nivelBateria;

            t[i].nivelBateria -= 5;
            if (t[i].nivelBateria < 0)
                t[i].nivelBateria = 0;

            // Registo no log
            char msg[64];
            snprintf(msg, sizeof(msg), "Descarregamento: %d%% → %d%%", antes, t[i].nivelBateria);
            registar_log_trotinete(t[i].numeroTrotinete, msg);
        }
    }
}
void simular_carregamento(Trotinete *t, int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        if (t[i].estadoEstacionamento && t[i].nivelBateria < 100) {
            int antes = t[i].nivelBateria;
            int nivelAtual = antes;

            printf("Trotinete %d em carregamento... (%d%%)\n", t[i].numeroTrotinete, nivelAtual);

            while (nivelAtual < 100) {
                int anterior = nivelAtual;
                nivelAtual += 10;
                if (nivelAtual > 100) nivelAtual = 100;

                // Registo no log de cada passo
                char msg[64];
                snprintf(msg, sizeof(msg), "Carregamento: %d%% → %d%%", anterior, nivelAtual);
                registar_log_trotinete(t[i].numeroTrotinete, msg);
            }

            // Atualiza bateria no array
            t[i].nivelBateria = 100;

            // Log final
            registar_log_trotinete(t[i].numeroTrotinete, "Carregamento concluído");
        }
    }
}



int contar_trotinetes_no_json(const char *nomeFicheiro) {
    FILE *ficheiro = fopen(nomeFicheiro, "r");
    if (ficheiro == NULL) {
        perror("Erro ao abrir ficheiro para contagem");
        return 0;
    }

    char linha[256];
    int contador = 0;

    while (fgets(linha, sizeof(linha), ficheiro)) {
        if (strstr(linha, "\"numeroTrotinete\"")) {
            contador++;
        }
    }

    fclose(ficheiro);
    return contador;
}

int carregar_estado_json(Trotinete trotinete[], int maxTamanho) {
    FILE *ficheiro = fopen("estado_trotinetes.json", "r");
    if (ficheiro == NULL) {
        perror("Erro ao abrir o ficheiro JSON");
        return 0;
    }

    char linha[256];
    int count = 0;
    int numero, bateria;
    char estado[6]; // "true" ou "false"

    while (fgets(linha, sizeof(linha), ficheiro) != NULL && count < maxTamanho) {
        if (strstr(linha, "\"numeroTrotinete\"")) {
            sscanf(linha, " %*[^:]: %d,", &numero);
            fgets(linha, sizeof(linha), ficheiro); // próxima linha: nivelBateria
            sscanf(linha, " %*[^:]: %d,", &bateria);
            fgets(linha, sizeof(linha), ficheiro); // próxima linha: estadoEstacionamento
            sscanf(linha, " %*[^:]: %5s", estado);
            trotinete[count].numeroTrotinete = numero;
            trotinete[count].nivelBateria = bateria;
            trotinete[count].estadoEstacionamento = strcmp(estado, "true") == 0 ? true : false;
            count++;
        }
    }

    fclose(ficheiro);
    return count;
}

void guardar_estado_json(Trotinete trotinete[], int tamanho) {
    FILE *ficheiro = fopen("estado_trotinetes.json", "w");
    if (ficheiro == NULL) {
        perror("Erro ao abrir o ficheiro JSON");
        return;
    }

    fprintf(ficheiro, "[\n");
    for (int i = 0; i < tamanho; i++) {
        fprintf(ficheiro, "  {\n");
        fprintf(ficheiro, "    \"numeroTrotinete\": %d,\n", trotinete[i].numeroTrotinete);
        fprintf(ficheiro, "    \"nivelBateria\": %d,\n", trotinete[i].nivelBateria);
        fprintf(ficheiro, "    \"estadoEstacionamento\": %s\n", trotinete[i].estadoEstacionamento ? "true" : "false");
        fprintf(ficheiro, "  }%s\n", (i < tamanho - 1) ? "," : "");
    }
    fprintf(ficheiro, "]\n");

    fclose(ficheiro);
}


void clear() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}
void space() {
    getchar();
    getchar();
}
void listarTrotinetes(Trotinete trotinete[], int tamanho) {
    printf("\n--- Lista de Trotinetes ---\n");
    for (int i = 0; i < tamanho; i++) {
        printf("Scooter Nº: %d\n", trotinete[i].numeroTrotinete);
        printf("Nível de bateria: %d%%\n", trotinete[i].nivelBateria);
        printf("Estacionada: %s\n\n", trotinete[i].estadoEstacionamento ? "Sim" : "Não");
    }
}

// Função para exibir o menu
void menu(Trotinete trotinete[], int tamanho) {
    int opcao, id;

    do {
        simular_descarregamento(trotinete, tamanho);
        simular_carregamento(trotinete, tamanho); 
        clear();
        printf("\n--- Menu ---\n");
        printf("1. Estacionar trotinete\n");
        printf("2. Alugar trotinete\n");
        printf("3. Listar trotinetes\n");
        printf("4. Sair\n");
        printf("Escolha uma opção: ");

        if (scanf("%d", &opcao) != 1) {
            printf("Entrada inválida! Insira um número.\n");
            while (getchar() != '\n'); // Limpa o buffer
            getchar();
            continue;
        }
        clear();
        switch (opcao) {
            case 1: // Estacionar trotinete
            case 2: // Alugar trotinete
                listarTrotinetes(trotinete, tamanho);
                char buffer[16];
                bool entradaValida = false;
                
                do {
                    printf("(9 para voltar ao menu)\n");
                    printf("Digite o número da trotinete: ");
                    scanf("%s", buffer);
                
                    entradaValida = true;
                    for (int i = 0; buffer[i] != '\0'; i++) {
                        if (buffer[i] < '0' || buffer[i] > '9') {
                            entradaValida = false;
                            break;
                        }
                    }
                
                    if (!entradaValida) {
                        printf("Entrada inválida! Insira apenas números.\n");
                    }
                
                } while (!entradaValida);
                
                id = atoi(buffer);
                
                // Novo: se for 9, volta ao menu
                if (id == 9) {
                    printf("Operação cancelada. A voltar ao menu...\n");
                    space();
                    break;
                }                

                for (int i = 0; i < tamanho; i++) {
                    if (trotinete[i].numeroTrotinete == id) {
                        if (opcao == 1) {
                            if (trotinete[i].estadoEstacionamento) {
                                printf("Trotinete já estacionada.\n");
                            } else {
                                trotinete[i].estadoEstacionamento = true;
                                printf("Trotinete %d estacionada com sucesso!\n", trotinete[i].numeroTrotinete);
                                registar_log_trotinete(trotinete[i].numeroTrotinete, "Estacionada");
                                simular_carregamento(&trotinete[i], 1);
                                guardar_estado_json(trotinete, tamanho);
                            }
                        } else {
                            if (!trotinete[i].estadoEstacionamento) {
                                printf("A trotinete %d não se encontra disponível.\n", trotinete[i].numeroTrotinete);
                            } else {
                                trotinete[i].estadoEstacionamento = false;
                                printf("Trotinete %d alugada com sucesso!\n", trotinete[i].numeroTrotinete);
                                registar_log_trotinete(trotinete[i].numeroTrotinete, "Alugada");
                                guardar_estado_json(trotinete, tamanho);
                            }
                        }
                        space();
                        break;
                    }
                }
                break;

            case 3: // Listar trotinetes
                listarTrotinetes(trotinete, tamanho);
                space();
                break;

            case 4: // Sair
                printf("A sair...\n");
                break;

            default:
                printf("Opção inválida!\n");
                space();
                break;
        }

    } while (opcao != 4);
}

int main() {
    clear();
    #ifdef _WIN32
    system("chcp 65001 > nul");
    #endif

    // Contar quantas trotinetes estão no ficheiro JSON
    int totalTrotinetes = contar_trotinetes_no_json("estado_trotinetes.json");

    // Alocar dinamicamente o array de trotinetes
    Trotinete *trotinete = malloc(totalTrotinetes * sizeof(Trotinete));
    if (trotinete == NULL) {
        perror("Erro ao alocar memória");
        return 1;
    }

    // Carregar os dados do JSON para o array
    int tamanho = carregar_estado_json(trotinete, totalTrotinetes);

    // Iniciar o menu
    menu(trotinete, tamanho);

    // Libertar memória no fim do programa
    free(trotinete);

    return 0;
}