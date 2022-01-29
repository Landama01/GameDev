#include "GuiSlider.h"
#include "App.h"
#include "Audio.h"
#include "Render.h"
#include "Scene.h"
#include "Window.h"

GuiSlider::GuiSlider(uint32 id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::SLIDER, id)
{
	this->bounds = bounds;
	this->boundsSlider = this->bounds;
	this->text = text;
	this->minValue = boundsSlider.x - 48;
	this->maxValue = this->minValue + 290;
	ChangeValue();
}

GuiSlider::~GuiSlider()
{
}

bool GuiSlider::Update(float dt)
{
	this->minValue = bounds.x - (this->value);
	this->maxValue = bounds.x + ((280 - this->value));

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
		}
		else if (state == GuiControlState::PRESSED)
		{
			if ((app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT) && (mouseX > minValue) && (mouseX < maxValue))
			{
				bounds.x = mouseX - (bounds.w / 2) / app->win->GetScale();
				ChangeValue();
				NotifyObserver();
				if(id==2) app->audio->PlayFx(guiButtonFx, 0);
			}
			
			else state = GuiControlState::NORMAL;
		}
		else state = GuiControlState::NORMAL;
	}

	return false;
}

bool GuiSlider::Draw(Render* render)
{
	SDL_Rect rect = { boundsSlider.x - 50, boundsSlider.y - 1, 300, 30 };
	render->DrawRectangle({ rect.x - 2, rect.y - 2,rect.w + 4,rect.h + 4 }, 20, 20, 20);
	render->DrawRectangle(rect, 0, 220, 120, 255);


	switch (state)
	{

	case GuiControlState::DISABLED:
	{
		render->DrawRectangle(bounds, 0, 0, 0, 0);
	} break;

	case GuiControlState::NORMAL: 
	{
		render->DrawRectangle(bounds, 255, 0, 0, 255);
	} break;

	case GuiControlState::FOCUSED:
	{
		render->DrawRectangle(bounds, 255, 255, 255, 160);
	} break;

	case GuiControlState::PRESSED: 
	{
		render->DrawRectangle(bounds, 255, 255, 255, 255);
	} break;

	case GuiControlState::SELECTED:	render->DrawRectangle(bounds, 0, 255, 0, 255);
		break;

	default:
		break;
	}

	if (app->scene->MenuState)
	{
		render->DrawRectangle({ rect.x - 2, rect.y - 2,rect.w + 4,rect.h + 4 }, 225, 225, 0, 100);
		render->DrawRectangle(bounds, 0, 200, 255, 150);
	}

	return false;
}


void GuiSlider::ChangeValue()
{
	this->value = bounds.x - minValue;

	if (this->value > 280) this->value = 280;

	else if (this->value <= 5) this->value = 0;
}

int GuiSlider::ReturnValue() const
{
	return this->value;
}