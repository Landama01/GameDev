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

	//size X2
	RangerIdleR.PushBack({ 120,9,50,62 });
	RangerIdleR.PushBack({ 216,9,50,60 });
	RangerIdleR.PushBack({ 312,11,62,60 });
	RangerIdleR.PushBack({ 408,11,60,60 });
	RangerIdleR.PushBack({ 504,11,48,60 });
	RangerIdleR.PushBack({ 600,11,48,60 });
	RangerIdleR.speed = 0.022f;


	RangerIdleL.PushBack({ 598,87,50,62 });
	RangerIdleL.PushBack({ 502,87,50,60 });
	RangerIdleL.PushBack({ 406,89,62,60 });
	RangerIdleL.PushBack({ 308,89,60,60 });
	RangerIdleL.PushBack({ 214,89,48,60 });
	RangerIdleL.PushBack({ 120,89,48,60 });
	RangerIdleL.speed = 0.022f;	
	

	RunRight.PushBack({ 24,171,50,62 });
	RunRight.PushBack({ 119,167,46,66 });
	RunRight.PushBack({ 211,165,46,68 });
	RunRight.PushBack({ 311,169,46,64 });
	RunRight.PushBack({ 407,171,50,62 });
	RunRight.PushBack({ 503,167,50,66 });
	RunRight.PushBack({ 599,165,50,68 });
	RunRight.PushBack({ 695,169,50,64 });
	RunRight.speed = 0.03f;


	RunLeft.PushBack({ 695,255,50,62 });
	RunLeft.PushBack({ 603,251,46,66 });
	RunLeft.PushBack({ 511,249,46,68 });
	RunLeft.PushBack({ 411,253,46,64 });
	RunLeft.PushBack({ 311,255,50,62 });
	RunLeft.PushBack({ 215,251,50,66 });
	RunLeft.PushBack({ 119,249,50,68 });
	RunLeft.PushBack({ 23,253,50,64 });
	RunLeft.speed = 0.03f;	

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
			position.x -= 1;
			currentAnimation = &RunLeft;
			goingLeft = true;
			goingRight = false;

		}
		else if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && position.x < 3200 - 32)
		{
			position.x += 1;
			currentAnimation = &RunRight;
			goingLeft = false;
			goingRight = true;
		}
		else if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT && position.y > -300 && GodMode == true)
		{
			position.y -= 1;
			currentAnimation = &RunRight;
			goingLeft = false;
			goingRight = true;
		}
		else if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && position.y < 530 && GodMode == true)
		{
			position.y += 1;
			currentAnimation = &RunRight;
			goingLeft = false;
			goingRight = true;
		}
		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
			Jumping = true;

		if (Jumping)
		{
			Jump();
			Jumping = false;
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
	position.y -= 5.0f;
}

bool Player::CleanUp()
{
	LOG("Freeing player");
	app->tex->UnLoad(RangerTex);

	return true;
}