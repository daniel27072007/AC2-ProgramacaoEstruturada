#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// STRUCT 1: Aninhada (contém dados do curso/disciplina)
typedef struct {
    char nome_curso[50];
    int carga_horaria;
} Curso;

// STRUCT 2: Principal (possui 5 campos, atendendo ao requisito de 4+)
typedef struct {
    int ra;
    char nome[100];
    int semestre;
    int ativo; // 1 = ativo, 0 = removido (exclusão lógica)
    Curso curso_matriculado; // Estrutura aninhada
} Aluno;

// Protótipos das funções
void carregar_arquivo(Aluno **cadastro, int *total, int *capacidade);
void salvar_e_sair(Aluno *cadastro, int total);
void cadastrar_aluno(Aluno **cadastro, int *total, int *capacidade);
void buscar_aluno(Aluno *cadastro, int total);
void atualizar_aluno(Aluno *cadastro, int total);
void remover_aluno(Aluno *cadastro, int total);
void listar_alunos(Aluno *cadastro, int total);

int main() {
    int capacidade = 2; // Capacidade inicial do vetor dinâmico
    int total = 0;      // Número atual de alunos cadastrados
    int op = 0;
    
    // Alocação dinâmica inicial (malloc)
    Aluno *cadastro = (Aluno *) malloc(capacidade * sizeof(Aluno));
    
    if (cadastro == NULL) {
        printf("Erro ao alocar memoria!\n");
        exit(1);
    }
    
    // Carrega os dados salvos anteriormente do arquivo binário
    carregar_arquivo(&cadastro, &total, &capacidade); 
    
    do {
        printf("\n=== SISTEMA ACADEMICO ===\n");
        printf("1 - Cadastrar\n");
        printf("2 - Buscar\n");
        printf("3 - Atualizar\n");
        printf("4 - Remover\n");
        printf("5 - Listar\n");
        printf("6 - Sair\n");
        printf("\nOpcao: ");
        scanf("%i", &op);
        getchar(); // Limpa o buffer do teclado
        printf("\n");
        
        switch(op) {
            case 1:
                cadastrar_aluno(&cadastro, &total, &capacidade);
                break;
            case 2:
                buscar_aluno(cadastro, total);
                break;
            case 3:
                atualizar_aluno(cadastro, total);
                break;
            case 4:
                remover_aluno(cadastro, total);
                break;
            case 5:
                listar_alunos(cadastro, total);
                break;
            case 6:
                salvar_e_sair(cadastro, total);
                break;
            default:
                printf("Opcao invalida! Tente novamente.\n");
        }
        
    } while(op != 6);

    return 0;
}

// Carrega dados do arquivo binário para a memória RAM ao iniciar
void carregar_arquivo(Aluno **cadastro, int *total, int *capacidade) {
    FILE *arq = fopen("alunos.bin", "rb");
    if (arq == NULL) {
        return; // Arquivo ainda não existe, primeira execução
    }
    
    Aluno temp;
    while (fread(&temp, sizeof(Aluno), 1, arq) == 1) {
        if (*total >= *capacidade) {
            *capacidade *= 2;
            Aluno *temp_realloc = (Aluno *) realloc(*cadastro, (*capacidade) * sizeof(Aluno));
            if (temp_realloc == NULL) {
                printf("Erro ao realocar memoria ao carregar arquivo!\n");
                fclose(arq);
                exit(1);
            }
            *cadastro = temp_realloc;
        }
        (*cadastro)[*total] = temp;
        (*total)++;
    }
    
    fclose(arq);
}

// Salva os dados no arquivo binário, exibe o tamanho em bytes com fseek/ftell e libera a memória
void salvar_e_sair(Aluno *cadastro, int total) {
    FILE *arq = fopen("alunos.bin", "wb");
    if (arq == NULL) {
        printf("Erro ao abrir arquivo para escrita!\n");
        free(cadastro);
        return;
    }
    
    // Grava o vetor inteiro de structs diretamente no arquivo binário
    fwrite(cadastro, sizeof(Aluno), total, arq);
    
    // Utilização de fseek e ftell conforme exigido
    fseek(arq, 0, SEEK_END);
    long tamanho_bytes = ftell(arq);
    
    printf("[Sistema] %ld bytes salvos com sucesso em 'alunos.bin'. Programa encerrado.\n", tamanho_bytes);
    
    fclose(arq);
    free(cadastro); // Liberação da memória dinâmica (free)
}

// Função que utiliza ponteiro para ponteiro (Aluno **cadastro) para permitir realocação
void cadastrar_aluno(Aluno **cadastro, int *total, int *capacidade) {
    if (*total >= *capacidade) {
        *capacidade *= 2;
        Aluno *temp = (Aluno *) realloc(*cadastro, (*capacidade) * sizeof(Aluno));
        if (temp == NULL) {
            printf("Erro ao realocar memoria!\n");
            exit(1);
        }
        *cadastro = temp;
    }
    
    Aluno novo;
    novo.ativo = 1; // Aluno ativo por padrão
    
    printf("=== CADASTRAR ALUNO ===\n");
    printf("Nome do aluno: ");
    fgets(novo.nome, sizeof(novo.nome), stdin);
    novo.nome[strcspn(novo.nome, "\n")] = 0;
    
    printf("RA do aluno: ");
    scanf("%d", &novo.ra);
    getchar();
    
    do {
        printf("Semestre: ");
        scanf("%d", &novo.semestre);
        getchar();
        if (novo.semestre < 1) {
            printf("ERRO: Numero de semestre invalido!\n");
        }
    } while (novo.semestre < 1);
    
    printf("Nome do curso: ");
    fgets(novo.curso_matriculado.nome_curso, sizeof(novo.curso_matriculado.nome_curso), stdin);
    novo.curso_matriculado.nome_curso[strcspn(novo.curso_matriculado.nome_curso, "\n")] = 0;
    
    printf("Carga horaria do curso: ");
    scanf("%d", &novo.curso_matriculado.carga_horaria);
    getchar();
    
    (*cadastro)[*total] = novo;
    (*total)++;
    
    printf("\nAluno cadastrado com sucesso!\n");
}

// Busca aluno por RA (utiliza ponteiro simples)
void buscar_aluno(Aluno *cadastro, int total) {
    int ra_busca;
    printf("=== BUSCAR ALUNO ===\n");
    printf("Digite o RA do aluno: ");
    scanf("%d", &ra_busca);
    getchar();
    
    for (int i = 0; i < total; i++) {
        if ((cadastro + i)->ativo == 1 && (cadastro + i)->ra == ra_busca) {
            printf("\n-----------------------------------------\n");
            printf("ALUNO ENCONTRADO:\n");
            printf("Nome: %s\n", (cadastro + i)->nome);
            printf("RA: %d\n", (cadastro + i)->ra);
            printf("Semestre: %d\n", (cadastro + i)->semestre);
            printf("Curso: %s (Carga Horaria: %dh)\n", 
                   (cadastro + i)->curso_matriculado.nome_curso,
                   (cadastro + i)->curso_matriculado.carga_horaria);
            printf("-----------------------------------------\n");
            return;
        }
    }
    printf("Aluno com RA %d nao encontrado.\n", ra_busca);
}

// Atualiza dados cadastrais do aluno perguntando campo a campo com (y/n)
void atualizar_aluno(Aluno *cadastro, int total) {
    int ra_busca;
    char resp;
    
    printf("=== ATUALIZAR ALUNO ===\n");
    printf("Digite o RA do aluno que deseja atualizar: ");
    scanf("%d", &ra_busca);
    getchar();
    
    for (int i = 0; i < total; i++) {
        if ((cadastro + i)->ativo == 1 && (cadastro + i)->ra == ra_busca) {
            printf("\nAluno encontrado: %s\n", (cadastro + i)->nome);
            printf("Responda com 'y' para sim ou 'n' para nao.\n\n");
            
            // Atualizar Nome
            printf("Deseja atualizar o nome do aluno? (y/n): ");
            scanf(" %c", &resp);
            getchar();
            if (resp == 'y' || resp == 'Y') {
                printf("Novo nome do aluno: ");
                fgets((cadastro + i)->nome, sizeof((cadastro + i)->nome), stdin);
                (cadastro + i)->nome[strcspn((cadastro + i)->nome, "\n")] = 0;
            }
            
            // Atualizar Semestre
            printf("Deseja atualizar o semestre? (y/n): ");
            scanf(" %c", &resp);
            getchar();
            if (resp == 'y' || resp == 'Y') {
                do {
                    printf("Novo semestre (1 a 3): ");
                    scanf("%d", &(cadastro + i)->semestre);
                    getchar();
                    if ((cadastro + i)->semestre < 1 || (cadastro + i)->semestre > 3) {
                        printf("ERRO: Semestre invalido! Digite entre 1 e 3.\n");
                    }
                } while ((cadastro + i)->semestre < 1 || (cadastro + i)->semestre > 3);
            }
            
            // Atualizar Nome do Curso
            printf("Deseja atualizar o nome do curso? (y/n): ");
            scanf(" %c", &resp);
            getchar();
            if (resp == 'y' || resp == 'Y') {
                printf("Novo nome do curso: ");
                fgets((cadastro + i)->curso_matriculado.nome_curso, sizeof((cadastro + i)->curso_matriculado.nome_curso), stdin);
                (cadastro + i)->curso_matriculado.nome_curso[strcspn((cadastro + i)->curso_matriculado.nome_curso, "\n")] = 0;
            }
            
            // Atualizar Carga Horária
            printf("Deseja atualizar a carga horaria do curso? (y/n): ");
            scanf(" %c", &resp);
            getchar();
            if (resp == 'y' || resp == 'Y') {
                printf("Nova carga horaria (em horas): ");
                scanf("%d", &(cadastro + i)->curso_matriculado.carga_horaria);
                getchar();
            }
            
            printf("\nDados atualizados com sucesso!\n");
            return;
        }
    }
    printf("Aluno com RA %d nao encontrado.\n", ra_busca);
}

// Exclusão lógica do aluno (muda ativo para 0)
void remover_aluno(Aluno *cadastro, int total) {
    int ra_busca;
    printf("=== REMOVER ALUNO ===\n");
    printf("Digite o RA do aluno que deseja remover: ");
    scanf("%d", &ra_busca);
    getchar();
    
    for (int i = 0; i < total; i++) {
        if ((cadastro + i)->ativo == 1 && (cadastro + i)->ra == ra_busca) {
            (cadastro + i)->ativo = 0;
            printf("Aluno %s removido com sucesso!\n", (cadastro + i)->nome);
            return;
        }
    }
    printf("Aluno com RA %d nao encontrado.\n", ra_busca);
}

// Listagem com opção de ver todos ou filtrar por nome de curso
void listar_alunos(Aluno *cadastro, int total) {
    int sub_op;
    printf("=== LISTAR ALUNOS ===\n");
    printf("1 - Listar todos os alunos ativos\n");
    printf("2 - Listar por nome de curso\n");
    printf("Opcao: ");
    scanf("%d", &sub_op);
    getchar();
    
    int encontrou = 0;
    
    if (sub_op == 1) {
        printf("\n--- LISTA DE TODOS OS ALUNOS ATIVOS ---\n");
        for (int i = 0; i < total; i++) {
            if ((cadastro + i)->ativo == 1) {
                printf("RA: %d | Nome: %s | Semestre: %d | Curso: %s (%dh)\n",
                       (cadastro + i)->ra,
                       (cadastro + i)->nome,
                       (cadastro + i)->semestre,
                       (cadastro + i)->curso_matriculado.nome_curso,
                       (cadastro + i)->curso_matriculado.carga_horaria);
                encontrou = 1;
            }
        }
    } else if (sub_op == 2) {
        char curso_busca[50];
        printf("Digite o nome do curso para filtrar: ");
        fgets(curso_busca, sizeof(curso_busca), stdin);
        curso_busca[strcspn(curso_busca, "\n")] = 0;
        
        printf("\n--- ALUNOS MATRICULADOS NO CURSO: %s ---\n", curso_busca);
        for (int i = 0; i < total; i++) {
            if ((cadastro + i)->ativo == 1 && strcmp((cadastro + i)->curso_matriculado.nome_curso, curso_busca) == 0) {
                printf("RA: %d | Nome: %s | Semestre: %d (%dh)\n",
                       (cadastro + i)->ra,
                       (cadastro + i)->nome,
                       (cadastro + i)->semestre,
                       (cadastro + i)->curso_matriculado.carga_horaria);
                encontrou = 1;
            }
        }
    } else {
        printf("Opcao invalida!\n");
        return;
    }
    
    if (!encontrou) {
        printf("Nenhum registro encontrado.\n");
    }
    printf("\n");
}