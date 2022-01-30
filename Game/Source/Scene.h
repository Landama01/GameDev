#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "GuiButton.h"
#include "GuiSlider.h"
#include "GuiCheckBox.h"
#include "Animation.h"
#include "Point.h"
#include "Timer.h"
struct SDL_Texture;

class Scene : public Module
{
public:

	Scene();

	// Destructor
	virtual ~Scene();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	void Menu();

	bool OnGuiMouseClickEvent(GuiControl* control);

	// Called before quitting
	bool CleanUp();

	bool mainStage = false;

	bool SceneIntro = true;
	bool WinningState = false;
	bool LosingState = false;
	bool MenuState = false;
	bool playerFalling = false;

	bool mute = false;
	bool exit = false;

	float MidCamPos;

	Timer sceneTimer;
	float realSceneTime=0;

	float timer = 0;
	float sec = 60;

	float delay = 0;

	int MenuCount;

	SDL_Texture* menu;

	iPoint point0 = { 0,0 };
	char scoreText[12] = { "\0" };
	char coinText[12] = { "\0" };
	char timerText[12] = { "\0" };

private:
	GuiButton* playButton;
	GuiSlider* sliderMusic;
	GuiSlider* sliderFx;
	GuiCheckBox* checkbox;

	SDL_Texture* background;
	SDL_Texture* intro;
	SDL_Texture* winScene;
	SDL_Texture* loseScene;	
	SDL_Texture* heart;		

	Animation heartAnim;

	uint UIFx = 0;
};

#endif // __SCENE_H__