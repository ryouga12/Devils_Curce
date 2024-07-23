#pragma once
#include"../Scene/InMapScene.h"
#include"../Scene/battleScene.h"
#include"../Manager/EventManager.h"
#include"../Object/NPC.h"
#include"../Manager/UiManager.h"

//---------------------------------------------------------------------------------------------------------
//イベント時の実行処理を管理するクラス

class EventNpc final {
public:

	//実行処理の初期化
	void InitEventFunction(const std::string& npc_name);

	//セーブ処理
	//引数を値を入れなかった場合、村になる
	void SaveProcces(const int curent_map_state = 0);

	//コメント処理
	//コメントを流しきったら行動に戻す
	void ComentProcess(InMapScene* inmap_scene, const int MAX_COMENT);

	//宿屋の回復処理
	void InnProcess(const int money);

	//コメント用の関数を呼び出す
	std::function<void(InMapScene*, int)>GetComentEvent(const std::string& npc_name) {

		//処理が存在していたら
		if (auto itr = coment_function_map.find(npc_name); itr != coment_function_map.end()) {
			if ((*itr).first == npc_name) {
				return coment_function_map[npc_name];
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

	//Npcの処理を呼び出す
	//第二引数からは任意で指定可能
	void EventSelectProcces(const std::string& npc_name, const int value , const int value_2 = 0 , const std::string& bgm_1 = "" , const std::string& bgm_2 = "");

	//敵に話かけた時のコメント処理
	void EventEnemyProcces(const int back_ground, const int& enemy_id,const std::string& bgm_1, const std::string& bgm_2);

private:

	//コメントを流しきるだけの処理
	std::map <std::string ,std::function<void(InMapScene*, int) >> coment_function_map;

	//引数にint型を持ったmap(主に選択ウィンドウなどの処理で使用する)
	std::map<std::string, std::function<void(int , int , const std::string& , const std::string&)>> function_map;

	//文字列アニメーションを行ったかのフラグ
	bool coment_animation_flag = false;
};