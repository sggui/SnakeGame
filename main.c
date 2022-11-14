#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_ttf.h>

// 3840 × 2160
#define WIDHT 1920
#define HEIGHT 1080

#define BLOCK 40 // a tela e divida em blocos

ALLEGRO_BITMAP* grama = NULL;


// o tamanho do array e baseado no tamanho do bloco e da tela por exemplo WIDTH / BLOCK = X || HEIGHT / 40 = Y então X * Y = MAXBLOCKS
// isso foi para evitar de usar a função malloc
#define MAXBLOCKSp1 1020
#define MAXBLOCKSp2 1020

double FRAMES = 16.0;

void teste_iniciar(bool test, const char* desciption)
{
	if (test)
		return;

	// printf("nao foi possivel inicializar => %s \n", descrição);
	exit(1);
}
void anti_serrilhado()
{
	al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
	al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
	al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);
}
void audio()
{
	teste_iniciar(al_install_audio(), "audio");
	teste_iniciar(al_init_acodec_addon(), "audio codecs");
	teste_iniciar(al_reserve_samples(16), "reserve samples");
}

// variaves globais
typedef struct
{
	bool pcp1, pbp1, pep1, pdp1;
} directp1;

directp1 direcaop1;
typedef struct
{
	bool pcp2, pbp2, pep2, pdp2;
} directp2;

directp2 direcaop2;

int colorfundoRGB[4] = { 70, 130, 30, 0 };

void fundo()
{ // Ess estrutura que desenha o fundo branco
	al_clear_to_color(al_map_rgb(255, 255, 255));

}

void area()
{ // desenha o local onde terá o gramado
	al_draw_rectangle(BLOCK * 2, BLOCK * 2, WIDHT - (BLOCK * 2), HEIGHT - (BLOCK * 2), al_map_rgb(colorfundoRGB[0], colorfundoRGB[1], colorfundoRGB[2]), 3);
	al_draw_bitmap(grama, BLOCK * 2, BLOCK * 2, WIDHT - (BLOCK * 2), HEIGHT - (BLOCK * 2));
}

struct SNAKEp1
{ //x e y da cobra (p1)
	int x1p1, y1p1, x2p1, y2p1;
};

struct SNAKEp2
{
	int x1p2, y1p2, x2p2, y2p2;
};

struct SNAKEp1 snakep1[MAXBLOCKSp1];
int pxp1 = BLOCK * 4, pyp1 = BLOCK * 4; // cordenadas que dao movimento X e Y;
int tamanhop1 = 4;

struct SNAKEp2 snakep2[MAXBLOCKSp2];
int pxp2 = BLOCK * 4, pyp2 = BLOCK * 4; // cordenadas que dao movimento X e Y;
int tamanhop2 = 4;



int corSnakeRGB[3];

int contadorp1 = 4;
int contadorp2 = 4;
ALLEGRO_BITMAP* olhosp1[4];
ALLEGRO_BITMAP* olhosp2[4];

void atualizaCorpop1()
{
	if (tamanhop1 == contadorp1)
	{ // troca a cor da serpente
		corSnakeRGB[0] = 100 + rand() % 155;
		corSnakeRGB[1] = 0 + rand() % 255;
		corSnakeRGB[2] = 60 + rand() % 135;
		contadorp1 += 6;
	}
	for (int i = tamanhop1; i > 0; i--)
	{
		snakep1[i] = snakep1[i - 1];


		al_draw_filled_rectangle(snakep1[i].x1p1, snakep1[i].y1p1, snakep1[i].x2p1, snakep1[i].y2p1, al_map_rgb(corSnakeRGB[0], corSnakeRGB[1], corSnakeRGB[2])); //desenha a snake
		al_draw_rectangle(snakep1[i].x1p1, snakep1[i].y1p1, snakep1[i].x2p1, snakep1[i].y2p1, al_map_rgba(0, 0, 0, 80), 0); //desenha a snake

		if (direcaop1.pcp1== true) //movimentação para cima
		{
			al_draw_bitmap(olhosp1[0], snakep1[0].x1p1, snakep1[0].y1p1, 0);
		}
		if (direcaop1.pbp1 == true) //movimentação para baixo
		{
			al_draw_bitmap(olhosp1[1], snakep1[0].x1p1, snakep1[0].y1p1, 0);
		}
		if (direcaop1.pep1 == true) //movimentação para esquerda
		{
			al_draw_bitmap(olhosp1[2], snakep1[0].x1p1, snakep1[0].y1p1, 0);
		}
		if (direcaop1.pdp1 == true) //movimentação para direita
		{
			al_draw_bitmap(olhosp1[3], snakep1[0].x1p1, snakep1[0].y1p1, 0);
		}
		if (direcaop1.pbp1 == false && direcaop1.pcp1 == false && direcaop1.pdp1 == false && direcaop1.pep1 == false) //seguir ultimo movimento informado, nenhum valor é TRUE
		{
			al_draw_bitmap(olhosp1[3], snakep1[0].x1p1, snakep1[0].y1p1, 0);
		}
	}
}

void atualizaCorpop2()
{
	if (tamanhop2 == contadorp2)
	{ // troca a cor da serpente
		corSnakeRGB[0] = 100 + rand() % 155;
		corSnakeRGB[1] = 0 + rand() % 255;
		corSnakeRGB[2] = 60 + rand() % 135;
		contadorp2 += 6;
	}
	for (int i = tamanhop2; i > 0; i--)
	{
		snakep2[i] = snakep2[i - 1];


		al_draw_filled_rectangle(snakep2[i].x1p2, snakep2[i].y1p2, snakep2[i].x2p2, snakep2[i].y2p2, al_map_rgb(corSnakeRGB[0], corSnakeRGB[1], corSnakeRGB[2])); //desenha a snake
		al_draw_rectangle(snakep2[i].x1p2, snakep2[i].y1p2, snakep2[i].x2p2, snakep2[i].y2p2, al_map_rgba(0, 0, 0, 80), 0); //desenha a snake

		if (direcaop2.pcp2 == true) //movimentação para cima
		{
			al_draw_bitmap(olhosp2[0], snakep2[0].x1p2, snakep2[0].y1p2, 0);
		}
		if (direcaop2.pbp2 == true) //movimentação para baixo
		{
			al_draw_bitmap(olhosp2[1], snakep2[0].x1p2, snakep2[0].y1p2, 0);
		}
		if (direcaop2.pep2 == true) //movimentação para esquerda
		{
			al_draw_bitmap(olhosp2[2], snakep2[0].x1p2, snakep2[0].y1p2, 0);
		}
		if (direcaop2.pdp2 == true) //movimentação para direita
		{
			al_draw_bitmap(olhosp2[3], snakep2[0].x1p2, snakep2[0].y1p2, 0);
		}
		if (direcaop2.pbp2 == false && direcaop2.pcp2 == false && direcaop2.pdp2 == false && direcaop2.pep2 == false) //seguir ultimo movimento informado, nenhum valor é TRUE
		{
			al_draw_bitmap(olhosp2[3], snakep2[0].x1p2, snakep2[0].y1p2, 0);
		}
	}
}

int comidax, comiday; // representa a cord. x e y da comida
int aleatorio[2]; //sorteia posicao x e y

void numbersAletorios() //funcao para sortear valores
{
	aleatorio[0] = (BLOCK * (2 + rand() % (WIDHT / BLOCK - 4)));
	aleatorio[1] = (BLOCK * (2 + rand() % (HEIGHT / BLOCK - 4)));
}
int numberfruit; //numero da fruta

void atualizaComida() //atualiza a contagem de pontos
{
	numbersAletorios();
	for (int i = tamanhop1; i > 0; i--) 
	{
		if (((snakep1[i].x1p1 == aleatorio[0]) && (snakep1[i].y1p1 == aleatorio[1])) || ((snakep2[i].x1p2 == aleatorio[0]) && (snakep2[i].y1p2 == aleatorio[1))) //se o valor da cobraX for igual ao X da comida (aleatorio[0]), contabiliza ponto. se o valor da cobraY for igual ao Y da comida (aleatorio[1]), contabiliza ponto.
		{
			numbersAletorios();
			i++;
		}
		else //se não bater, o valor fica la pra sempre.
		{
			comidax = aleatorio[0]; //comida X
			comiday = aleatorio[1]; //comida Y
		}
	}
	numberfruit = rand() % 16; //sorteio do numero da fruta (entre os sprites)
}
typedef struct SPRITES
{
	ALLEGRO_BITMAP* _Sprites;
	ALLEGRO_BITMAP* fruits[16];
} SPRITES;

SPRITES all_fruits;

ALLEGRO_BITMAP* sprites_fruits(int x, int y, int w, int h) //posicoes e tamanhos das frutas
{
	ALLEGRO_BITMAP* fruit = al_create_sub_bitmap(all_fruits._Sprites, x, y, w, h); //desenho das frutas
	teste_iniciar(fruit, "pegando a folha com os sprites"); //teste erro ao iniciar o bgl do ALLEGRO (esqueci o nome)
	return fruit;
}
void destroi_bitmaps() //destroi os bitmaps apos comida
{
	al_destroy_bitmap(all_fruits._Sprites);
	for (int i = 0; i < 16; i++)
	{
		al_destroy_bitmap(all_fruits.fruits[i]);
		if (i < 4)
		{
			al_destroy_bitmap(olhosp1[i]);
			al_destroy_bitmap(olhosp2[i]);
		}
	}
}
void desenhaComida() //desenho da comida
{
	al_draw_bitmap(all_fruits.fruits[numberfruit], comidax, comiday, 0);
}
// Fulllscren
#define DISP_SCALE 1
#define DISP_W (WIDHT * DISP_SCALE)
#define DISP_H (HEIGHT * DISP_SCALE)

int main()
{
	teste_iniciar(al_init(), "allegro"); //erro ao inciar allegro
	teste_iniciar(al_install_keyboard(), "teclado"); //erro ao instalar keys

	al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW); // Fullscreen

	// Inicializaçoes
	ALLEGRO_TIMER* tempo = al_create_timer(1.0 / FRAMES);
	teste_iniciar(tempo, "tempo"); //FPS

	ALLEGRO_EVENT_QUEUE* fila = al_create_event_queue();
	teste_iniciar(fila, "fila"); //fila

	ALLEGRO_DISPLAY* display = al_create_display(WIDHT, HEIGHT);
	teste_iniciar(display, "Display"); //diplay 

	anti_serrilhado();

	ALLEGRO_FONT* fonte = al_create_builtin_font();
	teste_iniciar(fonte, "fonte");

	teste_iniciar(al_init_primitives_addon(), "primitivas");
	teste_iniciar(al_init_image_addon(), "image addon"); 

	// menu
	ALLEGRO_BITMAP* controles = al_load_bitmap("./img/Menu.png");
	teste_iniciar(controles, "Imagen do menu");

	// Sprites os Olhos p1
	olhosp1[0] = al_load_bitmap("./img/cabeca_cima.png"); //olhos para cima
	olhosp1[1] = al_load_bitmap("./img/cabeca_baixo.png"); //olhos para baixo 
	olhosp1[2] = al_load_bitmap("./img/cabeca_esquerda.png"); //olhos para esquerda
	olhosp1[3] = al_load_bitmap("./img/cabeca_direita.png"); //olhos para direita

	//sprite de olhos p2
	olhosp2[0] = al_load_bitmap("./img/cabeca_cima.png"); //olhos para cima
	olhosp2[1] = al_load_bitmap("./img/cabeca_baixo.png"); //olhos para baixo
	olhosp2[2] = al_load_bitmap("./img/cabeca_esquerda.png"); //olhos para esquerda
	olhosp2[3] = al_load_bitmap("./img/cabeca_direita.png"); //olhos para direita 
	grama = al_load_bitmap("./img/grama.png"); //grama (background que sera o ring)

	// Inicia olhos p1
	for (int i = 0; i < 4; i++)
	{
		teste_iniciar(olhosp1[i], "olhos player1"); //erro ao inicializar olhos p1
	}

	// Inicia olhos p1
	for (int i = 0; i < 4; i++)
	{
		teste_iniciar(olhosp2[i], "olhos player2"); //erro ao inicializar olhos p2
	}

	// Sprites de frutas
	all_fruits._Sprites = al_load_bitmap("./img/all_fruits.png"); //sprite das frutas
	teste_iniciar(all_fruits._Sprites, "All Sprites"); //erro ao inciar o bitmap

	// Pegando os assets na folha
	int initX = 0, initY = 0;
	for (int i = 0; i < 16; i++)
	{
		if (i < 4)
		{
			all_fruits.fruits[i] = sprites_fruits(initX, initY, BLOCK, BLOCK);
			initX += BLOCK;
		}
		if (i < 8 && i > 3)
		{
			initY = BLOCK;
			all_fruits.fruits[i] = sprites_fruits(initX, initY, BLOCK, BLOCK);
			initX += BLOCK;
		}
		if (i < 12 && i > 7)
		{
			initY = BLOCK * 2; 
			all_fruits.fruits[i] = sprites_fruits(initX, initY, BLOCK, BLOCK);
			initX += BLOCK;
		}
		if (i < 17 && i > 11)
		{
			initY = BLOCK * 3;
			all_fruits.fruits[i] = sprites_fruits(initX, initY, BLOCK, BLOCK);
			initX += BLOCK;
		}
		if (initX == 120)
		{
			initX = 0;
		}
	}

	// sons
	audio();
	ALLEGRO_SAMPLE* moving = al_load_sample("./sons/move.wav");
	teste_iniciar(moving, "sons de Movimento"); //movimento cobra

	ALLEGRO_SAMPLE* alimentando = al_load_sample("./sons/food.wav");
	teste_iniciar(alimentando, "alimentando a cobrinha"); //alimentando a cobra (la ele)

	ALLEGRO_AUDIO_STREAM* music = al_load_audio_stream("./sons/theme_snake.opus", 2, 2048);
	teste_iniciar(music, "music");
	al_set_audio_stream_playmode(music, ALLEGRO_PLAYMODE_LOOP);
	al_attach_audio_stream_to_mixer(music, al_get_default_mixer()); // tocar musica

	al_register_event_source(fila, al_get_display_event_source(display));
	al_register_event_source(fila, al_get_timer_event_source(tempo));
	al_register_event_source(fila, al_get_keyboard_event_source());

	// FULLSCREEN
	const float scale_factor_x = ((float)al_get_display_width(display)) / WIDHT;
	const float scale_factor_y = ((float)al_get_display_height(display)) / HEIGHT;

	ALLEGRO_TRANSFORM t;

	al_identity_transform(&t);
	al_scale_transform(&t, scale_factor_x, scale_factor_y);
	al_use_transform(&t);

	// final fullscreen

	bool iniciar = false;
	bool redesenha = true;

	bool sair = false;
	bool redraw = true;
	ALLEGRO_EVENT evento;

	al_start_timer(tempo);

	srand(time(NULL));
	atualizaComida();

	int delay = 10;
	bool menu = true;
	ALLEGRO_KEYBOARD_STATE ks;
	// menu
	while (menu)
	{
		bool GameLoop = false; //gameloop
		iniciar = false; //variavel de começar o jogo

		al_wait_for_event(fila, &evento); //esperando um evento na fila

		switch (evento.type)
		{
		case ALLEGRO_EVENT_TIMER: //começa a contabilizar o tempo na console
			al_get_keyboard_state(&ks);
			if (al_key_down(&ks, ALLEGRO_KEY_ESCAPE)) //se teclar esc, sai do jogo
			{
				iniciar = true;
				menu = false;
				GameLoop = false;
			}
			if (al_key_down(&ks, ALLEGRO_KEY_ENTER)) //se key = enter, começa o jogo
			{
				iniciar = true;
			}
			// Todas as teclas vão a cima

			redesenha = true;
			break;

		case ALLEGRO_EVENT_DISPLAY_CLOSE: //fechar o jogo = fecha a tela
		{
			iniciar = true;
			menu = false;
			GameLoop = false;
		}
		}

		if (iniciar) { //se iniciar = true (alterado na linha 336), gameloop = true e o jogo starta
			GameLoop = true;
		}
		// aqui vai tudo que é para ser desenhado
		if (redesenha && al_is_event_queue_empty(fila))
		{
			fundo();
			al_draw_bitmap(controles, 350, 250, 0);
			al_flip_display();
			redesenha = false;
		}

		// Game
		while (GameLoop)
		{
			sair = false;
			al_wait_for_event(fila, &evento);

			switch (evento.type)
			{ // GameLogic goes here
			case ALLEGRO_EVENT_TIMER:

				//COBRA INICIO P1
				snakep1[0].x1p1 = pxp1; 
				snakep1[0].y1p1 = pyp1; 
				snakep1[0].x2p1 = BLOCK + pxp1; 
				snakep1[0].y2p1 = BLOCK + pyp1;

				//COBRA INICIO P2
				snakep2[0].x1p2 = pxp2; 
				snakep2[0].y1p2 = pyp2; 
				snakep2[0].x2p2 = BLOCK + pxp2; 
				snakep2[0].y2p2 = BLOCK + pyp2; 

				// toda vez que a coordenada de x e y for igual a da comida, ele entra nessa decisão
				if (((pxp1 == comidax) && (pyp1 == comiday)) || (pxp2 == comidax) && (pyp2 = comiday)) //LUQ MONSTRO.
				{
					al_play_sample(alimentando, 1.2, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
					tamanhop1++; //tamanho aumenta 1
					tamanhop2++;
					atualizaComida(); //desenha a comida em uma nova posicao
				}

				al_get_keyboard_state(&ks);

				// W A S D - MOVIMENTACAO PLAYER1
				if (al_key_down(&ks, ALLEGRO_KEY_W) == true && (direcaop1.pbp1 == false) && (direcaop1.pcp1 == false)) //para cima (player1)
				{
					direcaop1.pcp1 = true; direcaop1.pbp1 = false; direcaop1.pep1 = false; direcaop1.pdp1 = false;
					al_play_sample(moving, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL); //barulho de movimento a cada clique em uma tecla
					Sleep(delay);
				}
				if (al_key_down(&ks, ALLEGRO_KEY_S) && (direcaop1.pcp1 == false) && (direcaop1.pbp1 == false)) //para baixo (player1)
				{
					direcaop1.pbp1 = true; direcaop1.pcp1 = false; direcaop1.pep1 = false; direcaop1.pdp1 = false;
					al_play_sample(moving, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL); //barulho de movimento a cada clique em uma tecla
					Sleep(delay);
				}
				if (al_key_down(&ks, ALLEGRO_KEY_A) && (direcaop1.pdp1 == false) && (direcaop1.pep1 == false)) //para esquerda (player1) 
				{
					direcaop1.pep1 = true; direcaop1.pcp1 = false; direcaop1.pbp1 = false; direcaop1.pdp1 = false;
					al_play_sample(moving, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL); //barulho de movimento a cada clique em uma tecla
					Sleep(delay);
				}
				if (al_key_down(&ks, ALLEGRO_KEY_D) && (direcaop1.pep1 == false) && (direcaop1.pdp1 == false)) //para direita (player1)
				{
					direcaop1.pdp1 = true; direcaop1.pcp1 = false; direcaop1.pbp1 = false; direcaop1.pep1 = false;
					al_play_sample(moving, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL); //barulho de movimento a cada clique em uma tecla
					Sleep(delay);
				}

				// SETAS - MOVIMENTACAO PLAYER2
				if (al_key_down(&ks, ALLEGRO_KEY_UP) && (direcaop2.pbp2 == false) && (direcaop2.pcp2 == false)) //para cima (player2) //parsa que ngc chato.
				{
					direcaop2.pcp2 = true; direcaop2.pbp2 = false; direcaop2.pep2 = false; direcaop2.pdp2 = false; 
					al_play_sample(moving, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL); //barulho de movimento a cada clique em uma tecla
					Sleep(delay);
				}
				if (al_key_down(&ks, ALLEGRO_KEY_DOWN) && (direcaop2.pcp2 == false) && (direcaop2.pbp2 == false)) //para baixo (player2)
				{
					direcaop2.pbp2 = true; direcaop2.pcp2 = false; direcaop2.pep2 = false; direcaop2.pdp2 = false; 
					al_play_sample(moving, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL); //barulho de movimento a cada clique em uma tecla
					Sleep(delay);
				}
				if (al_key_down(&ks, ALLEGRO_KEY_LEFT) && (direcaop2.pdp2 == false) && (direcaop2.pep2 == false)) // para esquerda (player2)
				{
					direcaop2.pep2 = true; direcaop2.pcp2 = false; direcaop2.pbp2 = false; direcaop2.pdp2 = false; 
					al_play_sample(moving, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL); //barulho de movimento a cada clique em uma tecla
					Sleep(delay);
				}
				if (al_key_down(&ks, ALLEGRO_KEY_RIGHT) && (direcaop2.pep2 == false) && (direcaop2.pdp2 == false)) //para direita (player2)
				{
					direcaop2.pdp2 = true; direcaop2.pcp2 = false; direcaop2.pbp2 = false; direcaop2.pep2 = false; 
					al_play_sample(moving, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL); //barulho de movimento a cada clique em uma tecla
					Sleep(delay);

				}

				//ALTERACAO DIRECAO PLAYER 1
				if (direcaop1.pcp1 == true) pyp1 -= BLOCK; //se a direcao for pra cima, y py diminui
				if (direcaop1.pbp1 == true) pyp1 += BLOCK; //se a direcao for para baixo, py aumenta
				if (direcaop1.pep1 == true) pxp1 -= BLOCK; //se a direcao for para esquerda, px diminui
				if (direcaop1.pdp1 == true) pxp1 += BLOCK; //se a direcao for para direita, px aumenta

				// ALTERACAO DIRECAO PLAYER 2
				if (direcaop2.pcp2 == true) pyp2 -= BLOCK; //se a direcao for pra cima, y py diminui
				if (direcaop2.pbp2 == true) pyp2 += BLOCK; //se a direcao for para baixo, py aumenta
				if (direcaop2.pep2 == true) pxp2 -= BLOCK; //se a direcao for para esquerda, px diminui
				if (direcaop2.pdp2 == true) pxp2 += BLOCK; //se a direcao for para direita, px aumenta

				// Comeca
				if (((direcaop1.pdp1 == true) || (direcaop1.pcp1 == true) || (direcaop1.pbp1 == true) || (direcaop1.pep1 == true)) || ((direcaop2.pdp2 == true) || (direcaop2.pcp2 == true) || (direcaop2.pbp2 == true) || (direcaop2.pep2 == true)))//size inicial da cobra (todas as direcoes são true)
				{
					// colisao da cabeça com o corpo player1
					for (int i = 4; i <= tamanhop1; i++)
					{
						if ((pxp1 == snakep1[i].x1p1) && (pyp1 == snakep1[i].y1p1)) // se a cobra (tanto p1 quanto p2) bater nela mesma, jogo acaba.
						{
							sair = true; //derrota se o px e py da cabeça forem iguais ao corpo
						}
					}
				}
				{
					// colisao da cabeça com o corpo player2
					for (int i = 4; i <= tamanhop2; i++)
					{
						if ((pxp2 == snakep2[i].x1p2) && (pyp2 == snakep2[i].y1p2))
						{
							sair = true; //derrota se o px e py da cabeça forem iguais ao corpo
						}
					}
				}

				// Colisao maximo de tamanho player1
				if (tamanhop1 == MAXBLOCKSp1 - 8) //MAXBLOCKS = 1020. 1020 - 8 = 1012 (size total da cobra)
					sair = true; //se isso acontecer, sai do jogo

				//Colisao maximo de tamanho player2
				if (tamanhop2 == MAXBLOCKSp2 - 8) //MAXBLOCKS = 1020. 1020 - 8 = 1012 (size total da cobra)
					sair = true; //se isso acontecer, sai do jogo


				// atravessa a tela player1
				if (pxp1 == BLOCK) pxp1 = WIDHT - BLOCK * 3; // esquerda e direita
				if (pxp1 == WIDHT - BLOCK * 2) pxp1 = BLOCK * 2; // esquerda e direita
				if (pyp1 == BLOCK) pyp1 = HEIGHT - BLOCK * 3; //cima e baixo
				if (pyp1 == HEIGHT - BLOCK * 2) pyp1 = BLOCK * 2; //cima e baixo 

				//atravessa a tela player2

				if (pxp2 == BLOCK) pxp2 = WIDHT - BLOCK * 3; // esquerda e direita
				if (pxp2 == WIDHT - BLOCK * 2) pxp2 = BLOCK * 2; // esquerda e direita
				if (pyp2 == BLOCK) pyp2 = HEIGHT - BLOCK * 3; //cima e baixo
				if (pyp2 == HEIGHT - BLOCK * 2) pyp2 = BLOCK * 2; //cima e baixo


				if (al_key_down(&ks, ALLEGRO_KEY_ESCAPE)) // ESC PARA SAIR
				{
					sair = true;
				}

				// Fim da Logica
				redraw = true;
				break;

				// ALT F4
			case ALLEGRO_EVENT_DISPLAY_CLOSE: {
				sair = true;
				GameLoop = false;
				menu = false;
			}

			} // fim switch

			if (sair)
				break;

			// aqui vai tudo que é para ser desenhado
			if (redraw && al_is_event_queue_empty(fila))
			{
				al_clear_to_color(al_map_rgb(0, 0, 0));
				fundo();
				area();		 // desenha a grama
				desenhaComida(); // apenas desenha a comida
				atualizaCorpop1();
				atualizaCorpop2();

				al_draw_textf(fonte, al_map_rgb(255, 255, 255), 90, 90, 100, "PONTUACAO : %i", tamanhop1 + tamanhop2); // pontuacao
				al_flip_display();
				redraw = false;
			} // redesenhos acima

		} // FIM do GAME LOOP
		// reset variaveis
		tamanhop1 = 4;
		tamanhop2 = 4;
		contadorp1 = 4;
		contadorp2 = 4;
		pxp1 = BLOCK * 4;
		pyp1 = BLOCK * 4;
		pxp2 = BLOCK * 4;
		pyp2 = BLOCK * 4;
		direcaop1.pdp1 = false; direcaop1.pcp1 = false;
		direcaop1.pbp1 = false; direcaop1.pep1 = false;
		direcaop2.pdp2 = false; direcaop2.pcp2 = false;
		direcaop2.pbp2 = false; direcaop2.pep2 = false;
		atualizaCorpop1();
		atualizaCorpop2();
		sair = false;


	} // fim do menu

	al_destroy_sample(alimentando);
	al_destroy_sample(moving);
	al_destroy_audio_stream(music);
	al_destroy_font(fonte);
	al_destroy_display(display);
	al_destroy_timer(tempo);
	al_destroy_event_queue(fila);
	destroi_bitmaps();

	return 0;
}
