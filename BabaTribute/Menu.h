#ifndef _MENU_INCLUDE
#define _MENU_INCLUDE


#include "ShaderProgram.h"
#include "TexturedQuad.h"
#include "Text.h"

#define CAMERA_WIDTH 480
#define CAMERA_HEIGHT 480

#define TEXT_SIZE 32

class Menu
{
public:
	Menu();
	~Menu();

	void init();
	void update(int deltaTime);
	void render();

private:
	void initShaders();

private:
	float currentTime;
	float margin = 200;

	Text text;
	TexturedQuad *pointer;
	Texture tex;

	enum {PLAY, HOWTO, ABOUT, EXIT};
	int state;

	ShaderProgram simpleProgram, texProgram;
	glm::mat4 projection;
	string options[4] = { "PLAY", "HOW TO PLAY", "CREDITS", "EXIT" };
};

#endif