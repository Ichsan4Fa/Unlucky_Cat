#ifndef CAT_PLAYER_H
#define CAT_PLAYER_H

#include "Sprite.h"

//to do list: 
// add invul time after hit
// hp & dmg
// 
//

namespace Engine {
	enum class Cat_PlayerState {
		IDLE,
		WALK
	};
	class Cat_Player {
	public:
		Cat_Player(Sprite* sprite);
		void setPosition(float x, float y);
		void update(float time);
		float getWidth();
		float getLength();
		void setState(Cat_PlayerState current);
		vec2 getPosition();
		void flip(bool flip);
		void run(bool isrun);
		void draw();
		void setLastHit(float time);
		bool isInvul(float time);
		BoundingBox* getBoundingBox();
		void takeDmg(int damage);
		int getHP();
		bool isJump();
		void setJump(bool status);
		void resetPlayer();
	protected:
		Sprite* player = NULL;
		Cat_PlayerState state;
		bool isflip = false;
		bool isrun = false;
		bool jumpStatus = false;
		bool invul = false;
		float lastHitAt = 0.f;
		int hp = 3;
	};

}
#endif