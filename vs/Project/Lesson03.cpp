#include "Lesson03.h"

Engine::Lesson03::Lesson03(Setting* setting) :Engine::Game(setting)
{

}

Engine::Lesson03::~Lesson03()
{
	delete texture;
	delete sprite;
}

void Engine::Lesson03::Init()
{
	texture = new Texture("turtles.png");
	sprite = new Sprite(texture, defaultSpriteShader, defaultQuad);
	sprite->SetNumXFrames(14);
	sprite->SetNumYFrames(4);
	sprite->AddAnimation("hit", 2, 4);
	sprite->AddAnimation("spikes", 5, 12);
	sprite->AddAnimation("idle-1", 14, 27);
	sprite->AddAnimation("idle-2", 28, 41);
	sprite->AddAnimation("spikes-out", 42, 49);
	sprite->PlayAnim("spikes-out");
	sprite->SetScale(4);
	sprite->SetAnimationDuration(100);
}

int bezier(float p0, float p1, float p2, float p3, float t) {
	int result = ((1 - t) * (1 - t) * (1 - t) * p0) +
		(3 * (1 - t) * (1 - t) * t * p1) +
		(3 * (1 - t) * t * t * p2) +
		(t * t * t * p3);
	return result;
}

void Engine::Lesson03::Update()
{
	sprite->Update(GetGameTime());	
	/*if (y >= 450 || y <= 0) {
		mul *= -1;
		flip = !flip;
	}
	y+= mul * speed * GetGameTime();
	sprite->SetFlipHorizontal(flip);*/
	int tesBezierX = bezier(x, 100, 200, 300, t);
	int tesBezierY = bezier(y, 100, 200, 300, t);
	t += speed;
	if (t >= 1 || t <= 0) {
		speed *= -1;
	}
	sprite->SetPosition(tesBezierX, tesBezierY);
}

void Engine::Lesson03::Render()
{
	sprite->Draw();
}


//int main(int argc, char** argv) {
//	Engine::Setting* setting = new Engine::Setting();
//	setting->windowTitle = "Animation Demo";
//	setting->screenWidth = 800;
//	setting->screenHeight = 600;
//	setting->windowFlag = Engine::WindowFlag::WINDOWED;
//	setting->vsync = true;
//	setting->targetFrameRate = 0;
//	Engine::Game* game = new Engine::Lesson03(setting);
//	game->Run();
//	delete setting;
//	delete game;
//
//
//	return 0;
//}

