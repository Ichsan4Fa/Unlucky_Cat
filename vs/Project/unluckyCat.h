#ifndef UNLUCKYCAT_H
#define UNLUCKYCAT_H

#include "Game.h"
#include "Setting.h"
#include "Texture.h"
#include "Sprite.h"
#include "Button.h"
#include "Text.h"
#include "Music.h"
#include "Sound.h"
#include "GameObject.h"
#include <vector>
#include "Cat_Player.h"

namespace Engine {
	enum class ScreenState {
		MAIN_MENU,
		IN_GAME,
		GAME_OVER
	};
	class unluckyCat :public Engine::Game
	{
	public:
		unluckyCat(Setting* setting);
		~unluckyCat();
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
		Music* music = NULL;
		Sound* sound = NULL;
		Text* text1;
		Text* controltext1;
		Text* controltext2;
		Text* controltext3;
		Text* controltext4;
		Text* text2;
		Text* text3;
		Text* text4;
		Text* text5;
		Text* text6;
		Text* restarttext;
		Text* hometext;
		Text* pausetext;
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
		void resetInGameState();

		Engine::Sprite* CreateSprite(Texture* texture);
		void SpawnObjects();
		vector<GameObject*> objects;
		float spawnDuration = 0, maxSpawnTime = 0, numObjectsInPool = 0, numObjectPerSpawn = 0;
		float yvelocity, gravity;

		/*Sprite* collectible;
		int collectTimeout = 0;*/
		//int collected = 0;
		bool gameQuit = false;
	};
}

#endif