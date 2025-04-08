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
void simular_carregamento(Trotinete *t, int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        if (t[i].estadoEstacionamento && t[i].nivelBateria < 100) {
            int antes = t[i].nivelBateria;

            printf("⏳ A carregar Trotinete %d...\n", t[i].numeroTrotinete);
            while (t[i].nivelBateria < 100) {
                #ifdef _WIN32
                    Sleep(1000); // Windows
                #else
                    sleep(1);    // Linux/macOS
                #endif
                t[i].nivelBateria += 10;
                if (t[i].nivelBateria > 100) t[i].nivelBateria = 100;

                // Log
                char msg[64];
                snprintf(msg, sizeof(msg), "Carregamento: %d%% → %d%%", antes, t[i].nivelBateria);
                registar_log_trotinete(t[i].numeroTrotinete, msg);
                antes = t[i].nivelBateria;

                // Terminal: barra de progresso
                printf("\r🔋 Bateria: [");
                int blocos = t[i].nivelBateria / 10;
                for (int b = 0; b < 10; b++) {
                    printf(b < blocos ? "█" : " ");
                }
                printf("] %d%%", t[i].nivelBateria);
                fflush(stdout);
            }

            printf("\n✅ Trotinete %d totalmente carregada!\n", t[i].numeroTrotinete);
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
        clear();
        printf("\n--- Menu ---\n");
        printf("1. Estacionar trotinete\n");
        printf("2. Alugar trotinete\n");
        printf("3. Listar trotinetes\n");
        printf("4. Sair\n");
        printf("Escolha uma opção: ");
        if (scanf("%d", &opcao) != 1) {
            printf("Entrada inválida! Insira um número.\n");
            while (getchar() != '\n'); // Limpa o buffer de entrada
            getchar();
            continue;
        }
             

        clear();
        if (opcao == 1 || opcao == 2) {
            listarTrotinetes(trotinete, tamanho);
            printf("Digite o número da trotinete: ");
            scanf("%d", &id);
            // Procurar a trotinete pelo número
            int encontrada = 0;
            for (int i = 0; i < tamanho; i++) {
                if (trotinete[i].numeroTrotinete == id) {
                    encontrada = 1;
                    if (opcao == 1) { // Estacionar
                        if (trotinete[i].estadoEstacionamento) {
                            printf("Trotinete já estacionada.\n");
                        } else {
                            trotinete[i].estadoEstacionamento = true;
                            printf("Trotinete %d estacionada com sucesso!\n", trotinete[i].numeroTrotinete);
                            registar_log_trotinete(trotinete[i].numeroTrotinete, "Estacionada");
                            simular_carregamento(trotinete, tamanho);
                            guardar_estado_json(trotinete, tamanho);
                        }
                    } else if (opcao == 2) { // Alugar
                        if (!trotinete[i].estadoEstacionamento) {
                            printf("A trotinete %d não se encontra disponível.\n", trotinete[i].numeroTrotinete);
                        } else {
                            trotinete[i].estadoEstacionamento = false;
                            printf("\n Trotinete %d alugada com sucesso!\n", trotinete[i].numeroTrotinete);
                            registar_log_trotinete(trotinete[i].numeroTrotinete, "Alugada");
                            guardar_estado_json(trotinete, tamanho);
                        }
                    }
                    space();
                    break;
                }
            }
            if (!encontrada) {
                printf("Trotinete não encontrada!\n");
                space();
                clear();
            }
        } else if (opcao == 3) {
            listarTrotinetes(trotinete, tamanho);
            space();
        }else{printf("Opção inválida!");};

    } while (opcao != 4);
    if (opcao == 4){exit(0);}
    
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