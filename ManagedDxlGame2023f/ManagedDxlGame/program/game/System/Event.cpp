#include"Event.h"
#include"../Manager/SceneManager.h"
#include"../Scene/InMapScene.h"
#include"../Manager/GameManager.h"


Event::~Event()
{
	//処理格納用の配列をクリアする
	coment_function_map.clear();
	function_map.clear();
}

void Event::InitEventFunction(const std::list<Shared<Npc>>& npc_list)
{
	//npcによって処理を初期化する
	for (auto& npc_list_ : npc_list) {

		//ポインタが存在しない場合配列を抜ける
		if (!npc_list_) { break; }

		//名前を取得してそれぞれ実行処理を初期化する
		if (npc_list_->GetNpcName() == "村長") {
			coment_function_map[npc_list_->GetNpcName()] = std::bind(&Event::ComentProcess, this, std::placeholders::_1, std::placeholders::_2);
		}
		else if (npc_list_->GetNpcName() == "町長") {
			coment_function_map[npc_list_->GetNpcName()] = std::bind(&Event::ComentProcess, this, std::placeholders::_1, std::placeholders::_2);
		}
		else if (npc_list_->GetNpcName() == "神官") {
			function_map[npc_list_->GetNpcName()] = std::bind(&Event::SaveProcces, this, std::placeholders::_1);
		}
		else if (npc_list_->GetNpcName() == "宿屋") {
			function_map[npc_list_->GetNpcName()] = std::bind(&Event::InnProcess, this, std::placeholders::_1);
		}
		else if (npc_list_->GetNpcName() == "魔物") {
			function_map[npc_list_->GetNpcName()] = std::bind(&Event::EventEnemyProcces, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4 , std::placeholders::_5);
		}
		else if (npc_list_->GetNpcName() == "魔王") {
			function_map[npc_list_->GetNpcName()] = std::bind(&Event::EventEnemyProcces, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
		}
		else if (npc_list_->GetNpcName() == "王") {
			coment_function_map[npc_list_->GetNpcName()] = std::bind(&Event::ComentProcess, this, std::placeholders::_1, std::placeholders::_2);
		}
		else if (npc_list_->GetNpcName() == "王妃") {
			coment_function_map[npc_list_->GetNpcName()] = std::bind(&Event::ComentProcess, this, std::placeholders::_1, std::placeholders::_2);
		}
		else if (npc_list_->GetNpcName() == "女性村人") {
			coment_function_map[npc_list_->GetNpcName()] = std::bind(&Event::ComentProcess, this, std::placeholders::_1, std::placeholders::_2);
		}
		else if (npc_list_->GetNpcName() == "男性村人") {
			coment_function_map[npc_list_->GetNpcName()] = std::bind(&Event::ComentProcess, this, std::placeholders::_1, std::placeholders::_2);
		}
		else if (npc_list_->GetNpcName() == "男性町民") {
			coment_function_map[npc_list_->GetNpcName()] = std::bind(&Event::ComentProcess, this, std::placeholders::_1, std::placeholders::_2);
		}
		else if (npc_list_->GetNpcName() == "男性住民") {
			coment_function_map[npc_list_->GetNpcName()] = std::bind(&Event::ComentProcess, this, std::placeholders::_1, std::placeholders::_2);
		}
		else if (npc_list_->GetNpcName() == "学者") {
			coment_function_map[npc_list_->GetNpcName()] = std::bind(&Event::ComentProcess, this, std::placeholders::_1, std::placeholders::_2);
		}
		else if (npc_list_->GetNpcName() == "兵士") {
			coment_function_map[npc_list_->GetNpcName()] = std::bind(&Event::ComentProcess, this, std::placeholders::_1, std::placeholders::_2);
		}
		else if (npc_list_->GetNpcName() == "試練の番人") {
			function_map[npc_list_->GetNpcName()] = std::bind(&Event::EventEnemyProcces, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4 , std::placeholders::_5);
		}
		else if (npc_list_->GetNpcName() == "影の番人") {
			function_map[npc_list_->GetNpcName()] = std::bind(&Event::EventEnemyProcces, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
		}
		else if (npc_list_->GetNpcName() == "兵長") {
			function_map[npc_list_->GetNpcName()] = std::bind(&Event::EventEnemyProcces, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
		}
		else if (npc_list_->GetNpcName() == "船長") {
			function_map[npc_list_->GetNpcName()] = std::bind(&Event::EventEnemyProcces, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
		}
		else if (npc_list_->GetNpcName() == "僧侶") {
			coment_function_map[npc_list_->GetNpcName()] = std::bind(&Event::ComentProcess, this, std::placeholders::_1, std::placeholders::_2);
		}
	}
}

//セーブ処理
void Event::SaveProcces(const int& curent_map_state)
{
	//プレイヤーにマップの状態を伝える(ロードした際にどのマップに居るか把握する為)
	GameManager::GetGameManager()->GetPlayer()->SetPlayerCurentMapMemory(curent_map_state);
	//セーブを行う
	GameManager::GetGameManager()->GetPlayer()->PlayerSave();
	//セーブのテキストを呼び出す
	UIManager::GetUIManager()->SaveTextFlagChange();
}

//コメントを流す
void Event::ComentProcess(BaseScene* curent_map_scene, const int& MAX_COMENT)
{
	//コメントを表示する
	if (ComentEventChange(MAX_COMENT) && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

		//コメントフラグがfalseだったら
		//同じフレームを走らせない為
		if (!coment_animation_flag) {

			coment_animation_flag = true;

		}
		else {
			curent_map_scene->SequenceChangeMove();
			coment_animation_flag = false;
		}
	}
}

//宿屋処理
void Event::InnProcess(const int& money)
{
	//お金が足りなければそくリターンする
	if (auto player_curenent_money = GameManager::GetGameManager()->GetPlayer()->GetPlayerMoney() < money) { return; }

	//---宿屋処理用---//

	//現在のHPを取得する
	auto player_curent_hp = GameManager::GetGameManager()->GetPlayer()->GetPlayerStatusSave().GetcurentHp();

	//現在のMPを取得する
	auto player_curent_mp = GameManager::GetGameManager()->GetPlayer()->GetPlayerStatusSave().GetCurentMp();

	//最大HPを取得する
	auto player_max_hp = GameManager::GetGameManager()->GetPlayer()->GetPlayerStatusSave().GetMaxHp();

	//最大MPを取得する
	auto player_max_mp = GameManager::GetGameManager()->GetPlayer()->GetPlayerStatusSave().GetMaxMp();

	//シーンマネージャーのインスタンスを取得する
	auto mgr = SceneManager::GetSceneManager();

	if (mgr->GetSceneFlag()) {
		//フラグを切り替えてシーンを遷移させないようにする
		mgr->SceneFlagChange();
	}

	//フェードを入れる
	SceneManager::GetSceneManager()->FadeScene();

	//プレイヤーのお金を減少させる
	GameManager::GetGameManager()->GetPlayer()->ReducePlayerMoney(money);

	//回復音を鳴らす
	SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/heal_inn.mp3", DX_PLAYTYPE_BACK);

	//HPとMPを回復させる
	if (player_curent_hp < player_max_hp) {
		GameManager::GetGameManager()->GetPlayer()->GetPlayerStatusSave().SetPlayerCurentHp(player_max_hp);
	}
	if (player_curent_mp < player_max_mp) {
		GameManager::GetGameManager()->GetPlayer()->GetPlayerStatusSave().SetPlayerCurentMp(player_max_mp);
	}


}

//処理を呼び出す
void Event::EventSelectProcces(const std::string& npc_name, const int& value, const int& value_2, const std::string& bgm_1, const std::string& bgm_2 , const int& value_3)
{
	//処理が存在していたら
	if (auto itr = function_map.find(npc_name); itr != function_map.end()) {
		if ((*itr).first == npc_name) {
			(*itr).second(value , value_2 , bgm_1, bgm_2 , value_3);
		}
		itr++;
	}
	//処理が見つからなかったら
	else {
		tnl::DebugTrace("\n------------------------------------------------------------");
		tnl::DebugTrace("関数が見つかりませんでした");
		tnl::DebugTrace("\n------------------------------------------------------------");
	}
}

//敵に話かけた時のコメント処理
void Event::EventEnemyProcces(const int& back_ground , const int& enemy_id, const std::string& bgm_1, const std::string& bgm_2 , const int& in_map_state)
{
	//BGMを止める
	SoundManager::GetSoundManager()->StopSound(bgm_1);

	//バトル用のBGMを流す
	SoundManager::GetSoundManager()->Sound_Play(bgm_2, DX_PLAYTYPE_LOOP);

	auto mgr = SceneManager::GetSceneManager();

	//フラグがfalseの場合trueに変更する(シーンを遷移させる為)
	if (!mgr->GetSceneFlag()) { mgr->SceneFlagChange(); }

	//シーンを遷移させる(プレイヤーの座標を渡す,敵のID,敵のポインタを渡す,どこのマップにいるかの状態を渡す)
	mgr->changeScene(new BattleScene(GameManager::GetGameManager()->GetPlayer()->GetPlayerPos(), back_ground, std::make_shared<BossMonster>(enemy_id), in_map_state));

}

//コメントを流し切ってからEnterキーでシーンを遷移させる
bool Event::ComentEventChange(const int& count_max)
{
	//コメントをコマ送りで表示する
	UIManager::GetUIManager()->ComentNextByInput(count_max);

	//指定のコメント以上になったらtrueを返す
	if (UIManager::GetUIManager()->GetCount() >= count_max) {
		return true;
	}

	return false;
}