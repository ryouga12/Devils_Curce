#pragma once
#include "../../dxlib_ext/dxlib_ext.h"
#include"../Manager/ResourceManager.h"
#include"../Manager/GameManager.h"
#include"../Manager/SceneManager.h"
#include<random>

//-----------------------------------------------------------------------------------------------------
//
//�t�B�[���h�̐퓬�V�[��
//
//-----------------------------------------------------------------------------------------------------

class Player;
class Enemy;
class Menu;
class MenuWindow;
class MapScene;
class RusultScene;
class Battle_Log;
class BaseScene;
class Actor;

class BattleScene : public  BaseScene{

public:

	BattleScene() {};
	BattleScene(tnl::Vector3 pos ,int background, int EnemyID , Shared<Enemy>enemy);
	virtual~BattleScene()override;

	//�X�V����
	virtual void Update(float delta_time)override;

	//�`��
	virtual void Draw()override;

	//�o�g���E�B���h�E�̕`��
	void BattleDraw();

	//�o�g���V�[�����Ő؂�ւ���֐�
	void MenuUpdate(Player::PlayerStatus& playerStatus,Enemy::EnemyStatus& enemyStatus_);

	//�A�C�e���I�����̏���
	void ItemSelectProcess();

	//�A�C�e���̂��g�����ۂ̏���
	void ItemUseProcess();

	//�X�L���I�����̏���
	void SkillSelectProcess();

	//���j���[�̏�����
	void InitMenuWindow();

	//�A�j���[�V�������̃^�[���`�F���W
	void AnimationTurnChange(int enemyhp, float delta_time);

	//�G�̎��S�����A���o
	void DeadEnemyProcces(Player::PlayerStatus& playerStatus,Enemy::EnemyStatus& enemy_status);

	// ���x���A�b�v���� & ����
	void ChackPlayerLevelUp(Player::PlayerStatus& player_status);

	//HP��MP�o�[�̏���
	void PlayerStatusDraw();

	//�A�C�e���̃h���b�v����
	void ItemDropProcess();

	//�ꎞ�I�ɏオ�����U���͂Ȃǂ����Z�b�g����
	void BattleBuffResetProcess();

	//����̃^�C�v��Set����
	void SetWeaponType();

	//�������I�������ۂ̏���
	void FleeProcess(Player::PlayerStatus& playerStatus, Enemy::EnemyStatus& enemyStatus , float delta_time);


	//�o�g���V�[���̏�Ԃ��Ǘ�����
	enum class BattleState {
		IDLE,	//�o�g�����I�������ҋ@���
		BATTLE, //�퓬
	};

	//�o�g���̏�Ԃ��Z�b�g����
	void SetBattleState(BattleState newState) {
		battle_state = newState;
	}

private:

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

	//�v���C���[�̃^�[���������t���O
	bool PlayerTurn = false;

	//�X�L�����g�p�������̃t���O(�U���n)
	bool Skill_Attack_Flag = false;

	//�X�L�����g�p�������̃t���O(�o�t�n)
	bool Skill_Buff_Flag = false;

	//���Z���g���邩�̃t���O(���Mp�����肽��true�A����Ȃ�������false)
	bool Skill_Chaeck_Flag = true;


//------------------------------------------------------------------------------------------------------------------------
//�V�[�P���X

private:

	TNL_CO_SEQUENCE(BattleScene, &BattleScene::seqIdle);

	bool seqIdle(float delta_time);
	//�v���C���[�̃V�[�P���X
	bool seqPlayerAction(float delta_time);

	//�G�l�~�[�̃V�[�P���X
	bool seqEnemyAction(float delta_time);

	//�J�ڂ�����ׂ̃V�[�P���X
	bool seqChangeTurn(float delta_time);

	//�A�j���[�V�����𗬂��ׂ̃V�[�P���X
	bool seqAnimation(float delta_time);

	enum class Sequence {
		Idle,
		PlayerAction,
		EnemyAction
	};

	Sequence select_sequence = Sequence::Idle;

	//�o�g���V�[���̏��
	BattleState battle_state = BattleState::BATTLE;

//--------------------------------------------------------------------------------------------------------------------------
//�|�C���^�ϐ�
private:

	//---�X�}�[�g�|�C���^---//

	//�G
	Shared<Enemy>enemy = nullptr;
	//�s����I������E�B���h�E
	Shared<MenuWindow>select_comand_menu = nullptr;
	//�o�g�����O�𗬂��E�B���h�E
	Shared<Menu>battle_log_window = nullptr;
	//�X�e�[�^�X��\������E�B���h�E
	Shared<Menu>playerStatus_window = nullptr;
	//�o�g�����O
	Shared<BattleLog>battle_log = nullptr;
	//�U���̍ۂ̃E�B���h�E
	Shared<MenuWindow>attack_window = nullptr;
	//�A�C�e����\������E�B���h�E
	Shared<Menu>item_window = nullptr;
	//�A�C�e�����g���E�B���h�E
	Shared<MenuWindow>select_itemuse_window = nullptr;
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
	//�A�C�e����\������ۂ̍��W
	const tnl::Vector2i item_draw_pos = { 350 , 550 };
	//�A�C�e���̃y�[�W���̕`����W
	tnl::Vector2i item_curent_page = { 400 , 700 };
	//�e�s�̍���
	const int lineHeight = 30;
	//�e�y�[�W
	int currentPage = 0;
	//�J�[�\���̃T�C�Y
	const float cursourSize = 0.3f;
	//�J�[�\����Y���W
	const int curourY = 330;
	//�A�C�e�����g��
	const int ItemUse = 0;
	//�A�C�e�����g���E�B���h�E�����
	const int ItemUseMenuClose = 1;
	//���F
	const int Color_Black = 0;
	//���F
	const int Color_White = -1;
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

	//����̃^�C�v(�ʏ�U���̃G�t�F�N�g�̐؂�ւ��̈�)
	int weapon_type = 0;

public:

	//�o�g���V�[���p�̃X�L���\��
	void InventorySkillDraw();

	//�X�L�����g�p�����ۂ�Mp�m�F
	bool SkillUseMpChack(Player::PlayerStatus& playerStatus);

	//�X�L�����g�p�����ۂ̏���
	void SkillUseProcess(Player::PlayerStatus& playerStatus, Enemy::EnemyStatus& enemyStatus_);


//---------------------------------------------------
private:

	//�S�ł������ɗ���SE�̉�
	const float annihilation_Time = 3.5f;

};