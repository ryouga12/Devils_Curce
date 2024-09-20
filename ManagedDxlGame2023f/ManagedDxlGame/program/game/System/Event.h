#pragma once
#include<unordered_map>
#include"../Scene/BaseScene.h"
#include"../Scene/battleScene.h"
#include"../Manager/EventManager.h"
#include"../Object/NPC.h"
#include"../Manager/UiManager.h"

//---------------------------------------------------------------------------------------------------------
//イベント時の実行処理を管理するクラス

class Event final {
public:

	Event() = default;
	~Event();

	//実行処理の初期化
	//arg_1 : NPCクラスのリスト
	//NPC毎に処理の初期化を行う
	void InitEventFunction(const std::list<Shared<Npc>>& npc_list);

	//セーブ処理
	//arg_1 : どのマップにいるか(InMapSceneのenum classのInMapStateを入れる)
	void SaveProcces(const int& curent_map_state);

	//コメント処理
	//コメントを流しきったら行動に戻す
	//arg_1 : シーンのポインタ(シーンのシーケンスに触れる為)
	//arg_2 : コメントの数
	void ComentProcess(BaseScene* curent_map_scene, const int& MAX_COMENT);

	//宿屋の回復処理
	//arg_1 : 減るお金の量
	void InnProcess(const int& money);

	//コメント用の関数を呼び出す
	//arg_1 : NPCの名前
	std::function<void(BaseScene*, int)>GetComentEvent(const std::string& npc_name) {

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

		return 0;
	}

	//NPCの処理を呼び出す
	//第三引数からは特定の場合で使用
	//arg_1 : NPCの名前
	//arg_2 : 引数一つ目
	//arg_3 : 引数二つ目(バトルシーンに遷移する処理のみ使用)
	//arg_4 : 引数三つ目(バトルシーンに遷移する処理のみ使用)
	//arg_5 : 引数四つ目(バトルシーンに遷移する処理のみ使用)
	//arg_6 : 引数五つ目(バトルシーンに遷移する処理のみ使用)
	void EventSelectProcces(const std::string& npc_name, const int& value , const int& value_2 = 0 , const std::string& bgm_1 = "" , const std::string& bgm_2 = "", const int& value_3 = 0);

	//敵に話かけた時のコメント処理
	//主にバトルに移行させる
	//arg_1 : 背景
	//arg_2 : 敵のID
	//arg_3 : 止めるBGM
	//arg_4 : 新しく流すBGM
	//arg_5 : シーンのどこにいるのか(InMapSceneのInMapStateの列挙体から指定する)
	void EventEnemyProcces(const int& back_ground, const int& enemy_id,const std::string& bgm_1, const std::string& bgm_2, const int& in_map_state);

	//コメントを流し切ってからEnterキーでシーンを遷移させる
	bool ComentEventChange(const int& count_max);

private:

	//コメントを流しきるだけの処理
	std::unordered_map<std::string ,std::function<void(BaseScene*, int) >> coment_function_map;

	//引数にint型を持ったmap(主に選択ウィンドウなどの処理で使用する)
	std::unordered_map<std::string, std::function<void(int , int , const std::string& , const std::string& , int)>> function_map;

	//文字列アニメーションを行ったかのフラグ
	bool coment_animation_flag = false;
};