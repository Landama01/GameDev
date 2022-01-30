#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "Module.h"
#include "Animation.h"

struct SDL_Texture;

class Enemy : public Module
{

public:

	Enemy();

	// Destructor
	virtual ~Enemy();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	int CheckCollision(iPoint positionMap);

	// Called before quitting
	bool CleanUp();

	bool starting = true;
	bool goingLeft = false;
	bool goingRight = false;
	bool dead = false;

	static const int numPoints = 8;
	iPoint pointsCollision[numPoints] = { { 0,0 },{24 , 0},{ 24,48 },{0 ,48 } };

	Direction dir = Direction::RIGHT;

	float gravity = 0.02f;

	fPoint velocity;
	fPoint position;
	fPoint tmpPos;

	Animation* currentAnimation = nullptr;

	Animation RangerIdleR;
	Animation RunRight;
	Animation RunLeft;

	SDL_Texture* SkeletonTex;
};

#endif // __ENEMY_H__