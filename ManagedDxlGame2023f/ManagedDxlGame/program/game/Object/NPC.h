#pragma once
///
///NPCクラス
///


#include "../../dxlib_ext/dxlib_ext.h"
#include"../System/Camera.h"
#include"Actor.h"
#include"../Menu/MenuWindow.h"
#include<unordered_map>

class MenuWindow;
class GameManager;
class UIManager;


class Npc  : public Actor{
public:

	//NPCのアニメーション
	enum  class NPC_ANIMATION {
		//左
		NPC_DIR_LEFT,
		//右
		NPC_DIR_RIGHT,
		//上,
		NPC_DIR_UP,
		//下
		NPC_DIR_DOWN,
		//総数
		NPC_DIR_MAX
	};

	//Npcのコンストラクタ : 引数 : ( 名前 , 座標 , キャラのサイズ ,ハンドルのPass , 画像の大きさ , どのアニメーションから始めるか, ハンドルの数 )
	Npc(const std::string& name, const tnl::Vector3& pos ,const int CHARASIZE, std::string ghdl_pass[],float scale , const NPC_ANIMATION animation_namber , const int ghdl_num, const std::string& npc_icon_image);
	~Npc();

	//更新処理
	virtual void Update(float delta_time)override;

	//描画
	virtual void Draw(const KonCamera& camera)override;

	//Npcのキャラチップの幅と高さを取得する
	int GetNpcSizeWidth()const {
		return npc_chara_chip.x;
	}

	int GetNpcSizeHegiht()const {
		return npc_chara_chip.y;
	}

	//Npcのキャラのサイズを取得する
	tnl::Vector2i GetNpcCharaChip()const {
		return npc_chara_chip;
	}

	//Npcの座標を取得する
	tnl::Vector3& GetNpcPos(){ return npc_pos_; }

	//Npcの名前を取得する
	const std::string& GetNpcName()const {
		return npc_name_;
	}

	//Npcのアニメーションを取得する
	NPC_ANIMATION GetNpcAnimation()const {
		return anim_ctrl_dir;
	}

	//保管しているアニメーションをを取得する
	NPC_ANIMATION GetNpcAnimation_Before()const {
		return before_animation_save;
	}

	//変更する前のアニメーションを保管する
	void SetNpcAnimationSave(NPC_ANIMATION save_animation) {
		before_animation_save = save_animation;
	}

	//Npcのアイコンを取得する
	int GetNpcIconImage()const {
		return npc_icon_hdl;
	}

//------------------------------------------------------------------------------------------------------------------------
//アニメーション

private:

	//アニメーションのカウント
	float anim_time_count = 0;						

	//アニメーションの選択
	NPC_ANIMATION anim_ctrl_dir = NPC_ANIMATION::NPC_DIR_DOWN;

	//一個前のアニメーションを保存しておく為の変数
	//主にアニメーションを戻すために使用
	NPC_ANIMATION before_animation_save;

	//フレーム
	int anim_ctrl_frame = 0;						

	//アニメーションの配列
	int anim_hdls[static_cast<int>(NPC_ANIMATION::NPC_DIR_MAX)][5];


public:

	//アニメーションをセットする
	void SetNpcAnimation(NPC_ANIMATION new_animation) { anim_ctrl_dir = new_animation; }


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

	//Npcのキャラチップのサイズ
	tnl::Vector2i npc_chara_chip = {};

	//それぞれの画像で使う大きさを決める
	float scale_ = 0;

	//npcの画像
	int  npc_ghdl;

	//npcの座標
	tnl::Vector3 npc_pos_ = {};

	//npcのアイコン
	int npc_icon_hdl = 0;

};

