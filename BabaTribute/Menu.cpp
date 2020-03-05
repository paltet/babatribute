#include "Menu.h"
#include "Game.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <GL/glut.h>

Menu::Menu()
{
	pointer = nullptr;
}

Menu::~Menu()
{
	if (pointer != nullptr) delete pointer;
}

void Menu::init() {
	currentTime = 0.f;
	state = PLAY;
	
	initShaders();
	tex.loadFromFile("images/baba.png", TEXTURE_PIXEL_FORMAT_RGBA);

	pointer = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(1.f / 32.f, 1.f / 66.f), &tex, &texProgram);
	//pointer = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	pointer->setNumberAnimations(1);

	pointer->setAnimationSpeed(0, 4);
	pointer->addKeyframe(0, glm::vec2(1.f / 32.f, 0.f));
	pointer->addKeyframe(0, glm::vec2(1.f / 32.f, 1.f / 66.f));
	pointer->addKeyframe(0, glm::vec2(1.f / 32.f, 2.f / 66.f));

	pointer->changeAnimation(0);
	pointer->setPosition(glm::vec2(CAMERA_WIDTH / 2, CAMERA_HEIGHT / 2));


	if (!text.init("fonts/INVASION2000.ttf"))
		//if(!text.init("fonts/OpenSans-Bold.ttf"))
		//if(!text.init("fonts/DroidSerif.ttf"))
		cout << "Could not load font!!!" << endl;

	projection = glm::ortho(0.f, float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT - 1), 0.f);
}

void Menu::update(int deltaTime) {
	currentTime += deltaTime;
	pointer->update(deltaTime);

	if (currentTime > margin) {
		if (Game::instance().getSpecialKey(GLUT_KEY_UP))
		{
			if (state != PLAY) state--;
			currentTime = 0;
		}
		else if (Game::instance().getSpecialKey(GLUT_KEY_DOWN))
		{
			if (state != EXIT) state++;
			currentTime = 0;
		}
	}
	if (Game::instance().getKey(13) && state == EXIT) {
		exit(0);
	}

	pointer->setPosition(glm::vec2(CAMERA_WIDTH / 2 - 130 - 48, CAMERA_HEIGHT / 3 + 50 * state - 25));
}

void Menu::render() {

	glm::mat4 modelview;

	texProgram.use();

	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);

	modelview = glm::mat4(1.f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	pointer->render();

	for (int i = 0; i < 4; i++) {
		text.render(options[i], glm::vec2(CAMERA_WIDTH / 2 - 130, CAMERA_HEIGHT/3 + 50*i), TEXT_SIZE, glm::vec4(1.f, 1.f, 1.f, 1.f));
	}
}

void Menu::initShaders()
{
		Shader vShader, fShader;

		vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
		if (!vShader.isCompiled())
		{
			cout << "Vertex Shader Error" << endl;
			cout << "" << vShader.log() << endl << endl;
		}
		fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
		if (!fShader.isCompiled())
		{
			cout << "Fragment Shader Error" << endl;
			cout << "" << fShader.log() << endl << endl;
		}
		texProgram.init();
		texProgram.addShader(vShader);
		texProgram.addShader(fShader);
		texProgram.link();
		if (!texProgram.isLinked())
		{
			cout << "Shader Linking Error" << endl;
			cout << "" << texProgram.log() << endl << endl;
		}
		texProgram.bindFragmentOutput("outColor");
		vShader.free();
		fShader.free();
}

