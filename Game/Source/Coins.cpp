#include "App.h"
#include "Coins.h"
#include "Audio.h"
#include "Player.h"
#include "Module.h"
#include "Textures.h"
#include "Map.h"
#include "Log.h"
#include "Render.h"



Coins::Coins() : Module()
{
	name.Create("coin");
}

Coins::~Coins()
{}

bool Coins::Start()
{

	coinAnimation = new Animation();


	texCoin = app->tex->Load("Assets/textures/coin_square.png");
	coinFx = app->audio->LoadFx("Assets/audio/fx/coin.wav");

	
	pointsCollision = new iPoint[4]{ { 0, 0 }, { 48 , 0 }, { 48,-48 }, { 0 ,-48 } };

	coinAnimation->loop = true;
	coinAnimation->speed = 0.20f;

	for (int i = 0; i < 16; i++)
		coinAnimation->PushBack({ 0,(40 * i), 40, 40 });

	listCoin.Add(app->map->MapToWorld(25,9));
	listCoin.Add(app->map->MapToWorld(65,8));
	listCoin.Add(app->map->MapToWorld(77,8));
	active = false;

	return true;
}


bool Coins::Awake(pugi::xml_node& config)
{
	LOG("Loading Coins Parser");
	bool ret = true;


	return ret;
}

bool Coins::PreUpdate()
{
	iPoint currentPositionPlayer;
	currentPositionPlayer.x = app->player->position.x;
	currentPositionPlayer.y = app->player->position.y;
	iPoint auxPositionPlayer[4];
	for (int i = 0; i < 4; i++)
	{
		auxPositionPlayer[i] = { currentPositionPlayer.x + app->player->pointsCollisionBase[i].x,
			-48 + currentPositionPlayer.y + app->player->pointsCollisionBase[i].y };

	}

	for (int i = 0; i < listCoin.Count(); i++)
	{
		iPoint auxPositionCoin[4];
		for (int j = 0; j < 4; j++)
		{
			auxPositionCoin[j] = { listCoin.At(i)->data.x + pointsCollision[j].x,
				listCoin.At(i)->data.y + pointsCollision[j].y };
		}
		for (int j = 0; j < 4; j++)
		{
			if (listCoin.At(i)->data == auxPositionPlayer[j]) 
			{
				listCoin.Del(listCoin.At(i));
				app->player->coinCounter++;
			}
		}
	}
	
	return true;
}

bool Coins::Update(float dt)
{
	coinAnimation->Update();
	
	return true;
}
bool Coins::PostUpdate()
{

	SDL_Rect rectCoins;
		
	for (int i = 0; i < listCoin.Count(); i++)
	{
		rectCoins = coinAnimation->GetCurrentFrame();
		app->render->DrawTexture(texCoin, listCoin.At(i)->data.x, listCoin.At(i)->data.y, &rectCoins);
	}

	return true;

}

bool Coins::CleanUp()
{
	if (!active)
		return true;

	delete coinAnimation;	
	delete pointsCollision;	
	active = false;

	return true;
}