#pragma once
///
///�v���C���[�Ɋւ���N���X
/// 


#include "../../dxlib_ext/dxlib_ext.h"
#include"Actor.h"
#include"../System/Camera.h"
#include"Enemy.h"

class BattleLog;
class SoundManager;
class GameManager;
class SceneManager;
class Skill;
class Nomal_Attack;
class CsvManager;
class InMapScene;
class CsvManager;

class Player : public Actor {
public:

	
	Player();
	~Player();

	void Update(float delta_time);

	//player�̃X�e�[�^�X
	struct PlayerStatus {
	public:

		//level���擾����
		int GetLevel() const {
			return level;
		}
		//maxHp���擾����
		float GetMaxHp()const {
			return maxHp;
		}
		//hp���擾����
		float GetcurentHp() const{
			return curenthp;
		}
		//Attack���擾����
		int GetAttack() const{
			return Attack;
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
		float GetMaxMp() const {
			return maxMp;
		}
		//����Mp���擾����
		float GetCurentMp()const {
			return curentMp;
		}
		//���@�͂��擾����
		int GetMagicPower()const {
			return magicPower;
		}
		//�v���C���[��ID���擾����
		int GetPlayerID()const {
			return player_id;
		}

		//level���Z�b�g����
		void SetPlayerLevel(int newlevel) {
			level = newlevel;
		}
		//maxHp���Z�b�g����
		void SetPlayerMaxHp(float newmaxhp) {
			maxHp = newmaxhp;
		}
		//hp���Z�b�g����
		void SetPlayerCurentHp(float newhp) {
			curenthp = newhp;
		}
		//Attack���Z�b�g����
		void SetPlayerAttack(int newattack) {
			Attack = newattack;
		}
		//Defance���Z�b�g����
		void SetPlayerDefance(int newdefance) {
			Defance = newdefance;
		}
		//Speed���Z�b�g����
		void SetPlayerSpeed(int newspeed) {
			Speed = newspeed;
		}
		//�K�v�Ȍo���l���Z�b�g����
		void SetPlayerExpoint(int newexpoint) {
			R_expoint = newexpoint;
		}
		//�ő�Mp���Z�b�g����
		void SetPlayerMaxMp(float newmaxmp) {
			maxMp = newmaxmp;
		}
		//���݂�Mp���Z�b�g����
		void SetPlayerCurentMp(float newmp) {
			curentMp = newmp;
		}
		//���@�͂��Z�b�g����
		void SetMagicPower(int newmagicpower) {
			magicPower = newmagicpower;
		}
		//�v���C���[��ID���Z�b�g����
		void SetPlayerID(int new_player_id) {
			player_id = new_player_id;
		}

	private:

		//�v���C���[�̃��x��
		int level;
		//�v���C���[�̍ő�HP
		float maxHp;
		//�v���C���[�̌��݂�HP
		float curenthp;
		//�v���C���[�̍U����
		int Attack;
		//�v���C���[�̖h���
		int Defance;
		//�v���C���[�̑f����
		int Speed;
		//�v���C���[�̕K�v�Ȍo���l
		int R_expoint;
		//�v���C���[�̍ő�MP
		float maxMp;
		//�v���C���[�̌��݂�MP
		float curentMp;
		//�v���C���[�̖��@��
		int magicPower;
		//�v���C���[��ID
		int player_id;

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
		IDLE,
		NOMALATTACK,
		SKILL,
		FLEE,
		DEAD,
	};

	//�v���C���[�̏�Ԃ�؂�ւ���
	void SetPlayerState(PlayerState new_player_state) {
		player_state = new_player_state;
	}

	//�v���C���[�̏�Ԃ��擾����
	PlayerState GetPlayerState(){
		return player_state;
	}

	//�v���C���[�̍s��
	void PlayerMoveProcess(float delta_time, Shared<BattleLog>& basttle_log , Shared<Enemy>& enemy, Shared<Nomal_Attack>& nomal_attack);

	bool getPlayerControl()const {
		return plyControl;
	}

private:

	//---�v���C���[�֌W---//

	//���݂̃v���C���[�̏��
	PlayerState player_state = PlayerState::IDLE;

	float first_pos_x = 195;						//�Q�[�����n�܂������̂����W
	float first_pos_y = 312;						//�Q�[�����n�܂�������y���W
	int Total_Frame = 3;							//�v���C���[�̑��t���[��(3)
	int Horizontal_frame = 3;						//�v���C���[�̉��t���[��(3)
	int Vertical_frame = 1;							//�v���C���[�̏c�t���[��(1)
	int Horizontal_oneframe = 48;					//�������� 1�t���[��������̕�(48)
	int Vertical_oneframe = 48;						//�c������ 1�t���[��������̕�(48)

	//plyer�̕���
	int numberStep = 0;

	//plyer�̓����̐���
	bool plyControl = false;

	//player�̈ړ��O���W
	tnl::Vector3 prev_pos = { 0 ,0,0 };

	//�v���C���[�̍���
	//�v���C���[�̕�
	const tnl::Vector2i CHARASIZE = { 36 ,72 };

	//�v���C���[�̍��W
	tnl::Vector3 plyer_pos = {};

	//�v���C���[�̂���
	int money = 0;

	//�X�e�[�^�X�̃C���f�b�N�X
	int PlyerIndex = 0;

	//�X�e�[�^�X���
	PlayerStatus plyerstatusSave;

	//���O
	std::string name = "";

	//����̃^�C�v(�ʏ�U���̃G�t�F�N�g�̐؂�ւ��̈�)
	int weapon_type = 0;

public:
	
	 void SetPlayerPosition(const tnl::Vector3& new_pos) {
		plyer_pos = new_pos;
	 }

	//�v���C���[�̃X�e�[�^�X�����[�h����
	void PlyStatusLoad();

	//�v���C���[�̉摜���Z�b�g����
	void SetPlayerAnimationHdl(int ghdl_id);

	//�v���C���[�̓���
	//���� : Delta_time , �������x�@, �}�b�v�̍���(�ړ������̈�)
	void player_Move(float delta_time, const float& velocity ,const int MAPHEIGHT);

	//�v���C���[�̕`��
	//���� : �J���� , �v���C���[�̃T�C�Y
	void player_draw(const KonCamera& camera, float scale);

	//�v���C���[�̍��W���擾����
	tnl::Vector3&  getPlayerPos() { return plyer_pos; }

	//�v���C���[�̈ړ��O���W���擾����
	tnl::Vector3& getPlayerPrevPos(){ return prev_pos; }
	
	//�v���C���[�̓����𐧌䂷��
	void setPlayerControl() {
		plyControl = !plyControl;
	}


	//���������擾����
	inline int getPlayerMoney(){ return money; }

	//��������ǉ�����
	inline void AddPlayerMoney(int add_money) {
		money += add_money;
	}

	//�����������炷
	inline void ReducePlayerMoney(int reduce_money) {
		money -= reduce_money;
	}

	//�v���C���[�̃T�C�Y���擾����
	int GetPlayerWidth()const {
		return CHARASIZE.x;
	}
	int GetPlayerHight()const {
		return CHARASIZE.y;
	}
	tnl::Vector2i GetPlayerSize()const {
		return CHARASIZE;
	}

	//�v���C���[�̃X�e�[�^�X�z����擾����
	PlayerStatus& getPlayerStatusSave(){
		return plyerstatusSave;
	}

	//�v���C���[�Ƀ}�b�v�̏�Ԃ��Z�b�g����(��Ƀv���C���[���Z�[�u�̍ۂǂ��ɋ��邩�F�������)
	void SetPlayerCurentMapMemory(int curent_inmap) {
		player_curent_map_memory = curent_inmap;
	}

	//�v���C���[���ǂ̃}�b�v�̏�Ԃɂ��邩�擾����
	int GetPlayerCurentMapState()const {
		return player_curent_map_memory;
	}

	//�X�L����ǉ�����
	void SaveReadSkill(int skill_id);

private:

	//�A�j���[�V�����Ɏg���ϐ�
	float anim_time_count = 0;
	int anim_ctrl_dir = DIR_DOWN;
	int anim_ctrl_frame = 0;
	int anim_hdls[DIR_MAX][9];

	//�\���̂̑���
	int Ply_Status_Total_num;

	//�\���̂��i�[����z��
	std::list<PlayerStatus>Ply_Status_Type;

	//csv�ǂݎ��p�̔z��
	std::vector<std::vector<tnl::CsvCell>>PlyerStatus_Csv_Info;

	//�S�ł������ɗ���SE�̉�
	const float annihilation_Time = 3.5f;

	//���Z�p�̃C���x���g��
	std::vector<Shared<Skill>> player_skill_list;

	//���Z�̐�
	int SkillNum = 0;

	//�v���C���[�ɕۑ����Ă����}�b�v�̏�Ԃ̏��
	int player_curent_map_memory = 0;

	//---�t���O�֌W---//


public:

	//�v���C���[�̃X�e�[�^�X�𑼂̃N���X�ȂǂŎ擾����
	PlayerStatus GetPlyerStatus(int level) const;

	//�X�e�[�^�X���Z�b�g����
	void SetPlayerStatus(int level) {
		plyerstatusSave = GetPlyerStatus(level);
	}

	//Player�̃A�j���[�V������ω�������
	void AnimationChange(int number) {
		anim_ctrl_dir = number;
	}

	//������0�ɂ���
	void StepReset() {
		numberStep = 0;
	}
	//�������擾����
	int GetStep() {
		return numberStep;
	}

	//�X�L���̔z����擾����
	std::vector<Shared<Skill>>& getSkillList() {
		return player_skill_list;
	}

	//�X�L���̌����擾����
	int getSkillNum() {
		return SkillNum;
	}

	//�v���C���[�̎��S����
	void DeadPlayerProcess(Shared<BattleLog>& battle_log);

	//�v���C���[�̍U������
	void PlayerAttackProcess(Enemy::EnemyConnection& enemy_status,Shared<BattleLog>& battle_log , Shared<Nomal_Attack>& nomal_attack);

	//�X�L�����Z�b�g����
	void SkillSet(Shared<BattleLog> battle_log = nullptr);

	//���Z��ǉ�����
	void AddSkill(const Shared<Skill>& skill);

	//�Z�[�u���[�h�@�\���������i�̂��ɒǉ��\��j
	void PlyerSave();
	void PlyerLoad();

};

