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

/* Diretivas para pré-processamento incluindo bibliotecas
ALLEGRO - específicas para jogos, e outras "nativas" */
#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <stdlib.h>
#include <time.h>

/* Diretivas para pré-processamento incluindo arquivo de
definição tipos de objetos*/
#include "objetos.h"

/* Declarando Constatntes, Variáveis Globais
e tipos específicos da lib Allegro*/
const int DISPLAY_LARG = 800;
const int DISPLAY_ALT = 600;
const int FPS = 60;
const int NUM_TIROS = 10;
const int NUM_ALIENS = 10;


ALLEGRO_BITMAP *imagem_fundo = NULL;
ALLEGRO_BITMAP *imagem_nave = NULL;
ALLEGRO_BITMAP *imagem_alien1 = NULL;
ALLEGRO_BITMAP *imagem_alien2 = NULL;
ALLEGRO_BITMAP *imagem_alien3 = NULL;
ALLEGRO_BITMAP *imagem_alien4 = NULL;
ALLEGRO_BITMAP *imagem_alien5 = NULL;
ALLEGRO_BITMAP *imagem_alien6 = NULL;
ALLEGRO_BITMAP *imagem_alien7 = NULL;
ALLEGRO_BITMAP *imagem_alien8 = NULL;

ALLEGRO_SAMPLE_INSTANCE *inst_musica = NULL;
ALLEGRO_SAMPLE_INSTANCE *inst_som_tiro = NULL;
ALLEGRO_SAMPLE_INSTANCE *inst_som_explosao = NULL;

/* Declarando estrutura de enumeração */
enum KEYS {UP, BOTTON, LEFT, RIGHT, SPACE, ENTER};


/* Declarando Protótipos */
void InitNave(NaveEspacial *nave);
void DesenhaNave(NaveEspacial *nave);
void MoveNaveUP(NaveEspacial *nave);
void MoveNaveBOTTON(NaveEspacial *nave);
void MoveNaveRIGHT(NaveEspacial *nave);
void MoveNaveLEFT(NaveEspacial *nave);

void InitBalas(Tiros balas[], int tamanho);
void AtiraBalas(Tiros balas[], int tamanho, NaveEspacial nave);
void AtualizarBalas(Tiros balas[], int tamanho);
void DesenhaBalas(Tiros balas[], int tamanho);
void ColisaoTiro(Tiros balas[], int b_tamanho, Aliens Aliens[], int c_tamanho, NaveEspacial *nave);

void InitAliens(Aliens Aliens[], int tamanho);
void LiberaAliens(Aliens Aliens[], int tamanho);
void AtualizarAliens(Aliens Aliens[], int tamanho);
void DesenhaAliens(Aliens Aliens[], int tamanho);
void ColisaoAlien(Aliens Aliens[], int c_tamanho, NaveEspacial *nave);

/* Função principal */
int main()
{
    /*Declarando variáveis locais da função main */
	ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_FONT *font18 = NULL;
    ALLEGRO_FONT *font64 = NULL;

	bool end_exit = false;
	bool desenha = true;
	bool game_over = false;
	bool keys[] = {false, false, false, false, false, false};

    /* Chamada as funções de inicialização dos objetos do jogo */
	NaveEspacial nave;
	Tiros balas[NUM_TIROS];
	Aliens Aliens[NUM_ALIENS];

    /* Inicializações da Allegro */
	ALLEGRO_DISPLAY *display = NULL;

	if(!al_init())
	{
		al_show_native_message_box(NULL, "ATENÇÃO!", "ERRO!", "ERRO AO INICIALIZAR ALLEGRO!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return -1;
	}

    /* Procedimentos para maximizar a tela */
	ALLEGRO_MONITOR_INFO info;
	int res_x_comp, res_y_comp;
	al_get_monitor_info(0, &info);
	res_x_comp = info.x2 - info.x1;
	res_y_comp = info.y2 - info.y1;
	al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
	display = al_create_display(res_x_comp, res_y_comp);

	if(!display)
	{
		al_show_native_message_box(NULL, "ATENÇÃO!", "ERRO!", "ERRO AO CRIAR TELA!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return -1;
	}

	float red_x = res_x_comp / (float) DISPLAY_LARG;
	float red_y = res_y_comp / (float) DISPLAY_ALT;

	ALLEGRO_TRANSFORM transformar;

	al_identity_transform(&transformar);
	al_scale_transform(&transformar, red_x, red_y);
	al_use_transform(&transformar);


    /* Inicializando Addons e Instalações Allegro */
    al_init_image_addon();
	al_init_primitives_addon();
	al_install_keyboard();
	al_init_font_addon();
	al_init_ttf_addon();
	al_install_audio();
	al_init_acodec_addon();
	al_reserve_samples(10);


    /* Criando fila e dispositivos Allegro */
	fila_eventos = al_create_event_queue();
	timer = al_create_timer(1.0 / FPS);

	/* Carregando imagens utilizadas no jogo */
    imagem_fundo = al_load_bitmap("space.jpg");
    imagem_nave = al_load_bitmap("nave.png");

    /* Função Allegro para poder usar a cor rosa como transparencia */
    al_convert_mask_to_alpha(imagem_nave,al_map_rgb(255,0,255));

    /* Imagens para Aliens */
    imagem_alien1 = al_load_bitmap("inimigo1.png");
    al_convert_mask_to_alpha(imagem_alien1,al_map_rgb(255,0,255));

    imagem_alien2 = al_load_bitmap("inimigo2.png");
    al_convert_mask_to_alpha(imagem_alien2,al_map_rgb(255,0,255));

    imagem_alien3 = al_load_bitmap("inimigo3.png");
    al_convert_mask_to_alpha(imagem_alien3,al_map_rgb(255,0,255));

    imagem_alien4 = al_load_bitmap("inimigo4.png");
    al_convert_mask_to_alpha(imagem_alien4,al_map_rgb(255,0,255));

    imagem_alien5 = al_load_bitmap("inimigo5.png");
    al_convert_mask_to_alpha(imagem_alien5,al_map_rgb(255,0,255));

    imagem_alien6 = al_load_bitmap("inimigo6.png");
    al_convert_mask_to_alpha(imagem_alien6,al_map_rgb(255,0,255));

    imagem_alien7 = al_load_bitmap("inimigo7.png");
    al_convert_mask_to_alpha(imagem_alien7,al_map_rgb(255,0,255));

    imagem_alien8 = al_load_bitmap("inimigo8.png");
    al_convert_mask_to_alpha(imagem_alien8,al_map_rgb(255,0,255));


    /* Carregando fontes utilizadas no jogo */
	font18 = al_load_font("arial.ttf", 18, 0);
	font64 = al_load_font("arial.ttf", 64, 0);

    /* Preparando tela, instanciando sons e inicializando jogo */
	al_hide_mouse_cursor(display);
	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_draw_text(font64, al_map_rgb(0, 255, 0), DISPLAY_LARG / 2, DISPLAY_ALT / 2, ALLEGRO_ALIGN_CENTRE, "INICIALIZANDO...");
	al_flip_display();

    inst_musica = al_create_sample_instance(al_load_sample("musica.wav"));
	inst_som_tiro = al_create_sample_instance(al_load_sample("som_tiro.wav"));
    inst_som_explosao = al_create_sample_instance(al_load_sample("som_explosao.wav"));

	al_attach_sample_instance_to_mixer(inst_musica, al_get_default_mixer());
	al_attach_sample_instance_to_mixer(inst_som_tiro, al_get_default_mixer());
	al_attach_sample_instance_to_mixer(inst_som_explosao, al_get_default_mixer());

	al_set_sample_instance_playmode(inst_musica, ALLEGRO_PLAYMODE_LOOP);
	al_set_sample_instance_gain(inst_musica, 1.8);

	al_register_event_source(fila_eventos, al_get_display_event_source(display));
	al_register_event_source(fila_eventos, al_get_keyboard_event_source());
	al_register_event_source(fila_eventos, al_get_timer_event_source(timer));


    /* Funções de inicialização */
	srand(time(NULL)); /* gera "semente" baseada em time para rand */

	InitNave(&nave);
	InitBalas(balas, NUM_TIROS);
	InitAliens(Aliens, NUM_ALIENS);

	al_start_timer(timer);

    /* Loop Principal */
	while(!end_exit)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(fila_eventos, &ev);

		/* Eventos do jogo */
		/* Verifica status do display para finalizar o jogo */
	    if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			end_exit = true;
		}

        /* Leitura do teclado (pressionar) */
		else if(ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch(ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
				end_exit = true; /* Se pressionar Esq finaliza o jogo */
				break;
			case ALLEGRO_KEY_UP:
				keys[UP] = true;
				break;
			case ALLEGRO_KEY_DOWN:
				keys[BOTTON] = true;
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = true;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = true;
				break;
			case ALLEGRO_KEY_SPACE:
				keys[SPACE] = true;
				AtiraBalas(balas, NUM_TIROS, nave);
				break;
			case ALLEGRO_KEY_ENTER:
				keys[ENTER] = true;
				break;
			}
		}

        /* Leitura do teclado (soltar) */
		else if(ev.type == ALLEGRO_EVENT_KEY_UP)
		{
			switch(ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_UP:
				keys[UP] = false;
				break;
			case ALLEGRO_KEY_DOWN:
				keys[BOTTON] = false;
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = false;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = false;
				break;
			case ALLEGRO_KEY_ENTER:
				keys[ENTER] = false;
				break;
			}
		}

		/* Evento Timer, executa funções para "redesenhar" e criar novos objetos, verificar e
		 atualizar status definindo se continua ou se é fim de jogo */
		else if(ev.type == ALLEGRO_EVENT_TIMER)
		{
			desenha = true;

			if(keys[UP])
				MoveNaveUP(&nave);
			if(keys[BOTTON])
				MoveNaveBOTTON(&nave);
			if(keys[RIGHT])
				MoveNaveRIGHT(&nave);
			if(keys[LEFT])
				MoveNaveLEFT(&nave);
			if(keys[SPACE])
				AtualizarBalas(balas, NUM_TIROS);

			if(!game_over)
			{
				al_play_sample_instance(inst_musica);
                al_draw_bitmap(imagem_fundo, 0, 0, 0);

				LiberaAliens(Aliens, NUM_ALIENS);
				AtualizarAliens(Aliens, NUM_ALIENS);
				ColisaoTiro(balas, NUM_TIROS, Aliens, NUM_ALIENS, &nave);
				ColisaoAlien(Aliens, NUM_ALIENS, &nave);

				/* Se acabaram as vidas finaliza jogo */
				if(nave.vidas <= 0)
				{
					game_over = true;
				}
			}

			else
			{
				al_stop_sample_instance(inst_musica);
				al_stop_sample_instance(inst_som_explosao);
				al_stop_sample_instance(inst_som_tiro);

				/* Se for pressionado Enter, reinicia o jogo */
				if(keys[ENTER])
				{
					InitNave(&nave);
					InitBalas(balas, NUM_TIROS);
					InitAliens(Aliens, NUM_ALIENS);
					game_over = false;
				}
			}
		}

		/* Fila de eventos de desenho dos objetos */
		if(desenha && al_is_event_queue_empty(fila_eventos))
		{
			desenha = false;

			if(!game_over)
			{
				DesenhaBalas(balas, NUM_TIROS);
				DesenhaAliens(Aliens, NUM_ALIENS);
				DesenhaNave(&nave);
				al_draw_textf(font18, al_map_rgb(0, 255, 0), 0, 0, 0, "VIDAS: %d   /   PONTOS: %d       *** BONUS: %d ***",
                   nave.vidas, nave.pontos, nave.bonus);
			}

			else
			{
				al_draw_textf(font64, al_map_rgb(0, 255, 0), DISPLAY_LARG / 2, DISPLAY_ALT / 3, ALLEGRO_ALIGN_CENTRE,"GAME OVER");

				al_draw_textf(font18, al_map_rgb(0, 255, 0), DISPLAY_LARG / 2, DISPLAY_ALT / 2, ALLEGRO_ALIGN_CENTRE,
					"Your Score: %d", nave.pontos);

                al_draw_textf(font18, al_map_rgb(0, 255, 0), DISPLAY_LARG / 2, DISPLAY_ALT / 2+30, ALLEGRO_ALIGN_CENTRE,
					"Pressione ENTER para nova partida ou ESC para sair.");
			}
			al_flip_display();
			al_clear_to_color(al_map_rgb(0, 0, 0));
		}
	}

    /* Finalizando, destruindo tudo que foi alocado na memória para liberá-la */
	al_destroy_display(display);
	al_destroy_event_queue(fila_eventos);
	al_destroy_timer(timer);
	al_destroy_bitmap(imagem_fundo);
    al_destroy_bitmap(imagem_alien1);
    al_destroy_bitmap(imagem_alien2);
    al_destroy_bitmap(imagem_alien3);
    al_destroy_bitmap(imagem_alien4);
    al_destroy_bitmap(imagem_alien5);
    al_destroy_bitmap(imagem_alien6);
    al_destroy_bitmap(imagem_alien7);
    al_destroy_bitmap(imagem_alien8);
	al_destroy_sample_instance(inst_musica);
	al_destroy_sample_instance(inst_som_tiro);
	al_destroy_sample_instance(inst_som_explosao);
	al_destroy_font(font18);
    al_destroy_font(font64);

    return 0;
}



/* Funções para criação e manipulação dos objetos */

/* Nave-------------------------------------------------------------- */
void InitNave (NaveEspacial *nave)
{
	nave->x = 33;
	nave->y = DISPLAY_ALT / 2;
	nave->ID = ESPACONAVE;
	nave->vidas = 5;
	nave->velocidade = 7;
	nave->borda_x = 22;
	nave->borda_y = 22;
	nave->pontos = 0;
	nave->bonus = 0;
}

void DesenhaNave (NaveEspacial *nave)
{
	al_draw_bitmap(imagem_nave, nave->x-33, nave->y-49, 0);
}

void MoveNaveUP (NaveEspacial *nave)
{
	nave->y -= nave->velocidade;
	if(nave->y < 69)
		nave->y = 69;
}
void MoveNaveBOTTON (NaveEspacial *nave)
{
	nave->y += nave->velocidade;
	if(nave->y > DISPLAY_ALT - 49)
		nave->y = DISPLAY_ALT - 49;
}
void MoveNaveRIGHT (NaveEspacial *nave)
{
	nave->x += nave->velocidade;

	if(nave->x > DISPLAY_LARG-66)
		nave->x = DISPLAY_LARG-66;
}
void MoveNaveLEFT (NaveEspacial *nave)
{
	nave->x -= nave->velocidade;

	if(nave->x < 12)
		nave->x = 12;

}


/* Tiros-------------------------------------------------------------- */
void InitBalas (Tiros balas[], int tamanho)
{
	for (int i = 0; i < tamanho; i++)
	{
		balas[i].ID = TIRO;
		balas[i].velocidade = 10;
		balas[i].ativo = false;
	}
}
void AtiraBalas (Tiros balas[], int tamanho, NaveEspacial nave)
{
	for (int i = 0; i < tamanho; i++)
	{
		if(!balas[i].ativo)
		{
			al_stop_sample_instance(inst_som_tiro);
			al_play_sample_instance(inst_som_tiro);
			balas[i].x = nave.x + 17;
			balas[i].y = nave.y;
			balas[i].ativo = true;
			break;
		}
	}
}
void AtualizarBalas (Tiros balas[], int tamanho)
{
	for (int i = 0; i < tamanho; i++)
	{
		if(balas[i].ativo)
		{
			balas[i].x += balas[i].velocidade;

			if(balas[i].x > DISPLAY_LARG)
				balas[i].ativo = false;
		}
	}
}
void DesenhaBalas (Tiros balas[], int tamanho)
{
	for (int i = 0; i < tamanho; i++)
	{
		if(balas[i].ativo)
		{
			al_draw_filled_circle(balas[i].x, balas[i].y, 2, al_map_rgb(255, 255, 255));
		}
	}

}
void ColisaoTiro(Tiros balas[], int b_tamanho, Aliens Aliens[], int c_tamanho, NaveEspacial *nave)
{
	for(int i = 0; i < b_tamanho; i ++)
	{
		if(balas[i].ativo)
		{
			for(int j = 0; j < c_tamanho; j++)
			{
				if(Aliens[j].ativo)
				{
					if(balas[i].x > (Aliens[j].x - Aliens[j].borda_x) &&
						balas[i].x < (Aliens[j].x + Aliens[j].borda_x) &&
						balas[i].y > (Aliens[j].y - Aliens[j].borda_y) &&
						balas[i].y < (Aliens[j].y + Aliens[j].borda_y))
					{
						al_stop_sample_instance(inst_som_explosao);
						al_play_sample_instance(inst_som_explosao);
						balas[i].ativo = false;
						Aliens[j].ativo = false;

						nave->pontos++;
						nave->bonus++;
						if(nave->bonus == 20)
                        {
                            nave->vidas++;
                            nave->bonus=0;
                        }

					}
				}
			}
		}
	}
}


/* Aliens-------------------------------------------------------------- */
void InitAliens (Aliens Aliens[], int tamanho)
{
	for (int i = 0; i < tamanho; i++)
	{
		Aliens[i].ID = ALIEN;
		Aliens[i].velocidade = 3;
		Aliens[i].borda_x = 24;
		Aliens[i].borda_y = 24;
		Aliens[i].ativo = false;
        Aliens[i].tipo = 0;
	}
}
void LiberaAliens (Aliens Aliens[], int tamanho)
{
	for (int i = 0; i < tamanho; i++)
	{
		if(!Aliens[i].ativo)
		{
			if(rand() % 500 == 0)
			{
				Aliens[i].x = DISPLAY_LARG;

				Aliens[i].y = rand() % DISPLAY_ALT;

                if(Aliens[i].y > DISPLAY_ALT - 50)
                    Aliens[i].y = DISPLAY_ALT - 50;

				else if(Aliens[i].y < 60)
                    Aliens[i].y = 60;

                /* Sorteia caracteristicas do Alien */
                Aliens[i].tipo = rand() % 8;
                Aliens[i].velocidade = 1+ rand() % 6;
				Aliens[i].ativo = true;
				break;
			}
		}
	}
}
void AtualizarAliens(Aliens Aliens[], int tamanho)
{
	for (int i = 0; i < tamanho; i++)
	{
		if(Aliens[i].ativo)
		{
			Aliens[i].x -= Aliens[i].velocidade;
		}
	}
}
void DesenhaAliens (Aliens Aliens[], int tamanho)
{
	for (int i = 0; i < tamanho; i ++)
	{
		if(Aliens[i].ativo)
		{
            switch(Aliens[i].tipo)
            {
            case 0:
                al_draw_bitmap(imagem_alien1, Aliens[i].x-35, Aliens[i].y-35, 0);
                break;
            case 1:
                al_draw_bitmap(imagem_alien2, Aliens[i].x-35, Aliens[i].y-35, 0);
                break;
            case 2:
                al_draw_bitmap(imagem_alien3, Aliens[i].x-35, Aliens[i].y-35, 0);
                break;
            case 3:
                al_draw_bitmap(imagem_alien4, Aliens[i].x-35, Aliens[i].y-35, 0);
                break;
            case 4:
                al_draw_bitmap(imagem_alien5, Aliens[i].x-35, Aliens[i].y-35, 0);
                break;
            case 5:
                al_draw_bitmap(imagem_alien6, Aliens[i].x-35, Aliens[i].y-35, 0);
                break;
            case 6:
                al_draw_bitmap(imagem_alien7, Aliens[i].x-35, Aliens[i].y-35, 0);
                break;
            default:
                al_draw_bitmap(imagem_alien8, Aliens[i].x-24, Aliens[i].y-24, 0);
                break;
            }
		}
	}
}

void ColisaoAlien(Aliens Aliens[], int c_tamanho, NaveEspacial *nave)
{
	for(int i = 0; i < c_tamanho; i ++)
	{
		if(Aliens[i].ativo)
		{
			if((Aliens[i].x - Aliens[i].borda_x) < (nave->x + nave->borda_x) &&
				(Aliens[i].x + Aliens[i].borda_x) > (nave->x - nave->borda_x) &&
				(Aliens[i].y - Aliens[i].borda_y) < (nave->y + nave->borda_y) &&
				(Aliens[i].y + Aliens[i].borda_y) > (nave->y - nave->borda_y))
			{
				al_stop_sample_instance(inst_som_explosao);
				al_play_sample_instance(inst_som_explosao);
				Aliens[i].ativo = false;
				nave->vidas--;
			}

			else if(Aliens[i].x < 0)
			{
				Aliens[i].ativo = false;
				nave->vidas--;
			}
		}
	}
}





