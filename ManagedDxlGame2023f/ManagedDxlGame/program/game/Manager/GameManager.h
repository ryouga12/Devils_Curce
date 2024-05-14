#pragma once
///
///�V�X�e���ʂ������}�l�[�W���[
/// 

#include"../Menu/MenuWindow.h"
#include "../../dxlib_ext/dxlib_ext.h"
#include "../Item/Inventory.h"
#include"../Skill/Skill.h"
#include"../Object/Player.h"


class GameManager {
public:
//------------------------------------------
// �V���O���g���݌v
//�t���C�E�F�C�g�p�^�[�����g��

	static GameManager* getGameManager();
	static void delateGameManager() { delete getGameManager(); }

private:

	GameManager();
	~GameManager();

public:

	//�b���𐔂���
	bool TimeCount(float delta_time, float Time);

	//�`��
	void draw();

private:

	//����
	float count_Time = 0;


//------------------------------------------------------------------------------------------------------------------------
//---����֘A---//



//------------------------------------------------------------------------------------------------------------------------
//---�A�C�e���֘A---//
public:

	//�����_���Ȓl���Z�b�g����i����g���\��j
	int GetRandEx(int a, int b);

//------------------------------------------------------------------------------------------------------------------------
//---�|�C���^---//
private:

	Shared<Inventory>inventory = nullptr;
	Shared<Player>player = nullptr;
	Shared<KonCamera>camera = nullptr;
	Shared<Skill>skill = nullptr;


//------------------------------------------------------------------------------------------------------------------------
//���j���[�֘A
public:
	
	//�b���R�}���h�̏���
	void displayDialogue();

//------------------------------------------------------------------------------------------------------------------------
// ���ʏ�����C���x���g���̍X�V

public:

	//�X�V����
	void UpdateCommon(float delta_time) {
		inventory->Update(delta_time);
		player->Update(delta_time);
	}

	//�|�C���^�̎擾

	//�C���x���g��
	const Shared<Inventory>& getInventory(){
		return inventory;
	}
	//�v���C���[
	const Shared<Player>& getPlayer() {
		return player;
	}
	//�J����
	const Shared<KonCamera>& getCamera() {
		return camera;
	}
	//�X�L��
	const Shared<Skill>& getSkill() {
		return skill;
	}

};


