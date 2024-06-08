#pragma once
///
///NPCクラス
///

//NPCのアニメーション
enum  {
	//下
	NPC_DIR_DOWN,
	//上,
	NPC_DIR_UP,
	//左
	NPC_DIR_LEFT,
	//右
	NPC_DIR_RIGHT,
	//総数
	NPC_DIR_MAX
};


#include "../../dxlib_ext/dxlib_ext.h"
#include"../Manager/Camera.h"
#include"Actor.h"

class Npc  : public Actor{
public:

	//Npcのコンストラクタ : 引数 : ( 名前 , 座標 , キャラのサイズ ,ハンドルのPass , 画像の大きさ , どのアニメーションから始めるか, ハンドルの数 )
	Npc(const std::string& name, const tnl::Vector3& pos ,const int CHARASIZE, std::string ghdl_pass[], float scale , int animation_namber , int ghdl_num);
	~Npc();

	void ChengeAnimation();

	//更新処理
	void Update(float delta_time)override;

	//描画
	void Draw(const KonCamera& camera)override;

	//Npcのキャラチップの幅と高さを取得する
	int GetNpcSizeWidth()const {
		return npc_chara_chip_width;
	}

	int GetNpcSizeHegiht()const {
		return npc_chara_chip_height;
	}

	//Npcの座標を取得する
	tnl::Vector3 GetNpcPos() { return npc_pos_; }

	//Npcの名前を取得する
	const std::string& GetNpcName()const {
		return npc_name_;
	}


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

	//アニメーションを切り替える秒数
	const float CHANGEANIMATIONTIME = 0.1f;
	//フレーム
	const int ANIMATIONFLAME = 3;


public:

	//アニメーションセレクトを取得する
	inline int GetAnimationSelect() { return animation_select; }

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
	int npc_chara_chip_width = 0;

	//Npcのキャラチップの高さ
	int npc_chara_chip_height = 0;

	//それぞれの画像で使う大きさを決める
	float scale_ = 0;

	//npcの画像
	int  npc_ghdl;

	//npcの座標
	tnl::Vector3 npc_pos_ = {};


};