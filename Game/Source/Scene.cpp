#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"
#include "Player.h"

#include "Defs.h"
#include "Log.h"

#define topLimit 130
#define botLimit 300

Scene::Scene() : Module()
{
	name.Create("scene");
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;	

	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	background = app->tex->Load("Assets/textures/Background2.png");
	intro = app->tex->Load("Assets/textures/SceneIntro.png");
	winScene = app->tex->Load("Assets/textures/WinScene.png");
	loseScene = app->tex->Load("Assets/textures/LoseScene.png");

	// L03: DONE: Load map
	//app->map->Load("hello.tmx");
	app->map->Load("tilesetX2.tmx");
	
	// Load music
	//app->audio->PlayMusic("Assets/audio/music/music_spy.ogg");

	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	
	if (SceneIntro == false && WinningState == false && LosingState == false)
	{
		//DEBUG KEYS
		if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
		{
			app->player->position.x = initPosX;
			app->player->position.y = initPosY;
			app->player->goingRight = true;
			app->player->Jumping = true;
			app->render->camera.x = 0;
			app->render->camera.y = 0;
		}
		if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
			app->SaveGameRequest();

		if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
			app->LoadGameRequest();	
		if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN && app->player->GodMode == false)
			app->player->GodMode = true;			
		else if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN && app->player->GodMode == true)
			app->player->GodMode = false;			
				
		MidCamPos = -(app->render->camera.x - (1280 / 2));

		//LINKED CAMERA TO PLAYER
		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT && app->player->position.y <= topLimit + 200 && !app->player->GodMode)
			app->render->camera.y += app->player->velocity.y;

		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && app->player->position.y > 0 && !app->player->GodMode)
			app->render->camera.y -= app->player->velocity.y;

		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && app->player->position.x <= 3200 - 1280 / 2 && app->player->position.x <= MidCamPos)
			app->render->camera.x += app->player->velocity.x;

		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && app->player->position.x >= 1280 / 2 && app->player->position.x >= MidCamPos)
			app->render->camera.x -= app->player->velocity.x;

		//set camera limits
	}

	if (app->render->camera.x >= 0)
		app->render->camera.x = 0;

	if (app->render->camera.x <= -1920)
		app->render->camera.x = -1920;

	if (app->render->camera.y >= botLimit)
		app->render->camera.y = botLimit;

	if (app->render->camera.y <= topLimit)
		app->render->camera.y = topLimit;

	if (app->player->position.x >= 3100 && WinningState == false)
		WinningState = true;

	if (app->player->position.y >= 600 && LosingState == false)
		LosingState = true;	

	app->render->DrawTexture(background, 0, -topLimit);
	// Draw map
	app->map->Draw();
	
	// L03: DONE 7: Set the window title with map/tileset info
	/*SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d",
		app->map->mapData.width, app->map->mapData.height,
		app->map->mapData.tileWidth, app->map->mapData.tileHeight,
		app->map->mapData.tilesets.count());

	app->win->SetTitle(title.GetString());*/
	
	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;	
	
	if (SceneIntro == true)
	{
		app->render->DrawTexture(intro, 0, -topLimit);
	}

	if (WinningState == true && SceneIntro == false)
	{
		if (timer <= 6 * sec)
		{
			//Initial positions
			app->render->camera.x = 0;
			app->render->camera.y = 0;
			app->player->position.x = initPosX;
			app->player->position.y = initPosY;
			app->render->DrawTexture(winScene, 0, 0);
			timer++;
		}
		else
		{
			WinningState = false;
			timer = 0;
			app->player->GodMode = false;
			SceneIntro = true;
		}			
	}

	if (LosingState == true && SceneIntro == false)
	{
		if (timer <= 6 * sec)
		{
			//Initial positions
			app->render->camera.x = 0;
			app->render->camera.y = 0;
			app->player->position.x = initPosX;
			app->player->position.y = initPosY;
			app->render->DrawTexture(loseScene, 0, 0);
			timer++;
		}
		else
		{
			LosingState = false;
			timer = 0;
			app->player->GodMode = false;
			SceneIntro = true;
		}
	}
		

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && SceneIntro == true)
		SceneIntro = false;

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	app->tex->UnLoad(background);
	app->tex->UnLoad(intro);
	app->tex->UnLoad(winScene);
	app->tex->UnLoad(loseScene);

	return true;
}

