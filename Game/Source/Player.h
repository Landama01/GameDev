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

	void ControlsForGodMode();

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

	static const int numPoints = 6;
	iPoint pointsCollision[numPoints] = { { 0,0 },{45 , 0},{ 45,26 },{45 ,54 }, {0, 54},{0, 26} };

	Direction dir = Direction::RIGHT;
	bool GodMode = false;

	float gravity = 0.02f;

	fPoint velocity;
	fPoint position;
	fPoint tmpPos;

	Animation* currentAnimation = nullptr;

	Animation RangerIdleR;
	Animation RangerIdleL;
	Animation RunRight;
	Animation RunLeft;
	Animation JumpingR;
	Animation JumpingL;

	SDL_Texture* RangerTex;
};

#endif // __PLAYER_H__