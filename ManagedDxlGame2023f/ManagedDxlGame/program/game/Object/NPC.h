#pragma once
//------------------------------------------------------------------------------------------------------------
//
//NPCクラス
//
//-------------------------------------------------------------------------------------------------------------


#include "../../dxlib_ext/dxlib_ext.h"
#include"../System/Camera.h"
#include"Actor.h"
#include"../Menu/MenuWindow.h"
#include<unordered_map>

class MenuWindow;

class Npc  final : public Actor{
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

	//Npcのコンストラクタ 
	//arg_1  : 名前 
	//arg_2  : 座標 
	//arg_ 3 : キャラのサイズ 
	//arg_4  : 画像の大きさ 
	//arg_ 5 : どのアニメーションから始めるか
	//arg_ 6 : npcのアイコン )
	Npc(const std::string& name, const tnl::Vector3& pos ,const int& CHARASIZE, const float& scale , const NPC_ANIMATION& animation_namber , const std::string& npc_icon_image);
	
	//当たり判定のないNpc
	//arg_1 : 名前
	//arg_2 : アイコンのハンドル 
	Npc(const std::string& name , const std::string& icon_hdl);
		
	~Npc();

	//更新処理
	void Update(float delta_time)override;

	//描画
	void Draw(const KonCamera& camera)override;

	//Npcのキャラチップの幅と高さを取得する
	int GetNpcSizeWidth()const {
		return npc_chara_chip.x;
	}
	int GetNpcSizeHegiht()const {
		return npc_chara_chip.y;
	}

	//Npcのキャラのサイズを取得する
	const tnl::Vector2i& GetNpcCharaChip()const {
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

	//Npcのアイコンを取得する
	int GetNpcIconImage()const {
		return npc_icon_hdl;
	}

	//Npcの画像をロードする
	//arg_1 : Npcの名前,InMapScene内の現在のシーン
	//名前と現在のシーンを両方比較して該当した画像を格納する
	void SetNpcImage(const std::string& npc_name , const int& inmap_state);

//------------------------------------------------------------------------------------------------------------------------
//アニメーション

private:

	//アニメーションのカウント
	float anim_time_count = 0;						

	//アニメーションの選択
	NPC_ANIMATION anim_ctrl_dir = NPC_ANIMATION::NPC_DIR_DOWN;

	//フレーム
	int anim_ctrl_frame = 0;						

	//アニメーションの配列
	int anim_hdls[static_cast<int>(NPC_ANIMATION::NPC_DIR_MAX)][5];

public:

	//アニメーションをセットする
	void SetNpcAnimation(NPC_ANIMATION new_animation) { anim_ctrl_dir = new_animation; }

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

