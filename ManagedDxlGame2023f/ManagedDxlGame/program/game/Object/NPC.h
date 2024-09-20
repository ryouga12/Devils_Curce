#pragma once
//------------------------------------------------------------------------------------------------------------
//
//NPC�N���X
//
//-------------------------------------------------------------------------------------------------------------


#include "../../dxlib_ext/dxlib_ext.h"
#include"../System/Camera.h"
#include"Actor.h"
#include"../Menu/MenuWindow.h"
#include<unordered_map>

class MenuWindow;

class Npc  final : public Actor{
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

	//Npc�̃R���X�g���N�^ 
	//arg_1  : ���O 
	//arg_2  : ���W 
	//arg_ 3 : �L�����̃T�C�Y 
	//arg_4  : �摜�̑傫�� 
	//arg_ 5 : �ǂ̃A�j���[�V��������n�߂邩
	//arg_ 6 : npc�̃A�C�R�� )
	Npc(const std::string& name, const tnl::Vector3& pos ,const int& CHARASIZE, const float& scale , const NPC_ANIMATION& animation_namber , const std::string& npc_icon_image);
	
	//�����蔻��̂Ȃ�Npc
	//arg_1 : ���O
	//arg_2 : �A�C�R���̃n���h�� 
	Npc(const std::string& name , const std::string& icon_hdl);
		
	~Npc();

	//�X�V����
	void Update(float delta_time)override;

	//�`��
	void Draw(const KonCamera& camera)override;

	//Npc�̃L�����`�b�v�̕��ƍ������擾����
	int GetNpcSizeWidth()const {
		return npc_chara_chip.x;
	}
	int GetNpcSizeHegiht()const {
		return npc_chara_chip.y;
	}

	//Npc�̃L�����̃T�C�Y���擾����
	const tnl::Vector2i& GetNpcCharaChip()const {
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

	//Npc�̃A�C�R�����擾����
	int GetNpcIconImage()const {
		return npc_icon_hdl;
	}

	//Npc�̉摜�����[�h����
	//arg_1 : Npc�̖��O,InMapScene���̌��݂̃V�[��
	//���O�ƌ��݂̃V�[���𗼕���r���ĊY�������摜���i�[����
	void SetNpcImage(const std::string& npc_name , const int& inmap_state);

//------------------------------------------------------------------------------------------------------------------------
//�A�j���[�V����

private:

	//�A�j���[�V�����̃J�E���g
	float anim_time_count = 0;						

	//�A�j���[�V�����̑I��
	NPC_ANIMATION anim_ctrl_dir = NPC_ANIMATION::NPC_DIR_DOWN;

	//�t���[��
	int anim_ctrl_frame = 0;						

	//�A�j���[�V�����̔z��
	int anim_hdls[static_cast<int>(NPC_ANIMATION::NPC_DIR_MAX)][5];

public:

	//�A�j���[�V�������Z�b�g����
	void SetNpcAnimation(NPC_ANIMATION new_animation) { anim_ctrl_dir = new_animation; }

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

