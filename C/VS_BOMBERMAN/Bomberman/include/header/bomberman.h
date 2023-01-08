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
} player_t;

typedef struct bomb
{
	vec2_t position;
	unsigned int range;
	unsigned int cooldown;
} bomb_t;

enum CellType
{
	Ground,
	Wall,
	Undestroyable_Wall,
	Teleport
};

enum BonusType
{
	None,
	Speed,
	Bomb2,
	Bomb3
};

enum EnemyType
{
	Bat,
	Balloon
};

typedef struct cell
{
	enum CellType type;
	enum BonusType bonus;
} cell_t;

typedef struct enemy
{
	vec2_t position;
	enum EnemyType type;
} enemy_t;

typedef struct game_mode
{
	unsigned int timer;
} game_mode_t;