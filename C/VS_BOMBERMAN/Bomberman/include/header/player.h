#pragma once

typedef struct vec2
{
    int x;
    int y;
} vec2_t;

typedef struct player
{
    vec2_t position;
    unsigned int number_of_lifes;
    unsigned int number_of_bombs;
    unsigned int score;
    unsigned int speed;

    
    // struct sockaddr_in peer;
    // float transform[3];
    // uint32_t player_id;
    // char name[8];
    // time_t last_packet_timestamp;
} player_t;

static void bomberman_player_init(player_t* player)
{
    player->position.x = 0;
    player->position.y = 0;
    player->number_of_lifes = 1;
    player->number_of_bombs = 1;
    player->score = 0;
    player->speed = 1;
}