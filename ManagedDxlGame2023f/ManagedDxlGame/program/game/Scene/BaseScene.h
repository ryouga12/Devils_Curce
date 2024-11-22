//------------------------------------------------------------------------------------------------------------
//
//�V�[���̊��N���X
//
//-------------------------------------------------------------------------------------------------------------



#pragma once
#include"../Object/NPC.h"
#include "../../dxlib_ext/dxlib_ext.h"
#include"../Map/Mapchip.h"


class BaseScene {
public:

    virtual void Update(float delta_time) = 0;
    virtual void Draw() = 0;
  
    BaseScene() = default;
    virtual ~BaseScene() = default;

    //�V�[���̏��
    enum class SceneState {
        //�^�C�g��
        TITTLE,
        //�C���}�b�v(���⒬�A��Ȃ�)
        INMAP,
        //���[���h�}�b�v
        WORLDMAP,
        //�o�g��
        BATTLE,
        //���U���g
        RESULT
    };

     //���݂̃V�[�����擾����
    SceneState GetCurentScene()const {
        return curent_scene;
    }

    //���̏��ŃV�[�P���X��؂�ւ���
    virtual void SequenceChangeMove() {};

protected:

    

    //�X�g�[���[
    enum Story {    
        //�v�����[�O
        PROLOGUE,
        //�G�s���[�O
        EPILOGUE
    };

    SceneState curent_scene = SceneState::TITTLE;

    //�x���p�̃t���O
    bool warnig_message = false;

    //Npc���X�g
    std::list<Shared<Npc>>npc_list;

    //---mapChip�֘A�̕ϐ�---//
    std::string gpc_map_chip_hdls_pass_ = "graphics/image_(2024_0115_0231).png";							// �摜�p�X
    int map_chip_width_ = 32;																				// �}�b�v�`�b�v�̕�
    int map_chip_height_ = 32;																				// �}�b�v�`�b�v�̍���
    int map_chip_all_size_ = 0;																				// �}�b�v�`�b�v�̑��t���[����
    int map_chip_x_size_ = 8;																				// �}�b�v�`�b�v�̉��t���[����
    int map_chip_y_size_ = 262;																				// �}�b�v�`�b�v�̏c�t���[����
    int gpc_map_chip_hdls_[2096] = {};																		// �摜�f�[�^�i�[
    std::string map_data_csv_pass_;																			// �}�b�vCSV�f�[�^�̃p�X
    int map_height = 0;																					        //�}�b�v�̍���
    int map_width = 0;																					        //�}�b�v�̕�

    //�}�b�v�`�b�v���i�[����ꏊ
    std::list<Shared<MapChip>> map_chips;
    //�I�u�W�F�N�g�p�̃}�b�v�`�b�v
    std::list<Shared<MapChip>> map_chip_object;

    //�}�b�v�`�b�v�̃|�C���^
    Shared<MapChip>mapchip = nullptr;

    //�t�F�[�h�A�E�g����
    const float FADE_OUT_TIME = 0.1f;
    //�t�F�[�h�C������
    const float FADE_IN_TIME = 1.0f;

    //�����蔻��␳�l
    const float HIT_JUDGMENT_CORRECTION_VALUE = 0;
};


