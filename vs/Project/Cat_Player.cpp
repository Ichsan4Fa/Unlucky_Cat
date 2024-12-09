#include "Cat_Player.h"


Engine::Cat_Player::Cat_Player(Sprite* sprite) {
	sprite->SetNumXFrames(12);
	sprite->SetNumYFrames(8);
	sprite->AddAnimation("idle", 0, 2);
	sprite->AddAnimation("walk", 24, 26);
	sprite->AddAnimation("hit", 12, 14);
	sprite->PlayAnim("idle");
	sprite->SetScale(1.5);
	sprite->SetFlipHorizontal(0);
	sprite->SetAnimationDuration(400);

	this->player = sprite;
	state = Engine::Cat_PlayerState::IDLE;
}

void Engine::Cat_Player::setPosition(float x, float y) {
	player->SetPosition(x, y);
}

void Engine::Cat_Player::update(float time) {


	if (state == Cat_PlayerState::IDLE)
	{
		player->SetAnimationDuration(400);
		player->PlayAnim("idle");
		//set bounding box during idle
		float bbwidth = player->GetScaleWidth() - (24 * player->GetScale());
		float bbheight = player->GetScaleHeight() - (25 * player->GetScale());
		player->SetBoundingBoxSize(bbwidth, bbheight);
		if (invul) {
			player->PlayAnim("hit");
			player->SetAnimationDuration(200);
		}

	}
	else if (Cat_PlayerState::WALK == state) {
		player->SetAnimationDuration(400);
		player->PlayAnim("walk");
		if (isrun) {
			player->SetAnimationDuration(100);
		}

		if (invul) {
			player->PlayAnim("hit");
			player->SetAnimationDuration(200);
		}



		//set bounding box during walk
		float bbwidth = player->GetScaleWidth() - (12 * player->GetScale());
		float bbheight = player->GetScaleHeight() - (25 * player->GetScale());
		player->SetBoundingBoxSize(bbwidth, bbheight);

	}
	player->Update(time);
}

void Engine::Cat_Player::setState(Cat_PlayerState current) {
	this->state = current;
}

float Engine::Cat_Player::getWidth() {
	return player->GetScaleWidth();
}

float Engine::Cat_Player::getLength() {
	return player->GetScaleHeight();
}

vec2 Engine::Cat_Player::getPosition() {
	return player->GetPosition();
}

void Engine::Cat_Player::flip(bool flip) {
	player->SetFlipHorizontal(flip);

}

void Engine::Cat_Player::draw() {
	player->Draw();
}


void  Engine::Cat_Player::setLastHit(float time) {
	lastHitAt = time;
}


bool Engine::Cat_Player::isInvul(float currenttime) {
	if ((currenttime - lastHitAt) < 1500) {
		/*cout << "player invul, was last hit at " << lastHitAt << endl;
		cout << "time right now is" << currenttime << endl;
		*/
		invul = true;
		return true;
	}
	else {
		/*cout << "player damaged, was last hit at " << lastHitAt << endl;
		cout << "time right now is" << currenttime << endl;
		*/
		invul = false;
		return false;
	}


}

Engine::BoundingBox* Engine::Cat_Player::getBoundingBox() {
	return player->GetBoundingBox();
}
void Engine::Cat_Player::run(bool isrunning) {
	isrun = true;
}

void Engine::Cat_Player::takeDmg(int damage) {
	hp--;
	cout << hp << endl;
}

int Engine::Cat_Player::getHP() {
	return hp;
}

bool Engine::Cat_Player::isJump() {
	return jumpStatus;
}

void Engine::Cat_Player::setJump(bool status) {
	this->jumpStatus = status;
}

void Engine::Cat_Player::resetPlayer() {
	hp = 3;
	setLastHit(0);
}