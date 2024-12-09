#ifndef MainMenu_H
#define MainMenu_H

#include "Game.h"
#include "Setting.h"
#include "Texture.h"
#include "Sprite.h"
#include "Button.h"
#include "Text.h"
#include "GameObject.h"
#include <vector>

namespace Engine {
	enum class ScreenState {
		MAIN_MENU,
		IN_GAME
	};
	class MainMenu :public Engine::Game
	{
	public:
		MainMenu(Setting* setting);
		~MainMenu();
		virtual void Init();
		virtual void Update();
		virtual void Render();
	private:
		int currentButtonIndex = 0;
		ScreenState screenState;
		vector<Button*> buttons;
		Text* text;
		void InitMainMenu();
		void InitInGame();
		void RenderMainMenu();
		void RenderInGame();
		void UpdateMainMenu();
		void UpdateInGame();

		Engine::Sprite* CreateSprite(Texture* texture);
		void SpawnObjects();
		vector<GameObject*> objects;
		float spawnDuration = 0, maxSpawnTime = 0, numObjectsInPool = 0, numObjectPerSpawn = 0;

	};
}

#endif