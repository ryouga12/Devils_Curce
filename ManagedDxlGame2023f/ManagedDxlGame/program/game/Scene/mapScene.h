//------------------------------------------------------------------------------------------------------------
//
//WorldMap�N���X
//
//-------------------------------------------------------------------------------------------------------------

#pragma once
#include	<unordered_set>
#include "../../dxlib_ext/dxlib_ext.h"
#include"../Manager/ResourceManager.h"
#include"../Scene/InMapScene.h"
#include"../Manager/GameManager.h"

class BaseScene;
class Enemy;
class MapChip;

class MapScene final: public BaseScene {
public:
	MapScene();
	~MapScene()override;

	void Update(float delta_time)override;
	void Draw()override;

private:

	//�}�b�v�`�b�v�̓ǂݍ���
	void worldMapLoad();

	//���ɓ��������̍��W
	const tnl::Vector3 VILLAGE_POS = { 860 , 900 , 0 };

	//���ɓ��������̍��W
	const tnl::Vector3 TOWN_POS = { 1180,1360,0 };

	//�鉺���ɓ��������̍��W
	const tnl::Vector3 CASTLE_TOWN_POS = { 1950,950,0 };

	//�{�X�̏�ɓ��������̍��W
	const tnl::Vector3 BOSS_CASTLE_POS = { 769 , 895 , 0};

	//�E�҂̕�ɓ��������̍��W
	const tnl::Vector3 GRAVE_POS = { 805 , 895 , 0 };

	//�v���C���[�̑��x
	float player_velocity = 2.5f;
	
	//�v���C���[�̃A�j���[�V����
	int Animation_default = 0;

	//�f�o�b�N�p
	void DebugMapScene();

//------------------------------------------------------------------------------------------------------------------------
//�V�[�P���X

	tnl::Sequence<MapScene> sequence_ = tnl::Sequence<MapScene>(this, &MapScene::seqMove);

	//�v���C���[�̍s��(�ړ��Ȃ�)
	bool seqMove(float delta_time);
	//�C�x���g�֘A
	bool seqEvent(float delta_time);
	//�V�[����ς���ׂ̃V�[�P���X
	bool seqChangeScene(float delta_time);

	//���̏��ŃV�[�P���X��؂�ւ���
	void SequenceChangeMove()override;
	
//--------------------------------------------------------------------------------------------------------------------------
//mapchip�֘A
//

private:

	//���̒��̒l�œ����蔻������߂�
	std::unordered_set<int> colisionObjectValues = { 12 ,13 ,16 , 17 , 20 , 21 ,24 , 25 ,46 , 47, 97 ,432, 434 , 442  ,1244 , 1245 , 1328 , 1329 ,1336 , 1337,1670,1677, 2025 };

	//���Ȃǂ̓����蔻��Ɏg���l
	std::unordered_set<int>ObjectValues = { 334 ,335, 1247, 2032 , 2033  , 2034 , 2035 , 2069, 2089, 2090 };

	//�n�`�Ŏg�������蔻��̒l
	std::unordered_set<int>worldCollisionValues = {2024 , 2027};

	//�G���J�E���g�Ŏg�������l
	std::unordered_set<int>encount_kind = { 0 , 1 , 2 ,3 , 5 ,6 ,1320};

	//�����蔻��
	void WorldMapCollision();


//------------------------------------------------------------------------------------------------------------------------
//�G���J�E���g
private:

	//�����_���G���J�E���g�Ńo�g���V�[���őJ�ڂ���ׂ̊֐�
	bool ChackEncount(const int& step);

	//�����ŃG���J�E���g���邩
	int encounterThreshold = 0;

	//�o�g���V�[���Ŏg���n���h��
	int background_hdl = 0;

	//�G�̔z���ID
	int enemy_id = 0;

};