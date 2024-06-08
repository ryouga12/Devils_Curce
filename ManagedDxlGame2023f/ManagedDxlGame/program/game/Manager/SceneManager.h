//
//ƒV[ƒ“‚ÌØ‚è‘Ö‚¦‚ğs‚¤
//


#pragma once
#include"../Scene/BaseScene.h"
#include"../Scene/mapScene.h"
#include"../Scene/resultScene.h"
#include"../Scene/battleScene.h"
#include "../../dxlib_ext/dxlib_ext.h"

class GameManager;
class SoundManager;


class SceneManager {
public:
	static SceneManager* GetInstance(BaseScene* start_scene = nullptr);
	static void Destroy();

	void changeScene(BaseScene* next_scene, float transout_time = 0.5f, float transin_time = 0.5f);

	void update(float delta_time);

private:
	SceneManager(BaseScene* start_scene);
	float transin_time_ = 1.0f, transout_time_ = 1.0f;
	BaseScene* now_scene_ = nullptr;
	BaseScene* next_scene_ = nullptr;
	int tansition_graph_hdl_ = 0;

	tnl::Sequence<SceneManager> sequence_ = tnl::Sequence<SceneManager>(this, &SceneManager::seqTransIn);
	bool seqTransIn(const float delta_time);
	bool seqTransOut(const float delta_time);
	bool seqRunScene(const float delta_time);
};