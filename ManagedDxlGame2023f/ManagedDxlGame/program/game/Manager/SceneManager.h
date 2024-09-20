//------------------------------------------------------------------------------------------------------------
//
//シーンの切り替えを行うマネージャー
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

	//現在のシーンの更新処理と描画処理とシーケンス処理を呼び出す
	void update(float delta_time);

private:

	//arg_1 : 基底クラスのポインタ(BaseScene)
	//最初のシーンを設定する
	SceneManager(BaseScene* start_scene);

	//トランスイン時間, トランスアウト時間
	float transin_time_ = 0, transout_time_ = 0;
	//現在のシーン
	BaseScene* now_scene_ = nullptr;
	//変更するシーン
	BaseScene* next_scene_ = nullptr;
	//トランジション用の画像
	int tansition_graph_hdl_ = 0;
	//シーンを遷移させるかのフラグ
	//使い分け : シーンを遷移させたくないけどトランジションの処理は流したい時などにフラグを切り替える
	bool scene_change = true;

	//シーケンス
	tnl::Sequence<SceneManager> sequence_ = tnl::Sequence<SceneManager>(this, &SceneManager::seqTransIn);

	//トランスインの処理
	bool seqTransIn(const float delta_time);
	//トランスアウトの処理
	bool seqTransOut(const float delta_time);
	//待機シーケンス(主にトランジション処理を起こしたくない時)
	bool seqRunScene(const float delta_time);
};