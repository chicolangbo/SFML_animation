#include "stdafx.h"
#include "Player.h"
#include "InputMgr.h"
#include "Framework.h"

void Player::Init()
{
	std::string textureId = "graphics/RubySheet.png";

	// Idle
	{
		AnimationClip clip;
		clip.id = "IdleF";
		clip.fps = 20;
		clip.loopType = AnimationLoopTypes::Loop;
		
		sf::IntRect coord(256, 768, 256, 256);
		clip.frames.push_back({ textureId, coord });
		animation.AddClip(clip);
	}

	{
		AnimationClip clip;
		clip.id = "IdleLR";
		clip.fps = 20;
		clip.loopType = AnimationLoopTypes::Loop;

		sf::IntRect coord(0, 768, 256, 256);
		clip.frames.push_back({ textureId, coord });
		animation.AddClip(clip);
	}

	{
		AnimationClip clip;
		clip.id = "IdleB";
		clip.fps = 20;
		clip.loopType = AnimationLoopTypes::Loop;

		sf::IntRect coord(512, 768, 256, 256);
		clip.frames.push_back({ textureId, coord });
		animation.AddClip(clip);
	}

	// Move
	{
		AnimationClip clip;
		clip.id = "MoveLR";
		clip.fps = 10;
		clip.loopType = AnimationLoopTypes::Loop;
		
		sf::IntRect coord(0, 0, 256, 256);
		for (int i = 0; i < 4; ++i)
		{
			clip.frames.push_back({textureId, coord});
			coord.left += coord.width;
		}
		animation.AddClip(clip);
	}

	{
		AnimationClip clip;
		clip.id = "MoveB";
		clip.fps = 10;
		clip.loopType = AnimationLoopTypes::Loop;
		
		sf::IntRect coord(0, 256, 256, 256);
		for (int i = 0; i < 4; ++i)
		{
			clip.frames.push_back({textureId, coord});
			coord.left += coord.width;
		}
		animation.AddClip(clip);
	}

	{
		AnimationClip clip;
		clip.id = "MoveF";
		clip.fps = 10;
		clip.loopType = AnimationLoopTypes::Loop;
		
		sf::IntRect coord(0, 512, 256, 256);
		for (int i = 0; i < 4; ++i)
		{
			clip.frames.push_back({textureId, coord});
			coord.left += coord.width;
		}
		animation.AddClip(clip);
	}

	animation.SetTarget(&sprite);
	SetOrigin(Origins::BC);

	//// 바닥
	//floor.setFillColor(sf::Color::Blue);
	//floor.setSize({ FRAMEWORK.GetWindowSize().x, 100.f });
	//Utils::SetOrigin(floor, Origins::BC);
	//floor.setPosition(FRAMEWORK.GetWindowSize().x * 0.5f, FRAMEWORK.GetWindowSize().y);
}

void Player::Reset()
{
	animation.Play("IdleF");
	SetOrigin(origin);
	SetPosition({ FRAMEWORK.GetWindowSize().x * 0.5f, 620.f });
	SetFlipX(false);
}

void Player::Update(float dt)
{
	// TEST CODE
	{
		if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num1))
		{
			animation.Play("IdleF");
		}
		if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num2))
		{
			animation.Play("IdleLR");
		}
		if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num3))
		{
			animation.Play("IdleB");
		}
		if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num4))
		{
			animation.Play("MoveLR");
		}
		if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num5))
		{
			animation.Play("MoveB");
		}
		if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num6))
		{
			animation.Play("MoveF");
		}
	}

	animation.Update(dt);
	direction.x = INPUT_MGR.GetAxis(Axis::Horizontal);
	direction.y = INPUT_MGR.GetAxis(Axis::Vertical);

	// USING CODE
	{
		// 플립
		if (direction.x != 0.f)
		{
			bool flip = direction.x > 0.f;
			if (GetFlipX() != flip)
			{
				SetFlipX(flip);
			}
		}

		// 점프
		//if (isCollide && INPUT_MGR.GetKeyDown(sf::Keyboard::Space))
		//{
		//	velocity.y += JumpForce;
		//	animation.Play("Jump");
		//	isCollide = false;
		//}

		// 이동
		position += direction * speed * dt;

		// 바닥 충돌 처리
		if (position.y >= FRAMEWORK.GetWindowSize().y)
		{
			isCollide = true;
			position.y = FRAMEWORK.GetWindowSize().y;
		}

		// 위 충돌 처리
		if (position.y <= 256.f)
		{
			isCollide = true;
			position.y = 256.f;
		}

		// 좌 충돌 처리
		if (position.x <= 128.f)
		{
			isCollide = true;
			position.x = 128.f;
		}

		// 우 충돌 처리
		if (position.x >= FRAMEWORK.GetWindowSize().x - 128.f)
		{
			isCollide = true;
			position.x = FRAMEWORK.GetWindowSize().x - 128.f;
		}

		SetPosition(position);

		// 애니메이션
		//if (INPUT_MGR.GetKey(sf::Keyboard::A) || INPUT_MGR.GetKey(sf::Keyboard::D))
		//{
		//	animation.Play("MoveLR");
		//	isCollide = false;
		//}

		//if (INPUT_MGR.GetKey(sf::Keyboard::W))
		//{
		//	animation.Play("MoveB");
		//	isCollide = false;
		//}

		//if (INPUT_MGR.GetKey(sf::Keyboard::S))
		//{
		//	animation.Play("MoveF");
		//	isCollide = false;
		//}

		//else
		//{
		//	animation.Play("IdleF");
		//}


		// 애니메이션 기존 코드
		{
			if (animation.GetCurrentClipId() == "IdleF" || 
				animation.GetCurrentClipId() == "IdleLR" || 
				animation.GetCurrentClipId() == "IdleB")
			{
				if (direction.x != 0.f)
				{
					animation.Play("MoveLR");
				}
				if (direction.y < 0)
				{
					animation.Play("MoveB");
				}
				if (direction.y > 0)
				{
					animation.Play("MoveF");
				}
			}
			else if (animation.GetCurrentClipId() == "MoveLR")
			{
				if (direction.x == 0.f && direction.y == 0.f)
				{
					animation.Play("IdleLR");
				}
			}
			else if (animation.GetCurrentClipId() == "MoveB")
			{
				if (direction.x == 0.f && direction.y == 0.f)
				{
					animation.Play("IdleB");
				}
			}
			else if (animation.GetCurrentClipId() == "MoveF")
			{
				if (direction.x == 0.f && direction.y == 0.f)
				{
					animation.Play("IdleF");
				}
			}
		}
	}
}

void Player::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
	//window.draw(floor);
}

bool Player::GetFlipX() const
{
	return flipX;
}

void Player::SetFlipX(bool flip)
{
	flipX = flip;

	sf::Vector2f scale = sprite.getScale();
	scale.x = !flip ? abs(scale.x) : -abs(scale.x);
	sprite.setScale(scale);
}
