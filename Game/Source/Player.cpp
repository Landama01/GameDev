#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Map.h"
#include "Player.h"
#include "Scene.h"

#include "Defs.h"
#include "Log.h"



Player::Player() : Module()
{
	name.Create("player");
	Jumping = false;

	position.x = initPosX;
	position.y = initPosY;

	velocity.y = 0.0f;
	velocity.x = 1.0f;

	//size X2
	RangerIdleR.PushBack({ 120,9,50,62 });
	RangerIdleR.PushBack({ 216,9,50,62 });
	RangerIdleR.PushBack({ 312,11,62,62 });
	RangerIdleR.PushBack({ 408,11,60,62 });
	RangerIdleR.PushBack({ 504,11,48,62 });
	RangerIdleR.PushBack({ 600,11,48,62 });
	RangerIdleR.speed = 0.022f;


	RangerIdleL.PushBack({ 598,87,50,62 });
	RangerIdleL.PushBack({ 502,87,50,62 });
	RangerIdleL.PushBack({ 406,89,62,62 });
	RangerIdleL.PushBack({ 308,89,60,62 });
	RangerIdleL.PushBack({ 214,89,48,62 });
	RangerIdleL.PushBack({ 120,89,48,62 });
	RangerIdleL.speed = 0.022f;	
	

	RunRight.PushBack({ 24,171,50,62 });
	RunRight.PushBack({ 119,167,46,62 });
	RunRight.PushBack({ 211,165,46,62 });
	RunRight.PushBack({ 311,169,46,62 });
	RunRight.PushBack({ 407,171,50,62 });
	RunRight.PushBack({ 503,167,50,62 });
	RunRight.PushBack({ 599,165,50,62 });
	RunRight.PushBack({ 695,169,50,62 });
	RunRight.speed = 0.03f;


	RunLeft.PushBack({ 695,255,50,62 });
	RunLeft.PushBack({ 603,251,46,62 });
	RunLeft.PushBack({ 511,249,46,62 });
	RunLeft.PushBack({ 411,253,46,62 });
	RunLeft.PushBack({ 311,255,50,62 });
	RunLeft.PushBack({ 215,251,50,62 });
	RunLeft.PushBack({ 119,249,50,62 });
	RunLeft.PushBack({ 23,253,50,62 });
	RunLeft.speed = 0.03f;	

	//JumpingR.PushBack({21, 330, 50, 78});
	JumpingR.PushBack({117, 330, 52, 78});
	JumpingR.PushBack({209, 330, 58, 78});
	//JumpingR.PushBack({305, 330, 58, 78});
	JumpingR.PushBack({401, 330, 58, 78});
	JumpingR.PushBack({497, 330, 58, 78});
	//JumpingR.PushBack({599, 330, 48, 78});
	//JumpingR.PushBack({695, 330, 52, 78});
	JumpingR.loop = false;
	JumpingR.speed = 0.02f;

	JumpingL.PushBack({ 600, 420, 52, 78 });
	JumpingL.PushBack({ 500, 420, 58, 78 });
	JumpingL.PushBack({ 309, 420, 58, 78 });
	JumpingL.PushBack({ 213, 420, 58, 78 });
	JumpingL.loop = false;
	JumpingL.speed = 0.02f;
}

Player::~Player()
{}

bool Player::Awake()
{
	LOG("Loading Player");
	bool ret = true;

	return ret;
}

bool Player::Start()
{
	RangerTex = app->tex->Load("Assets/textures/Ranger_Spritesheet.png");

	return true;
}

bool Player::Update(float dt)
{
	if (starting == true)
	{
		app->render->camera.x = position.x;
		app->render->camera.y = 0;
		starting = false;
		goingRight = true;
	}


	if (GodMode)
	{
		

		if (position.y <= -130)
		{
			velocity.y = 0.0f;
			position.y += 1;
		}
		else velocity.y = 1.0f;
	}		

	if (goingRight)
	{
		if (Jumping) currentAnimation = &JumpingR;
		else currentAnimation = &RangerIdleR;
	}
	else if (goingLeft)
	{
		if(Jumping) currentAnimation = &JumpingL;
		else currentAnimation = &RangerIdleL;
	}
	if (app->scene->SceneIntro == false && GodMode == false)
	{
		tmpPos = position;

		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && position.x > 0 && !Jumping)
		{
			position.x -= velocity.x;
			currentAnimation = &RunLeft;
			goingLeft = true;
			goingRight = false;
			dir = Direction::LEFT;

		}
		else if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && position.x > 0 && Jumping)
		{
			position.x -= velocity.x;
			goingLeft = true;
			goingRight = false;
			dir = Direction::LEFT;

		}
		else if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && position.x < 3200 - 32 && !Jumping)
		{
			position.x += velocity.x;
			currentAnimation = &RunRight;
			goingLeft = false;
			goingRight = true;
			dir = Direction::RIGHT;

		}
		else if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && position.x < 3200 - 32 && Jumping)
		{
			position.x += velocity.x;
			goingLeft = false;
			goingRight = true;
			dir = Direction::RIGHT;

		}
		for (int i=0; i < numPoints;i++) 
		{			
			if (CheckCollision(iPoint(position.x + pointsCollision[i].x + (velocity.x * dir), position.y + pointsCollision[i].y)))
			{
				position = tmpPos;
				break;

			}					

		}


		ControlsGameMode();
		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN  && !Jumping)
		{
			Jump();
		}

		//if (position.y < 530 && !GodMode)
		if ( !GodMode)
		{
			
			tmpPos = position;
			velocity.y =(velocity.y <-0.5f)? -0.5f : velocity.y-=gravity;
			position.y -= velocity.y;
			for (int i = 0; i < numPoints; i++)
			{
				if (CheckCollision(iPoint(position.x + pointsCollision[i].x , position.y + pointsCollision[i].y)))
				{
					position = tmpPos;
					Jumping = false;
					break;

				}

			}

			

		}
		else
		{
			velocity.y = 0;
		}
	}
	if (GodMode)
	{
		ControlsForGodMode();
	}
	currentAnimation->Update();

	return true;
}

void Player::ControlsGameMode()
{
	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT && position.y > -300 && GodMode)
	{
		position.y -= velocity.y;

		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			currentAnimation = &RunLeft;
			goingLeft = true;
			goingRight = false;
		}
		else
		{
			currentAnimation = &RunRight;
			goingLeft = false;
			goingRight = true;
		}
	}
	else if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && position.y < 530 && GodMode)
	{
		position.y += velocity.y;

		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			currentAnimation = &RunLeft;
			goingLeft = true;
			goingRight = false;
		}
		else
		{
			currentAnimation = &RunRight;
			goingLeft = false;
			goingRight = true;
		}
	}
}

void Player::ControlsForGodMode()
{
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && position.x > 0)
	{
		position.x -= velocity.x;
		currentAnimation = &RunLeft;
		goingLeft = true;
		goingRight = false;

	}
	else if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && position.x < 3200 - 32)
	{
		position.x += velocity.x;
		currentAnimation = &RunRight;
		goingLeft = false;
		goingRight = true;

	}

	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT && position.y >= -130)
	{
		position.y -= velocity.y;
	}
	else if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && position.y <= 528)
	{
		position.y += velocity.y;
	}
}

bool Player::PostUpdate()
{
	if (app->scene->SceneIntro == false && app->scene->WinningState == false && app->scene->LosingState == false)
	{
		SDL_Rect rect = currentAnimation->GetCurrentFrame();
		app->render->DrawTexture(RangerTex, position.x, position.y, &rect);
	}

	return true;
}

void Player::Jump()
{
	
	Jumping = true;
	velocity.y = 2.0f;

}


int Player::CheckCollision(iPoint positionMap)
{
	
	iPoint pos = app->map->WorldToMap(positionMap.x, positionMap.y);
	uint typeTilePlayer = app->map->mapData.layers.At(4)->data->Get(pos.x,pos.y);
	uint firstgidLayerCollisions = app->map->mapData.tilesets.At(2)->data->firstgid;
	typeTilePlayer -= firstgidLayerCollisions;


	LOG(" Cordeada Y %d  cordenada  X %d", pos.y, pos.x);

	if (typeTilePlayer == 0) {
	
		return true;
	}
		switch (typeTilePlayer)
		{
		case VICTORY:
			// Victory
			if (positionMap == app->map->WorldToMap(position.x, position.y))/*win :D*/;
				
			return VICTORY;
			break;

		case COLLISION:
			// Collision
			return COLLISION;
			break;

		case CHECK_POINT:
			// Checkpoint
			if (positionMap == app->map->WorldToMap(position.x,position.y) )
			return CHECK_POINT;
			break;

		default:
			return false;
			break;
		}
	

	return false;
}


bool Player::CleanUp()
{
	LOG("Freeing player");
	app->tex->UnLoad(RangerTex);

	return true;
}