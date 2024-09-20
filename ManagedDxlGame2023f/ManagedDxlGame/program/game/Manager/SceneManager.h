//------------------------------------------------------------------------------------------------------------
//
//�V�[���̐؂�ւ����s���}�l�[�W���[
//
//-------------------------------------------------------------------------------------------------------------


#pragma once
#include"../Scene/BaseScene.h"
#include "../../dxlib_ext/dxlib_ext.h"
#include"../../koni_name_space/common/common_value.h"

class GameManager;
class SoundManager;


class SceneManager final{
public:
	static SceneManager* GetSceneManager(BaseScene* start_scene = nullptr);
	static void DeleteSceneManager();

	//�V�[����ς���
	//arg_1 : �V�[���̃|�C���^
	//arg_2 : �g�����X�A�E�g�̕b��
	//arg_3 : �g�����X�C���̕b��
	void changeScene(BaseScene* next_scene, const float& transout_time = 0.5f, const float& transin_time = 0.5f);

	//��ʏ�Ƀt�F�[�h������
	void FadeScene(const float& transout_time = 0.5f, const float& transin_time = 0.5f);

	//�t���O��؂�ւ���
	void SceneFlagChange() {
		scene_change = !scene_change;
	}

	//�t���O���擾����
	bool GetSceneFlag() { return scene_change; }

	//���݂̃V�[���̍X�V�����ƕ`�揈���ƃV�[�P���X�������Ăяo��
	void update(float delta_time);

private:

	//arg_1 : ���N���X�̃|�C���^(BaseScene)
	//�ŏ��̃V�[����ݒ肷��
	SceneManager(BaseScene* start_scene);

	//�g�����X�C������, �g�����X�A�E�g����
	float transin_time_ = 0, transout_time_ = 0;
	//���݂̃V�[��
	BaseScene* now_scene_ = nullptr;
	//�ύX����V�[��
	BaseScene* next_scene_ = nullptr;
	//�g�����W�V�����p�̉摜
	int tansition_graph_hdl_ = 0;
	//�V�[����J�ڂ����邩�̃t���O
	//�g������ : �V�[����J�ڂ��������Ȃ����ǃg�����W�V�����̏����͗����������ȂǂɃt���O��؂�ւ���
	bool scene_change = true;

	//�V�[�P���X
	tnl::Sequence<SceneManager> sequence_ = tnl::Sequence<SceneManager>(this, &SceneManager::seqTransIn);

	//�g�����X�C���̏���
	bool seqTransIn(const float delta_time);
	//�g�����X�A�E�g�̏���
	bool seqTransOut(const float delta_time);
	//�ҋ@�V�[�P���X(��Ƀg�����W�V�����������N���������Ȃ���)
	bool seqRunScene(const float delta_time);
};