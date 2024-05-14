#pragma once
#include"../Scene/battleScene.h"
#include"../Manager/GameManager.h"
#include "../../dxlib_ext/dxlib_ext.h"

//-----------------------------------------------------------------------------------------
//
//�{�X�ł̐퓬�V�[��
//
//------------------------------------------------------------------------------------------

class Player;
class Enemy;
class BattleLog;

class BossScene : public BattleScene {
public:

	BossScene();
	~BossScene()override;

	//�X�V����
	void Update(float delta_time)override;

	//�`��
	void Draw()override;

	//�o�g���V�[�����Ő؂�ւ���֐�
	void MenuUpdate(Player::PlayerStatus& playerStatus, Enemy::EnemyStatus& enemyStatus_)override;

	//Inventory��\��������
	/*void InventoryItemDraw()override;*/

private:

	//------------------------------------------------------------------------------------------------------------------
	//�X�}�[�g�|�C���^

	//�o�g�����O
	Shared<BattleLog>battle_log = nullptr;
	//�G�̃|�C���^
	Shared<Enemy>enemy = nullptr;



};