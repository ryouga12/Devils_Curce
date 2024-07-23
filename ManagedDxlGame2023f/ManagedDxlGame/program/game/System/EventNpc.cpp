#include"EventNpc.h"
#include"../Manager/SceneManager.h"
#include"../Scene/InMapScene.h"


void EventNpc::InitEventFunction(const std::string& npc_name)
{
	//名前を取得してそれぞれ実行処理を初期化する
	if (npc_name == "村長") {
		coment_function_map[npc_name] = std::bind(&EventNpc::ComentProcess, this, std::placeholders::_1, std::placeholders::_2);
	}
	else if (npc_name == "町長") {
		coment_function_map[npc_name] = std::bind(&EventNpc::ComentProcess, this, std::placeholders::_1, std::placeholders::_2);
	}
	else if (npc_name == "神官") {
		function_map[npc_name] = std::bind(&EventNpc::SaveProcces, this, std::placeholders::_1);
	}
	else if (npc_name == "宿屋") {
		function_map[npc_name] = std::bind(&EventNpc::InnProcess, this, std::placeholders::_1);
	}
	else if (npc_name == "魔物") {
		function_map[npc_name] = std::bind(&EventNpc::EventEnemyProcces, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
	}
	else if (npc_name == "魔王") {
		function_map[npc_name] = std::bind(&EventNpc::EventEnemyProcces, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
	}
	else if (npc_name == "王") {
		coment_function_map[npc_name] = std::bind(&EventNpc::ComentProcess, this, std::placeholders::_1, std::placeholders::_2);
	}
	else if (npc_name == "王妃") {
		coment_function_map[npc_name] = std::bind(&EventNpc::ComentProcess, this, std::placeholders::_1, std::placeholders::_2);
	}
	else if (npc_name == "女性村人") {
		coment_function_map[npc_name] = std::bind(&EventNpc::ComentProcess, this, std::placeholders::_1, std::placeholders::_2);
	}

}

//セーブ処理
void EventNpc::SaveProcces(int curent_map_state)
{
	//プレイヤーにマップの状態を伝える(ロードした際にどのマップに居るか把握する為)
	GameManager::getGameManager()->getPlayer()->SetPlayerCurentMapMemory(curent_map_state);
	//セーブを行う
	GameManager::getGameManager()->getPlayer()->PlyerSave();
	//セーブのテキストを呼び出す
	UIManager::getUIManager()->SaveTextFlagChange();
}

void EventNpc::ComentProcess(InMapScene* inmap_scene, const int MAX_COMENT)
{
	//コメントを表示する
	if (EventManager::getEventManager()->ComentEventChange(MAX_COMENT) && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

		//コメントフラグがfalseだったら
		//同じフレームを走らせない為
		if (!coment_animation_flag) {

			coment_animation_flag = true;

		}
		else {
			inmap_scene->SequenceChange(InMapScene::Sequence_num::MOVE);
			coment_animation_flag = false;
		}
	}
}

//宿屋処理
void EventNpc::InnProcess(const int money)
{
	//---宿屋処理用---//

	//現在のHPを取得する
	auto player_curent_hp = GameManager::getGameManager()->getPlayer()->getPlayerStatusSave().GetcurentHp();

	//現在のMPを取得する
	auto player_curent_mp = GameManager::getGameManager()->getPlayer()->getPlayerStatusSave().GetCurentMp();

	//最大HPを取得する
	auto player_max_hp = GameManager::getGameManager()->getPlayer()->getPlayerStatusSave().GetMaxHp();

	//最大MPを取得する
	auto player_max_mp = GameManager::getGameManager()->getPlayer()->getPlayerStatusSave().GetMaxMp();

	//シーンマネージャーのインスタンスを取得する
	auto mgr = SceneManager::GetInstance();

	if (mgr->GetSceneFlag()) {
		//フラグを切り替えてシーンを遷移させないようにする
		mgr->SceneFlagChange();
	}

	//フェードを入れる
	SceneManager::GetInstance()->FadeScene();

	//プレイヤーのお金を減少させる
	GameManager::getGameManager()->getPlayer()->ReducePlayerMoney(money);

	//回復音を鳴らす
	SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/heal_inn.mp3", DX_PLAYTYPE_BACK);

	//HPとMPを回復させる
	if (player_curent_hp < player_max_hp) {
		GameManager::getGameManager()->getPlayer()->getPlayerStatusSave().SetPlayerCurentHp(player_max_hp);
	}
	if (player_curent_mp < player_max_mp) {
		GameManager::getGameManager()->getPlayer()->getPlayerStatusSave().SetPlayerCurentMp(player_max_mp);
	}


}

void EventNpc::EventSelectProcces(const std::string& npc_name, const int value, const int value_2, const std::string& bgm_1, const std::string& bgm_2)
{
	//処理が存在していたら
	if (auto itr = function_map.find(npc_name); itr != function_map.end()) {
		if ((*itr).first == npc_name) {
			(*itr).second(value , value_2 , bgm_1, bgm_2);
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
void EventNpc::EventEnemyProcces(const int back_ground , const int& enemy_id, const std::string& bgm_1, const std::string& bgm_2)
{
	//BGMを止める
	SoundManager::getSoundManager()->StopSound(bgm_1);

	//バトル用のBGMを流す
	SoundManager::getSoundManager()->sound_Play(bgm_2, DX_PLAYTYPE_LOOP);

	auto mgr = SceneManager::GetInstance();

	//フラグがfalseの場合trueに変更する(シーンを遷移させる為)
	if (!mgr->GetSceneFlag()) { mgr->SceneFlagChange(); }

	//シーンを遷移させる(プレイヤーの座標を渡す,敵のID,敵のポインタを渡す,どこのマップにいるかの状態を渡す)
	mgr->changeScene(new BattleScene(GameManager::getGameManager()->getPlayer()->getPlayerPos(), back_ground, std::make_shared<BossMonster>(enemy_id) , static_cast<int>(InMapScene::InMapState::TOWN)));

}
