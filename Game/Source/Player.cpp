#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Map.h"
#include "Player.h"
#include "Enemy.h"
#include "Scene.h"
#include "GuiManager.h"

#include "Defs.h"
#include "Log.h"



Player::Player() : Module()
{
	name.Create("player");
	Jumping = true;

	position.x = initPosX;
	position.y = initPosY;

	velocity.y = 0.0f;
	velocity.x = 0.4f;

	//size X2
	RangerIdleR.PushBack({ 120,9,50,62 });
	RangerIdleR.PushBack({ 216,9,50,62 });
	RangerIdleR.PushBack({ 312,11,62,62 });
	RangerIdleR.PushBack({ 408,11,60,62 });
	RangerIdleR.PushBack({ 504,11,48,62 });
	RangerIdleR.PushBack({ 600,11,48,62 });
	RangerIdleR.speed = 0.088f;


	RangerIdleL.PushBack({ 598,87,50,62 });
	RangerIdleL.PushBack({ 502,87,50,62 });
	RangerIdleL.PushBack({ 406,89,62,62 });
	RangerIdleL.PushBack({ 308,89,60,62 });
	RangerIdleL.PushBack({ 214,89,48,62 });
	RangerIdleL.PushBack({ 120,89,48,62 });
	RangerIdleL.speed = 0.088f;	
	

	RunRight.PushBack({ 24,171,50,62 });
	RunRight.PushBack({ 119,167,46,62 });
	RunRight.PushBack({ 211,165,46,62 });
	RunRight.PushBack({ 311,169,46,62 });
	RunRight.PushBack({ 407,171,50,62 });
	RunRight.PushBack({ 503,167,50,62 });
	RunRight.PushBack({ 599,165,50,62 });
	RunRight.PushBack({ 695,169,50,62 });
	RunRight.speed = 0.12f;


	RunLeft.PushBack({ 695,255,50,62 });
	RunLeft.PushBack({ 603,251,46,62 });
	RunLeft.PushBack({ 511,249,46,62 });
	RunLeft.PushBack({ 411,253,46,62 });
	RunLeft.PushBack({ 311,255,50,62 });
	RunLeft.PushBack({ 215,251,50,62 });
	RunLeft.PushBack({ 119,249,50,62 });
	RunLeft.PushBack({ 23,253,50,62 });
	RunLeft.speed = 0.12f;	

	//JumpingR.PushBack({21, 330, 50, 78});
	JumpingR.PushBack({117, 330, 52, 78});
	JumpingR.PushBack({209, 330, 58, 78});
	//JumpingR.PushBack({305, 330, 58, 78});
	JumpingR.PushBack({401, 330, 58, 78});
	JumpingR.PushBack({497, 330, 58, 78});
	//JumpingR.PushBack({599, 330, 48, 78});
	//JumpingR.PushBack({695, 330, 52, 78});
	JumpingR.loop = false;
	JumpingR.speed = 0.06f;

	JumpingL.PushBack({ 600, 420, 52, 78 });
	JumpingL.PushBack({ 500, 420, 58, 78 });
	JumpingL.PushBack({ 309, 420, 58, 78 });
	JumpingL.PushBack({ 213, 420, 58, 78 });
	JumpingL.loop = false;
	JumpingL.speed = 0.06f;

	ShootingR.PushBack({  70, 499, 58, 69});
	ShootingR.PushBack({ 169, 499, 58, 69 });
	ShootingR.PushBack({ 264, 499, 58, 69 });
	ShootingR.PushBack({ 358, 499, 58, 69 });
	ShootingR.PushBack({ 454, 499, 58, 69 });
	ShootingR.loop = false;
	ShootingR.speed = 0.12f;

	ShootingL.PushBack({ 644, 585, 58, 69 });
	ShootingL.PushBack({ 554, 585, 58, 69 });
	ShootingL.PushBack({ 438, 585, 58, 69 });
	ShootingL.PushBack({ 350, 585, 58, 69 });
	ShootingL.PushBack({ 255, 585, 58, 69 });
	ShootingL.loop = false;
	ShootingL.speed = 0.12f;

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
	Poster = app->tex->Load("Assets/textures/TextoCartel.png");
	ArrowShootFx = app->audio->LoadFx("Assets/audio/fx/shoot.wav");

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
	if (velocity.y >= 2) {
		velocity.y = 2;
	}

	if (GodMode)
	{
		if (position.y <= -130)
		{
			velocity.y = 0.0f;
			position.y += 1;
		}
		else velocity.y = 0.5f;
	}	

	if (goingRight)
	{
		if (Jumping)
		{
			currentAnimation = &JumpingR;
		}
		else currentAnimation = &RangerIdleR;
	}
	else if (goingLeft)
	{
		if (Jumping)
		{
			currentAnimation = &JumpingL;
		}
		else currentAnimation = &RangerIdleL;
	}

	if (JumpingR.HasFinished() && !Jumping || !Jumping)
	{
		JumpingR.Reset();
	}
	if (JumpingL.HasFinished() && !Jumping || !Jumping)
	{
		JumpingL.Reset();
	}

	if (goingRight)
	{
		if (Shooting)
		{
			currentAnimation = &ShootingR;			
			position.y = shootPos.y;
		}
		else currentAnimation = &RangerIdleR;
	}
	else if (goingLeft)
	{
		if (Shooting)
		{
			currentAnimation = &ShootingL;			
			position.y = shootPos.y;
		}
		else currentAnimation = &RangerIdleL;
	}

	if (ShootingR.HasFinished() && Shooting)
	{
		ShootingR.Reset();
		Shooting = false;
		app->audio->PlayFx(ArrowShootFx, 0);
		if (position.y == shootPos.y)
		{
			position.y + 5;
		}		
	}
	if (ShootingL.HasFinished() && Shooting)
	{
		ShootingL.Reset();
		Shooting = false;
		app->audio->PlayFx(ArrowShootFx, 0);
		if (position.y == shootPos.y)
		{
			position.y + 5;
		}		
	}


	if (!app->scene->SceneIntro && !GodMode && !app->scene->MenuState)
	{
		tmpPos = position;
		LOG("X: %f Y: %f", position.x, position.y);
		//using stairs
		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT && position.x >= 1610 && position.x <= 1640 && position.y >= 340)
		{
			climbing = true;
		}
		else if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && position.x >= 1610 && position.x <= 1640 && position.y <= 130)
		{
			climbing = true;
		}
		else if (position.y >= 380 || position.y <= 130)
		{
			climbing = false;
		}		

		if (climbing)
		{
			velocity.y = 0.2f;

			if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
			{
				position.y -= velocity.y * dt;
			}
			else if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
			{
				position.y += velocity.y * dt;
			}
			else position.y = position.y;
		}		

		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && position.x > 0 && !Jumping && !Shooting && !climbing)
		{
			position.x -= velocity.x *dt;
			currentAnimation = &RunLeft;
			goingLeft = true;
			goingRight = false;
			dir = Direction::LEFT;

		}
		else if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && position.x > 0 && Jumping && !Shooting && !climbing)
		{
			position.x -= velocity.x *dt ;
			goingLeft = true;
			goingRight = false;
			dir = Direction::LEFT;

		}
		else if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && position.x < 3200 - 32 && !Jumping && !Shooting && !climbing)
		{
			position.x += velocity.x*dt;
			currentAnimation = &RunRight;
			goingLeft = false;
			goingRight = true;
			dir = Direction::RIGHT;

		}
		else if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && position.x < 3200 - 32 && Jumping && !Shooting && !climbing)
		{
			position.x += velocity.x*dt;
			goingLeft = false;
			goingRight = true;
			dir = Direction::RIGHT;

		}

		if ((app->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT && !Shooting && !Jumping && !onlyOneArrow && !climbing))
		{			
			Shooting = true;
			onlyOneArrow = true;
			normalPos = position;
			shootPos.y = position.y - 5;
		}

		if (!climbing)
		{
			for (int i = 0; i < numPoints; i++)
			{
				if (CheckCollision(iPoint(position.x + pointsCollision[i].x + (velocity.x * dir), position.y + pointsCollision[i].y)))
				{
					position = tmpPos;
					break;
				}
			}
		}
		
		if (!app->scene->MenuState)
		{
			ControlsGameMode();
		}
		
		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN  && !Jumping && !climbing)
		{
			Jump();
		}

		//if (position.y < 530 && !GodMode)
		if ( !GodMode && !Shooting && !climbing && !app->scene->playerFalling)
		{
			
			tmpPos = position;
			velocity.y -=gravity*dt;
			position.y -= velocity.y*dt;
			for (int i = 0; i < numPoints; i++)
			{

				if (CheckCollision(iPoint(position.x + pointsCollision[i].x , position.y + pointsCollision[i].y)))
				{
					position = tmpPos;
					velocity.y = 0;
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
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && position.x > 0)
		{
			position.x -= velocity.x * dt;
			currentAnimation = &RunLeft;
			goingLeft = true;
			goingRight = false;

		}
		else if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && position.x < 3200 - 32)
		{
			position.x += velocity.x * dt;
			currentAnimation = &RunRight;
			goingLeft = false;
			goingRight = true;

		}

		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT && position.y >= -130)
		{
			position.y -= velocity.y * dt;
		}
		else if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && position.y <= 528)
		{
			position.y += velocity.y * dt;
		}
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

bool Player::PostUpdate()
{
	if (app->scene->SceneIntro == false && app->scene->WinningState == false && app->scene->LosingState == false)
	{
		SDL_Rect rect = currentAnimation->GetCurrentFrame();
		app->render->DrawTexture(RangerTex, position.x, position.y, &rect);
	}
	//GUI Menu
	if (app->scene->MenuState)
	{
		app->render->DrawTexture(app->scene->menu, -(app->render->camera.x - 200), -(app->render->camera.y - 50));
		app->guiManager->Draw();
	}
	//Tree Poster Text
	if (position.x >= 1370 && position.x <= 1425)
	{
		if (position.y >= 155 && position.y <= 165)
		{
			app->render->DrawTexture(Poster, -(app->render->camera.x - 200), -(app->render->camera.y - 50));
		}
	}

	return true;
}

void Player::Jump()
{
	Jumping = true;
	velocity.y = 1.5f;
}


int Player::CheckCollision(iPoint positionMap)
{
	
	iPoint pos = app->map->WorldToMap(positionMap.x, positionMap.y);
	uint typeTilePlayer = app->map->mapData.layers.At(4)->data->Get(pos.x,pos.y);
	uint firstgidLayerCollisions = app->map->mapData.tilesets.At(2)->data->firstgid;
	typeTilePlayer -= firstgidLayerCollisions;


	//LOG(" Cordenada Y %d  cordenada  X %d", pos.y, pos.x);

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
	app->tex->UnLoad(app->scene->menu);
	app->tex->UnLoad(Poster);

	return true;
}