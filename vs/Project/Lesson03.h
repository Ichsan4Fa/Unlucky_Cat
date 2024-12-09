#ifndef LESSON03_H
#define LESSON03_H

#include "Game.h"
#include "Setting.h"
#include "Texture.h"
#include "Sprite.h"

namespace Engine {
	class Lesson03 :public Engine::Game
	{
	public:
		Lesson03(Setting* setting);
		~Lesson03();
		virtual void Init();
		virtual void Update();
		virtual void Render();
	private:
		Engine::Texture* texture = NULL;
		Engine::Sprite* sprite = NULL;
		float speed = 0.01f;
		float x = 0;
		float y = 0;
		int mul = 1;
		float t = 0;
		bool flip = true;
	};
}

#endif

