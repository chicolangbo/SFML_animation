#pragma once
#include "SpriteGo.h"
#include "AnimationController.h"
#include "RectGo.h"

class Player :
    public SpriteGo
{
protected:
    AnimationController animation;
    sf::Vector2f direction = { 0.f, 0.f };
    float speed = 300.f;
    sf::Vector2f velocity = { 0.f, 0.f };
    sf::Vector2f gravity = {0.f, 2000.f};

    sf::RectangleShape floor;

    bool bottomCollide = true;
    bool moving = false;

public:
    Player(const std::string& textureId = "", const std::string& n = "") : SpriteGo(textureId, n) {};
    virtual ~Player() override { Release(); };

    virtual void Init() override;
    virtual void Reset() override;
    virtual void Update(float dt) override;
    virtual void Draw(sf::RenderWindow& window) override;

    void SetVelocity(sf::Vector2f v);
    void MovePlayer(float dt);
    void CheckSideCollide();
};

