#include "BossScene.h"

BossScene::BossScene()
{
	enemy = std::make_shared<Enemy>();
	battle_log = std::make_shared<BattleLog>();

	//バトルフラグを切り替える(バトルのみでつかるアイテムの為)
	GameManager::getGameManager()->getInventory()->BattleFlagSwitch(0);

	//武器のタイプを取得する
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
