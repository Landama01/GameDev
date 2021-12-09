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

#include "Defs.h"
#include "Log.h"



Enemy::Enemy() : Module()
{
	name.Create("enemy");

	position.x = 1800;
	position.y = 235;

	velocity.y = 0.0f;
	velocity.x = 0.2f;

	//Set anims
	RangerIdleR.PushBack({ 120,9,50,62 });
	RangerIdleR.PushBack({ 216,9,50,62 });
	RangerIdleR.PushBack({ 312,11,62,62 });
	RangerIdleR.PushBack({ 408,11,60,62 });
	RangerIdleR.PushBack({ 504,11,48,62 });
	RangerIdleR.PushBack({ 600,11,48,62 });
	RangerIdleR.speed = 0.022f;

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

}

Enemy::~Enemy()
{}

bool Enemy::Awake()
{
	LOG("Loading Enemy");
	bool ret = true;

	return ret;
}

bool Enemy::Start()
{
	RangerTex = app->tex->Load("Assets/textures/Ranger_Spritesheet.png");

	return true;
}

bool Enemy::Update(float dt)
{
	if (starting == true)
	{
		currentAnimation = &RangerIdleR;
		goingRight = true;
		starting = false;
	}

	if (app->scene->SceneIntro)
	{
		position.x = 1800;
		position.y = 235;
	}

	if (position.x >= 1450 && position.x < 1460)
	{
		goingRight = true;
		goingLeft = false;
	}
	else if (position.x >= 1820 && position.x < 1830)
	{
		goingRight = false;
		goingLeft = true;
	}

	if (goingRight == true)
	{
		position.x += velocity.x * dt;
		currentAnimation = &RunRight;
	}
	if (goingLeft == true)
	{
		position.x -= velocity.x * dt;
		currentAnimation = &RunLeft;
	}

	if (app->scene->SceneIntro == false)
	{
		tmpPos = position;

		for (int i = 0; i < numPoints; i++)
		{
			if (CheckCollision(iPoint(position.x + pointsCollision[i].x + (velocity.x * dir), position.y + pointsCollision[i].y)))
			{
				position = tmpPos;
				break;

			}
		}
	}

	tmpPos = position;
	velocity.y -= gravity*dt;
	position.y -= velocity.y*dt;
	for (int i = 0; i < numPoints; i++)
	{

		if (CheckCollision(iPoint(position.x + pointsCollision[i].x, position.y + pointsCollision[i].y)))
		{
			position = tmpPos;
			velocity.y = 0;
			break;
		}
	}

	currentAnimation->Update();

	return true;
}

bool Enemy::PostUpdate()
{
	if (app->scene->SceneIntro == false && app->scene->WinningState == false && app->scene->LosingState == false)
	{
		SDL_Rect rect = currentAnimation->GetCurrentFrame();
		app->render->DrawTexture(RangerTex, position.x, position.y, &rect);
	}

	return true;
}

int Enemy::CheckCollision(iPoint positionMap)
{

	iPoint pos = app->map->WorldToMap(positionMap.x, positionMap.y);
	uint typeTileEnemy = app->map->mapData.layers.At(4)->data->Get(pos.x, pos.y);
	uint firstgidLayerCollisions = app->map->mapData.tilesets.At(2)->data->firstgid;
	typeTileEnemy -= firstgidLayerCollisions;

	if (typeTileEnemy == 0) {

		return true;
	}
	switch (typeTileEnemy)
	{
	case 1:
		// Collision
		return 1;
		break;
	default:
		return false;
		break;
	}


	return false;
}


bool Enemy::CleanUp()
{
	LOG("Freeing player");
	app->tex->UnLoad(RangerTex);

	return true;
}