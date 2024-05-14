#include"SceneManager.h"
#include"../Scene/BaseScene.h"

SceneManager::SceneManager(BaseScene* start_scene) : now_scene_(start_scene) {
	tansition_graph_hdl_ = LoadGraph("graphics/black.bmp");
}

SceneManager* SceneManager::GetInstance(BaseScene* start_scene) {
	static SceneManager* instance = nullptr;
	if (!instance) {
		instance = new SceneManager(start_scene);
	}
	return instance;
}

void SceneManager::Destroy() {
	delete GetInstance();
}


void SceneManager::changeScene(BaseScene* next_scene, float trans_time) {
	next_scene_ = next_scene;
	trans_time_ = trans_time;
	sequence_.change(&SceneManager::seqTransOut);
}


void SceneManager::update(float delta_time) {

	if (now_scene_) now_scene_->Update(delta_time);
	if (now_scene_) now_scene_->Draw();

	sequence_.update(delta_time);

}

bool SceneManager::seqTransOut(const float delta_time) {
	int alpha = (sequence_.getProgressTime() / trans_time_ * 255.0f);
	if (alpha >= 255) {
		sequence_.change(&SceneManager::seqTransIn);
		delete now_scene_;
		now_scene_ = nullptr;
		now_scene_ = next_scene_;
	}
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	DrawExtendGraph(0, 0, DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT, tansition_graph_hdl_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	return true;
}

bool SceneManager::seqTransIn(const float delta_time) {
	int alpha = 255 - (sequence_.getProgressTime() / trans_time_ * 255.0f);
	if (alpha <= 0) {
		sequence_.change(&SceneManager::seqRunScene);
	}
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	DrawExtendGraph(0, 0, DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT, tansition_graph_hdl_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	return true;
}


bool SceneManager::seqRunScene(const float delta_time) {
	return true;
}
