#pragma once
#include "../../dxlib_ext/dxlib_ext.h"
#include"../Manager/ResourceManager.h"
#include"../Manager/SceneManager.h"
#include"../Item/Item.h"
#include"../Object/Player.h"
#include"../Menu/MenuWindow.h"
#include<random>

//-----------------------------------------------------------------------------------------------------
//
//�t�B�[���h�̐퓬�V�[��
//
//-----------------------------------------------------------------------------------------------------

class MapScene;
class RusultScene;
class BattleLog;
class BaseScene;

class BattleScene final: public  BaseScene{

public:

	//�o�g���V�[���̃R���X�g���N�^
	//arg1 �ۊǂ�����W 
	//atg2 �o�g���̔w�i
	//arg3 �G�l�~�[�̃|�C���^ 
	//arg4 �{�X�̎��̓}�b�v���ׂ̈ǂ��̃}�b�v�ɂ��邩���i�[���Ă��� 
	BattleScene(const tnl::Vector3& pos ,const int& background , Shared<Enemy> enemy_pointer ,const int& inmap_state = 0);
	~BattleScene()override;

	//�o�g���V�[���̏�����
	void InitBattleScene();

	//�X�V����
	void Update(float delta_time)override;

	//�`��
	void Draw()override;

	//�o�g���E�B���h�E�̕`��
	void BattleDraw();

	//�o�g���V�[�����Ő؂�ւ���֐�
	void MenuUpdate(Player::PlayerStatus& playerStatus,Enemy::EnemyConnection& enemy_);

	//�A�C�e���I�����̏���
	void ItemSelectProcess();

	//�A�C�e���̂��g�����ۂ̏���
	void ItemUseProcess();

	//�X�L���I�����̏���
	void SkillSelectProcess();

	//���j���[�̏�����
	void InitMenuWindow();

	//�G�̎��S�����A���o
	void DeadEnemyProcces(Player::PlayerStatus& playerStatus,Enemy::EnemyConnection& enemy_);

	// ���x���A�b�v���� & ����
	void ChackPlayerLevelUp(Player::PlayerStatus& player_status);

	//HP��MP�o�[�̏���
	void PlayerStatusDraw();

	//�A�C�e���̃h���b�v����
	void ItemDropProcess(Enemy::EnemyConnection& enemy_);

	//�ϓ������l�����Z�b�g����
	void BattleBuffResetProcess();

	//����̃^�C�v��Set����
	void SetWeaponType();

	//�������I�������ۂ̏���
	void FleeProcess(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemy_ , float delta_time);

	//�o�g�����I�������ɓG�̎�ނɂ���ď�����ς���
	void BattleEndProcces(const int& enemy_id);

	//����̃{�X��ł̏���
	bool CheckRequiredItemEnemyProcess();

	//�o�g���V�[���̏�Ԃ��Ǘ�����
	enum class BattleState {
		//�o�g�����I��
		BATTLE_END,
		//�퓬
		BATTLE,
		//�퓬�I�����̃V�[���J�ڂ̑ҋ@���
		IDLE
	};

	//�o�g���̏�Ԃ��Z�b�g����
	void SetBattleState(const BattleState& newState) {
		battle_state = newState;
	}

private:

	//�w�i�̉摜
	int back_ground_ = 0;

	//���W���ꎞ�I�ɕۑ����Ă����ϐ�
	tnl::Vector3 map_pos = {};

	//�������m��
	int probability = 50;

	//InMapScene�̏�Ԃ�ۊǂ��Ă����ϐ�
	//�����n���Đ퓬�I�����ɂǂ̃}�b�v���ɋ��邩�𔻒f����
	int inmap_state_save = 0;

	//���X�{�X��|������̍��W
	const tnl::Vector3 FIRST_POS_ = { 195, 312, 0 };

	//�o�g���V�[���̐؂�ւ�
	void BattleSceneUpdate(const float delta_time);

	//�G�̍U��
	void EnemyTurnProcess();


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

	//�V�[�P���X
	enum class Sequence {
		//���ꂼ��̃X�s�[�h�����ɂ߂ă^�[�������߂�
		IDLE,
		//�v���C���[�̍s���^�[��
		PLAYERACTION,
		//�G�l�~�[�̍s���^�[��
		ENEMYACTION
	};

	//�V�[�P���X�̏��
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
	//�G���{�X�̏ꍇ�̂ݎg�p����X�}�[�g�|�C���^
	Weak<BossMonster>boss_monster_;
	
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
		//�ŏ��̑I��(�������� , ���� , ������)
		FIRST_ACTION,
		//����������I�񂾎��̑I��(�U�� , ���Z , �߂�)
		FIGHT_ACTION,
		//�����I�񂾎��̑I��(����ꗗ)
		ITEM_ACTION,
		//�A�C�e����I�������ۂ̎g�����ǂ����̑I��
		ITEM_USE_ACTION,
		//���Z��I���������̑I��(���Z�ꗗ)
		SKILL_ACTION,
	};

	//���ǂ̏�Ԃ�
	MenuAction select_action_menu = MenuAction::FIRST_ACTION;

	//�\��������A�C�e����
	const int ITEMPERPAGE_ = 4;

	//�A�C�e����\������y���W
	const int STARTY = 550;

	//�A�C�e����\������ۂ̍��W
	const tnl::Vector2i ITEM_DRAW_POS = { 400 , 540};

	//�X�L����\������ʒu
	const tnl::Vector2i Skill_DRAW_POS = { 350 , 540 };

	//�A�C�e���̃y�[�W���̕`����W
	const tnl::Vector2i ITEM_CURENT_PAGE = { 400 , 690};

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
	const tnl::Vector2i MENU_COMENT_POS_FIRST = { 100 , 540};
	//�v�f���
	const tnl::Vector2i MENU_COMENT_POS_SECOND = { 100 , 590 };
	//�v�f�O��
	const tnl::Vector2i MENU_COMENT_POS_THIRD = { 100 , 640};

	//�A�C�e���g�p���̃��j���[�E�B���h�E���W
	
	//�g��
	const tnl::Vector2i MENU_ITEM_USE_POS = { 650 , 550 };
	//����
	const tnl::Vector2i MENU_ITEM_CLOSE = { 650 , 600 };

	//�o�g�����O�E�B���h�E�̍��W
	const tnl::Vector2i BATTLE_LOG_WINDOW_POS = { 670, 30 };

	//�o�g�����O�E�B���h�E�̃T�C�Y
	const int BATTLE_LOG_WINDOW_WIDTH = 600;
	const int BATTLE_LOG_WINDOW_HEIGHT = 180;

	//���j���[�E�B���h�E�̍��W
	const tnl::Vector2i MENU_WINDOW_POS = { 50, 500 };

	//���j���[�E�B���h�E�̃T�C�Y
	const int MENU_WINDOW_WIDTH = 250;
	const int MENU_WINDOW_HEIGHT = 200;

	//�o�g���V�[���Ŏg���A�C�e���E�B���h�E�̍��W
	const tnl::Vector2i ITEM_WINDOW_POS = { 300, 500 };

	//�o�g���V�[���Ŏg���A�C�e���E�B���h�E�̃T�C�Y
	const int ITEM_WINDOW_WIDTH = 300;
	const int ITEM_WINDOW_HEIGHT = 215;

	//�A�C�e����I�������ۂ̎g���I���R�}���h�̃E�B���h�E�̍��W
	const tnl::Vector2i SELECT_ITEM_USE_WINDOW_POS = { 600, 500 };

	//�X�L���E�B���h�E�̍��W
	const tnl::Vector2i SKILL_WINDOW_POS = { 300, 500 };

	//�X�L���E�B���h�E�̃T�C�Y
	const int SKILL_WINDOW_WIDTH = 270;
	const int SKILL_WINDOW_HEIGHT = 215;

	//�v���C���[�X�e�[�^�X�̃E�B���h�E�̍��W
	const tnl::Vector2i PLAYER_STATUS_WINDOW_POS = { 50, 50 };

	//�v���C���[�X�e�[�^�X�E�B���h�E�̃T�C�Y
	const int PLAYER_STATUS_WINDOW_WIDTH = 250;
	const int PLAYER_STATUS_WINDOW_HEIGHT = 250;

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
	void SkillUseProcess(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemy_);


//---------------------------------------------------
private:

	//�S�ł������ɗ���SE�̉�
	const float ANNIHILATION_TIME = 3.5f;

};