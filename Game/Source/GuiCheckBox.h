#ifndef __GUICHECKBOX_H__
#define __GUICHECKBOX_H__

#include "GuiControl.h"

#include "Point.h"
#include "SString.h"

class GuiCheckBox : public GuiControl
{
public:

	GuiCheckBox(uint32 id, SDL_Rect bounds, const char* text);
	virtual ~GuiCheckBox();

	bool Update(float dt);
	bool Draw(Render* render);


	int mouseYFix;

private:

	// GuiCheckBox specific properties
	// Maybe some animation properties for state change?
	bool checked;
	bool soundDone = false;
};

#endif // __GUICHECKBOX_H__
