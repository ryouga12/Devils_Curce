#pragma once
#include "../../dxlib_ext/dxlib_ext.h"
#include"../Manager/ResourceManager.h"
#include"../Manager/SceneManager.h"
#include"../Item/Item.h"
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
class BattleLog;
class BaseScene;
class GameManager;
class UIManager;

class BattleScene : public  BaseScene{

public:

	BattleScene() {};
	BattleScene(tnl::Vector3 pos ,int background , Shared<Enemy> enemy);
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
	int back_ground_ = 0;

	//���W���ꎞ�I�ɕۑ����Ă����ϐ�
	tnl::Vector3 map_pos = {};

	//�x��������b��
	const float Time = 1.0f;

	//�������m��
	int probability = 50;

	//�ʏ�h���b�v
	const float NOMALDROP = 30;

	//���A�h���b�v
	const float RAREDROP = 10;

//------------------------------------------------------------------------------------------------------------------------
//���ꂼ��̃t���O

	//�v���C���[�̃^�[���������t���O
	bool PlayerTurn = false;

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
		IDLE,
		PLAYERACTION,
		ENEMYACTION
	};

	Sequence select_sequence = Sequence::IDLE;

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
	//�E�B���h�E
	Weak<Menu>window_log;
	//�o�g�����O
	Shared<BattleLog>battle_log = nullptr;
	//�U���̍ۂ̃E�B���h�E
	Shared<MenuWindow>attack_window = nullptr;
	//�A�C�e�����g���E�B���h�E
	Shared<MenuWindow>select_itemuse_window = nullptr;
	//�ʏ�U��
	Shared<Nomal_Attack>nomal_attack = nullptr;
	//�A�C�e��
	Shared<Item>item_ = nullptr;
	
	//---�|�C���^---//

	//---�E�B���h�E---//
	
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
		FIRST_ACTION,
		FIGHT_ACTION,
		ITEM_ACTION,
		ITEM_USE_ACTION,
		SKILL_ACTION,
	};

	MenuAction select_action_menu = MenuAction::FIRST_ACTION;

	//�\��������A�C�e����
	const int ITEMPERPAGE_ = 4;
	//�A�C�e����\������y���W
	const int STARTY = 550;
	//�A�C�e����\������ۂ̍��W
	const tnl::Vector2i ITEM_DRAW_POS = { 350 , 550 };
	//�A�C�e���̃y�[�W���̕`����W
	const tnl::Vector2i ITEM_CURENT_PAGE = { 400 , 700 };
	//�e�s�̍���
	const int LINEHEIGHT = 30;
	//�e�y�[�W
	int currentPage = 0;
	//�J�[�\���̃T�C�Y
	const float CURSOURSIZE = 0.3f;
	//�J�[�\����Y���W
	const int CUROURY = 330;
	//�A�C�e�����g��
	const int ITEMUSE_ = 0;
	//�A�C�e�����g���E�B���h�E�����
	const int ITEMUSEMENUCLOSE_ = 1;
	//���j���[�I���ōU�����I�����ꂽ�ꍇ
	const int ATTACK_ = 0;
	//���j���[�I���œ���I�����ꂽ�ꍇ
	const int ITEM_ = 1;
	//���j���[��I���œ����邪�I�����ꂽ��
	const int FLEE_ = 2;
	//�U���̑I����ʂœ��Z���I�����ꂽ��
	const int SPECIAL_SKILL_ = 1;
	//�I����ʂŕ��邪�����ꂽ��
	const int MENU_CLOSE_ = 2;

	//---���j���[�n�̍��W---//

	//���j���[�E�B���h�E�̋��ʍ��W
	
	//�v�f���
	const tnl::Vector2i MENU_COMENT_POS_FIRST = { 100 , 550 };
	//�v�f���
	const tnl::Vector2i MENU_COMENT_POS_SECOND = { 100 , 600 };
	//�v�f�O��
	const tnl::Vector2i MENU_COMENT_POS_THIRD = { 100 , 650 };

	//�A�C�e���g�p���̃��j���[�E�B���h�E���W
	
	//�g��
	const tnl::Vector2i MENU_ITEM_USE_POS = { 600 , 550 };
	//����
	const tnl::Vector2i MENU_ITEM_CLOSE = { 600 , 600 };
//--------------------------------------------------------------------------------------------------------------------------
//���O�֘A
private:

//--------------------------------------------------------------------------------------------------------------------------
//�X�L���֘A
private:

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
	const float ANNIHILATION_TIME = 3.5f;

};