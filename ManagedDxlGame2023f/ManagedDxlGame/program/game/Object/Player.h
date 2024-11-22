#pragma once
//------------------------------------------------------------------------------------------------------------
//
//�v���C���[�Ɋւ���N���X
//
//-------------------------------------------------------------------------------------------------------------


#include "../../dxlib_ext/dxlib_ext.h"
#include"Actor.h"
#include"../System/Camera.h"
#include"Enemy.h"

class BattleLog;
class Skill;
class Nomal_Attack;
class InMapScene;

class Player final: public Actor {
public:

	Player();
	~Player();

	//player�̃X�e�[�^�X
	struct PlayerStatus {
	public:

		//level���擾����
		int GetLevel() const {
			return level;
		}
		//maxHp���擾����
		int GetMaxHp()const {
			return maxHp;
		}
		//hp���擾����
		int GetcurentHp() const{
			return curenthp;
		}
		//Attack���擾����
		int GetAttack() const{
			return Attack;
		}
		//��b�U���͂��擾����
		int GetBaseAttack()const {
			return base_attack;
		}
		//��b�h��͂��擾����
		int GetBaseDefance()const {
			return base_defance;
		}
		//Defance���擾����
		int GetDefance() const {
			return Defance;
		}
		//Speed���擾����
		int GetSpeed() const {
			return Speed;
		}
		//�K�v�Ȍo���l���擾����
		int GetExpoint() const {
			return R_expoint;
		}
		//�ő�MP���擾����
		int GetMaxMp() const {
			return maxMp;
		}
		//����Mp���擾����
		int GetCurentMp()const {
			return curentMp;
		}
		//���@�͂��擾����
		int GetMagicPower()const {
			return magicPower;
		}

		//level���Z�b�g����
		void SetPlayerLevel(const int newlevel) {
			level = newlevel;
		}
		//maxHp���Z�b�g����
		void SetPlayerMaxHp(const int newmaxhp) {
			maxHp = newmaxhp;
		}
		//hp���Z�b�g����
		void SetPlayerCurentHp(const int newhp) {
			curenthp = newhp;
		}
		//Attack���Z�b�g����
		void SetPlayerAttack(const int newattack) {
			Attack = newattack;
		}
		//Defance���Z�b�g����
		void SetPlayerDefance(const int newdefance) {
			Defance = newdefance;
		}
		//��b�U���͂��Z�b�g����
		void SetPlayerBaseAttack(const int new_attack) {
			base_attack = new_attack;
		}
		//��b�h��͂��Z�b�g����
		void SetBaseDefance(const int new_defance) {
			base_defance = new_defance;
		}
		//Speed���Z�b�g����
		void SetPlayerSpeed(const int newspeed) {
			Speed = newspeed;
		}
		//�K�v�Ȍo���l���Z�b�g����
		void SetPlayerExpoint(const int newexpoint) {
			R_expoint = newexpoint;
		}
		//�ő�Mp���Z�b�g����
		void SetPlayerMaxMp(const int newmaxmp) {
			maxMp = newmaxmp;
		}
		//���݂�Mp���Z�b�g����
		void SetPlayerCurentMp(const int newmp) {
			curentMp = newmp;
		}
		//���@�͂��Z�b�g����
		void SetMagicPower(const int newmagicpower) {
			magicPower = newmagicpower;
		}

	private:

		//�v���C���[�̃��x��
		int level;
		//�v���C���[�̍ő�HP
		int maxHp;
		//�v���C���[�̌��݂�HP
		int curenthp;
		//�v���C���[�̊�b�U����
		int base_attack;
		//�v���C���[�̍U����
		int Attack;
		//�v���C���[�̊�b�h���
		int base_defance;
		//�v���C���[�̖h���
		int Defance;
		//�v���C���[�̑f����
		int Speed;
		//�v���C���[�̕K�v�Ȍo���l
		int R_expoint;
		//�v���C���[�̍ő�MP
		int maxMp;
		//�v���C���[�̌��݂�MP
		int curentMp;
		//�v���C���[�̖��@��
		int magicPower;
	};

	//�A�j���[�V����
	enum {
		//��
		DIR_DOWN,
		//��
		DIR_LEFT,
		//�E
		DIR_RIGHT,
		//��
		DIR_UP,
		//����
		DIR_DIAGONAL_DOWN_LEFT,
		//�E��
		DIR_DIAGONA_DOWN_RIGTH,
		//����
		DIR_DIAGONAL_UPPER_LEFT,
		//�E��
		DIR_DIAGONAL_UPPER_RIGTH,
		//�ő�l
		DIR_MAX
	};

	//�v���C���[�̏��
	enum class PlayerState {
		//�I�����
		CHOICE,
		//�ʏ�U��
		NOMALATTACK,
		//�X�L���g�p��
		SKILL,
		//������
		FLEE,
		//���S
		DEAD,
		//�ҋ@���
		IDLE
	};

	//�v���C���[�̌����ڂ̏��
	enum PlayerEquipState {
		//�f��
		BARE_HANDS,
		//��
		SWORD,
		//�n���}�[
		HAMMER,
		//��
		SPEAR,
		//�����O�\�[�h
		LONGSWORD
	};

	//�v���C���[�̏�Ԃ�؂�ւ���
	void SetPlayerState(PlayerState new_player_state) {
		player_state = new_player_state;
	}

	//�v���C���[�̏�Ԃ��擾����
	PlayerState GetPlayerState(){
		return player_state;
	}

	//�v���C���[�̃o�g�����̍s��
	//arg_1 : Delta_time
	//arg_2 : �o�g�����O�̃X�}�[�g�|�C���^
	//arg_3 : �G�̃X�}�[�g�|�C���^
	//arg_4 : �ʏ�U���̃X�}�[�g�|�C���^
	void PlayerMoveProcess(float delta_time, Shared<BattleLog>& basttle_log , Shared<Enemy>& enemy, Shared<Nomal_Attack>& nomal_attack);

	//�v���C���[�̍s�������̃t���O���擾����
	bool GetPlayerControl()const {
		return play_control;
	}

	//�v���C���[�̖��O���Z�b�g����
	void SetPlayerName(const std::string& new_player_name) {
		name = new_player_name;
	}

	//�v���C���[�̖��O���擾����
	const std::string& GetPlayerName()const {
		return name;
	}

	//�v���C���[��ID���擾����
	int GetPlayerID()const {
		return player_id;
	}

	//�v���C���[��ID���Z�b�g����
	void SetPlayerID(const int new_player_id) {
		player_id = new_player_id;
	}

	//�v���C���[�̃A�C�R�����擾����
	int GetPlayerIcon()const {
		return player_icon;
	}
	//�v���C���[�̍��W���Z�b�g����
	void SetPlayerPosition(const tnl::Vector3& new_pos) {
		plyer_pos = new_pos;
		prev_pos = new_pos;
	}

	//�v���C���[�̃X�e�[�^�X�����[�h����
	void PlyStatusLoad();

	//�v���C���[�̉摜���Z�b�g����
	//���csv�ȂǂɋL�q���Ă܂Ƃ߂Đݒ肷��
	//arg_1 : �摜��ID(�����Ȃǂŕ�����ۂ�ID)
	//arg_2 : �v���C���[��ID
	void SetPlayerAnimationHdl(const int& ghdl_id, const int& player_id);

	//�v���C���[�̓���
	//arg_1 : delta_time 
	//arg_2 : �������x�@
	//arg_3 : �}�b�v�̍���(�ړ������̈�)
	void Player_Move(const float& delta_time, const float& velocity, const int& MAPHEIGHT);

	//�v���C���[�̕`��
	//arg_1 : �J���� 
	//arg_2 : �v���C���[�̃T�C�Y
	void Player_draw(const KonCamera& camera, const float& scale);

	//�v���C���[�̍��W���擾����
	tnl::Vector3& GetPlayerPos() { return plyer_pos; }

	//�v���C���[�̈ړ��O���W���擾����
	tnl::Vector3& GetPlayerPrevPos() { return prev_pos; }

	//�v���C���[�̓����𐧌䂷��
	void PlayerControlChangeFlag() {
		play_control = !play_control;
	}

	//�v���C���[�̃A�j���[�V��������
	void PlayerAnimation(const float& delta_time);

	//���������擾����
	int GetPlayerMoney()const { return money; }

	//��������ǉ�����
	void AddPlayerMoney(const int& add_money) {
		money += add_money;
	}

	//�����������炷
	void ReducePlayerMoney(const int& reduce_money) {
		money -= reduce_money;
	}

	//�v���C���[�̃T�C�Y���擾����
	const tnl::Vector2i& GetPlayerSize()const {
		return CHARASIZE;
	}

	//�v���C���[�̃X�e�[�^�X�z����擾����
	PlayerStatus& GetPlayerStatusSave() {
		return player_status_save;
	}

	//�v���C���[�Ƀ}�b�v�̏�Ԃ��Z�b�g����(��Ƀv���C���[���Z�[�u�̍ۂǂ��ɋ��邩�F�������)
	void SetPlayerCurentMapMemory(int curent_inmap) {
		player_curent_map_memory = curent_inmap;
	}

	//�v���C���[���ǂ̃}�b�v�̏�Ԃɂ��邩�擾����
	int GetPlayerCurentMapState()const {
		return player_curent_map_memory;
	}

	//�擾����ID�̃X�L����ǉ�����
	//��ɃZ�[�u��f�o�b�N�Ɏg�p����
	//arg_1 : �X�L��ID
	void SaveReadSkill(const int& skill_id);

	//�ő僌�x�����擾����
	int GetMaxLevel() const {
		return MAX_LEVEL;
	}

	//�v���C���[�̃X�e�[�^�X�𑼂̃N���X�ȂǂŎ擾����
	PlayerStatus GetPlyerStatus(const int& level) const;

	//�X�e�[�^�X���Z�b�g����
	void SetPlayerStatus(const int& level) {
		player_status_save = GetPlyerStatus(level);
	}

	//Player�̃A�j���[�V������ω�������
	void AnimationChange(const int& number) {
		anim_ctrl_dir = number;
	}

	//������0�ɂ���
	void StepReset() {
		numberStep = 0;
	}
	//�������擾����
	int GetStep()const {
		return numberStep;
	}

	//�X�L���̔z����擾����
	const std::vector<Shared<Skill>>& getSkillList() {
		return player_skill_list;
	}

	//�v���C���[�̎��S����
	void DeadPlayerProcess(Shared<BattleLog>& battle_log);

	//�v���C���[�̍U������
	void PlayerAttackProcess(Enemy::EnemyConnection& enemy_status, Shared<BattleLog>& battle_log, Shared<Nomal_Attack>& nomal_attack);

	//�X�L�����Z�b�g����
	//���x���ɉ����ăX�L����ǉ�����
	//arg_1 : �o�g�����O�̃|�C���^(��Ƀo�g���V�[���Ŏ擾�����Ƃ������O�𗬂���)
	void SkillSet(const Shared<BattleLog>& battle_log);

	//���Z��ǉ�����
	void AddSkill(const Shared<Skill>& skill);

	//�Z�[�u���[�h
	void PlayerSave();
	bool PlayerLoad();


	//�v���C���[�̕\���t���O��؂�ւ���
	//�v���C���[���ꎞ�I�Ɍ����Ȃ������
	void PlayerDisplayChange() {
		player_display_flag = !player_display_flag;
	}

	//�v���C���[�̕\���t���O���擾����
	bool GetPlayerDisplayFlag()const {
		return player_display_flag;
	}

	//�X�L����S�Ēǉ�����(�f�o�b�N�p)
	void DebugAddSkill();

	//�I�������X�L������������
	void SkillRemove(const int& skill_id);

private:

	//---�v���C���[�֌W---//

	//���O
	std::string name;

	//�v���C���[�̃}�b�N�X���x��
	const int MAX_LEVEL = 20;

	//�v���C���[��ID
	int player_id = 0;

	//���݂̃v���C���[�̏��
	PlayerState player_state = PlayerState::CHOICE;

	//plyer�̕���
	int numberStep = 0;

	//plyer�̓����̐���
	bool play_control = false;

	//player�̈ړ��O���W
	tnl::Vector3 prev_pos = { 0 ,0,0 };

	//�v���C���[�̍���
	//�v���C���[�̕�
	const tnl::Vector2i CHARASIZE = { 36 ,72 };

	//�v���C���[�̍��W
	tnl::Vector3 plyer_pos = {};

	//�v���C���[�̂���
	int money = 50;

	//�X�e�[�^�X���
	PlayerStatus player_status_save;

	//�v���C���[�̃X�e�[�^�X�\����퓬���̃A�C�R���̃n���h��
	std::string PLAYER_BATTLE_ICON[4] =
	{
	 "graphics/Player/player_image_0/Player_Icon_sentou.png",
	 "graphics/Player/player_image_01/Player_Icon_sentou.png",
	 "graphics/Player/player_girl_image_02/Player_Icon_sentou.png",
	 "graphics/Player/player_girl_image_03/Player_Icon_sentou.png"
	};

	//�v���C���[�̃X�e�[�^�X�\����퓬���̃A�C�R��
	int player_icon = 0;

	//�X�L���̃X�}�[�g�|�C���^
	Shared<Skill>skill = nullptr;

	//�A�j���[�V�����Ɏg���ϐ�
	float anim_time_count = 0;
	int anim_ctrl_dir = DIR_DOWN;
	int anim_ctrl_frame = 0;
	int anim_hdls[DIR_MAX][9];

	//�\���̂��i�[����z��
	std::list<PlayerStatus>player_status_type;

	//csv�ǂݎ��p�̔z��
	std::vector<std::vector<tnl::CsvCell>>plyer_status_csv_info;

	//���Z�p�̃C���x���g��
	std::vector<Shared<Skill>> player_skill_list;

	//���Z�̐�
	int skill_num = 0;

	//�v���C���[�ɕۑ����Ă����}�b�v�̏�Ԃ̏��
	int player_curent_map_memory = 0;

	//---�t���O�֌W---//

	//�v���C���[�̕\�����Ǘ�����t���O
	bool player_display_flag = true;
};

