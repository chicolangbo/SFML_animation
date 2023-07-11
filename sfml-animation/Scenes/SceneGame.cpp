#include "stdafx.h"
#include "SceneGame.h"
#include "SceneMgr.h"
#include "InputMgr.h"
#include "ResourceMgr.h"
#include "GameObject.h"
#include "Player.h"
#include "Framework.h"
#include "RectGo.h"

SceneGame::SceneGame() : Scene(SceneId::Game)
{
	resources.push_back(std::make_tuple(ResourceTypes::Texture, "graphics/RubySheet.png"));
}

void SceneGame::Init()
{
	Release();

	Player* player = (Player*)AddGo(new Player("player"));
	RectGo* floor = (RectGo*)AddGo(new RectGo("floor"));

	for (auto go : gameObjects)
	{
		go->Init();
	}
}

void SceneGame::Release()
{
	for (auto go : gameObjects)
	{
		//go->Release();
		delete go;
	}
}

void SceneGame::Enter()
{
	auto size = FRAMEWORK.GetWindowSize();
	sf::Vector2f centerPos = size * 0.5f;
	worldView.setSize(size);
	worldView.setCenter(centerPos);
	uiView.setSize(size);
	uiView.setCenter(centerPos);

	Scene::Enter();
}

void SceneGame::Exit()
{
	Scene::Exit();
}

void SceneGame::Update(float dt)
{
	Scene::Update(dt);
}

void SceneGame::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}
