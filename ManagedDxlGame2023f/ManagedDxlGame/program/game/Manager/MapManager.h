#pragma once
#include "../../dxlib_ext/dxlib_ext.h"
#include	<unordered_set>
#include"../Map/Mapchip.h"


//-----------------------------------------------------------------------------------------
//
//�V���O���g���p�^�[��
// �}�b�v�֌W�̕ϐ��Ȃǂ������ň���
//
//------------------------------------------------------------------------------------------

//class MapManager {
//
//public:
//
//	static MapManager* getMapManager();
//	static void deleteMapmanager() { delete getMapManager(); }
//
//private:
//
//	MapManager();
//	~MapManager() {};
//
////------------------------------------------------------------------------------------------------------------------------
//
//	//��ڂ̑��ɓ��������̍��W
//	tnl::Vector3 village_Pos = { 860 , 900 , 0 };
//
//	//�x��������b��
//	float BattleSceneTime = 0.5f;
//
//	//�x��������SE�̕b��
//	float VillageSceneTime = 0.8f;
//
//	//�v���C���[�̃T�C�Y
//	int PlayerSize = 1;
//
//	//�v���C���[�̑��x
//	float PlayerVelocity = 2.5f;
//
//	//�v���C���[�̃A�j���[�V����
//	int Animation_default = 0;
//
//	//���̒��̒l�œ����蔻������߂�
//	std::unordered_set<int> colisionObjectValues = { 10 , 11 ,12, 13 , 18 , 19, 20 , 21 ,25, 36 , 37  ,41 , 234  ,335,432,440,442  , 827 , 835 ,1302,1303, 1336, 1337, 1363 ,1364 ,1365, 1476, 1477, 2024 };
//
//	//���Ȃǂ̓����蔻��Ɏg���l
//	std::unordered_set<int>villageValues = { 334 , 2032 , 2033  , 2034 , 2035 , 2069, 2089, 2090 };
//
//	//�n�`�Ŏg�������蔻��̒l
//	std::unordered_set<int>worldCollisionValues = { 2024 , 2027 };
//
//	//�G���J�E���g�Ŏg�������l
//	std::unordered_set<int>encount_kind = { 0 , 1 , 2 , 4 , 5 };
//
//	//���̃}�b�v�`�b�v
//	int mapChip_village = 2035;
//
//
////------------------------------------------------------------------------------------------------------------------------
////�G���J�E���g
//private:
//
//	//����
//	int plain = 2;
//
//	//����2
//	int plain_second = 0;
//
//	//�r��
//	int wilderness[2] = { 1 , 4 };
//
//	//�{�X�G���A
//	int boss_area = 5;
//
//	//�G�̔z���ID
//	int enemy_1 = 1;
//	int enemy_2 = 2;
//	int enemy_3 = 3;
//	int enemy_4 = 4;
//
//
//public:
//
//	//��ڂ̑��ɓ��������̍��W
//	const tnl::Vector3& getVillageFirstPlayerPos()const {
//		return village_Pos;
//	}
//
//	//�x��������b�����擾����
//	float getTimeCount(int type) const{
//
//		//�o�g���V�[���ɑJ�ڂ���p�̃^�C��
//		if (type == 0) {
//			return BattleSceneTime;
//		}
//		//���ɑJ�ڂ���p�̒x���^�C��
//		else {
//			return VillageSceneTime;
//		}
//	}
//
//
//
//};