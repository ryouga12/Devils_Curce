#pragma once
//------------------------------------------------------------------------------------------------------------
//
//���ʂŎg�����̊Ǘ����钆�Ԗ��̖����̃}�l�[�W���[
//
//-------------------------------------------------------------------------------------------------------------


#include"../Menu/MenuWindow.h"
#include "../../dxlib_ext/dxlib_ext.h"
#include "../Item/Inventory.h"

class Player;
class Skill;

class GameManager final{
public:
//------------------------------------------
// �V���O���g���݌v

	static GameManager* GetGameManager();
	static void DelateGameManager();

private:

	GameManager()= default;
	~GameManager()= default;

public:

	//�b���𐔂���
	bool TimeCount(float delta_time, float Time);

	//�|�C���^�𐶐�����
	void PoiterCreate();

	//�|�C���^�����Z�b�g����
	void Reset();

private:

	//����
	float count_time = 0;

//------------------------------------------------------------------------------------------------------------------------
//---�A�C�e���֘A---//
public:

	//�����_���Ȓl���Z�b�g����i����g���\��j
	int GetRandEx(int a, int b);

//------------------------------------------------------------------------------------------------------------------------
//---�|�C���^---//
private:

	//�C���x���g��
	Shared<Inventory>inventory = nullptr;
	//�v���C���[
	Shared<Player>player = nullptr;
	//�J����
	Shared<KonCamera>camera = nullptr;

//------------------------------------------------------------------------------------------------------------------------
// ���ʏ�����C���x���g���̍X�V

public:

	//---�|�C���^�̎擾---//

	//�C���x���g��
	const Shared<Inventory>& GetInventory(){
		return inventory;
	}
	//�v���C���[
	const Shared<Player>& GetPlayer() {
		return player;
	}
	//�J����
	const Shared<KonCamera>& GetCamera() {
		return camera;
	}

};


