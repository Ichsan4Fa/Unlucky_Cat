#include "Lesson07.h"

Engine::Lesson07::Lesson07(Setting* setting) :Engine::Game(setting)
{
	// Set to mainmenu
	screenState = Engine::ScreenState::MAIN_MENU;
	//state game over
	screenStateOver = Engine::ScreenState::GAME_OVER;
}

Engine::Lesson07::~Lesson07()
{
}

void Engine::Lesson07::Init()
{
	InitMainMenu();
	InitInGame();
	InitGameOver();

}

void Engine::Lesson07::Update()
{

	if (Engine::ScreenState::MAIN_MENU == screenState) {
		UpdateMainMenu();
	}
	else if (Engine::ScreenState::IN_GAME == screenState) {
		UpdateInGame();
	}
	else if (Engine::ScreenState::GAME_OVER == screenStateOver) {
		UpdateGameOver();
	}
}

void Engine::Lesson07::Render()
{
	if (Engine::ScreenState::MAIN_MENU == screenState) {
		RenderMainMenu();
	}
	else if (Engine::ScreenState::IN_GAME == screenState) {
		RenderInGame();
	}
	else if (Engine::ScreenState::GAME_OVER == screenStateOver) {
		RenderGameOver();
	}
}

void Engine::Lesson07::InitMainMenu()
{
	// Create a Texture
	Texture* texturePlay = new Texture("play03.png");
	Texture* textureHome = new Texture("home03.png");

	// Create Sprites
	Sprite* playSprite = new Sprite(texturePlay, defaultSpriteShader, defaultQuad);
	playSprite->SetNumXFrames(1);
	playSprite->SetNumYFrames(1);
	/*playSprite->AddAnimation("normal", 5, 5);
	playSprite->AddAnimation("hover", 3, 4);
	playSprite->AddAnimation("press", 3, 4);
	playSprite->SetAnimationDuration(400);*/

	Sprite* exitSprite = new Sprite(textureHome, defaultSpriteShader, defaultQuad);
	exitSprite->SetNumXFrames(1);
	exitSprite->SetNumYFrames(1);
	/*exitSprite->AddAnimation("normal", 2, 2);
	exitSprite->AddAnimation("hover", 0, 1);
	exitSprite->AddAnimation("press", 0, 1);
	exitSprite->SetAnimationDuration(400);*/

	//Create Buttons
	Button* playButton = new Button(playSprite, "play");
	playButton->SetPosition((setting->screenWidth / 4) - (playSprite->GetScaleWidth() / 2),
		180);
	buttons.push_back(playButton);
	Button* exitButton = new Button(exitSprite, "exit");
	exitButton->SetPosition((setting->screenWidth / 4) - (exitSprite->GetScaleWidth() / 2),
		80);
	buttons.push_back(exitButton);
	
	// Set play button into active button
	currentButtonIndex = 0;
	buttons[currentButtonIndex]->SetButtonState(Engine::ButtonState::HOVER);

	// Create Text
	text1 = new Text("CuteMeow-51Pra.otf", 100, defaultTextShader);
	text1->SetText("Project Cat");
	text1->SetPosition(100, setting->screenHeight - 200);
	text1->SetColor(105, 42, 0);

	// Add input mappings
	inputManager->AddInputMapping("next", SDLK_DOWN);
	inputManager->AddInputMapping("prev", SDLK_UP);
	inputManager->AddInputMapping("press", SDLK_RETURN);

}

void Engine::Lesson07::InitInGame()
{
	// Spawn setting
	maxSpawnTime = 900;
	numObjectPerSpawn = 1;
	numObjectsInPool = 20;

	// Load a texture
	Texture* texture = new Texture("Bongo.png");
	
	//Bongo
	for (int i = 0; i < numObjectsInPool; i++) {
		GameObject* o = new GameObject(CreateSprite(texture));
		objects.push_back(o);
	}

	//Create background
	Texture* bgTexture = new Texture("Sunny Cloud Background.png");
	backgroundSprite = new Sprite(bgTexture, defaultSpriteShader, defaultQuad);
	backgroundSprite->SetSize(setting->screenWidth, setting->screenHeight);

	//Create platform
	Texture* platformTexture = new Texture("grass-tiles.png");
	vec2 start = vec2(0, 0);
	for (int i = -1; i < 30; i++) {
		Sprite* platformSprite = new Sprite(platformTexture, defaultSpriteShader, defaultQuad);
		platformSprite->SetPosition(start.x + i * 56, start.y - 2);
		platformSprite->SetSize(64, 64);
		platformSprite->SetBoundingBoxSize(platformSprite->GetScaleWidth(), platformSprite->GetScaleHeight());
		platforms.push_back(platformSprite);
	}

	//load player sprite
	Texture* playertexture = new Texture("Assets/CatRecolors3.png");
	Sprite* sprite = new Sprite(playertexture, defaultSpriteShader, defaultQuad);
	player = new Cat_Player(sprite);
	player->setPosition(0, 64);

	//load object(s)
	Texture* bottle = new Texture("Assets/Bottle.png");
	int bottleCount = 3;

	for (int i = 0; i < bottleCount; i++) {
		debugObject = new Sprite(bottle, defaultSpriteShader, defaultQuad);
		debugObject->SetNumXFrames(1);
		debugObject->SetNumYFrames(1);
		debugObject->SetScale(3);
		debugObject->SetBoundingBoxSize(debugObject->GetScaleWidth() - (5 * debugObject->GetScale()),
			debugObject->GetScaleHeight());
		debugObject->SetPosition(185 + (i * 75), 64);

		debugobjects.push_back(debugObject);
	}
	
	// Add input mappings
	inputManager->AddInputMapping("mainmenu", SDLK_ESCAPE);
	inputManager->AddInputMapping("gameover", SDLK_t);
	inputManager->AddInputMapping("walk-left", SDLK_LEFT);
	inputManager->AddInputMapping("walk-left", SDL_BUTTON_LEFT);
	inputManager->AddInputMapping("walk-left", SDL_CONTROLLER_BUTTON_DPAD_LEFT);
	inputManager->AddInputMapping("walk-left", SDL_CONTROLLER_AXIS_LEFTX);
	inputManager->AddInputMapping("walk-right", SDLK_RIGHT);
	inputManager->AddInputMapping("walk-right", SDL_BUTTON_RIGHT);
	inputManager->AddInputMapping("walk-right", SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
	inputManager->AddInputMapping("jump", SDLK_UP);
	inputManager->AddInputMapping("jump", SDL_CONTROLLER_BUTTON_DPAD_UP);
	inputManager->AddInputMapping("run", SDLK_LSHIFT);

	//Score
	text3 = new Text("8-bit Arcade In.ttf", 36, defaultTextShader);
	text3->SetText("Time: "+to_string(totalTime) + "Score: " + to_string(score) + " HP: " + to_string(player->getHP()));
	text3->SetPosition(60, setting->screenHeight - 50);
	text3->SetColor(235, 229, 52);

	//Highscore
	text2 = new Text("8-bit Arcade In.ttf", 36, defaultTextShader);
	text2->SetText("Highscore "+to_string(highscore));
	text2->SetPosition(60, setting->screenHeight - 100);
	text2->SetColor(235, 229, 52);
	
}

void Engine::Lesson07::InitGameOver()
{
	//Create background
	Texture* bgTexture = new Texture("Sunny Cloud Background.png");
	backgroundSprite = new Sprite(bgTexture, defaultSpriteShader, defaultQuad);
	backgroundSprite->SetSize(setting->screenWidth, setting->screenHeight);

	//Create platform
	Texture* platformTexture = new Texture("grass-tiles.png");
	vec2 start = vec2(0, 0);
	for (int i = -1; i < 30; i++) {
		Sprite* platformSprite = new Sprite(platformTexture, defaultSpriteShader, defaultQuad);
		platformSprite->SetPosition(start.x + i * 56, start.y - 2);
		platformSprite->SetSize(64, 64);
		platformSprite->SetBoundingBoxSize(platformSprite->GetScaleWidth(), platformSprite->GetScaleHeight());
		platforms.push_back(platformSprite);
	}

	// Add input mappings
	inputManager->AddInputMapping("mainmenu", SDLK_ESCAPE);

	//Score
	text3 = new Text("8-bit Arcade In.ttf", 36, defaultTextShader);
	text3->SetText("Time: " + to_string(totalTime) + "Score: " + to_string(score) + " HP: " + to_string(player->getHP()));
	text3->SetPosition(60, setting->screenHeight - 50);
	text3->SetColor(235, 229, 52);

	//Highscore
	text2 = new Text("8-bit Arcade In.ttf", 36, defaultTextShader);
	text2->SetText("Highscore " + to_string(highscore));
	text2->SetPosition(60, setting->screenHeight - 100);
	text2->SetColor(235, 229, 52);

}

void Engine::Lesson07::RenderMainMenu()
{
	// Render background
	backgroundSprite->Draw();
	// Render all buttons
	for (Button* b : buttons) {
		b->Draw();
	}
	// Render Platform
	for (Sprite* s : platforms) {
		s->Draw();
	}
	// Render title 
	text1->Draw();

}

void Engine::Lesson07::RenderInGame()
{
	// Render background
	backgroundSprite->Draw();
	// Render all objects
	for (GameObject* o : objects) {
		o->Draw();
	}
	for each (Sprite * obj in debugobjects)
	{
		obj->Draw();
	}
	// Render Platform
	for (Sprite* s : platforms) {
		s->Draw();
	}
	player->draw();
	// Render Score
	text3->Draw();
	// Render Highscore
	text2->Draw();

}

void Engine::Lesson07::RenderGameOver() {
	backgroundSprite->Draw();

	for (Sprite* s : platforms) {
		s->Draw();
	}
}

void Engine::Lesson07::UpdateMainMenu()
{
	if (inputManager->IsKeyReleased("next")) {
		// Set previous button to normal state
		buttons[currentButtonIndex]->SetButtonState(Engine::ButtonState::NORMAL);
		// Next Button
		currentButtonIndex = currentButtonIndex < buttons.size() - 1 ? currentButtonIndex + 1 : currentButtonIndex;
		// Set current button to hover state
		buttons[currentButtonIndex]->SetButtonState(Engine::ButtonState::HOVER);
	}

	if (inputManager->IsKeyReleased("prev")) {
		// Set previous button to normal state
		buttons[currentButtonIndex]->SetButtonState(Engine::ButtonState::NORMAL);
		// Prev Button
		currentButtonIndex = currentButtonIndex > 0 ? currentButtonIndex - 1 : currentButtonIndex;
		// Set current button to hover state
		buttons[currentButtonIndex]->SetButtonState(Engine::ButtonState::HOVER);
	}


	if (inputManager->IsKeyReleased("press")) {
		// Set current button to press state
		Button* b = buttons[currentButtonIndex];
		b->SetButtonState(Engine::ButtonState::PRESS);
		// If play button then go to InGame, exit button then exit
		if ("play" == b->GetButtonName()) {
			screenState = Engine::ScreenState::IN_GAME;
		}
		else if ("exit" == b->GetButtonName()) {
			state = Engine::State::EXIT;
		}
	}

	// Update All buttons
	for (Button* b : buttons) {
		b->Update(GetGameTime());
	}
}

void Engine::Lesson07::UpdateInGame()
{
	// Back to main menu
	if (inputManager->IsKeyReleased("mainmenu")) {
		screenState = Engine::ScreenState::MAIN_MENU;
	}
	// Time to spawn objects
	if (spawnDuration >= maxSpawnTime) {
		SpawnObjects();
		spawnDuration = 0;
	}
	// Update all objects
	for (GameObject* o : objects) {
		o->Update(GetGameTime());
	}
	// Count spawn duration
	spawnDuration += GetGameTime();

	//score&time
	float time = GetGameTime();
	score += time/60;
	totalTime += time;

	//movement character
	player->update(time);
	float x = player->getPosition().x;
	float y = player->getPosition().y;
	float velocity = 0.1f;

	//reset player ke idle bila tidak ditekan tombol apapun
	player->setState(Engine::Cat_PlayerState::IDLE);

	//Score update
	text3 = new Text("8-bit Arcade In.ttf", 36, defaultTextShader);
	text3->SetText("Time " + to_string(time) + " Score " + to_string(score) + " HP " + to_string(player->getHP()));
	text3->SetPosition(60, setting->screenHeight - 50);
	text3->SetColor(235, 229, 52);

	if (inputManager->IsKeyReleased("run")) {
		isRunning = 0;
		player->run(isRunning);
		//cout << "relased" << endl;
	}
	if (inputManager->IsKeyPressed("run")) {
		isRunning = 1;
		player->run(isRunning);
	}

	if (inputManager->IsKeyPressed("jump")) {

	}

	if (inputManager->IsKeyPressed("walk-right")) {
		if (isRunning) {
			x += 5 * velocity * GetGameTime();
		}
		else {
			x += velocity * GetGameTime();
		}
		player->flip(false);
		player->setState(Engine::Cat_PlayerState::WALK);
	}

	if (inputManager->IsKeyPressed("walk-left")) {
		if (isRunning) {
			x -= 5 * velocity * GetGameTime();
		}
		else {
			x -= velocity * GetGameTime();
		}
		//x -= velocity * GetGameTime();
		player->flip(true);
		player->setState(Engine::Cat_PlayerState::WALK);
	}

	player->setPosition(x, y);
	player->update(GetGameTime());


	//Cek collision
	for each (Sprite * obj in debugobjects)
	{
		if (obj->GetBoundingBox()->CollideWith(player->getBoundingBox())) {
			if (!player->isInvul(totalTime)) {
				/*cout << "Player is damaged" << endl;*/
				player->setLastHit(totalTime);
				player->takeDmg(1);
				if (player->getHP() == 0) {
					//game over state
					screenState = Engine::ScreenState::GAME_OVER;
				}
			}
			else if (player->isInvul(totalTime)) {
				/*cout << "Player is invulnerable" << endl;*/
			}
		}
	}
	// Game Over
	
}

void Engine::Lesson07::UpdateGameOver() {
	// Back to main menu
	if (inputManager->IsKeyReleased("mainmenu")) {
		screenState = Engine::ScreenState::MAIN_MENU;
	}

}

Engine::Sprite* Engine::Lesson07::CreateSprite(Texture* texture)
{
	Sprite* sprite = new Sprite(texture,
		defaultSpriteShader,
		defaultQuad);
	sprite->SetScale(3.5);
	sprite->SetSize(15, 15);
	sprite->SetBoundingBoxSize(sprite->GetScaleWidth(),sprite->GetScaleHeight());
	/*sprite->SetNumXFrames(14);
	sprite->SetNumYFrames(4);
	sprite->AddAnimation("hit", 2, 4);
	sprite->AddAnimation("spikes", 5, 12);
	sprite->AddAnimation("idle-1", 14, 27);
	sprite->AddAnimation("idle-2", 28, 41);
	sprite->AddAnimation("spikes-out", 42, 49);
	sprite->PlayAnim("spikes");
	sprite->SetScale(1.5);
	sprite->SetAnimationDuration(100);*/

	return sprite;
}

void Engine::Lesson07::SpawnObjects()
{
	//Find Die object to reuse for spawning
	int spawnCount = 0;
	for (GameObject* o : objects) {
		if (spawnCount == numObjectPerSpawn) {
			break;
		}
		if (o->IsDie()) {
			// Set state to spawn
			o->SetSpawn();
			// Random spawn position
			int min = 0;
			int max = setting->screenWidth - o->GetWidth();
			float x = rand() % (max - min + 1) + min;
			float y = setting->screenHeight + o->GetHeight();
			o->SetPosition(x, y);
			spawnCount++;
		}
	}
}


//int main(int argc, char** argv) {
//	Engine::Setting* setting = new Engine::Setting();
//	setting->windowTitle = "Unlucky Cat";
//	setting->screenWidth = 720;
//	setting->screenHeight = 576;
//	setting->windowFlag = Engine::WindowFlag::WINDOWED;
//	setting->vsync = false;
//	setting->targetFrameRate = 144;
//	Engine::Game* game = new Engine::unluckyCat(setting);
//	game->Run();
//	delete setting;
//	delete game;
//
//	return 0;
//}



