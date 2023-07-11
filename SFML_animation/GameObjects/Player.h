#pragma once
#include "SpriteGo.h"
#include "ObjectPool.h"
#include "Bullet.h"

class Player : public SpriteGo
{
protected:
	sf::Vector2f look;
	sf::Vector2f direction;

	sf::FloatRect wallBounds;
	sf::Vector2f wallBoundsLT;
	sf::Vector2f wallBoundsRB;

	float speed = 300.f;
	float addSpeed = 0.f;
	int maxHp = 100;
	int hp = 0;
	int addHp = 0;

	// �����, 230709, ���� �߰�
	sf::Sound sound;
	///////////////////////////

	int projectile = 1;

	//230707 �����
	//ź ���� �ɼ�
	int curAmmo; 
	int magCapacity;
	int remainAmmo;
	float reloadingSpeed;
	float reloadTimer;

	ObjectPool<Bullet> poolBullets;

public:
	Player(const std::string& textureId, const std::string& n);

	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	void SetWallBounds(const sf::FloatRect& bounds);

	bool isAlive = false;
	//230707 �����
	//������ ����
	void ReloadMagazine();
	//230710 �����
	//�������� �̿��� �� ���� �� ȹ��
	void AddAmmo(int amount) { remainAmmo += amount; }
	void AddHealth(int amount);
	int GetAmmo() { return remainAmmo + curAmmo; }
	int GetHealth() { return hp; }

	void OnHitted(int damage);
	void OnDie();

	// �����, 230708, hpBar ������ get �Լ�
	int GetHp();
	int GetCurAmmo() { return curAmmo; }
	int GetRemainAmmo() { return remainAmmo; }
	//////////////////////////////////////

	//2023-07-09 �̳���
	void IncreaseHealth(int hp);
	void IncreaseSpeed(float speed);
	void IncreaseProjectile(int num);

};

