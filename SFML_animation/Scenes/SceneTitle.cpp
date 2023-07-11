#include "stdafx.h"
#include "SceneTitle.h"
#include "ResourceMgr.h"
#include "TextGo.h"
#include "InputMgr.h"
#include "SceneMgr.h"
#include "SpriteGo.h"
#include "Framework.h"
#include "StringTable.h"
#include "DataTableMgr.h"

SceneTitle::SceneTitle(SceneId id)
	: Scene(SceneId::Title)
{
	resources.push_back(std::make_tuple(ResourceTypes::Texture, "graphics/background_resize.png"));
	resources.push_back(std::make_tuple(ResourceTypes::Font, "fonts/zombiecontrol.ttf"));
}

SceneTitle::~SceneTitle()
{
}

void SceneTitle::Init()
{
	Release();

	AddGo(new SpriteGo("graphics/background_resize.png", "TitleBackground"));
	AddGo(new TextGo("Title", "fonts/HANAMDAUM(Windows).ttf"));
	AddGo(new TextGo("hiScore", "fonts/zombiecontrol.ttf"));

	for (auto go : gameObjects)
	{
		go->Init();
	}

	// Ÿ��Ʋ ��
	sf::Vector2f windowSize = FRAMEWORK.GetWindowSize();
	sf::Vector2f centerPos = windowSize * 0.5f;
	titleView.setSize(windowSize);
	titleView.setCenter(centerPos);
}

void SceneTitle::Release()
{
	for (auto go : gameObjects)
	{
		delete go;
	}
}

void SceneTitle::Enter()
{
	Scene::Enter();

	StringTable* stringTable = DATATABLE_MGR.Get<StringTable>(DataTable::Ids::String);
	std::string title = stringTable->Get("TITLE");

	SpriteGo* titleBackground = (SpriteGo*)FindGo("TitleBackground");
	TextGo* titleText = (TextGo*)FindGo("Title");
	TextGo* hiScore = (TextGo*)FindGo("hiScore");

	// ���
	titleBackground->SetOrigin(Origins::MC);
	titleBackground->SetPosition(FRAMEWORK.GetWindowSize() / 2.f);

	// �ؽ�Ʈ
	titleText->text.setString(title);
	titleText->text.setCharacterSize(75);
	titleText->text.setFillColor(sf::Color::White);
	titleText->SetOrigin(Origins::MC);
	titleText->SetPosition(FRAMEWORK.GetWindowSize() / 2.f);

}

void SceneTitle::Exit()
{
	Scene::Exit();
}

void SceneTitle::Update(float dt)
{
	Scene::Update(dt);

	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Enter))
	{
		//SCENE_MGR.ChangeScene(SceneId::Game);
	}
}

void SceneTitle::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);

	window.setView(titleView);
	for (auto go : gameObjects)
	{
		if (go->sortLayer >= 100)
			continue;

		if (go->GetActive())
		{
			go->Draw(window);
		}
	}
}
