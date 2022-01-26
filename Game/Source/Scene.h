#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "GuiButton.h"
#include "GuiSlider.h"
#include "GuiCheckBox.h"

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

	bool SceneIntro = true;
	bool WinningState = false;
	bool LosingState = false;
	bool MenuState = false;

	float MidCamPos;

	float timer = 0;
	float sec = 60;

	int MenuCount;

	SDL_Texture* menu;

private:
	GuiButton* playButton;
	GuiSlider* slider;
	GuiCheckBox* checkbox;

	SDL_Texture* background;
	SDL_Texture* intro;
	SDL_Texture* winScene;
	SDL_Texture* loseScene;	
};

#endif // __SCENE_H__