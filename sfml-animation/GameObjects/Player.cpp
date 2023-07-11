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

	// ¹Ù´Ú
	floor.setFillColor(sf::Color::Blue);
	floor.setSize({ FRAMEWORK.GetWindowSize().x, 100.f });
	Utils::SetOrigin(floor, Origins::BC);
	floor.setPosition(FRAMEWORK.GetWindowSize().x * 0.5f, FRAMEWORK.GetWindowSize().y);
}

void Player::Reset()
{
	animation.Play("Idle");

	SetOrigin(origin);
	SetPosition(FRAMEWORK.GetWindowSize().x * 0.5f, 620.f);
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

	// USING CODE
	if (bottomCollide)
	{
		direction = { 0.f, 0.f };
		animation.PlayQueue("Idle");
	}

	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Left))
	{
		animation.Play("Move");
	}
	if (INPUT_MGR.GetKey(sf::Keyboard::Left))
	{
		moving = true;
		sprite.setScale(-1.f, 1.f);
		direction = { -1.f, 0.f };
		animation.PlayQueue("Move");
		if (INPUT_MGR.GetKeyDown(sf::Keyboard::Space))
		{
			animation.Play("Jump");
			bottomCollide = false;
			velocity = { 0.f,-1000.f };
		}
	}
	if (INPUT_MGR.GetKeyUp(sf::Keyboard::Left))
	{
		animation.Play("Idle");
	}

	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Right))
	{
		animation.Play("Move");
	}
	if (INPUT_MGR.GetKey(sf::Keyboard::Right))
	{
		moving = true;
		sprite.setScale(1.f, 1.f);
		direction = { 1.f, 0.f };
		animation.PlayQueue("Move");
		if (INPUT_MGR.GetKeyDown(sf::Keyboard::Space))
		{
			animation.Play("Jump");
			bottomCollide = false;
			velocity = { 0.f,-1000.f };
		}
	}
	if (INPUT_MGR.GetKeyUp(sf::Keyboard::Right))
	{
		animation.Play("Idle");
	}

	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Space))
	{
		animation.Play("Jump");
		bottomCollide = false;
		direction = { 0.f,0.f };
		velocity = { 0.f,-1000.f };
	}

	MovePlayer(dt);
	CheckSideCollide();

	SpriteGo::Update(dt);
	animation.Update(dt);
}

void Player::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
	window.draw(floor);
}

void Player::SetVelocity(sf::Vector2f v)
{
	velocity = v;
}

void Player::MovePlayer(float dt)
{
	// ÁÂ¿ì ÀÌµ¿
	sf::Vector2f tempPosition = GetPosition();
	tempPosition.x += direction.x * speed * dt;

	//»óÇÏ ÀÌµ¿
	if (velocity == sf::Vector2f{ 0.f,0.f })
	{
		velocity = { 0.f,0.f };
	}
	else
	{
		velocity += gravity * dt;
	}

	SetPosition(tempPosition + velocity * dt);
}

void Player::CheckSideCollide()
{
	sf::Vector2f windowSize = FRAMEWORK.GetWindowSize();
	sf::Vector2f centerPos = windowSize * 0.5f;
	sf::FloatRect playerBound = sprite.getGlobalBounds(); // ÇÃ·¹ÀÌ¾î °´Ã¼ ¹Ù¿îµå
	sf::FloatRect floorBound = floor.getGlobalBounds(); // ¹Ù´Ú °´Ã¼ ¹Ù¿îµå

	if (playerBound.intersects(floorBound)) // ¹Ù´Ú Ãæµ¹
	{
		bottomCollide = true;
		SetPosition(GetPosition().x, floorBound.top);
	}

	if (playerBound.left <= 0.f) // ÁÂ Ãæµ¹
	{
		SetPosition(playerBound.width * 0.5f, GetPosition().y);
	}

	if (playerBound.left + playerBound.width >= windowSize.x) // ¿ì Ãæµ¹
	{
		SetPosition(windowSize.x - playerBound.width * 0.5f, GetPosition().y);
	}
}
