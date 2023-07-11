#pragma once
#include "SpriteGo.h"
#include "AnimationController.h"

class Player :
    public SpriteGo
{
protected:
    AnimationController animation;
    //sf::View gameView;

public:
    Player(const std::string& textureId = "", const std::string& n = "") : SpriteGo(textureId, n) {};
    virtual ~Player() override { Release(); };

    virtual void Init() override;
    virtual void Reset() override;
    virtual void Update(float dt) override;
    virtual void Draw(sf::RenderWindow& window) override;
};

