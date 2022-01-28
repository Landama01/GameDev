#ifndef __ARROW_H__
#define __ARROW_H__


#include "PugiXml\src\pugixml.hpp"
#include "Animation.h"
#include "Point.h"

struct SDL_Texture;

class Arrow : public Module
{
public:

	Arrow();

	virtual ~Arrow();

	bool Awake(pugi::xml_node&);

	bool Start();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

	bool ShootingR = false;
	bool ShootingL = false;
	bool hit = false;

	int ArrowTimer;

	fPoint velocity;
	fPoint position;
	fPoint tmpPos;

private:

	Animation* currentAnimation = nullptr;
	
	Animation ArrowRight;
	Animation ArrowLeft;

	SDL_Texture* ArrowTex;
	
};
#endif // _COINS_H_

