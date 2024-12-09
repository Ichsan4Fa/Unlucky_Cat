#include "Lesson05.h"



Engine::Lesson05::Lesson05(Setting* setting) :Engine::Game(setting)
{

}

Engine::Lesson05::~Lesson05()
{
	delete texture;
	delete sprite;
}

void Engine::Lesson05::Init()
{
	texture = new Texture("turtles.png");
	sprite = new Sprite(texture, defaultSpriteShader, defaultQuad);
	sprite->SetNumXFrames(14);
	sprite->SetNumYFrames(4);
	sprite->AddAnimation("idle", 2, 12);
	sprite->AddAnimation("walk", 6, 11);
	sprite->PlayAnim("idle");
	sprite->SetScale(3);
	sprite->SetAnimationDuration(90);

	SetBackgroundColor(65, 180, 242);

	inputManager->AddInputMapping("walk-left", SDLK_LEFT);
	inputManager->AddInputMapping("walk-left", SDL_BUTTON_LEFT);
	inputManager->AddInputMapping("walk-left", SDL_CONTROLLER_BUTTON_DPAD_LEFT);
	inputManager->AddInputMapping("walk-left", SDL_CONTROLLER_AXIS_LEFTX);
	inputManager->AddInputMapping("walk-right", SDLK_RIGHT);
	inputManager->AddInputMapping("walk-right", SDL_BUTTON_RIGHT);
	inputManager->AddInputMapping("walk-right", SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
	inputManager->AddInputMapping("jump", SDLK_UP);
}

void Engine::Lesson05::Update()
{
	sprite->Update(GetGameTime());
	float x = sprite->GetPosition().x;
	float y = sprite->GetPosition().y;
	float velocity = 0.07f;
	bool jump = false;
	float gravity = 0;
	float yVelocity = 0;
	// s = v * t;
	sprite->PlayAnim("idle");

	if (inputManager->IsKeyPressed("walk-right")) {
		x += velocity * GetGameTime();
		sprite->SetFlipHorizontal(false);
		sprite->PlayAnim("walk");
	}

	if (inputManager->IsKeyPressed("walk-left")) {
		x -= velocity * GetGameTime();
		sprite->SetFlipHorizontal(true);
		sprite->PlayAnim("walk");
	}

	if (inputManager->IsKeyPressed("jump")&& !jump) {
		float ratio = (GetGameTime() / 16.7f);
		gravity = 0.18 * ratio;
		jump = true;
		yVelocity = 1.8f;
		y += gravity*yVelocity;

		if (y > 0) {
			yVelocity -= gravity;
			y -= yVelocity;
		}
		else if (y < 0) {
			jump = false;
			yVelocity = 0;
		}
	}

	sprite->SetPosition(x, y);
}

void Engine::Lesson05::Render()
{
	sprite->Draw();
}

/*
int main(int argc, char** argv) {
	Engine::Setting* setting = new Engine::Setting();
	setting->windowTitle = "Animation Demo";
	setting->screenWidth = 600;
	setting->screenHeight = 400;
	setting->windowFlag = Engine::WindowFlag::WINDOWED;
	setting->vsync = false;
	setting->targetFrameRate = 0;
	Engine::Game* game = new Engine::Lesson05(setting);
	game->Run();
	delete setting;
	delete game;

	return 0;
}*/

