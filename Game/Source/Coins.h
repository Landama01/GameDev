#ifndef __COINS_H__
#define __COINS_H__


#include "PugiXml\src\pugixml.hpp"
#include "Animation.h"
#include "Point.h"

struct SDL_Texture;

class Coins : public Module
{
public:

	Coins();

	virtual ~Coins();

	bool Awake(pugi::xml_node&);

	bool Start();

	bool PreUpdate();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();



private:
	List<iPoint>listCoin;	
	iPoint* pointsCollision;

	Animation* coinAnimation = new Animation();
	
	uint coinFx;

	SDL_Texture* texCoin;
	
};
#endif // _COINS_H_

