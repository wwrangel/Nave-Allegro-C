//////////////////////////////////////////////////////
//      Trabalho de Linguagem de Programação C      //
//      MESC UFF Rio das Ostras   - 2018            //
//      Aluno: Wagner Rangel Campos                 //
//      Professores:                                //
//      Dr. Carlos Bazilio Martins                  //
//      Dr. Patrick Barbosa Moratori                //
//      Neste trabalho foi desenvolvido um jogo     //
//      utilizando conceitos apresentados nas aulas //
//      de Linguagem de programação C               //
//                                                  //
//      JOGO DE NAVES UTILIZANDO BIBLIOTECA ALLEGRO //
//      TECLAS:                                     //
//      SETAS PARA MOVIMENTAÇÃO                     //
//      ESPAÇO PARA TIROS                           //
//////////////////////////////////////////////////////

enum {ESPACONAVE, TIRO, ALIEN};

/* Criando estruturas e definindo tipos para os objetos */
typedef struct
{
	int ID;
	int x;
	int y;
	int vidas;
	int velocidade;
	int borda_x;
	int borda_y;
	int pontos;
	int bonus;
}NaveEspacial;

typedef struct
{
	int ID;
	int x;
	int y;
	int velocidade;
	bool ativo;
}Tiros;

typedef struct
{
	int ID;
	int x;
	int y;
	int velocidade;
	int borda_x;
	int borda_y;
	bool ativo;
	int tipo;
}Aliens;
