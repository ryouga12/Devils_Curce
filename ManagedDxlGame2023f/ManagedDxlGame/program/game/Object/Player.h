#pragma once
///
///�v���C���[�Ɋւ���N���X
/// 


#include "../../dxlib_ext/dxlib_ext.h"
#include"Actor.h"
#include"../Manager/Camera.h"

//�A�j���[�V������؂�ւ���b��
static const float ChangeAnimationTime = 0.1f;
//�t���[��
static const int animationFlame = 3;

//�A�j���[�V����
enum {
	//��
	DIR_DOWN,
	//��
	DIR_UP,
	//��
	DIR_LEFT,
	//�E
	DIR_RIGHT,
	//�ő�l
	DIR_MAX
};

class Player : public Actor {
public:

	
	Player();
	~Player();

	void Update(float delta_time);

	//player�̃X�e�[�^�X
	struct PlayerStatus {
	public:

		//level��Ԃ��Q�b�^�[
		int getLevel() const {
			return level;
		}
		//maxHp��Ԃ��Q�b�^�[
		float getMaxHp()const {
			return maxHp;
		}
		//hp��Ԃ��Q�b�^�[
		float getcurentHp() const{
			return curenthp;
		}
		//Attack��Ԃ��Q�b�^�[
		int getAttack() const{
			return Attack;
		}
		//Defance��Ԃ��Q�b�^�[
		int getDefance() const {
			return Defance;
		}
		//Speed��Ԃ��Q�b�^�[
		int getSpeed() const {
			return Speed;
		}
		//�K�v�Ȍo���l��Ԃ��Q�b�^�[
		int getExpoint() const {
			return R_expoint;
		}
		//�ő�MP��Ԃ��Q�b�^�[
		float getMaxMp() const {
			return maxMp;
		}
		//����Mp��Ԃ��Q�b�^�[
		float getCurentMp()const {
			return curentMp;
		}
		//���@�͂�Ԃ��Q�b�^�[
		int getMagicPower()const {
			return magicPower;
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
			
	private:

		int level;
		float maxHp;
		float curenthp;
		int Attack;
		int Defance;
		int Speed;
		int R_expoint;
		float maxMp;
		float curentMp;
		int magicPower;

	};

private:

	//---�v���C���[�֌W---//
	float first_pos_x = 195;						//�Q�[�����n�܂������̂����W
	float first_pos_y = 312;						//�Q�[�����n�܂�������y���W
	int Total_Frame = 3;							//�v���C���[�̑��t���[��(3)
	int Horizontal_frame = 3;						//�v���C���[�̉��t���[��(3)
	int Vertical_frame = 1;							//�v���C���[�̏c�t���[��(1)
	int Horizontal_oneframe = 32;					//�������� 1�t���[��������̕�(32)
	int Vertical_oneframe = 32;						//�c������ 1�t���[��������̕�(32)

	//plyer�̕���
	int numberStep = 0;

	//plyer�̓����̐���
	bool plyControl = false;

	//player�̈ړ��O���W
	tnl::Vector3 prev_pos = { 0 ,0,0 };

	//�v���C���[�̍���
	const int CHARA_HEIGHT = 48;	

	//�v���C���[�̕�
	const int CHARA_WIDTH = 48;						

	//�v���C���[�̃T�C�Y���擾����ׂ̃^�C�v
	int Width = 1; int Height = 2;

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

public:
	
	 void SetPlayerPosition(const tnl::Vector3& new_pos) {
		plyer_pos = new_pos;
	 }

	//�v���C���[�̃X�e�[�^�X�����[�h����
	void PlyStatusLoad();


	//�v���C���[�̓���
	void player_Move(float delta_time, const float& velocity);

	//�v���C���[�̕`��
	void player_draw(const KonCamera& camera, float scale);

	//�v���C���[�̍��W���擾����
	tnl::Vector3&  getPlayerPos() { return plyer_pos; }

	//�v���C���[�̈ړ��O���W���擾����
	tnl::Vector3& getPlayerPrevPos(){ return prev_pos; }
	
	//�v���C���[�̓����𐧌䂷��
	void setPlayerControl(int controlFlag);


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
	int getPlayerSize(int type);

	//�v���C���[�̃X�e�[�^�X�z����擾����
	PlayerStatus& getPlayerStatusSave(){
		return plyerstatusSave;
	}

private:

	//�A�j���[�V�����Ɏg���ϐ�
	float anim_time_count = 0;
	int anim_ctrl_dir = DIR_DOWN;
	int anim_ctrl_frame = 0;
	int anim_hdls[DIR_MAX][5];

	//Status������ϐ�
	/*PlyerStatus plyer_Status;*/

	//�\���̂̑���
	int Ply_Status_Total_num;

	//�\���̂��i�[����z��
	std::list<PlayerStatus>Ply_Status_Type;

	//csv�ǂݎ��p�̔z��
	std::vector<std::vector<tnl::CsvCell>>PlyerStatus_Csv_Info;


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

	bool getPlayerControl() {
		return plyControl;
	}

	//�Z�[�u���[�h�@�\���������i�̂��ɒǉ��\��j
	/*void PlyerSave();
	void PlyerLoad();*/



};

