#pragma once
///
///NPC�N���X
///

//NPC�̃A�j���[�V����
enum  {
	//��
	NPC_DIR_DOWN,
	//��,
	NPC_DIR_UP,
	//��
	NPC_DIR_LEFT,
	//�E
	NPC_DIR_RIGHT,
	//����
	NPC_DIR_MAX
};


#include "../../dxlib_ext/dxlib_ext.h"
#include"../Manager/Camera.h"
#include"Actor.h"

class Npc  : public Actor{
public:

	//Npc�̃R���X�g���N�^ : ���� : ( ���O , ���W , �L�����̃T�C�Y ,�n���h����Pass , �摜�̑傫�� , �ǂ̃A�j���[�V��������n�߂邩, �n���h���̐� )
	Npc(const std::string& name, const tnl::Vector3& pos ,const int CHARASIZE, std::string ghdl_pass[], float scale , int animation_namber , int ghdl_num);
	~Npc();

	void ChengeAnimation();

	//�X�V����
	void Update(float delta_time)override;

	//�`��
	void Draw(const KonCamera& camera)override;

	//Npc�̃L�����`�b�v�̕��ƍ������擾����
	int GetNpcSizeWidth()const {
		return npc_chara_chip_width;
	}

	int GetNpcSizeHegiht()const {
		return npc_chara_chip_height;
	}

	//Npc�̍��W���擾����
	tnl::Vector3 GetNpcPos() { return npc_pos_; }

	//Npc�̖��O���擾����
	const std::string& GetNpcName()const {
		return npc_name_;
	}


//------------------------------------------------------------------------------------------------------------------------
//�A�j���[�V����

private:

	//Npc�̃A�j���[�V������؂�ւ���
	int animation_select = 0;			

	//�A�j���[�V�����̃J�E���g
	float anim_time_count = 0;						

	//�A�j���[�V�����̑I��
	int anim_ctrl_dir = NPC_DIR_DOWN;				

	//�t���[��
	int anim_ctrl_frame = 0;						

	//�A�j���[�V�����̔z��
	int anim_hdls[NPC_DIR_MAX][5];		

	//�A�j���[�V������؂�ւ���b��
	const float CHANGEANIMATIONTIME = 0.1f;
	//�t���[��
	const int ANIMATIONFLAME = 3;


public:

	//�A�j���[�V�����Z���N�g���擾����
	inline int GetAnimationSelect() { return animation_select; }

	//�A�j���[�V���������Z�b�g����
	void NpcAnimationReset() { animation_select = 0; }

	//�A�j���[�V�������Z�b�g����
	void SetNpcAnimation(int animationID) { animation_select = animationID; }


//------------------------------------------------------------------------------------------------------------------------
//���j���[�֌W

private:

	//���퉮�̘b���������Ƃ��̃R�����g
	int armsdealer_first_comet = 0;

	//���퉮�̕����̍��W
	tnl::Vector2i weaponShop = { 60 , 520 };

	//���폤�l�̃R�����g�̍��W
	tnl::Vector2i armsdealerComent = { 150, 550 };

	//���퉮�̍w���R�����g�̍��W
	tnl::Vector2i WeaponShop_buy = { 70, 100 };

	//���퉮�̃��j���[�����R�����g�̍��W
	tnl::Vector2i menu_close = { 70, 150 };



//------------------------------------------------------------------------------------------------------------------------
//Npc�֌W
private:

	//npc�̖��O
	std::string npc_name_;

	//Npc�̃L�����`�b�v�̕�
	int npc_chara_chip_width = 0;

	//Npc�̃L�����`�b�v�̍���
	int npc_chara_chip_height = 0;

	//���ꂼ��̉摜�Ŏg���傫�������߂�
	float scale_ = 0;

	//npc�̉摜
	int  npc_ghdl;

	//npc�̍��W
	tnl::Vector3 npc_pos_ = {};


};