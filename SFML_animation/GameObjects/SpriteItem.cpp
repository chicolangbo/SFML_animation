#include "stdafx.h"
#include "SpriteItem.h"
#include "Player.h"
#include "SceneMgr.h"
#include "ResourceMgr.h"

const std::string SpriteItem::TextureIds[2] =
{
	"graphics/ammo_icon.png",
	"graphics/health_pickup.png",
};
//std::fuction<void (Player* player, int amount)>�� ���迡 ����ϰ� �;����� �����Լ��� �ʱ�ȭ�ϴ°� ������ �ȵȴٰ� �ؼ� using�� ����Ͽ� ������ ��Ī�� ��� �Ͽ� ����
const SpriteItem::EffectFunction SpriteItem::Effects[2] =
{
	[](Player* player, int amount) {player->AddAmmo(amount); },
	[](Player* player, int amount) {player->AddHealth(amount); },
};
int SpriteItem::Amounts[2] = {120, 20};

SpriteItem::SpriteItem(const std::string& n)
	:SpriteGo("", n),amount(0),itemType(Types::Default)
{
}

SpriteItem::~SpriteItem()
{
}

void SpriteItem::Reset()
{
	int ammo = player->GetAmmo();
	int hp = player->GetHealth();
	std::cout << "������ ����" << std::endl;
	if (ammo < 50)
	{
		SetType(Types::Ammo);
	}
	else if(hp < 20)
	{
		SetType(Types::MediKit);
	}
	else if(ammo < Utils::RandomRange(50, 1000))//ź���� �� ������ 1000 �� ������ �� �Ȼ��⵵�� ����
	{
		SetType(Types::Ammo);
	}
	else
	{
		SetType(Types::MediKit);
	}

	SpriteGo::Reset();
}

void SpriteItem::Update(float dt)
{
	if (sprite.getGlobalBounds().intersects(player->sprite.getGlobalBounds()))
	{
		UseItem();
	}
}

void SpriteItem::SetType(Types type)
{
	//std::cout << "������ �� Ÿ��" << std::endl;
	itemType = type;
	int index = (int)itemType;
	textureId = TextureIds[index];
	amount = Amounts[index];
}


void SpriteItem::UseItem()
{
	sound.setBuffer(*RESOURCE_MGR.GetSoundBuffer("sound/pickup.wav"));
	sound.play();

	Effects[(int)itemType](player, amount);	

	SCENE_MGR.GetCurrScene()->RemoveGo(this);
	pool->Return(this);
}

void SpriteItem::SetPlayer(Player* player)
{
	this->player = player;
}

void SpriteItem::ResetAmount()
{
	SpriteItem::Amounts[0] = 120;
	SpriteItem::Amounts[1] = 20;
}
