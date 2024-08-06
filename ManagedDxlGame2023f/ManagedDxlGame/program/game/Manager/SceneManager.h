//
//シーンの切り替えを行う
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

	//シーンを変える
	//arg_1 : シーンのポインタ
	//arg_2 : トランスアウトの秒数
	//arg_3 : トランスインの秒数
	void changeScene(BaseScene* next_scene, const float& transout_time = 0.5f, const float& transin_time = 0.5f);

	//画面上にフェードを入れる
	void FadeScene(const float& transout_time = 0.5f, const float& transin_time = 0.5f);

	//フラグを切り替える
	void SceneFlagChange() {
		scene_change = !scene_change;
	}

	//フラグを取得する
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