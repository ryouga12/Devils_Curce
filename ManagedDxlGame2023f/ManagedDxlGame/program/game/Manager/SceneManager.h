//
//�V�[���̐؂�ւ����s��
//


#pragma once
#include"../Scene/BaseScene.h"
#include "../../dxlib_ext/dxlib_ext.h"
#include"../../koni_name_space/common/common_value.h"

class GameManager;
class SoundManager;


class SceneManager {
public:
	static SceneManager* GetInstance(BaseScene* start_scene = nullptr);
	static void Destroy();

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

	void update(float delta_time);

private:
	SceneManager(BaseScene* start_scene);
	float transin_time_ = 1.0f, transout_time_ = 1.0f;
	BaseScene* now_scene_ = nullptr;
	BaseScene* next_scene_ = nullptr;
	int tansition_graph_hdl_ = 0;
	bool scene_change = true;

	tnl::Sequence<SceneManager> sequence_ = tnl::Sequence<SceneManager>(this, &SceneManager::seqTransIn);
	bool seqTransIn(const float delta_time);
	bool seqTransOut(const float delta_time);
	bool seqRunScene(const float delta_time);
};