#include "unluckyCat.h"

Engine::unluckyCat::unluckyCat(Setting* setting) :Engine::Game(setting)
{
	// Set to mainmenu
	screenState = Engine::ScreenState::MAIN_MENU;
	//state game over
	screenStateOver = Engine::ScreenState::GAME_OVER;
}

Engine::unluckyCat::~unluckyCat()
{
}

void Engine::unluckyCat::Init()
{
	InitMainMenu();
	InitInGame();
	InitGameOver();

}

void Engine::unluckyCat::Update()
{

	if (Engine::ScreenState::MAIN_MENU == screenState) {
		UpdateMainMenu();
	}
	else if (Engine::ScreenState::IN_GAME == screenState) {
		if (gameQuit) {
			resetInGameState();
			InitInGame();
		}
		else {
			UpdateInGame();
		}
	}
	else if (Engine::ScreenState::GAME_OVER == screenStateOver) {
		UpdateGameOver();
		gameQuit = true;
	}
}

void Engine::unluckyCat::Render()
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

void Engine::unluckyCat::InitMainMenu()
{
	// Create a Texture
	Texture* texture = new Texture("Assets/buttons_prev_ui.png");

	// Create Sprites
	Sprite* playSprite = new Sprite(texture, defaultSpriteShader, defaultQuad);
	playSprite->SetNumXFrames(5);
	playSprite->SetNumYFrames(2);
	playSprite->AddAnimation("normal", 6, 6);
	playSprite->AddAnimation("hover", 6, 8);
	playSprite->AddAnimation("press", 8, 10);
	playSprite->SetAnimationDuration(350);

	Sprite* exitSprite = new Sprite(texture, defaultSpriteShader, defaultQuad);
	exitSprite->SetNumXFrames(5);
	exitSprite->SetNumYFrames(2);
	exitSprite->AddAnimation("normal", 1, 1);
	exitSprite->AddAnimation("hover", 1, 3);
	exitSprite->AddAnimation("press", 3, 5);
	exitSprite->SetAnimationDuration(350);

	//Create Buttons
	Button* playButton = new Button(playSprite, "play");
	playButton->SetPosition((setting->screenWidth / 6) - (playSprite->GetScaleWidth() / 2),
		180);
	buttons.push_back(playButton);
	Button* exitButton = new Button(exitSprite, "exit");
	exitButton->SetPosition((setting->screenWidth / 6) - (exitSprite->GetScaleWidth() / 2),
		60);
	buttons.push_back(exitButton);

	// Set play button into active button
	currentButtonIndex = 0;
	buttons[currentButtonIndex]->SetButtonState(Engine::ButtonState::HOVER);

	// Playing music
	music = new Music("Assets/xDeviruchi-And-The-Journey-Begins-.ogg");
	music->SetVolume(40);
	music->Play(true);

	sound = new Sound("jump.wav");
	sound->SetVolume(80);

	// Create Text
	text1 = new Text("Assets/CuteMeow-51Pra.otf", 100, defaultTextShader);
	text1->SetText("Unlucky Cat");
	text1->SetPosition(100, setting->screenHeight - 200);
	text1->SetColor(105, 42, 0);

	controltext1 = new Text("Assets/CuteMeow-51Pra.otf", 36, defaultTextShader);
	controltext1->SetText("Controls");
	controltext1->SetPosition(setting->screenWidth - 240, setting->screenHeight - 300);
	controltext1->SetColor(105, 42, 0);

	controltext2 = new Text("Assets/charybdis.ttf", 32, defaultTextShader);
	controltext2->SetText("Move   Left Or Right");
	controltext2->SetPosition(setting->screenWidth - 300, setting->screenHeight - 360);
	controltext2->SetColor(105, 42, 0);

	controltext3 = new Text("Assets/charybdis.ttf", 32, defaultTextShader);
	controltext3->SetText("Sprint    Shift");
	controltext3->SetPosition(setting->screenWidth - 300, setting->screenHeight - 400);
	controltext3->SetColor(105, 42, 0);

	controltext4 = new Text("Assets/charybdis.ttf", 32, defaultTextShader);
	controltext4->SetText("Jump        z");
	controltext4->SetPosition(setting->screenWidth - 300, setting->screenHeight - 440);
	controltext4->SetColor(105, 42, 0);

	// Add input mappings
	inputManager->AddInputMapping("next", SDLK_DOWN);
	inputManager->AddInputMapping("prev", SDLK_UP);
	inputManager->AddInputMapping("press", SDLK_RETURN);

}

void Engine::unluckyCat::InitInGame()
{
	gameQuit = false;
	// Spawn setting
	maxSpawnTime = 900;
	numObjectPerSpawn = 1;
	numObjectsInPool = 10; //number of each object

	// Load a texture
	Texture* texture = new Texture("Assets/Bongo.png");
	Texture* bottle = new Texture("Assets/Bottle.png");
	/*Texture* fish = new Texture("Assets/le fishe.png");*/

	//Falling objects
	for (int i = 0; i < numObjectsInPool; i++) {
		GameObject* o = new GameObject(CreateSprite(texture));
		/*GameObject* o2 = new GameObject(CreateSprite(bottle));*/
		Sprite* bottleSprite = new Sprite(bottle, defaultSpriteShader, defaultQuad);
		bottleSprite->SetNumXFrames(1);
		bottleSprite->SetNumYFrames(1);
		bottleSprite->SetScale(3.5);
		bottleSprite->SetBoundingBoxSize(bottleSprite->GetScaleWidth() - (5 * bottleSprite->GetScale()),
			bottleSprite->GetScaleHeight());

		GameObject* o2 = new GameObject(bottleSprite);

		objects.push_back(o);
		objects.push_back(o2);
	}
	/*collectible = new Sprite(fish,defaultSpriteShader,defaultQuad);
	collectible->SetPosition(-50, 0);
	collectible->SetScale(2);*/

	//Create background
	Texture* bgTexture = new Texture("Assets/Sunny Cloud Background.png");
	backgroundSprite = new Sprite(bgTexture, defaultSpriteShader, defaultQuad);
	backgroundSprite->SetSize(setting->screenWidth, setting->screenHeight);

	//Create platform
	Texture* platformTexture = new Texture("Assets/grass-tiles.png");
	vec2 start = vec2(0, 0);
	for (int i = -1; i < 30; i++) {
		Sprite* platformSprite = new Sprite(platformTexture, defaultSpriteShader, defaultQuad);
		platformSprite->SetPosition(start.x + i * 56, start.y - 2);
		platformSprite->SetSize(64, 64);
		platformSprite->SetBoundingBoxSize(platformSprite->GetScaleWidth(), platformSprite->GetScaleHeight() - 5);
		platforms.push_back(platformSprite);
	}

	//load player sprite
	Texture* playertexture = new Texture("Assets/CatRecolors3.png");
	Sprite* sprite = new Sprite(playertexture, defaultSpriteShader, defaultQuad);
	player = new Cat_Player(sprite);
	player->setPosition(20, 60);
	player->setJump(false);

	//load debug object(s)
	/*int bottleCount = 3;
	for (int i = 0; i < bottleCount; i++) {
		debugObject = new Sprite(texture, defaultSpriteShader, defaultQuad);
		debugObject->SetNumXFrames(1);
		debugObject->SetNumYFrames(1);
		debugObject->SetScale(3);
		debugObject->SetBoundingBoxSize(debugObject->GetScaleWidth() - (5 * debugObject->GetScale()),
			debugObject->GetScaleHeight());
		debugObject->SetPosition(185 + (i * 120), 64);
		debugobjects.push_back(debugObject);
	}*/

	// Add input mappings
	inputManager->AddInputMapping("mainmenu", SDLK_ESCAPE);
	inputManager->AddInputMapping("walk-left", SDLK_LEFT);
	inputManager->AddInputMapping("walk-left", SDL_BUTTON_LEFT);
	inputManager->AddInputMapping("walk-left", SDL_CONTROLLER_BUTTON_DPAD_LEFT);
	inputManager->AddInputMapping("walk-left", SDL_CONTROLLER_AXIS_LEFTX);
	inputManager->AddInputMapping("walk-right", SDLK_RIGHT);
	inputManager->AddInputMapping("walk-right", SDL_BUTTON_RIGHT);
	inputManager->AddInputMapping("walk-right", SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
	inputManager->AddInputMapping("jump", SDLK_z);
	inputManager->AddInputMapping("jump", SDL_CONTROLLER_BUTTON_DPAD_UP);
	inputManager->AddInputMapping("run", SDLK_LSHIFT);

	//Score
	text3 = new Text("Assets/charybdis.ttf", 36, defaultTextShader);
	text3->SetText("Time: " + to_string(totalTime) + "Score: " + to_string(score) + " HP: " + to_string(player->getHP()));
	text3->SetPosition(60, setting->screenHeight - 50);
	text3->SetColor(105, 42, 0);

	//Highscore
	text2 = new Text("Assets/charybdis.ttf", 36, defaultTextShader);
	text2->SetText("Highscore " + to_string(highscore));
	text2->SetPosition(60, setting->screenHeight - 100);
	text2->SetColor(105, 42, 0);

}

void Engine::unluckyCat::InitGameOver()
{

	//Create background
	Texture* bgTexture = new Texture("Assets/Sunny Cloud Background.png");
	backgroundSprite = new Sprite(bgTexture, defaultSpriteShader, defaultQuad);
	backgroundSprite->SetSize(setting->screenWidth, setting->screenHeight);

	//Create platform
	Texture* platformTexture = new Texture("Assets/grass-tiles.png");
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
	inputManager->AddInputMapping("retry", SDLK_r);
	inputManager->AddInputMapping("next", SDLK_RIGHT);
	inputManager->AddInputMapping("prev", SDLK_LEFT);
	inputManager->AddInputMapping("press", SDLK_RETURN);


	//Text
	text4 = new Text("Assets/CuteMeow-51Pra.otf", 80, defaultTextShader);
	text5 = new Text("Assets/charybdis.ttf", 40, defaultTextShader);
	text6 = new Text("Assets/charybdis.ttf", 40, defaultTextShader);
	
	restarttext = new Text("Assets/charybdis.ttf", 32, defaultTextShader);
	restarttext->SetText("Press 'r' to retry");
	restarttext->SetPosition(setting->screenWidth / 4, setting->screenHeight - 380);
	restarttext->SetColor(105, 42, 0);
	hometext = new Text("Assets/charybdis.ttf", 32, defaultTextShader);
	hometext->SetText("Press ESC to main menu");
	hometext->SetPosition(setting->screenWidth / 4, setting->screenHeight - 420);
	hometext->SetColor(105, 42, 0);
}

void Engine::unluckyCat::RenderMainMenu()
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
	controltext1->Draw();
	controltext2->Draw();
	controltext3->Draw();
	controltext4->Draw();
}

void Engine::unluckyCat::RenderInGame()
{
	// Render background
	backgroundSprite->Draw();
	// Render all objects
	for (GameObject* o : objects) {
		o->Draw();
	}

	// Render Platform
	for (Sprite* s : platforms) {
		s->Draw();
	}

	//render player
	player->draw();

	//render collectible
	//collectible->Draw();

	// Render Score
	text3->Draw();
	// Render Highscore
	text2->Draw();

}

void Engine::unluckyCat::RenderGameOver() {
	backgroundSprite->Draw();

	for (Sprite* s : platforms) {
		s->Draw();
	}

	text4->Draw();
	text5->Draw();
	text6->Draw();
	restarttext->Draw();
	hometext->Draw();
}

void Engine::unluckyCat::UpdateMainMenu()
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

void Engine::unluckyCat::UpdateInGame()
{
	// Back to main menu

	if (inputManager->IsKeyReleased("mainmenu")) {
		screenState = Engine::ScreenState::MAIN_MENU;
		gameQuit = true;
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
	totalTime += time;
	score = totalTime / 60;

	//movement character
	player->update(time);
	float x = player->getPosition().x;
	float y = player->getPosition().y;
	float velocity = 0.4f;
	/*cout << "x" << x << endl;
	cout << "y" << y << endl;*/

	//reset player ke idle bila tidak ditekan tombol apapun
	player->setState(Engine::Cat_PlayerState::IDLE);
	player->isInvul(totalTime); //cek invul agar animasi benar

	//Score update
	text3 = new Text("Assets/charybdis.ttf", 36, defaultTextShader);
	text3->SetText("Time " + to_string(totalTime / 1000) +
		" Score " + to_string(score) + " HP " + to_string(player->getHP()));
	text3->SetPosition(60, setting->screenHeight - 50);
	text3->SetColor(105, 42, 0);

	if (inputManager->IsKeyReleased("run")) {
		isRunning = 0;
		player->run(isRunning);
		//cout << "relased" << endl;
	}
	if (inputManager->IsKeyPressed("run")) {
		isRunning = 1;
		player->run(isRunning);
	}

	if (inputManager->IsKeyPressed("walk-right")) {
		if (isRunning) {
			x += 1.5 * velocity * GetGameTime();
		}
		else {
			x += velocity * GetGameTime();
		}
		player->flip(false);
		player->setState(Engine::Cat_PlayerState::WALK);
	}

	if (inputManager->IsKeyPressed("walk-left")) {
		if (isRunning) {
			x -= 1.5 * velocity * GetGameTime();
		}
		else {
			x -= velocity * GetGameTime();
		}
		//x -= velocity * GetGameTime();
		player->flip(true);
		player->setState(Engine::Cat_PlayerState::WALK);
	}

	//limiting player movement to window
	if (player->getPosition().x < -50) {
		x = -49;
	}
	else if (player->getPosition().x > setting->screenWidth - 20) {
		x = setting->screenWidth - 20;
	}


	if (inputManager->IsKeyPressed("jump") && !player->isJump()) {
		float rate = (GetGameTime() / 16.7f);
		gravity = 0.098f * rate;
		yvelocity = 1.2f;
		player->setJump(true);
		sound->Play(false);
	}

	if (y > 0) {
		yvelocity -= gravity;
	}
	else if (y < 0) {
		player->setJump(false);
		yvelocity = 0;
		y = 0;
	}
	y += yvelocity * GetGameTime();

	player->setPosition(x, y);
	//if (totalTime/1000 % 5 == 0) {
	//	float x = setting->screenWidth/2;
	//	float y = 60;
	//	collectible->SetPosition(x, y);
	//	collectTimeout = totalTime/1000;
	//} 
	//if (totalTime/1000 - collectTimeout >= 3) {
	//	collectible->SetPosition(-200, 0);
	//	collectTimeout = 0;
	//} else if (collectible->GetBoundingBox()->CollideWith(player->getBoundingBox())) {
	//	collectible->SetPosition(-200, 0);
	//	collectTimeout = 0;
	//	collected += 1;
	//}
	//collectible->Update(GetGameTime());

	//platform collision
	for each (Sprite * p in platforms)
	{
		if (p->GetBoundingBox()->CollideWith(player->getBoundingBox())) {
			y = 60;
			player->setPosition(x, 60);

			player->setJump(false);
			yvelocity = 0;
			/*cout << "collide y" << endl;
			cout << "y=" << y << endl;*/
			break;
		}
	}


	player->update(GetGameTime());

	//Cek collision
	for each (GameObject * o in objects)
	{
		if (o->collidesWith(player)) {
			if (!player->isInvul(totalTime)) {
				player->setLastHit(totalTime);
				player->takeDmg(1);
				o->setState(Engine::GameObjectState::DIE);
				if (player->getHP() == 0) {
					//game over state
					screenState = Engine::ScreenState::GAME_OVER;
				}
			}
			else if (player->isInvul(totalTime)) {
			}
			break;
		}
	}

	//object for debugging
	//for each (Sprite * obj in debugobjects)
	//{
	//	if (obj->GetBoundingBox()->CollideWith(player->getBoundingBox())) {
	//		if (!player->isInvul(totalTime)) {
	//			/*cout << "Player is damaged" << endl;*/
	//			player->setLastHit(totalTime);
	//			player->takeDmg(1);
	//			if (player->getHP() == 0) {
	//				//game over state
	//				screenState = Engine::ScreenState::GAME_OVER;
	//			}
	//		}
	//		else if (player->isInvul(totalTime)) {
	//			/*cout << "Player is invulnerable" << endl;*/
	//		}
	//		break;
	//	}
	//}

}

void Engine::unluckyCat::UpdateGameOver() {

	// Back to main menu
	if (score > highscore) {
		highscore = score;
	}
	if (inputManager->IsKeyReleased("mainmenu")) {
		screenState = Engine::ScreenState::MAIN_MENU;
	}
	else if (inputManager->IsKeyReleased("retry")) {
		screenState = Engine::ScreenState::IN_GAME;
	}

	text4->SetText("Game Over");
	text4->SetPosition(setting->screenWidth / 4, setting->screenHeight - 200);
	text4->SetColor(105, 42, 0);

	text5->SetText("Score: " + to_string(score));
	text5->SetPosition(setting->screenWidth / 4, setting->screenHeight - 260);
	text5->SetColor(105, 42, 0);

	text6->SetText("Highscore: " + to_string(highscore));
	text6->SetPosition(setting->screenWidth / 4, setting->screenHeight - 320);
	text6->SetColor(105, 42, 0);
}

Engine::Sprite* Engine::unluckyCat::CreateSprite(Texture* texture)
{
	Sprite* sprite = new Sprite(texture,
		defaultSpriteShader,
		defaultQuad);
	sprite->SetScale(3.5);
	sprite->SetBoundingBoxSize(sprite->GetScaleWidth() - (6 * sprite->GetScale())
		, sprite->GetScaleHeight() - (2 * sprite->GetScale()));

	return sprite;
}

void Engine::unluckyCat::SpawnObjects()
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

void Engine::unluckyCat::resetInGameState() {
	score = 0;
	totalTime = 0;
	player = NULL;
	objects.clear();
	platforms.clear();
	isRunning = false;

}


int main(int argc, char** argv) {
	Engine::Setting* setting = new Engine::Setting();
	setting->windowTitle = "Unlucky Cat";
	setting->screenWidth = 720;
	setting->screenHeight = 576;
	setting->windowFlag = Engine::WindowFlag::WINDOWED;
	//setting->windowFlag = Engine::WindowFlag::FULLSCREEN;
	setting->vsync = false;
	setting->targetFrameRate = 60;
	Engine::Game* game = new Engine::unluckyCat(setting);
	game->Run();
	delete setting;
	delete game;

	return 0;
}