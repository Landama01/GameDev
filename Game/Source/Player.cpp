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

#define initPosX 150
#define initPosY 388

Player::Player() : Module()
{
	name.Create("player");

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
	JumpingR.speed = 0.02f;

	JumpingL.PushBack({ 600, 420, 52, 78 });
	JumpingL.PushBack({ 500, 420, 58, 78 });
	JumpingL.PushBack({ 309, 420, 58, 78 });
	JumpingL.PushBack({ 213, 420, 58, 78 });
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
		app->render->camera.x = 0;
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
		currentAnimation = &RangerIdleR;
	}		
	else if (goingLeft)
	{
		currentAnimation = &RangerIdleL;
	}
	if (app->scene->SceneIntro == false)
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
		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && position.y >= 530)
		{
			Jumping = true;
			velocity.y = 2.0f;
		}

		if (Jumping)
		{
			Jump();
			Jumping = false;
		}

		if (position.y < 530 && !GodMode)
		{
			velocity.y -= gravity;
			position.y -= velocity.y;
			if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
			{
				currentAnimation = &JumpingL;				
			}
			else currentAnimation = &JumpingR;
			
		}
		else
		{
			velocity.y = 0;
		}
	}
	currentAnimation->Update();

	return true;
}

bool Player::PostUpdate()
{
	if (app->scene->SceneIntro == false)
	{
		SDL_Rect rect = currentAnimation->GetCurrentFrame();
		app->render->DrawTexture(RangerTex, position.x, position.y, &rect);
	}

	return true;
}

void Player::Jump()
{
	velocity.y -= gravity;
	position.y -= velocity.y;
}

bool Player::CleanUp()
{
	LOG("Freeing player");
	app->tex->UnLoad(RangerTex);

	return true;
}