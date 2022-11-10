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
#define MAXBLOCKS 1020

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
	bool pc, pb, pe, pd;
} direct;
direct direcao;

int colorfundoRGB[4] = { 70, 130, 30, 0 };

void fundo()
{ // Ess estrutura que desenha o fundo colorido
	al_clear_to_color(al_map_rgb(255, 255, 255));

}

void area()
{
	al_draw_rectangle(BLOCK * 2, BLOCK * 2, WIDHT - (BLOCK * 2), HEIGHT - (BLOCK * 2), al_map_rgb(colorfundoRGB[0], colorfundoRGB[1], colorfundoRGB[2]), 3);
	al_draw_bitmap(grama, BLOCK * 2, BLOCK * 2, WIDHT - (BLOCK * 2), HEIGHT - (BLOCK * 2));
}

struct SNAKE
{
	int x1, y1, x2, y2;
};

struct SNAKE snake[MAXBLOCKS];
int px = BLOCK * 4, py = BLOCK * 4; // cordenadas que dao movimento X e Y;
int tamanho = 4;

int corSnakeRGB[3];

int contador = 4;
ALLEGRO_BITMAP* olhos[4];

void atualizaCorpo()
{
	if (tamanho == contador)
	{ // troca a cor da serpente
		corSnakeRGB[0] = 100 + rand() % 155;
		corSnakeRGB[1] = 0 + rand() % 255;
		corSnakeRGB[2] = 60 + rand() % 135;
		contador += 6;
	}
	for (int i = tamanho; i > 0; i--)
	{
		snake[i] = snake[i - 1];


		al_draw_filled_rectangle(snake[i].x1, snake[i].y1, snake[i].x2, snake[i].y2, al_map_rgb(corSnakeRGB[0], corSnakeRGB[1], corSnakeRGB[2]));
		al_draw_rectangle(snake[i].x1, snake[i].y1, snake[i].x2, snake[i].y2, al_map_rgba(0, 0, 0, 80), 0);

		if (direcao.pc == true)
		{
			al_draw_bitmap(olhos[0], snake[0].x1, snake[0].y1, 0);
		}
		if (direcao.pb == true)
		{
			al_draw_bitmap(olhos[1], snake[0].x1, snake[0].y1, 0);
		}
		if (direcao.pe == true)
		{
			al_draw_bitmap(olhos[2], snake[0].x1, snake[0].y1, 0);
		}
		if (direcao.pd == true)
		{
			al_draw_bitmap(olhos[3], snake[0].x1, snake[0].y1, 0);
		}
		if (direcao.pb == false && direcao.pc == false && direcao.pd == false && direcao.pe == false)
		{
			al_draw_bitmap(olhos[3], snake[0].x1, snake[0].y1, 0);
		}
	}
}

int comidax, comiday; // representa a cord. x e y da comida
int aleatorio[2];

void numbersAletorios()
{
	aleatorio[0] = (BLOCK * (2 + rand() % (WIDHT / BLOCK - 4)));
	aleatorio[1] = (BLOCK * (2 + rand() % (HEIGHT / BLOCK - 4)));
}
int numberfruit;

void atualizaComida()
{
	numbersAletorios();

	for (int i = tamanho; i > 0; i--)
	{
		if ((snake[i].x1 == aleatorio[0]) && (snake[i].y1 == aleatorio[1]))
		{
			numbersAletorios();
			i++;
		}
		else
		{
			comidax = aleatorio[0];
			comiday = aleatorio[1];
		}
	}
	numberfruit = rand() % 16;
}
typedef struct SPRITES
{
	ALLEGRO_BITMAP* _Sprites;
	ALLEGRO_BITMAP* fruits[16];
} SPRITES;

SPRITES all_fruits;

ALLEGRO_BITMAP* sprites_fruits(int x, int y, int w, int h)
{
	ALLEGRO_BITMAP* fruit = al_create_sub_bitmap(all_fruits._Sprites, x, y, w, h);
	teste_iniciar(fruit, "pegando a folha com os sprites");
	return fruit;
}
void destroi_bitmaps()
{
	al_destroy_bitmap(all_fruits._Sprites);
	for (int i = 0; i < 16; i++)
	{
		al_destroy_bitmap(all_fruits.fruits[i]);
		if (i < 4)
		{
			al_destroy_bitmap(olhos[i]);
		}
	}
}
void desenhaComida()
{
	al_draw_bitmap(all_fruits.fruits[numberfruit], comidax, comiday, 0);
}
// Fulllscren
#define DISP_SCALE 1
#define DISP_W (WIDHT * DISP_SCALE)
#define DISP_H (HEIGHT * DISP_SCALE)

int main()
{
	teste_iniciar(al_init(), "allegro");
	teste_iniciar(al_install_keyboard(), "teclado");

	al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW); // Fullscreen

	// Inicializaçoes
	ALLEGRO_TIMER* tempo = al_create_timer(1.0 / FRAMES);
	teste_iniciar(tempo, "tempo");

	ALLEGRO_EVENT_QUEUE* fila = al_create_event_queue();
	teste_iniciar(fila, "fila");

	ALLEGRO_DISPLAY* display = al_create_display(WIDHT, HEIGHT);
	teste_iniciar(display, "Display");

	anti_serrilhado();

	ALLEGRO_FONT* fonte = al_create_builtin_font();
	teste_iniciar(fonte, "fonte");

	teste_iniciar(al_init_primitives_addon(), "primitivas");
	teste_iniciar(al_init_image_addon(), "image addon"); 

	// menu
	ALLEGRO_BITMAP* controles = al_load_bitmap("./img/Menu.png");
	teste_iniciar(controles, "Imagen do menu");
	// Sprites os Olhos
	olhos[0] = al_load_bitmap("./img/cabeca_cima.png");
	olhos[1] = al_load_bitmap("./img/cabeca_baixo.png");
	olhos[2] = al_load_bitmap("./img/cabeca_esquerda.png");
	olhos[3] = al_load_bitmap("./img/cabeca_direita.png");
	grama = al_load_bitmap("./img/grama.png");

	for (int i = 0; i < 4; i++)
	{
		teste_iniciar(olhos[i], "olhos");
	}
	// Sprites de frutas
	all_fruits._Sprites = al_load_bitmap("./img/all_fruits.png");
	teste_iniciar(all_fruits._Sprites, "All Sprites");

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
	teste_iniciar(moving, "sons de Movimento");

	ALLEGRO_SAMPLE* alimentando = al_load_sample("./sons/food.wav");
	teste_iniciar(alimentando, "alimentando a cobrinha");

	ALLEGRO_AUDIO_STREAM* music = al_load_audio_stream("./sons/theme_snake.opus", 2, 2048);
	teste_iniciar(music, "music");
	al_set_audio_stream_playmode(music, ALLEGRO_PLAYMODE_LOOP);
	al_attach_audio_stream_to_mixer(music, al_get_default_mixer()); // tocas musica

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
		bool GameLoop = false;
		iniciar = false;

		al_wait_for_event(fila, &evento);

		switch (evento.type)
		{
		case ALLEGRO_EVENT_TIMER:
			al_get_keyboard_state(&ks);
			if (al_key_down(&ks, ALLEGRO_KEY_ESCAPE))
			{
				iniciar = true;
				menu = false;
				GameLoop = false;
			}
			if (al_key_down(&ks, ALLEGRO_KEY_ENTER))
			{
				iniciar = true;
			}
			// Todas as teclas vão a cima

			redesenha = true;
			break;

		case ALLEGRO_EVENT_DISPLAY_CLOSE:
		{
			iniciar = true;
			menu = false;
			GameLoop = false;
		}
		}

		if (iniciar) {
			GameLoop = true;
		}
		// aqui vai tudo que e para ser desenhado
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

				snake[0].x1 = px;
				snake[0].y1 = py;
				snake[0].x2 = BLOCK + px;
				snake[0].y2 = BLOCK + py;

				// toda vez que a coordenada de x e y for giual a da comida ele entra nessa decisão
				if ((px == comidax) && (py == comiday))
				{
					al_play_sample(alimentando, 1.2, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
					tamanho++;
					atualizaComida();
				}

				al_get_keyboard_state(&ks);
				// W A S D
				if (al_key_down(&ks, ALLEGRO_KEY_W) == true && (direcao.pb == false) && (direcao.pc == false))
				{
					direcao.pc = true; direcao.pb = false; direcao.pe = false; direcao.pd = false;
					al_play_sample(moving, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
					Sleep(delay);
				}
				if (al_key_down(&ks, ALLEGRO_KEY_S) && (direcao.pc == false) && (direcao.pb == false))
				{
					direcao.pb = true; direcao.pc = false; direcao.pe = false; direcao.pd = false;
					al_play_sample(moving, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
					Sleep(delay);
				}
				if (al_key_down(&ks, ALLEGRO_KEY_A) && (direcao.pd == false) && (direcao.pe == false))
				{
					direcao.pe = true; direcao.pc = false; direcao.pb = false; direcao.pd = false;
					al_play_sample(moving, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
					Sleep(delay);
				}
				if (al_key_down(&ks, ALLEGRO_KEY_D) && (direcao.pe == false) && (direcao.pd == false))
				{
					direcao.pd = true; direcao.pc = false; direcao.pb = false; direcao.pe = false;
					al_play_sample(moving, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
					Sleep(delay);
				}

				// SETAS | Tentei colocar os comando ds setas junto ao WASD mais nao ficou muito bom 
				if (al_key_down(&ks, ALLEGRO_KEY_UP) && (direcao.pb == false) && (direcao.pc == false))
				{
					direcao.pc = true; direcao.pb = false; direcao.pe = false; direcao.pd = false;
					al_play_sample(moving, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
					Sleep(delay);
				}
				if (al_key_down(&ks, ALLEGRO_KEY_DOWN) && (direcao.pc == false) && (direcao.pb == false))
				{
					direcao.pb = true; direcao.pc = false; direcao.pe = false; direcao.pd = false;
					al_play_sample(moving, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
					Sleep(delay);
				}
				if (al_key_down(&ks, ALLEGRO_KEY_LEFT) && (direcao.pd == false) && (direcao.pe == false))
				{
					direcao.pe = true; direcao.pc = false; direcao.pb = false; direcao.pd = false;
					al_play_sample(moving, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
					Sleep(delay);
				}
				if (al_key_down(&ks, ALLEGRO_KEY_RIGHT) && (direcao.pe == false) && (direcao.pd == false))
				{
					direcao.pd = true; direcao.pc = false; direcao.pb = false; direcao.pe = false;
					al_play_sample(moving, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
					Sleep(delay);

				}

				if (direcao.pc == true) py -= BLOCK;
				if (direcao.pb == true) py += BLOCK;
				if (direcao.pe == true) px -= BLOCK;
				if (direcao.pd == true) px += BLOCK;

				// Comeca
				if ((direcao.pd == true) || (direcao.pc == true) || (direcao.pb == true) || (direcao.pe == true))
				{
					// colisao da cabeça com o corpo
					for (int i = 4; i <= tamanho; i++)
					{
						if ((px == snake[i].x1) && (py == snake[i].y1))
						{
							sair = true;
						}
					}
				}
				// Colisao maximo de tamanho
				if (tamanho == MAXBLOCKS - 8)
					sair = true;


				// atravessa a tela;
				if (px == BLOCK) px = WIDHT - BLOCK * 3;
				if (px == WIDHT - BLOCK * 2) px = BLOCK * 2;
				if (py == BLOCK) py = HEIGHT - BLOCK * 3;
				if (py == HEIGHT - BLOCK * 2) py = BLOCK * 2;


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

			// aqui vai tudo que e para ser desenhado
			if (redraw && al_is_event_queue_empty(fila))
			{
				al_clear_to_color(al_map_rgb(0, 0, 0));
				fundo();
				area();		 // desenha a grade de linhas
				desenhaComida(); // apenas desenha a comida
				atualizaCorpo();

				al_draw_textf(fonte, al_map_rgb(255, 255, 255), 90, 90, 100, "PONTUACAO : %i", tamanho);
				al_flip_display();
				redraw = false;
			} // redesenhos acima

		} // FIM do GAME LOOP
		// reset variaveis
		tamanho = 4;
		contador = 4;
		px = BLOCK * 4;
		py = BLOCK * 4;
		direcao.pd = false; direcao.pc = false;
		direcao.pb = false; direcao.pe = false;
		atualizaCorpo();
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