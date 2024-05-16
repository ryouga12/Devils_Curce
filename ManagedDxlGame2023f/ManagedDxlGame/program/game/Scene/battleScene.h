#pragma once
#include"../Scene/BaseScene.h"
#include "../../dxlib_ext/dxlib_ext.h"
#include"../Menu/MenuWindow.h"
#include"../Manager/ResourceManager.h"
#include"../Menu/BattleLog.h"
#include"../Manager/animation.h"
#include"../Manager/GameManager.h"
#include"../../game/Skill/Skill.h"
#include"../Manager/Camera.h"
#include<random>

//-----------------------------------------------------------------------------------------------------
//
//�t�B�[���h�̐퓬�V�[��
//
//-----------------------------------------------------------------------------------------------------

class Player;
class Enemy;

class BattleScene : public  BaseScene{

public:

	BattleScene() {};
	BattleScene(tnl::Vector3 pos ,int background, int EnemyID);
	virtual~BattleScene()override;

	//�X�V����
	virtual void Update(float delta_time)override;

	//�`��
	virtual void Draw()override;

	//�o�g���E�B���h�E�̕`��
	void BattleDraw();

	//�o�g���V�[�����Ő؂�ւ���֐�
	virtual void MenuUpdate(Player::PlayerStatus& playerStatus,Enemy::EnemyStatus& enemyStatus_);

	//Inventory��\��������
	virtual void InventoryItemDraw();

	//�A�C�e���I�����̏���
	void ItemSelectProcess();

	//�A�C�e���̂��g�����ۂ̏���
	void ItemUseProcess();

	//�X�L���I�����̏���
	void SkillSelectProcess();

	//�v���C���[�̍X�V����
	void PlayerUpdate();

	//���j���[�̏�����
	void InitMenuWindow();

	//�A�j���[�V�������̃^�[���`�F���W
	void AnimationTurnChange(int enemyhp, float delta_time);

	//�������I�������ۂ̏���
	void FleeProcess(float delta_time);

	// ���񂾃G�l�~�[�̎��S���o
	void deadEnemyProcess(Player::PlayerStatus& playerStatus, Enemy::EnemyStatus& enemyStatus_);

	// ���x���A�b�v����ƃ��x���A�b�v����
	void levelUpProcess(Player::PlayerStatus& playerStatus);

	//HP��MP�o�[�̏���
	void PlayerStatusDraw();

	//�A�C�e���̃h���b�v����
	void ItemDropProcess();

	//�ꎞ�I�ɏオ�����U���͂Ȃǂ����Z�b�g����
	void BattleBuffResetProcess();

	//����̃^�C�v��Set����
	void SetWeaponType();

private:

	//�G�̃O���t�B�b�N�n���h��
	int Enemy_ghdl;

	//�G���擾����C���f�b�N�X
	int enemy_index;

	//�w�i�̉摜
	int background_ = 0;

	//���W���ꎞ�I�ɕۑ����Ă����ϐ�
	tnl::Vector3 map_pos = {};

	//�����������̍U���͂Ɩh���
	const int equipAttack = 0;
	const int equipDefance = 1;

	//�x��������b��
	const float Time = 1.0f;

	//�������m��
	int probability = 50;

	//�ʏ�h���b�v
	const float NomalDrop = 30;

	//���A�h���b�v
	const float RareDrop = 10;


//------------------------------------------------------------------------------------------------------------------------
//���ꂼ��̃t���O


	//���x���A�b�v���̃t���O
	bool levelUp_flag = false;

	//�����鎞�̃t���O
	bool flee_flag = false;

	//�S�ł������̃t���O
	bool annihilation_flag = false;

	//�G�l�~�[��|�������̃t���O
	bool enemy_annihilation_flag = false;

	//�ʏ�U�����������̃t���O
	bool Nomal_Attack_Flag = false;

	//�v���C���[�̃^�[���������t���O
	bool PlayerTurn = false;

	//�G��|�������̃t���O
	bool DeadEnemy_flag = false;

//------------------------------------------------------------------------------------------------------------------------
//�V�[�P���X

private:

	tnl::Sequence<BattleScene> sequence_ = tnl::Sequence<BattleScene>(this, &BattleScene::seqIdle);
	bool seqIdle(float delta_time);
	//�v���C���[�̃V�[�P���X
	bool seqPlayerAction(float delta_time);
	//�G�l�~�[�̃V�[�P���X
	bool seqEnemyAction(float delta_time);

	//�J�ڂ�����ׂ̃V�[�P���X
	bool seqChangeScene(float delta_time);

	TNL_CO_SEQUENCE(BattleScene, &BattleScene::seqIdle);

	enum class Sequence {
		Idle,
		PlayerAction,
		EnemyAction
	};

	Sequence select_sequence = Sequence::Idle;

//--------------------------------------------------------------------------------------------------------------------------
//�|�C���^�ϐ�
private:

	//---�X�}�[�g�|�C���^---//

	//�G
	Shared<Enemy>enemy = nullptr;
	//�s����I������E�B���h�E
	Shared<MenuWindow>Select_Action_Menu = nullptr;
	//�o�g�����O�𗬂��E�B���h�E
	Shared<Menu>BattleLogMenu = nullptr;
	//�X�e�[�^�X��\������E�B���h�E
	Shared<Menu>playerStatus_window = nullptr;
	//�o�g�����O
	Shared<BattleLog>battle_log = nullptr;
	//�U���̍ۂ̃E�B���h�E
	Shared<MenuWindow>attack_window = nullptr;
	//�A�C�e����\������E�B���h�E
	Shared<Menu>item_window = nullptr;
	//�A�C�e�����g���E�B���h�E
	Shared<MenuWindow>select_use_window = nullptr;
	//�X�L����\������E�B���h�E
	Shared<Menu>skill_menu_window = nullptr;
	//�ʏ�U��
	Shared<Nomal_Attack>nomal_attack = nullptr;
	
	//---�|�C���^---//

	//---�E�B�h�E---//
	
	//�ŏ��̍s���̑I��
	MenuWindow::MenuElement_t* select_action_coment = nullptr;
	//�U���s���̑I��
	MenuWindow::MenuElement_t* select_action_attack = nullptr;
	//�A�C�e���̎g�p�̑I��
	MenuWindow::MenuElement_t* select_detail = nullptr;
	

//--------------------------------------------------------------------------------------------------------------------------
//���j���[�Ȃǂ̕\���֘A

private:

	//�ǂ̓�����s�����Ń��j���[�����߂�
	enum class MenuAction {
		first_action,
		fight_action,
		item_action,
		item_use_action,
		skill_action,
	};

	MenuAction select_action_menu = MenuAction::first_action;

	//�\��������A�C�e����
	const int itemPerPage = 4;
	//�A�C�e����\������y���W
	const int StartY = 550;
	//�e�s�̍���
	const int lineHeight = 30;
	//�e�y�[�W
	int currentPage = 0;
	//�ŏ��͈�ԏ�Ȃ̂�0�ɐݒ肷��
	int selectedIndex = 0;
	//�擾�����A�C�e����ID���ꎞ�I�ɕێ����邽�߂̕ϐ�
	int selectedItemId = 0;
	//�J�[�\���̃T�C�Y
	const float cursourSize = 0.3f;
	//�A�C�e�����g��
	const int ItemUse = 0;
	//�A�C�e�����g���E�B���h�E�����
	const int ItemUseMenuClose = 1;
	//���F
	const int Color_Black = 0;
	//���F
	const int Color_White = -1;
	//�S�ł������ɗ���SE�̉�
	const float annihilation_Time = 3.5f;
	//�^�[����؂�ւ��鎞�ɏ����x��������
	const float Change_Turn_Time = 1;
	//�A�j���[�V�����𗬂�����
	const float Animation_Time = 1;
	//�����鎞��SE�𗬂�����
	const float flee_Time = 1;
	//���j���[�I���ōU�����I�����ꂽ�ꍇ
	int Attack = 0;
	//���j���[�I���œ���I�����ꂽ�ꍇ
	int Item = 1;
	//���j���[��I���œ����邪�I�����ꂽ��
	int Flee = 2;
	//�U���̑I����ʂœ��Z���I�����ꂽ��
	int SpecialSkill = 1;
	//�I����ʂŕ��邪�����ꂽ��
	int MenuClose = 2;
	//�J�[�\�����䂷�邽�߂̕ϐ�
	int False = 1, True = 0;

//--------------------------------------------------------------------------------------------------------------------------
//���O�֘A
private:

//--------------------------------------------------------------------------------------------------------------------------
//�X�L���֘A
private:

	//�X�L�����g�p�����ۂ̃_���[�W
	int SkillDamage = 0;

	//�X�L���ׁ[�W
	int SkillCurentPage = 0;

	//�ǂ̃X�L����I�����Ă��邩
	int selectedSkillIndex = 0;

	//�X�L����ID
	int selectedSkillId = 0;

	//�o�g���V�[���p�̃X�L���\��
	void InventorySkillDraw();

	//�X�L�����g�p�������̃t���O
	bool SkillUseFlag = false;

	//�X�L�����g�p�������̃t���O(�U���n)
	bool Skill_Attack_Flag = false;

	//�X�L�����g�p�������̃t���O(�o�t�n)
	bool Skill_Buff_Flag = false;

	//���Z���g���邩�̃t���O(���Mp�����肽��true�A����Ȃ�������false)
	bool Skill_Chaeck_Flag = true;

	//�X�L�����g�p�����ۂ�Mp�m�F
	void SkillUseMpChack(Player::PlayerStatus& playerStatus);

	//�X�L�����g�p�����ۂ̏���
	void SkillUseProcess(Player::PlayerStatus& playerStatus, Enemy::EnemyStatus& enemyStatus_);

	//����̃^�C�v(�ʏ�U���̃G�t�F�N�g�̐؂�ւ��̈�)
	int WeaponType = 0;

};