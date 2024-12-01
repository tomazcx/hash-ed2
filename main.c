#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define TABLE_MAX_SIZE 100
#define NAME_MAX_SIZE 100
#define PHONE_MAX_SIZE 100

typedef struct HashNode {
	char* key;
	char* phone;
	struct HashNode* next;
} HashNode;

typedef struct {
	HashNode** nodes;
	int size;
	int count;
} HashTable;

unsigned int hash(char* key) {
	unsigned int result = 0;

	for(int i = 0; key[i]; i++){
		result += key[i];
	}

	return result % TABLE_MAX_SIZE;
}

HashTable* initTable(){
	HashTable* table = malloc(sizeof(HashTable));
	table->size = 0;
	table->count = 0;
	table->nodes = malloc(sizeof(HashNode) * TABLE_MAX_SIZE);
	return table;
}

void freeTable(HashTable* table){
	if (table == NULL) return;

	for (int i = 0; i < TABLE_MAX_SIZE; i++) {
		HashNode* node = table->nodes[i];

		while (node != NULL) {
			HashNode* temp = node; 
			node = node->next;
			free(temp->key);
			free(temp->phone);
			free(temp);
		}
	}

	free(table->nodes);
	free(table);
}


void adicionarContato(HashTable* table) {	
	char name[NAME_MAX_SIZE];
	char phone[PHONE_MAX_SIZE];
	printf("Nome: ");
	scanf("%s", name); 
	printf("Telefone: ");
	scanf("%s", phone); 

	if(table->size == TABLE_MAX_SIZE){
		printf("Erro ao adicionar contato: tamanho máximo atigindo.\n");
		return;
	}

	unsigned int key = hash(name);

	if(table->nodes[key] != NULL && strcmp(table->nodes[key]->key, name) == 0){
		printf("Nome já registrado.\n");
		return;
	}

	HashNode *node = (HashNode*)malloc(sizeof(HashNode));
	node->key = malloc(strlen(name) * sizeof(char));
	node->phone = malloc(strlen(phone) * sizeof(char));
	strcpy(node->key, name);
	strcpy(node->phone, phone);
	node->next = table->nodes[key];
	table->nodes[key] = node;
	table->size++;
}

void buscarContato(HashTable* table) {
	clock_t start = clock();
	char name[NAME_MAX_SIZE];
	HashNode* node;
	printf("Nome: ");
	scanf("%s", name); 

	unsigned int key = hash(name);

	node = table->nodes[key];
	while(node != NULL){
		if(strcmp(node->key, name) == 0){
			clock_t end = clock();
			double time_spent = ((double)end - start) / CLOCKS_PER_SEC;
			printf("Telefone de %s: %s (tempo de busca: %.4f ms)\n", node->key, node->phone, time_spent);
			return;
		}
		node = node->next;
	}

	printf("\nContato com o nome %s não encontrado\n", name);

	return;
}

void removerContato(HashTable* table) {
	char name[NAME_MAX_SIZE];
	HashNode* node;
	printf("Nome: ");
	scanf("%s", name); 

	unsigned int key = hash(name);
	node = table->nodes[key];
	if(node == NULL){
		printf("Contato com o nome %s não encontrado\n", name);
		return;
	}

	free(node);
	table->nodes[key] =  NULL;
	printf("Contato Removido.\n");
	table->size--;
}

void exibirContatos(HashTable* table) {
	printf("Contatos registrados: \n");

	for(int i = 0; i < TABLE_MAX_SIZE; i++){
		HashNode* crrNode = table->nodes[i];
		while(crrNode != NULL){
			printf("%s - %s\n", crrNode->key, crrNode->phone);
			crrNode = crrNode->next;
		}
	}
}

int main() {
	int opcao;
	HashTable* table = initTable();

	do {
		printf("\nEscolha uma opcao:\n");
		printf("1 - Adicionar contato\n");
		printf("2 - Buscar contato por nome\n");
		printf("3 - Remover contato\n");
		printf("4 - Exibir todos os contatos\n");
		printf("0 - Sair\n");
		printf("Digite uma opcao: ");

		scanf("%d", &opcao);

		switch (opcao) {
			case 1:
				adicionarContato(table);
				break;
			case 2:
				buscarContato(table);
				break;
			case 3:
				removerContato(table);
				break;
			case 4:
				exibirContatos(table);
				break;
			case 0:
				printf("Saindo...\n");
				break;
			default:
				printf("Opcao invalida! Tente novamente.\n");
		}
	} while (opcao != 0);

	freeTable(table);
	return 0;
}
