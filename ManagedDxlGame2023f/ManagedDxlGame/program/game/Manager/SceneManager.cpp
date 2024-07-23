#include"SceneManager.h"
#include"GameManager.h"
#include"../Scene/BaseScene.h"
#include"../Manager/SoundManager.h"

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


void SceneManager::changeScene(BaseScene* next_scene, const float transout_time, const float transin_time) {
	next_scene_ = next_scene;
	transout_time_ = transout_time;
	transin_time_ = transin_time;
	sequence_.change(&SceneManager::seqTransOut);
}

//画面上にフェードを入れる
void SceneManager::FadeScene(const float transout_time, const float transin_time)
{
	transout_time_ = transout_time;
	transin_time_ = transin_time;
	sequence_.change(&SceneManager::seqTransOut);
}

void SceneManager::update(float delta_time) {

	if (now_scene_) now_scene_->Update(delta_time);
	if (now_scene_) now_scene_->Draw();	

	sequence_.update(delta_time);

}

bool SceneManager::seqTransOut(const float delta_time) 
{
	int alpha = static_cast<int>((sequence_.getProgressTime() / transout_time_ * 255.0f));

	if (alpha >= 255) {

		sequence_.change(&SceneManager::seqTransIn);

		//シーン遷移させる場合
		if (scene_change) {

			delete now_scene_;
			now_scene_ = nullptr;
			now_scene_ = next_scene_;

			//座標をセーブする
			GameManager::getGameManager()->getCamera()->SavePosition(GameManager::getGameManager()->getPlayer()->getPlayerPos());

		}
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	DrawExtendGraph(0, 0, DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT, tansition_graph_hdl_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	return true;
}

bool SceneManager::seqTransIn(const float delta_time)
{
	if (sequence_.isStart()&& scene_change) {
		//カメラの座標を合わせる
		GameManager::getGameManager()->getCamera()->SetTargetPos();
	}

	int alpha = static_cast<int>(255 - (sequence_.getProgressTime() / transin_time_ * 255.0f));

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
