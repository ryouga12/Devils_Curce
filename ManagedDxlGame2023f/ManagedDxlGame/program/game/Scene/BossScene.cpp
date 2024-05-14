#include "BossScene.h"

BossScene::BossScene()
{
	enemy = std::make_shared<Enemy>();
	battle_log = std::make_shared<BattleLog>();

	//�o�g���t���O��؂�ւ���(�o�g���݂̂ł���A�C�e���̈�)
	GameManager::getGameManager()->getInventory()->BattleFlagSwitch(0);

	//����̃^�C�v���擾����
	SetWeaponType();


}

BossScene::~BossScene()
{
	
}

void BossScene::Update(float delta_time)
{
	__super::Update(delta_time);
}

void BossScene::Draw()
{
	__super::Draw();
}

void BossScene::MenuUpdate(Player::PlayerStatus& playerStatus, Enemy::EnemyStatus& enemyStatus_)
{
	__super::MenuUpdate(playerStatus, enemyStatus_);
}

//void BossScene::InventoryItemDraw()
//{
//	__super::InventoryItemDraw();
//}
