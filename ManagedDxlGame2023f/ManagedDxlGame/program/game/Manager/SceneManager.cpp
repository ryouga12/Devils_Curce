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


void SceneManager::changeScene(BaseScene* next_scene, const float& transout_time, const float& transin_time) {
	next_scene_ = next_scene;
	transout_time_ = transout_time;
	transin_time_ = transin_time;
	sequence_.change(&SceneManager::seqTransOut);
}

//��ʏ�Ƀt�F�[�h������
void SceneManager::FadeScene(const float& transout_time, const float& transin_time)
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

		//�����𐧌�����
		if (GameManager::getGameManager()->getPlayer()->getPlayerControl()) {
			GameManager::getGameManager()->getPlayer()->setPlayerControl();
		}

		//�V�[���J�ڂ�����ꍇ
		if (scene_change) {

			delete now_scene_;
			now_scene_ = nullptr;
			now_scene_ = next_scene_;

			//���W���Z�[�u����
			GameManager::getGameManager()->getCamera()->SavePosition(GameManager::getGameManager()->getPlayer()->getPlayerPos());

		}
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	DrawExtendGraph(0, 0, DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT, tansition_graph_hdl_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, koni::Numeric::ALPHA_OPAQUE);

	return true;
}

bool SceneManager::seqTransIn(const float delta_time)
{
	if (sequence_.isStart()&& scene_change) {
		//�J�����̍��W�����킹��
		GameManager::getGameManager()->getCamera()->SetTargetPos();
	}

	//�v���C���[��������悤�ɂ���
	if (!GameManager::getGameManager()->getPlayer()->GetPlayerDisplayFlag()) {
		GameManager::getGameManager()->getPlayer()->PlayerDisplayChange();
	}

	int alpha = static_cast<int>(koni::Numeric::ALPHA_OPAQUE - (sequence_.getProgressTime() / transin_time_ * 255.0f));

	if (alpha <= 0) {

		sequence_.change(&SceneManager::seqRunScene);

		//������悤�ɂ���
		if (!GameManager::getGameManager()->getPlayer()->getPlayerControl()) {
			GameManager::getGameManager()->getPlayer()->setPlayerControl();
		}
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	DrawExtendGraph(0, 0, DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT, tansition_graph_hdl_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, koni::Numeric::ALPHA_OPAQUE);


	return true;
}


bool SceneManager::seqRunScene(const float delta_time) {

	return true;
}
