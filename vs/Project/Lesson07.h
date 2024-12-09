#ifndef Lesson07_H
#define Lesson07_H

#include "Game.h"
#include "Setting.h"
#include "Texture.h"
#include "Sprite.h"
#include "Button.h"
#include "Text.h"
#include "GameObject.h"
#include <vector>
#include "Cat_Player.h"

namespace Engine {
	enum class ScreenState {
		MAIN_MENU,
		IN_GAME,
		GAME_OVER
	};
	class Lesson07 :public Engine::Game
	{
	public:
		Lesson07(Setting* setting);
		~Lesson07();
		virtual void Init();
		virtual void Update();
		virtual void Render();
	private:
		int currentButtonIndex = 0;
		int score = 0;
		int highscore = 0;
		int totalTime = 0;
		bool isRunning = false;
		vector<Sprite*> debugobjects;
		Cat_Player* player = NULL;
		Sprite* debugObject;
		Sprite* debugObject2;
		ScreenState screenState;
		ScreenState screenStateOver;
		vector<Button*> buttons;
		vector<Sprite*> platforms;
		Text* text1;
		Text* text2;
		Text* text3;
		Sprite* backgroundSprite = NULL;
		void InitMainMenu();
		void InitInGame();
		void InitGameOver();
		void RenderMainMenu();
		void RenderInGame();
		void RenderGameOver();
		void UpdateMainMenu();
		void UpdateInGame();
		void UpdateGameOver();
		
		Engine::Sprite* CreateSprite(Texture* texture);
		void SpawnObjects();
		vector<GameObject*> objects;
		float spawnDuration = 0, maxSpawnTime = 0, numObjectsInPool = 0, numObjectPerSpawn = 0;

	};
}

#endif

