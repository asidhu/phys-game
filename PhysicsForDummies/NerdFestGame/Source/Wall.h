#include "GameObjects.h"

class Wall :public GameObject{

	int m_tex;
public:
	Wall(body* b, int texid);

	void render(RenderList* lst);
};