#include "App.h"
#include "Arrow.h"
#include "Audio.h"
#include "Player.h"
#include "Module.h"
#include "Textures.h"
#include "Map.h"
#include "Log.h"
#include "Render.h"
#include "Scene.h"
#include "Enemy.h"



Arrow::Arrow() : Module()
{
	name.Create("arrow");

	velocity.y = 0.0f;
	velocity.x = 1.0f;

	ArrowTimer = 0;

	ArrowRight.PushBack({ 0, 0, 38, 10 });
	ArrowLeft.PushBack({ 42, 0, 38, 10 });

}

Arrow::~Arrow()
{
}

bool Arrow::Awake(pugi::xml_node& config)
{
	LOG("Loading Arrow");
	bool ret = true;


	return ret;
}

bool Arrow::Start()
{
	ArrowTex = app->tex->Load("Assets/textures/Arrow.png");
	ArrowHitFx = app->audio->LoadFx("Assets/audio/fx/arrowHit.wav");

	return true;
}

bool Arrow::Update(float dt)
{

	//CHECK: implement map collision
	if (!app->scene->SceneIntro && !app->scene->MenuState)
	{
		if (app->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT && app->player->goingRight && !ShootingL && !ShootingR && !app->player->Jumping)
		{
			hit = false;
			ShootingR = true;
			tmpPos = app->player->position;
			position.x = tmpPos.x + 20;
			position.y = tmpPos.y + 25;
		}
		else if (app->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT && app->player->goingLeft && !ShootingL && !ShootingR && !app->player->Jumping)
		{
			hit = false;
			ShootingL = true;
			tmpPos = app->player->position;
			position.x = tmpPos.x - 5;
			position.y = tmpPos.y + 25;
		}

		if (ShootingR && !app->player->Shooting)
		{
			position.x += velocity.x * dt;
			position.y += velocity.y * dt;
			currentAnimation = &ArrowRight;
			if (position.x >= tmpPos.x + 400)
			{
				ArrowTimer++;
				velocity.y += 0.1;
			}
			if (ArrowTimer * dt >= 100)
			{
				ShootingR = false;
				app->player->onlyOneArrow = false;
				velocity.y = 0;
				ArrowTimer = 0;
				position.x = NULL;
				position.y = NULL;
			}
		}

		if (ShootingL && !app->player->Shooting)
		{
			position.x -= velocity.x * dt;
			position.y += velocity.y * dt;
			currentAnimation = &ArrowLeft;
			if (position.x <= tmpPos.x - 400)
			{
				ArrowTimer++;
				velocity.y += 0.1;
			}
			if (ArrowTimer * dt >= 100)
			{
				ShootingL = false;
				app->player->onlyOneArrow = false;
				velocity.y = 0;
				ArrowTimer = 0;
				position.x = NULL;
				position.y = NULL;
			}
		}
	}

	if (position.x >= app->enemy->position.x && position.x <= app->enemy->position.x + 50)
	{
		if (position.y >= app->enemy->position.y && position.y <= app->enemy->position.y + 62)
		{
			hit = true;
			app->audio->PlayFx(ArrowHitFx, 0);
		}
	}
	
	return true;
}

bool Arrow::PostUpdate()
{
	if (app->scene->SceneIntro == false && app->scene->WinningState == false && app->scene->LosingState == false && ShootingR && !hit && !app->player->Shooting)
	{
		SDL_Rect rect = currentAnimation->GetCurrentFrame();
		app->render->DrawTexture(ArrowTex, position.x, position.y, &rect);
	}
	
	if (app->scene->SceneIntro == false && app->scene->WinningState == false && app->scene->LosingState == false && ShootingL && !hit && !app->player->Shooting)
	{
		SDL_Rect rect = currentAnimation->GetCurrentFrame();
		app->render->DrawTexture(ArrowTex, position.x, position.y, &rect);		
	}

	return true;

}

bool Arrow::CleanUp()
{
	app->tex->UnLoad(ArrowTex);

	return true;
}