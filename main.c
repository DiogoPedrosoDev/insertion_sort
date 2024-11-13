#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include <locale.h>
#include "algoritmos.h"

// Função para preencher o vetor com valores aleatórios
void preencherVetorAleatorio(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 1000000;
    }
}

// Função para preencher o vetor em ordem crescente
void preencherVetorCrescente(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = i;
    }
}

// Função para preencher o vetor em ordem decrescente
void preencherVetorDecrescente(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = n - i - 1;
    }
}

// Função para salvar o vetor em um arquivo
void salvarVetor(int arr[], int n, char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Erro ao criar o arquivo %s\n", filename);
        return;
    }
    for (int i = 0; i < n; i++) {
        fprintf(file, "%d\n", arr[i]);
    }
    fclose(file);
}

// Função para criar diretórios recursivamente no Windows
void criarDiretorio(const char *path) {
    char comando[512];
    snprintf(comando, sizeof(comando), "mkdir \"%s\"", path);
    system(comando);
}

// Função para executar o algoritmo e salvar os resultados
void principal(int arr[], int n, void (*sortFunc)(int[], int), char *algorithmName, char *tipoVetor) {
    char folderPath[256];
    snprintf(folderPath, sizeof(folderPath), "%s", algorithmName);

    // Definindo caminhos para entrada, tempo e saída nas categorias
    char inputPath[256], timePath[256], outputPath[256];
    snprintf(inputPath, sizeof(inputPath), "%s\\entrada\\%s", folderPath, tipoVetor);
    snprintf(timePath, sizeof(timePath), "%s\\tempo\\%s", folderPath, tipoVetor);
    snprintf(outputPath, sizeof(outputPath), "%s\\saida\\%s", folderPath, tipoVetor);

    // Criando diretórios
    criarDiretorio(inputPath);
    criarDiretorio(timePath);
    criarDiretorio(outputPath);

    // Salvando o vetor antes da ordenação com o tamanho no nome do arquivo
    char inputFile[256];
    snprintf(inputFile, sizeof(inputFile), "%s\\array_%d.txt", inputPath, n);
    salvarVetor(arr, n, inputFile);

    // Executando a ordenação e medindo o tempo
    clock_t start = clock();
    sortFunc(arr, n);
    clock_t end = clock();
    double timeTaken = ((double)(end - start)) / CLOCKS_PER_SEC;

    // Salvando o tempo em arquivo com o tamanho no nome do arquivo
    char timeFile[256];
    snprintf(timeFile, sizeof(timeFile), "%s\\time_%d.txt", timePath, n);
    FILE *timeOutput = fopen(timeFile, "w");
    if (timeOutput != NULL) {
        fprintf(timeOutput, "Tempo de execução: %.6f segundos\n", timeTaken);
        fclose(timeOutput);
    } else {
        printf("Erro ao criar o arquivo de tempo %s\n", timeFile);
    }

    // Salvando o vetor ordenado com o tamanho no nome do arquivo
    char outputFile[256];
    snprintf(outputFile, sizeof(outputFile), "%s\\array_%d.txt", outputPath, n);
    salvarVetor(arr, n, outputFile);
}

int main() {
      // Configurar o encoding para Latin-1 no CMD (código de página 1252)
    system("chcp 1252 > nul");

    // Definir o locale para exibir acentos corretamente
    setlocale(LC_ALL, "Portuguese_Brazil.1252");
    int option, size, tipoVetorOption;
    int *arr = NULL;
    char *tipoVetor;

    printf("Escolha o algoritmo:\n");
    printf("1. Insertion Sort\n");
    printf("2. Selection Sort\n");
    printf("3. Shell Sort\n");
    printf("4. Bubble Sort\n");
    printf("Opção: ");
    scanf("%d", &option);

    printf("Escolha o tamanho do array:\n");
    printf("1. 10\n");
    printf("2. 100\n");
    printf("3. 1000\n");
    printf("4. 10000\n");
    printf("5. 100000\n");
    printf("6. 1000000\n");
    printf("Opção: ");
    scanf("%d", &size);

    switch (size) {
        case 1: size = 10; break;
        case 2: size = 100; break;
        case 3: size = 1000; break;
        case 4: size = 10000; break;
        case 5: size = 100000; break;
        case 6: size = 1000000; break;
        default: printf("Tamanho inválido!\n"); return 1;
    }

    printf("Escolha o tipo de vetor:\n");
    printf("1. Crescente\n");
    printf("2. Decrescente\n");
    printf("3. Aleatório\n");
    printf("Opção: ");
    scanf("%d", &tipoVetorOption);

    arr = (int *)malloc(size * sizeof(int));
    if (arr == NULL) {
        printf("Erro ao alocar memória\n");
        return 1;
    }

    switch (tipoVetorOption) {
        case 1:
            tipoVetor = "crescente";
            preencherVetorCrescente(arr, size);
            break;
        case 2:
            tipoVetor = "decrescente";
            preencherVetorDecrescente(arr, size);
            break;
        case 3:
            tipoVetor = "aleatorio";
            preencherVetorAleatorio(arr, size);
            break;
        default:
            printf("Tipo de vetor inválido!\n");
            free(arr);
            return 1;
    }

    // Executar o algoritmo selecionado
    switch (option) {
        case 1: principal(arr, size, insertionSort, "InsertionSort", tipoVetor); break;
        case 2: principal(arr, size, selectionSort, "SelectionSort", tipoVetor); break;
        case 3: principal(arr, size, shellSort, "ShellSort", tipoVetor); break;
        case 4: principal(arr, size, bubbleSort, "BubbleSort", tipoVetor); break;
        default: printf("Opção inválida!\n"); free(arr); return 1;
    }

    free(arr);
    return 0;
}
