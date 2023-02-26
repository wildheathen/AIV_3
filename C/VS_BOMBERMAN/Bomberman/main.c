#include "bomberman.h"
#define SDL_MAIN_HANDLED
#include "SDL.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "TCP_Client.h"
#include "TCP_Server.h"
#include "Server.h"
#include <stdio.h>
#include <string.h>

// #pragma comment(lib, "ws2_32.lib")

//evaluates the executable parameters
int connection_mgr(int argc, char* argv[]);

player_t player;


static void bomberman_game_mode_init(game_mode_t* game_mode)
{
	game_mode->timer = 60;
}

static void bomberman_map_init(cell_t* map)
{
	//TODO
}

static void bomberman_player_init(player_t* player)
{
	player->position.x = 0;
	player->position.y = 0;
	player->number_of_lifes = 1;
	player->number_of_bombs = 1;
	player->score = 0;
	player->speed = 1;
}

int bomberman_graphics_init(SDL_Window** window, SDL_Renderer** renderer, SDL_Texture** texture)
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER) != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return -1;
	}

	*window = SDL_CreateWindow("SDL is active!", 100, 100, 512, 512, 0);
	if (!*window)
	{
		SDL_Log("Unable to create window: %s", SDL_GetError());
		SDL_Quit();
		return -1;
	}

	*renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!*renderer)
	{
		SDL_Log("Unable to create renderer: %s", SDL_GetError());
		SDL_DestroyWindow(*window);
		SDL_Quit();
		return -1;
	}

	int width;
	int height;
	int channels;
	unsigned char* pixels = stbi_load("menacing.png", &width, &height, &channels, 4);

	if (!pixels)
	{
		SDL_Log("Unable to open image");
		SDL_DestroyRenderer(*renderer);
		SDL_DestroyWindow(*window);
		SDL_Quit();
		return -1;
	}

	SDL_Log("Image width: %d height: %d channels: %d", width, height, channels);

	*texture = SDL_CreateTexture(*renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STATIC, width, height);

	if (!*texture)
	{
		SDL_Log("Unable to create texture: %s", SDL_GetError());
		free(pixels);
		SDL_DestroyRenderer(*renderer);
		SDL_DestroyWindow(*window);
		SDL_Quit();
		return -1;
	}

	SDL_UpdateTexture(*texture, NULL, pixels, width * 4);
	SDL_SetTextureAlphaMod(*texture, 255);
	SDL_SetTextureBlendMode(*texture, SDL_BLENDMODE_BLEND);
	free(pixels);

	return 0;
}

void sdl_main()
{
	game_mode_t game_mode;
	cell_t map[64 * 64];
	

	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* texture;

	bomberman_game_mode_init(&game_mode);
	bomberman_map_init(map);
	bomberman_player_init(&player);

	if (bomberman_graphics_init(&window, &renderer, &texture))
		return; //-1

	// Game loop
	int running = 1;

	while (running)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				running = 0;
		}

		SDL_PumpEvents();
		const Uint8* keys = SDL_GetKeyboardState(NULL);
		player.position.x += keys[SDL_SCANCODE_RIGHT];
		player.position.x -= keys[SDL_SCANCODE_LEFT];
		player.position.y += keys[SDL_SCANCODE_DOWN];
		player.position.y -= keys[SDL_SCANCODE_UP];

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		SDL_Rect target_rect = { player.position.x, player.position.y, 32, 32 };
		SDL_RenderCopy(renderer, texture, NULL, &target_rect);
		SDL_RenderPresent(renderer);
	}	
}

int connection_mgr(int argc, char* argv[])
{
	if (argc < 3) {
		printf("Usage: %s <server/client> <port>\n", argv[0]);
		return 1;
	}

	char* mode = argv[1];
	int port = atoi(argv[2]);

	if (strcmp(mode, "server") == 0)
	{
		//start_server(port);
		//nuova gestione server
		struct Server server;
		Server_init(&server, port);
		//Server_run(&server);
		Server_basic_run(&server);
	}
	else if (strcmp(mode, "client") == 0)
	{
		start_client("127.0.0.1", port, &player);
	}
	else
	{
		printf("Invalid mode: %s\n", mode);
		return 1;
	}    
}


/*####################################################*/
int main(int argc, char* argv[])
{
	//AVVIO VALIDO
    if (connection_mgr(argc, argv) != 1) 
    {
		puts("Avvio valido");
        
    }
    
    return 0;    
}


