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

	position.x = 150;
	position.y = 300;

	//RangerIdle.PushBack({ 12,17,25,31 });
	//RangerIdle.PushBack({ 60,17,25,31 });
	//RangerIdle.PushBack({ 108,18,25,30 });
	//RangerIdle.PushBack({ 156,18,26,30 });
	//RangerIdle.PushBack({ 204,18,25,30 });
	//RangerIdle.PushBack({ 252,18,24,30 });
	//RangerIdle.speed = 0.02f;


	//size X2
	RangerIdle.PushBack({ 24,34,50,62 });
	RangerIdle.PushBack({ 120,34,50,62 });
	RangerIdle.PushBack({ 216,36,50,60 });
	RangerIdle.PushBack({ 312,36,62,60 });
	RangerIdle.PushBack({ 408,36,60,60 });
	RangerIdle.PushBack({ 504,36,48,60 });
	RangerIdle.speed = 0.022f;

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
	idleTex = app->tex->Load("Assets/textures/RangerIdleX2.png");

	return true;
}

bool Player::Update(float dt)
{
	if (starting == true)
	{
		app->render->camera.x = 0;
		app->render->camera.y = 0;
		starting = false;
	}

	currentAnimation = &RangerIdle;

	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && position.x > 0 )
		position.x -= 1;
	
	else if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && position.x < 3200 - 32)
		position.x += 1;
	

	currentAnimation->Update();

	return true;
}

bool Player::PostUpdate()
{
	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(idleTex, position.x, position.y, &rect);

	return true;
}

bool Player::CleanUp()
{
	LOG("Freeing player");
	app->tex->UnLoad(idleTex);

	return true;
}