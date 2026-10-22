#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_MATERIAS_SEMESTRE 8
#define MAX_SEMESTRE  3

// STRUCT 1
typedef struct {     
    char nome_materia[50];    // Ex: "webdesign"
    float ac1;                 
    float ac2;
	float af;                 
    float media;     
	int preenchida;         
}Materia;

//STRUCT 2
typedef struct{
	int num_semestre; // de 1 a 3 semestres
	Materia lista_materias[MAX_MATERIAS_SEMESTRE];  // Vetor de materias deste semestre
	int qtd_materias; // quantas materias foram lancadas nesse semestre
}Semestre;

// STRUCT 3 
typedef struct {
    char nome[100];           // Nome do aluno (com tamanho fixo para o arquivo binario)
    int ra;                   // Registro Academico
    int semestre_atual;       // Semestre atual dele
    int ativo;             // exclusao do aluno
    Semestre Historico[MAX_SEMESTRE];   // ANINHAMENTO DE STRUCT: O aluno carrega o seu historico completo de semestres
} Aluno;

// escopo das funcoes
void cadastrar_aluno(Aluno **cadastro, int *total, int *capacidade);
void listar_alunos(Aluno *cadastro, int total);
void carregar_arquivo(Aluno **cadastro, int *total, int *capacidade);
void salvar_arquivo(Aluno *cadastro, int total);
void buscar_aluno(Aluno *cadastro, int total);

int main() {
	
	int capacidade = 1; //limite de vagas (todos semestres)
	int total = 0; //numero de vagas (todos semestres)
	int op = 0;
	
	Aluno *cadastro = (Aluno *) malloc(capacidade * sizeof(Aluno));
	
	if(cadastro == NULL){
		printf("Erro ao alocar memoria!");
		exit(1);
	}
	
	carregar_arquivo(&cadastro, &total, &capacidade);  // carrega o arquivo binario no inicio 
	
	do{
		
		printf("=== MENU ===\n");
		printf("1 - Aluno\n");
		printf("2 - Materias\n");
		printf("3 - Semestre\n");
		printf("0 - Fechar Programa\n");
		printf("\nOpcao: ");
		scanf("%i", &op);
		getchar();
		printf("\n");
		
		switch(op){
			
			int opt;
			
			case 1:
				
				do{
				
					printf("=== MENU ALUNO ===\n");
					printf("1 - Cadastrar aluno\n");
					printf("2 - Listar alunos\n");
					printf("3 - Buscar aluno\n");
					printf("4 - Atualizar aluno\n");
					printf("0 - Voltar ao menu principal\n");
					printf("\nOpcao: ");
					scanf("%i", &opt);
					getchar();
					printf("\n");
					
					switch(opt){
						
						case 1:
							cadastrar_aluno(&cadastro, &total, &capacidade);
						break;
							
						case 2:
							listar_alunos(cadastro, total);
						break;
						
						case 3:
							buscar_aluno(cadastro, total);
						break;
						
						case 4:
						break;
						
						case 0:
						break;
						
					}
				
				}while(opt != 0);
			
			break;
				
			case 2:
				
				do{
				
					printf("=== MENU MATERIAS ===\n");
					printf("1 - Cadastrar materias\n");
					printf("2 - Listar materias\n");
					printf("3 - Buscar materias\n");
					printf("4 - Atualizar materias\n");
					printf("0 - Voltar ao menu principal\n");
					printf("\nOpcao: ");
					scanf("%i", &opt);
					getchar();
					printf("\n");
					
					switch(opt){
						
						case 1:
						break;
							
						case 2:
						break;
						
						case 3:
						break;
						
						case 4:
						break;
						
						case 0:
						break;
						
					}
				
				}while(opt != 0);
			
			break;
			
			case 3:
				
				do{
				
					printf("=== MENU NOTAS ===\n");
					printf("1 - Cadastrar notas\n");
					printf("2 - Listar notas\n");
					printf("3 - Atualizar notas\n");
					printf("0 - Voltar ao menu principal\n");
					printf("\nOpcao: ");
					scanf("%i", &opt);
					getchar();
					printf("\n");
					
					switch(opt){
						
						case 1:
						break;
							
						case 2:
						break;
						
						case 3:
						break;
						
						case 4:
						break;
						
						case 0:
						break;
						
					}
				
				}while(opt != 0);
				
			break;
			
			case 0:
			break;
			
			default: printf("Opcao invalida");		
		}
		
		
	}while(op != 0);
	
	salvar_arquivo(cadastro, total);
	
	free(cadastro);
    return 0;
}

void cadastrar_aluno(Aluno **cadastro, int *total, int *capacidade){
	
	if(*total >= *capacidade){
		*capacidade += 1;
		Aluno *temp = (Aluno *) realloc(*cadastro, (*capacidade) * sizeof(Aluno));
		if(temp == NULL){
			printf("Erro ao alocar memoria");
			exit(1);
		}
		*cadastro = temp;
	}
	
	Aluno novo;
	novo.ativo = 1; // aluno ativo sempre que for criado
	
	// Inicializa o historico deste novo aluno para limpar lixo de memoria
	int i, m;
    for (i = 0; i < MAX_SEMESTRE; i++) {
        novo.Historico[i].num_semestre = i + 1;
        novo.Historico[i].qtd_materias = 0;
        for (int m = 0; m < MAX_MATERIAS_SEMESTRE; m++) {
            novo.Historico[i].lista_materias[m].preenchida = 0;
        }
    }
    
    printf("=== CADASTRANDO ALUNO N %i ===\n", *total + 1);
    printf("Nome do aluno: ");
    fgets(novo.nome, sizeof(novo.nome), stdin);
    novo.nome[strcspn(novo.nome, "\n")] = 0;
    
    printf("RA do aluno: ");
    scanf("%d", &novo.ra);
    
    do{
    	printf("Semestre atual (1 a 3): ");
	    scanf("%d", &novo.semestre_atual);
	    getchar();
	    if(novo.semestre_atual > 3 || novo.semestre_atual < 1){
	    	printf("ERRO: semestre invalido, tente novamente.\n");
		}
	}while(novo.semestre_atual > 3 || novo.semestre_atual < 1 );
	
	(*cadastro)[*total] = novo;   // pega o array cadastro e coloca a struct aluno novo em um endereco 
	(*total)++;    // vai somando o numero de alunos cadastrados
	
	printf("Aluno cadastrado com sucesso!\n\n");
}

void listar_alunos(Aluno *cadastro, int total){
	int encontrou = 0;
	printf("\n=== LISTAGEM DE ALUNOS ATIVO ===\n");
	int i;
	for(i = 0; i < total; i++){
		if((cadastro+i)->ativo == 1){
			printf("RA: %d | Nome: %s | Semestre Atual: %d\n", (cadastro+i)->ra, (cadastro+i)->nome, (cadastro+i)->semestre_atual);
            encontrou = 1;
		}
	}
    if (!encontrou) {
        printf("Nenhum aluno ativo registrado no sistema.\n");
    }
    printf("\n");
}

void carregar_arquivo(Aluno **cadastro, int *total, int *capacidade){
	
	FILE *arq = fopen("alunos.bin", "rb");
	if(arq == NULL){
		return;
	}
	
	Aluno temp;
	
	while(fread(&temp, sizeof(Aluno), 1, arq) == 1){  // le uma struct por vez direto do disco para a memoria RAM
	
		if(*total >= *capacidade){
			*capacidade *= 2; // vai aumentando o tamanho da capacidade
			*cadastro = (Aluno *) realloc(*cadastro, *capacidade * sizeof(Aluno)); // realloca para aumentar o array, para cadastrar mais alunos
		}
		(*cadastro)[*total] = temp;  // pega o array cadastro e coloca a struct aluno novo em um endereco
		(*total)++;       // vai somando o numero de alunos cadastrados
	}
	
	fclose(arq);
}

void salvar_arquivo(Aluno *cadastro, int total){
	
	FILE *arq = fopen("alunos.bin", "wb");
    if (arq == NULL) {
        return;
    }
	
	fwrite(cadastro, sizeof(Aluno), total, arq);   // Escreve o vetor inteiro de structs diretamente no arquivo
	
	fseek(arq, 0, SEEK_END);    // Move o cursor invisivel para o fim do arquivo
	long tamanho_bytes = ftell(arq); // Conta quantos bytes existem ate a posicao atual do cursor
	printf("\n[Sistema] %ld bytes salvos com sucesso em 'alunos.bin'!\n", tamanho_bytes);
	
	fclose(arq); // Fecha o arquivo garantindo a gravacao no HD
}

void buscar_aluno(Aluno *cadastro, int total){
	int ra_busca, i;
	int encontrou = 0;
	
	printf("=== BUSCAR ALUNO ===\n");
    printf("Digite o RA do aluno para pesquisar: ");
    scanf("%d", &ra_busca);
    getchar();
    
    for(i = 0; i < total; i++){
    	if((cadastro+i)->ativo == 1 && (cadastro+i)->ra == ra_busca){
    		printf("\n=========================================\n");
            printf("ALUNO ENCONTRADO:\n");
            printf("Nome: %s\n", (cadastro+i)->nome);
            printf("RA: %d\n", (cadastro+i)->ra);
            printf("Semestre Atual: %d\n", (cadastro+i)->semestre_atual);
            printf("=========================================\n");
            
            // Exibe o historico de materias e notas do aluno detalhado por semestre
            int tem_materias = 0, s, m;
            for (s = 0; s < MAX_SEMESTRE; s++) {   // percorre todos os semestres do aluno 
                int qtd_mat = (cadastro+i)->Historico[s].qtd_materias;  // armazena as materias daquele semestre
                
                if (qtd_mat > 0) {
                    printf("\n[Semestre %d]\n", (cadastro+i)->Historico[s].num_semestre);
                    for (m = 0; m < qtd_mat; m++) {    // percorre as maaterias do semestre daquele aluno
                        Materia mat = (cadastro+i)->Historico[s].lista_materias[m];   // variavel para pegar a materia 
                        printf("  * Materia: %-15s | AC1: %4.1f | AC2: %4.1f | AF: %4.1f | Media: %4.1f\n",
                               mat.nome_materia, mat.ac1, mat.ac2, mat.af, mat.media);
                    }
                    tem_materias = 1;
                }
            }
            
            if (!tem_materias) {
                printf("\nEste aluno ainda nao possui materias cadastradas.\n");
            }
            printf("=========================================\n\n");
            return; // Encontrou o aluno, imprime tudo e sai da função imediatamente
        }
    }

    // Se o laço 'for' terminar e a variável 'encontrou' continuar 0, o aluno não existe
    if (!encontrou) {
        printf("Erro: Aluno com o RA %d nao foi encontrado ou foi removido.\n\n", ra_busca);
    }
}