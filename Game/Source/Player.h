#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Module.h"
#include "Animation.h"

struct SDL_Texture;

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

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	bool starting = true;
	bool goingLeft = false;
	bool goingRight = false;

	iPoint position;

	Animation* currentAnimation = nullptr;

	Animation RangerIdleR;
	Animation RangerIdleL;
	Animation RunRight;
	Animation RunLeft;

	SDL_Texture* RangerTex;
};

#endif // __PLAYER_H__