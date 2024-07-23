#pragma once
///
///NPC�N���X
///


#include "../../dxlib_ext/dxlib_ext.h"
#include"../System/Camera.h"
#include"Actor.h"
#include"../Menu/MenuWindow.h"
#include<unordered_map>

class MenuWindow;
class GameManager;
class UIManager;


class Npc  : public Actor{
public:

	//NPC�̃A�j���[�V����
	enum  class NPC_ANIMATION {
		//��
		NPC_DIR_LEFT,
		//�E
		NPC_DIR_RIGHT,
		//��,
		NPC_DIR_UP,
		//��
		NPC_DIR_DOWN,
		//����
		NPC_DIR_MAX
	};

	//Npc�̃R���X�g���N�^ : ���� : ( ���O , ���W , �L�����̃T�C�Y ,�n���h����Pass , �摜�̑傫�� , �ǂ̃A�j���[�V��������n�߂邩, �n���h���̐� )
	Npc(const std::string& name, const tnl::Vector3& pos ,const int CHARASIZE, std::string ghdl_pass[],float scale , const NPC_ANIMATION animation_namber , const int ghdl_num, const std::string& npc_icon_image);
	~Npc();

	//�X�V����
	virtual void Update(float delta_time)override;

	//�`��
	virtual void Draw(const KonCamera& camera)override;

	//Npc�̃L�����`�b�v�̕��ƍ������擾����
	int GetNpcSizeWidth()const {
		return npc_chara_chip.x;
	}

	int GetNpcSizeHegiht()const {
		return npc_chara_chip.y;
	}

	//Npc�̃L�����̃T�C�Y���擾����
	tnl::Vector2i GetNpcCharaChip()const {
		return npc_chara_chip;
	}

	//Npc�̍��W���擾����
	tnl::Vector3& GetNpcPos(){ return npc_pos_; }

	//Npc�̖��O���擾����
	const std::string& GetNpcName()const {
		return npc_name_;
	}

	//Npc�̃A�j���[�V�������擾����
	NPC_ANIMATION GetNpcAnimation()const {
		return anim_ctrl_dir;
	}

	//�ۊǂ��Ă���A�j���[�V���������擾����
	NPC_ANIMATION GetNpcAnimation_Before()const {
		return before_animation_save;
	}

	//�ύX����O�̃A�j���[�V������ۊǂ���
	void SetNpcAnimationSave(NPC_ANIMATION save_animation) {
		before_animation_save = save_animation;
	}

	//Npc�̃A�C�R�����擾����
	int GetNpcIconImage()const {
		return npc_icon_hdl;
	}

//------------------------------------------------------------------------------------------------------------------------
//�A�j���[�V����

private:

	//�A�j���[�V�����̃J�E���g
	float anim_time_count = 0;						

	//�A�j���[�V�����̑I��
	NPC_ANIMATION anim_ctrl_dir = NPC_ANIMATION::NPC_DIR_DOWN;

	//��O�̃A�j���[�V������ۑ����Ă����ׂ̕ϐ�
	//��ɃA�j���[�V������߂����߂Ɏg�p
	NPC_ANIMATION before_animation_save;

	//�t���[��
	int anim_ctrl_frame = 0;						

	//�A�j���[�V�����̔z��
	int anim_hdls[static_cast<int>(NPC_ANIMATION::NPC_DIR_MAX)][5];


public:

	//�A�j���[�V�������Z�b�g����
	void SetNpcAnimation(NPC_ANIMATION new_animation) { anim_ctrl_dir = new_animation; }


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

	//Npc�̃L�����`�b�v�̃T�C�Y
	tnl::Vector2i npc_chara_chip = {};

	//���ꂼ��̉摜�Ŏg���傫�������߂�
	float scale_ = 0;

	//npc�̉摜
	int  npc_ghdl;

	//npc�̍��W
	tnl::Vector3 npc_pos_ = {};

	//npc�̃A�C�R��
	int npc_icon_hdl = 0;

};

