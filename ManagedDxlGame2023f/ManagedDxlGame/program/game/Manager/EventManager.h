#pragma once

//------------------------------------------
// シングルトン設計


#include"../Manager/ResourceManager.h"
#include "../../dxlib_ext/dxlib_ext.h"
#include"../Manager/GameManager.h"
#include<unordered_map>


class EventManager final {

public:

	static EventManager* getEventManager();
	static void delateEventManager() { delete getEventManager(); }

private:

	//メニュー画面で表示するアイコン的なもの
	int Armsdealer_icon_hdl = 0;
	int herdman_icon_hdl = 0;
	int Priest_icon_hdl = 0;
	int Player_icon_hdl = 0;

	EventManager();

	~EventManager();

	//メニューの使い分け
	enum class MenuOpenNum {
		Empty,
		WeaponShop,			//武器屋
		Herdman,			//村長
		Priest,				//神官
		Object				//オブジェクト
	};

	//どのメニューを開いているか
	MenuOpenNum select_menu_num = MenuOpenNum::Empty;

	//Npcによって動作を変える
	enum class NpcMove {
		Empty,				//空
		Herdman,			//村長
		Priest				//神官
	};

	//誰が動作を行うか
	NpcMove npcMove = NpcMove::Empty;

	//---村長---//

	//村長のアイコンの座標
	tnl::Vector2i herdman_icon_pos{ 110 , 600 };

	//村長のコメントの座標
	tnl::Vector2i herdman_coment_pos = { 150 , 520 };

	//村長のサイズ
	float herdman_scale = 1.5f;

	//村長のコメント
	std::string herdman_coment[3] = {"今までこの大地はあの悪魔に支配されている","もうこれ以上耐えきれない","あの悪魔は右の枯れた大地に" };
	std::string herdman_coment_2[2] = {"城を建てて住んでいると聞いた","どうか力をつけて倒してくれ"};

	//どのコメントを表示するか
	bool herdman_first_coment = true;
	bool herdman_second_coment = false;

	//文字列アニメーションを何文字表記ずつ表示するか
	int add_num = 1;

	//行数
	int line = 0;
	
	//文字数を制限する
	int count = 0;

	//文字列を格納する配列
	std::string text[2];

	//文字列アニメーションを行ったかのフラグ
	bool ComentAnimation_flag = false;

public:

	//menu関連の初期化
	void InitMenu();

	//npcのDrawを切り替える
	void NpcComentDraw();

	//武器屋のコメントの表示
	void armsdealerComentDraw(int type);

	//オブジェクトのコメント表示
	void objectComentDraw();

	// 条件式を定義した関数
	bool isNpcMenuOpen();

	//メニューの切替
	MenuOpenNum SetMenuNpc(int npcType);

	//Npcのコメントを表示する
	void NpcSwitchComent();

	//NPCのコメントの切り替え
	NpcMove SetNpcComent(int npcType);

	//コメントのアニメーション
	void NpcComent(int x , int y ,int max_coment_num , std::string npc_coment[]);

	//コメントを読み込む
	void ComentLoad(const std::string& newComent);


private:

	//武器商人のコメントの座標
	tnl::Vector2i armsdealerComent = { 150, 550 };

	//武器屋の話しかけたときのコメント
	int armsdealer_first_comet = 0;

	//武器屋の文字の座標
	tnl::Vector2i weaponShop = { 60 , 520 };

	//武器屋の購入コメントの座標
	tnl::Vector2i WeaponShop_buy = { 70, 100 };

	//武器屋のメニューを閉じるコメントの座標
	tnl::Vector2i menu_close = { 70, 150 };

	//白色
	int Color_White = -1;

	//武器屋のコメント
	int armsdealer_comet = 1;

	//武器屋に話かけたときのコメント
	int armsdealer_speak_coment = 0;

	//オブジェクトを触った時のコメントの座標
	tnl::Vector2i object_coment = { 150 , 550 };

	//メニューを閉じるコメントの描画座標
	tnl::Vector2i object_menu_close = { 150 ,650 };

	//神官のコメント
	std::string priest_coment[2] = {"よく来たな","記録していくか？"};

	//神官のコメントの座標
	tnl::Vector2i priest_coment_pos = { 150, 570 };

	//コメントの最大数
	int coment_max_num = 3;

	//コメントを管理するためのマップ
	std::vector<std::string>Npc_coment_Array;


//--------------------------------------------------------------------------------------------------------
//それぞれのフラグ

private:

	//---バトルシーンで使うフラグ---//

//--------------------------------------------------------------------------------------------------------
//ポインタ
private:

	//選択メニューウィンドウ
	Shared<Menu>menuSelectWindow = nullptr;
	//メニューのコメントウィンドウ
	Shared<Menu>menuComentWindiow = nullptr;

};