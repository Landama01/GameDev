#include "GuiCheckBox.h"
#include "App.h"
#include "Render.h"
#include "Audio.h"
#include "Scene.h"
#include "Window.h"
#include "ModuleFonts.h"
#include "GuiManager.h"

GuiCheckBox::GuiCheckBox(uint32 id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::CHECKBOX, id)
{
	this->bounds = bounds;
	this->text = text;
}

GuiCheckBox::~GuiCheckBox()
{
}

bool GuiCheckBox::Update(float dt)
{
	if (state != GuiControlState::DISABLED)
	{
		int mouseX, mouseY;
		app->input->GetMousePosition(mouseX, mouseY);
		mouseYFix = mouseY - 130;

		mouseX += -app->render->camera.x / app->win->GetScale();
		mouseY += -app->render->camera.y / app->win->GetScale();

		if ((mouseX > bounds.x) && (mouseX < (bounds.x + bounds.w)) && (mouseY > bounds.y) && (mouseY < (bounds.y + bounds.h)))
		{
			state = GuiControlState::FOCUSED;

			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT) state = GuiControlState::PRESSED;

			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP)
			{
				checked = !checked;
				NotifyObserver();
			}
		}
		else
		{
			state = GuiControlState::NORMAL;
		}
	}
	if (app->scene->mute) checked = true;
	else if (!app->scene->mute) checked = false;

	return false;
}

bool GuiCheckBox::Draw(Render* render)
{
	render->DrawRectangle({ bounds.x - 2, bounds.y - 2,bounds.w + 4,bounds.h + 4 }, 20, 20, 20);

	app->font->BlitText(bounds.x - 250, bounds.y, app->guiManager->hudFont, text.GetString());

	switch (state)
	{
	case GuiControlState::DISABLED:
	{
		render->DrawRectangle(bounds, 0, 0, 0, 0);
	} break;

	case GuiControlState::NORMAL:
	{
		if (id == 1 && checked)	render->DrawRectangle(bounds, 0, 255, 0, 255);
		if (id == 1 && !checked)	render->DrawRectangle(bounds, 255, 0, 0, 255);
	} break;

	case GuiControlState::FOCUSED:
	{
		render->DrawRectangle(bounds, 255, 255, 255, 160);
	} break;

	case GuiControlState::PRESSED:
	{
		render->DrawRectangle(bounds, 255, 255, 255, 255);
	} break;

	case GuiControlState::SELECTED:
	{
		render->DrawRectangle(bounds, 0, 255, 0, 255);
	} break;

	default:
		break;
	}

	return false;
}
