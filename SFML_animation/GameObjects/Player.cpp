#include "stdafx.h"
#include "Player.h"
#include "InputMgr.h"
#include "SceneMgr.h"
#include "Scene.h"
#include "Utils.h"
#include <iostream>
#include "ResourceMgr.h"

Player::Player(const std::string& textureId = "", const std::string& n = "")
	: SpriteGo(textureId, n)
{
}

void Player::Init()
{
	SpriteGo::Init();
	SetOrigin(Origins::MC);

	poolBullets.OnCreate = [this](Bullet* bullet) {
		bullet->textureId = "graphics/bullet.png";
		bullet->pool = &poolBullets;
	};

	poolBullets.Init();
}

void Player::Release()
{
	SpriteGo::Release();

	poolBullets.Release();
}

void Player::Reset()
{
	SpriteGo::Reset();

	isAlive = true;
	hp = maxHp + addHp;
	projectile = 1;
	
	for (auto bullet : poolBullets.GetUseList())
	{
		SCENE_MGR.GetCurrScene()->RemoveGo(bullet);
	}
	poolBullets.Clear();

	//230707�����
	//ź�� �ɼ� �ʱ�ȭ
	magCapacity = 30;
	remainAmmo = 210;
	reloadTimer = 0.f;
	reloadingSpeed = 0.3f;
	curAmmo = magCapacity;
}

void Player::Update(float dt)
{
	SpriteGo::Update(dt);
	//230707�����
	//�������϶� �Ѿȳ����� reloadTimer 0 ���Ϸ� �ȳ�������
	if(reloadTimer >= 0)
		reloadTimer -= dt;

	sf::Vector2f mousePos = INPUT_MGR.GetMousePos();
	sf::Vector2f mouseWorldPos = SCENE_MGR.GetCurrScene()->ScreenToWorldPos(mousePos);
	sf::Vector2f playerScreenPos = SCENE_MGR.GetCurrScene()->WorldPosToScreen(position);

	// ȸ��
	look = Utils::Normalize(mousePos - playerScreenPos);
	sprite.setRotation(Utils::Angle(look));

	// �̵�
	direction.x = INPUT_MGR.GetAxis(Axis::Horizontal);
	direction.y = INPUT_MGR.GetAxis(Axis::Vertical);
	float magnitude = Utils::Magnitude(direction);
	if (magnitude > 1.f)
	{
		direction /= magnitude;
	}

	position += direction * (speed + addSpeed) * dt;
	if (!wallBounds.contains(position))
	{
		position = Utils::Clamp(position, wallBoundsLT, wallBoundsRB);
	}
	sprite.setPosition(position);
	
	if (INPUT_MGR.GetMouseButtonDown(sf::Mouse::Left) && reloadTimer < 0 && curAmmo > 0)
	{
		// �����, 230709, �ѹ߻� ���� (���߿� �Ѿ� ������ �Ҹ� �� ���� �ٲٱ�)
		sound.setBuffer(*RESOURCE_MGR.GetSoundBuffer("sound/shoot.wav"));
		sound.play();
		////////////////////////////////////////////////
		//2023-07-09 �̳���
		//�Ѿ� projectile ��ŭ �߻�ǰ� ����
		Scene* scene = SCENE_MGR.GetCurrScene();
		SceneDev1* sceneDev1 = dynamic_cast<SceneDev1*>(scene);

		curAmmo--;
		float totalAngle = (projectile - 1) * 3;
		float eachAngle = totalAngle / projectile;
		for (int i = 0; i < projectile; i++)
		{
			float fireAngle = Utils::Angle(look) - (totalAngle * 0.5f);
			float newAngle = fireAngle + (eachAngle * i);
			sf::Vector2f newDir;
			newDir.x = cos(newAngle * (M_PI / 180.0f));
			newDir.y = sin(newAngle * (M_PI / 180.0f));


			Bullet* bullet = poolBullets.Get();
			//bullet->Fire(GetPosition(), look, 1000.f);
			bullet->Fire(GetPosition(), newDir, 1000.f);

			std::cout << bullet->addDamage << std::endl;

			if (sceneDev1 != nullptr)
			{
				bullet->SetZombieList(sceneDev1->GetZombieList());
				sceneDev1->AddGo(bullet);
			}
		}
	}
	
	/*
	//230707 �����
	//���� �� �ּ��� ����, ���콺 Ŭ��, źâ �� ź ��, �������� ����
	if (INPUT_MGR.GetMouseButtonDown(sf::Mouse::Left) && reloadTimer < 0 && curAmmo > 0)
	{		
		sound.setBuffer(*RESOURCE_MGR.GetSoundBuffer("sound/shoot.wav"));
		sound.play();

		Bullet* bullet = poolBullets.Get();
		bullet->Fire(GetPosition(), look, 1000.f);
		curAmmo--;

		Scene* scene = SCENE_MGR.GetCurrScene();
		SceneDev1* sceneDev1 = dynamic_cast<SceneDev1*>(scene);
		if (sceneDev1 != nullptr)
		{
			bullet->SetZombieList(sceneDev1->GetZombieList());
			sceneDev1->AddGo(bullet);
		}		
	}
	*/
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::R))
	{
		ReloadMagazine();
	}
	//test//std::cout<< curAmmo << "/" << remainAmmo << "/" << reloadTimer <<std::endl;
}

void Player::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
}

void Player::SetWallBounds(const sf::FloatRect& bounds)
{
	wallBounds = bounds;

	wallBoundsLT = { wallBounds.left, wallBounds.top };
	wallBoundsRB = { wallBounds.left + wallBounds.width, wallBounds.top + wallBounds.height };
}

void Player::ReloadMagazine()
{
	reloadTimer = reloadingSpeed;

	if (remainAmmo + curAmmo == 0)//���� ����
	{
		sound.setBuffer(*RESOURCE_MGR.GetSoundBuffer("sound/reload_failed.wav"));
		sound.play();
		return;
	}

	sound.setBuffer(*RESOURCE_MGR.GetSoundBuffer("sound/reload.wav"));
	sound.play();

	int tempAmmo = remainAmmo + curAmmo - magCapacity;// ������ ���� �ӽð�ü
	remainAmmo = std::max(tempAmmo, 0); // �� ���� ź ���� 0���� ���ٸ� remainAmmo�� 0���� ����
	curAmmo = remainAmmo == 0 ? magCapacity + tempAmmo : magCapacity; // remainAmmo�� 0 �̸� �� ���� ź ���� curAmmo�� ���� �ƴϸ� magCapacity�� curAmmo�� ����

}

void Player::AddHealth(int amount)
{
	hp += amount;
	if (hp > maxHp)
		hp = maxHp;	
}

void Player::OnHitted(int damage)
{
	// �����, 230709, ���� �� ���� �߰�
	sound.setBuffer(*RESOURCE_MGR.GetSoundBuffer("sound/hit.wav"));
	sound.play();
	//////////////////////////////////

	if (!isAlive)
		return;

	hp = std::max(hp - damage, 0);
	if (hp == 0)
	{
		OnDie();
	}
}

void Player::OnDie()
{
	isAlive = false;

	Scene* scene = SCENE_MGR.GetCurrScene();
	SceneDev1* sceneDev1 = dynamic_cast<SceneDev1*>(scene);
	if (sceneDev1 != nullptr)
	{
		sceneDev1->OnDiePlayer();
	}
}

int Player::GetHp()
{
	return hp;
}

void Player::IncreaseHealth(int hp)
{
	addHp += hp;
	this->hp += addHp;
}

void Player::IncreaseSpeed(float speed)
{
	addSpeed += speed;
}

void Player::IncreaseProjectile(int num)
{
	projectile += num;
}
