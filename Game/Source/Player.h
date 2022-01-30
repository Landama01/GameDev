#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Module.h"
#include "Animation.h"

struct SDL_Texture;
enum TypeCollision {

	VICTORY = 0,
	COLLISION = 1,
	CHECK_POINT = 2,
	AIR,
};
enum Direction {

	RIGHT=1,
	LEFT=-1,
};

class Player : public Module
{

public:

	Player();

	// Destructor
	virtual ~Player();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update(float dt);

	void ControlsGameMode();

	// Called before all Updates
	bool PostUpdate();

	void Jump();

	int CheckCollision(iPoint positionMap);

	// Called before quitting
	bool CleanUp();

	bool starting = true;
	bool goingLeft = false;
	bool goingRight = false;
	bool Jumping = false;
	int coinCounter;
	int lifes = 3;
	int playerScore=0;

	static const int numPoints = 8;
	iPoint pointsCollision[numPoints] = { { 0,0 },{50 , 0},{ 50,31 },{50 ,62 }, {0, 62},{0, 31} ,{25,0 } ,{25,62} };
	iPoint pointsCollisionBase[4] = { { 0,0 },{50 , 0},{50 ,62 }, {0, 62} };

	Direction dir = Direction::RIGHT;
	bool GodMode = false;

	bool Shooting = false;
	bool onlyOneArrow = false;
	bool climbing = false;

	float gravity = 0.01f;

	fPoint velocity;
	fPoint position;
	fPoint tmpPos;
	fPoint shootPos;
	fPoint normalPos;

	Animation* currentAnimation = nullptr;

	Animation RangerIdleR;
	Animation RangerIdleL;
	Animation RunRight;
	Animation RunLeft;
	Animation JumpingR;
	Animation JumpingL;
	Animation ShootingR;
	Animation ShootingL;

	SDL_Texture* RangerTex;
	SDL_Texture* Poster;

	uint ArrowShootFx = 0;
};

#endif // __PLAYER_H__