#pragma once
///
///NPCクラス
///

enum  {
	NPC_DIR_DOWN,
	NPC_DIR_UP,
	NPC_DIR_LEFT,
	NPC_DIR_RIGHT,
	NPC_DIR_MAX
};


#include "../../dxlib_ext/dxlib_ext.h"
#include"../Manager/Camera.h"

class Npc {
public:

	//Npcのコンストラクタ : 引数 : ( 名前 , 座標 , ハンドルのPass , 画像の大きさ , どのアニメーションから始めるか, ハンドルの数 )
	Npc(const std::string& name, const tnl::Vector3& pos , std::string ghdl_pass[], float scale , int animation_namber , int ghdl_num);
	~Npc();

	void ChengeAnimation();

	//更新処理
	void Update(float delta_time);

	//描画
	void Draw(const KonCamera& camera);

	//Npcのキャラチップの幅と高さを取得する
	int getNpcSize(int type);

	//Npcの座標を取得する
	tnl::Vector3 getNpcPos() { return npc_pos_; }


//------------------------------------------------------------------------------------------------------------------------
//アニメーション

private:

	//Npcのアニメーションを切り替える
	int animation_select = 0;			

	//アニメーションのカウント
	float anim_time_count = 0;						

	//アニメーションの選択
	int anim_ctrl_dir = NPC_DIR_DOWN;				

	//フレーム
	int anim_ctrl_frame = 0;						

	//アニメーションの配列
	int anim_hdls[NPC_DIR_MAX][5];		

public:

	//アニメーションセレクトを取得する
	inline int getAnimationSelect() { return animation_select; }

	//アニメーションをリセットする
	void NpcAnimationReset() { animation_select = 0; }

	//アニメーションをセットする
	void SetNpcAnimation(int animationID) { animation_select = animationID; }


//------------------------------------------------------------------------------------------------------------------------
//メニュー関係

private:

	//武器屋の話しかけたときのコメント
	int armsdealer_first_comet = 0;

	//武器屋の文字の座標
	tnl::Vector2i weaponShop = { 60 , 520 };

	//白色
	int Color_White = -1;

	//武器商人のコメントの座標
	tnl::Vector2i armsdealerComent = { 150, 550 };

	//武器屋の購入コメントの座標
	tnl::Vector2i WeaponShop_buy = { 70, 100 };

	//武器屋のメニューを閉じるコメントの座標
	tnl::Vector2i menu_close = { 70, 150 };



//------------------------------------------------------------------------------------------------------------------------
//Npc関係
private:

	//npcの名前
	std::string npc_name_;

	//Npcのキャラチップの幅
	int NpcCharaChipWidth = 48;

	//Npcのキャラチップの高さ
	int NpcCharaChipHeight = 48;

	//それぞれの画像で使う大きさを決める
	float scale_ = 0;

	//npcの画像
	int  npc_ghdl;

	//npcの座標
	tnl::Vector3 npc_pos_ = {};

	//npcのサイズを取得する為のタイプ
	int Width = 1; int Height = 2;

};