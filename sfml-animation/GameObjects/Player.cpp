#include "stdafx.h"
#include "Player.h"
#include "InputMgr.h"
#include "Framework.h"

void Player::Init()
{
	std::string textureId = "graphics/sprite_sheet.png";

	// Idle
	{
		AnimationClip clip;
		clip.id = "Idle";
		clip.fps = 20;
		clip.loopType = AnimationLoopTypes::Loop;
		
		sf::IntRect coord(0, 0, 120, 120);
		for (int i = 0; i < 8; ++i)
		{
			clip.frames.push_back({textureId, coord});
			coord.left += coord.width;
		}
		animation.AddClip(clip);
	}

	// Move
	{
		AnimationClip clip;
		clip.id = "Move";
		clip.fps = 10;
		clip.loopType = AnimationLoopTypes::Loop;
		
		sf::IntRect coord(0, 120, 120, 120);
		for (int i = 0; i < 8; ++i)
		{
			clip.frames.push_back({textureId, coord});
			coord.left += coord.width;
		}
		clip.frames.push_back({ textureId,sf::IntRect(0, 240, 120, 120) });
		animation.AddClip(clip);
	}

	// Jump
	{
		AnimationClip clip;
		clip.id = "Jump";
		clip.fps = 10;
		clip.loopType = AnimationLoopTypes::Single;
		
		sf::IntRect coord(0, 360, 120, 120);
		for (int i = 0; i < 7; ++i)
		{
			clip.frames.push_back({textureId, coord});
			coord.left += coord.width;
		}

		clip.frames[6].action = []() {
			std::cout << "On Complete Jump Clip" << std::endl;
		};

		animation.AddClip(clip);
	}

	animation.SetTarget(&sprite);
	SetOrigin(Origins::BC);
}

void Player::Reset()
{
	animation.Play("Idle");

	SetOrigin(origin);
	SetPosition(0, 0);
}

void Player::Update(float dt)
{
	// TEST CODE
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num1))
	{
		animation.Play("Idle");
	}
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num2))
	{
		animation.Play("Move");
	}
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num3))
	{
		animation.Play("Jump");
	}
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num4))
	{
		animation.PlayQueue("Idle");
	}
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num5))
	{
		animation.PlayQueue("Move");
	}
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num6))
	{
		animation.PlayQueue("Jump");
	}

	SpriteGo::Update(dt);
	animation.Update(dt);
}

void Player::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
}
