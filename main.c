#include <stdio.h>
#include <stdlib.h>
#include <locale.h >	//habilita o portugu�s
#include <string.h >	//habilita o uso de string
#include <time.h >	//habilita o uso da data e hora
#include <conio.h >	//habilita fun��es e macros para manipula��o de caracteres
#include <ctype.h >	//habilita fun��es para manipula caracteres na tela
	//definindo consatntes do programa
#define ARQ "Contatos.txt"	// nome do arquivo com os contatos
#define INSERIR '1'
#define ALTERAR '2'
#define APAGAR '3'
#define LISTAR '4'
#define NIVER_MES '5'
#define PESQUISAR '6'
#define PESQ_LETRA '1'
#define PESQ_NOME '2'
#define SAIR '0'
//Fun��es das op��es do menu
char *MainMenu[] = { "1. Inserir contato",
	"2. Alterar contato",
	"3. Apagar contato",
	"4. Listar de todos contatos",
	"5. Aniversariantes do m�s",
	"6. Pesquisar",
	"0. Sair",
	NULL /*ACABARAM AS OP��ES*/
};
//Sub-Menu de pesquisa
char *SubMenu[] = { "1. Pesquisar pela primeira letra do nome",
	"2. Pesquisar por nome",
	"0. Sair",
	NULL /*ACABARAM AS OP��ES*/
};
/*vari�vel global pois � �til ao longo do programa, referente ao arquivo*/
FILE * ctt;
//struct com os dados do contato
typedef struct
{
	char nome[40];
	int *dd;
	int *telefone;
	int dia;
	int mes;
	char Status;	//indica que o contato est� apagado
}

Contato;
//prototipo da mensagem S
void Mensagem(char *msg);
//l� o contato de um registro introduzido pelo usu�rio
void Ler_Contato(Contato *c)
{
	printf("Nome: ");
	gets(c->nome);
	printf("Digite o DD do contato: ");
	scanf("%d", &c->dd);
	printf("Telefone: ");
	scanf("%d", &c->telefone);	// (->) simbolo para pegar os valores do struct
	printf("Dia do anivers�rio: ");
	scanf("%d", &c->dia);
	printf("M�s do anivers�rio: ");
	scanf("%d", &c->mes);
	c->Status = ' ';
	fflush(stdin);	//limpa a memoria do buffer
}

//Mostra na tela os contatos existentes no registro
void Mostrar_Contato(Contato c)
{
	// (%-40s) alinhamento a esquerda antes da largura do campo
	printf("Nome: %-40s", c.nome);
	printf("Telefone: (%3d) %8d ", c.dd, c.telefone);
	printf("Data de Anivers�rio: %02d / %02d\n", c.dia, c.mes);
}

//Adiciona um contato ao arquivo(agenda)
void Adiciona_Contato(Contato c)
{
	//Reposiciona o indicador de posi��o do fluxo em fun��o do deslocamento.
	/*fluxo - Fluxo a ter seu indicador de posi��o alterado.
	deslocamento - Diferen�a de posi��o relativa � origem.
	origem - De onde partir� o deslocamento de posi��o.*/
	fseek(ctt, 0L, SEEK_END);
	//fwrite tenta escrever para o fluxo numero_itens elementos
	if (fwrite(&c, sizeof(c), 1, ctt) != 1)
		Mensagem("Adicionar contato: Falhou a escrita do registro");
}

//Colocar uma mensagem na tela
void Mensagem(char *msg)
{
	printf(msg);
	getchar();
}

//fun��o que verifica se o arquivo exite se n�o existir, cria um.
//se j� existir, abre-o em modo de leitura e escrita (r+b)
void Inicio_arq()
{
	ctt = fopen(ARQ, "r+b");	// tenta abrir
	if (ctt == NULL)
	{
		ctt = fopen(ARQ, "w+b");
		if (ctt == NULL)
		{
			printf(" Erro fatal: imposs�vel criar arquivo de dados\n");	//stderr - erro padr�o
			exit(1);
		}
	}
}

/*Faz um menu simples com as op��es do vetor de strings.
seleciona a op��o, usando o primeiro caracter de cada string.
devolve o primeiro caracter da op��o.
*/
char Menu(char *opcoes[])
{
	int i;
	char ch;
	while (1)
	{
		//system("cls");
		printf("\n\n\n\n\n");
		for (i = 0; opcoes[i] != NULL; i++)
			printf("\t\t%s\n\n", opcoes[i]);
		printf("\n\n\t\tOpcao: ");
		ch = getchar();
		fflush(stdin);
		for (i = 0; opcoes[i] != NULL; i++)
			if (opcoes[i][0] == ch)
				return ch;
	}
}

//inseri o contato
void Inserir_Contato()
{
	Contato x;
	Ler_Contato(&x);
	Adiciona_Contato(x);
}

//altera o contato apartir do id
void Alterar_Contato()
{
	Contato x;
	long int n_reg;
	printf("Qual o n�mero do registro: ");
	scanf("%ld", &n_reg);
	fflush(stdin);
	if (fseek(ctt, (n_reg - 1) *sizeof(Contato), SEEK_SET) != 0)
	{
		Mensagem("Registro inexistente ou problemas no posicionamento!!!");
		return;
	}

	if (fread(&x, sizeof(Contato), 1, ctt) != 1)
	{
		Mensagem("Problemas na leitura do registro!!!");
		return;
	}

	if (x.Status == '*')
	{
		Mensagem("Um registro apagado n�o pode ser alterado!!! \n\n");
		return;
	}

	printf("\n\n Dados Atuais \n\n");
	Mostrar_Contato(x);
	printf("\n\n Novos dados \n\n");
	Ler_Contato(&x);
	// recuar um registro no arquivo
	fseek(ctt, -(long) sizeof(Contato), SEEK_CUR);
	// reescrever o registro;
	fwrite(&x, sizeof(Contato), 1, ctt);
	fflush(ctt); /*despejar os arquivos no disco r�gido*/
}

//apaga um contato na lista de contatos
void Apagar_Contato()
{
	Contato x;
	long int n_reg;
	char resp;
	printf("Qual o n�mero do registro: ");
	scanf("%ld", &n_reg);
	fflush(stdin);
	if (fseek(ctt, (n_reg - 1) *sizeof(Contato), SEEK_SET) != 0)
	{
		Mensagem("Registro inexistente ou problemas no registro!!!");
		return;
	}

	if (fread(&x, sizeof(Contato), 1, ctt) != 1)
	{
		Mensagem("Problema na leitura do registro!!!");
		return;
	}

	if (x.Status == '*')
	{
		Mensagem("Registro j� est� apagado!!!\n\n");
	}

	printf("\n\n Dados atuais \n\n");
	Mostrar_Contato(x);
	printf("\n\n Apagar o registro (s/n)???: ");
	resp = getchar();
	fflush(stdin);
	if (toupper(resp) != 'S') return;
	x.Status = '*';
	// recuar um registro no arquivo
	fseek(ctt, -(long) sizeof(Contato), SEEK_CUR);
	// reescrever o registro;
	fwrite(&x, sizeof(Contato), 1, ctt);
	fflush(ctt); /*Despejar os arquivos no disco r�gido*/
}

//lista os contatos
void Listar()
{
	long int n_Linhas = 0;
	Contato reg;
	rewind(ctt);
	while (1)
	{
		if (fread(&reg, sizeof(reg), 1, ctt) != 1) break; /*Sair do la�o*/
		if (reg.Status == '*') continue; /*Passa ao pr�ximo*/
		Mostrar_Contato(reg);
		n_Linhas++;
		if (n_Linhas % 20 == 0)
			Mensagem("Pressione<Enter> para continuar . . .");
	}

	Mensagem("\n\n Pressione<Enter> para continuar . . ."); /*No fim da listagem*/
}

//pesquisa por nome
void Pesquisar_Nome(char *s)
{
	Contato reg;
	//Volta o indicador de posi��o do fluxo para a posi��o inicial. Isto �, o come�o do arquivo.
	// Essa fun��o � equivalente a utilizar fseek(fluxo, 0, SEEK_SET); .
	rewind(ctt);
	while (fread(&reg, sizeof(Contato), 1, ctt))
		if (reg.Status != '*' && strstr(reg.nome, s))
			Mostrar_Contato(reg);
	// else
	// printf("Contato n�o encontrado");
	Mensagem("\n\n Pressione<Enter> para continuar . . ."); /*No fim da listagem*/
}

//pesquisa apartir da primeira letra do nome
void Pesquisar_Letra(char *s)
{
	Contato reg;
	rewind(ctt);
	while (fread(&reg, sizeof(Contato), 1, ctt))
		if (reg.Status != '*' && strstr(reg.nome, s))
			Mostrar_Contato(reg);
	// else
	// printf("Contato n�o encontrado");
	Mensagem("\n\n Pressione<Enter> para continuar . . ."); /*No fim da listagem*/
}

//pesquisa aniversariantes do mes
void Mes_Niver()
{
	Contato reg;
	rewind(ctt);
	//fun��o que pega a data atual
	struct tm agora_data = { 0 };
	time_t tempo_data;
	time(&tempo_data);
	agora_data = *localtime(&tempo_data);
	printf("\n Data atual:\n");
	printf(" %d / %d / %d\n\n", agora_data.tm_mday, agora_data.tm_mon + 1,
		agora_data.tm_year + 1900);
	printf(" Os aniversariantes do m�s �:\n");
	//pesquisa o contato que � aniversariante do m�s
	while (fread(&reg, sizeof(Contato), 1, ctt))
	{
		if (reg.Status != '*' && reg.mes == agora_data.tm_mon + 1)
			Mostrar_Contato(reg);
		// else
		// printf("N�o h� aniversariantes neste m�s");
	}
}

int main(int argc, char *argv[])
{
	setlocale(LC_ALL, "Portuguese");
	char opcao, subOpcao;
	Inicio_arq();
	while ((opcao = Menu(MainMenu)) != SAIR)
		switch (opcao)
		{
			case INSERIR:
				Inserir_Contato();
				break;
			case ALTERAR:
				Alterar_Contato();
				break;
			case APAGAR:
				Apagar_Contato();
				break;
			case LISTAR:
				Listar();
				break;
			case NIVER_MES:
				Mes_Niver();
				break;
			case PESQUISAR:
				while ((subOpcao = Menu(SubMenu)) != SAIR)
					switch (subOpcao)
					{
						case PESQ_LETRA:
							{
								char string[BUFSIZ + 1];
								printf("Digite uma letra para buscar os contatos exitentes com essa letra: ");
								gets(string);fflush(stdin);
								Pesquisar_Letra(string);
								break;
							}

						case PESQ_NOME:
							{
								char string[BUFSIZ + 1];
								printf("Qual nome deseja procurar: ");
								gets(string);fflush(stdin);
								Pesquisar_Nome(string);
								break;
							}
					}
		}

	system("pause");
	return 0;
}
