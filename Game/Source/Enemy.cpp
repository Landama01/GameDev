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
#include "Arrow.h"

#include "Defs.h"
#include "Log.h"



Enemy::Enemy() : Module()
{
	name.Create("enemy");

	position.x = 1800;
	position.y = 235;

	velocity.y = 0.0f;
	velocity.x = 1.5f;

	//Set anims

	RunRight.PushBack({ 0, 0, 24, 48 });
	RunRight.PushBack({ 55, 0, 24, 48 });
	RunRight.PushBack({ 112, 0, 24, 48 });
	RunRight.PushBack({ 168, 0, 24, 48 });
	RunRight.speed = 0.1f;

	RunLeft.PushBack({ 0, 52, 24, 48 });
	RunLeft.PushBack({ 56, 52, 24, 48 });
	RunLeft.PushBack({ 111, 52, 24, 48 });
	RunLeft.PushBack({ 169, 52, 24, 48 });
	RunLeft.speed = 0.1f;

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
	SkeletonTex = app->tex->Load("Assets/textures/Skeleton.png");

	return true;
}

bool Enemy::Update(float dt)
{
	LOG("enemy X: %f Y: %f", position.x, position.y);

	if (starting == true)
	{
		currentAnimation = &RunRight;
		goingRight = true;
		starting = false;
	}

	if (app->scene->SceneIntro)
	{
		dead = false;
		position.x = 1800;
		position.y = 235;
	}
	if (dead)
	{
		position.x = -100;
		position.y = 0;
	}

	if (!dead)
	{
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
			position.x += velocity.x;
			currentAnimation = &RunRight;
		}
		if (goingLeft == true)
		{
			position.x -= velocity.x;
			currentAnimation = &RunLeft;
		}
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

	//CHECK: hardcoded arrow collision with enemy
	if (app->arrow->position.x >= position.x && app->arrow->position.x <= position.x + 50)
	{
		if (app->arrow->position.y >= position.y && app->arrow->position.y <= position.y + 62)
		{
			dead = true;
			app->player->playerScore += 100;
		}
	}

	tmpPos = position;
	velocity.y -= gravity;
	position.y -= velocity.y;
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
	if (app->scene->SceneIntro == false && app->scene->WinningState == false && app->scene->LosingState == false && !dead)
	{
		SDL_Rect rect = currentAnimation->GetCurrentFrame();
		app->render->DrawTexture(SkeletonTex, position.x, position.y, &rect);
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
	app->tex->UnLoad(SkeletonTex);

	return true;
}