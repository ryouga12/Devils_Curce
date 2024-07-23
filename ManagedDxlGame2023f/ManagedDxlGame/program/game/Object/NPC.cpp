#include "NPC.h"
#include"../Manager/ResourceManager.h"
#include"../Manager/GameManager.h"
#include"../Manager/UiManager.h"


//Npcのコンストラクタ : 引数 : ( 名前 , 座標 ,キャラのサイズ , ハンドルのPass , 画像の大きさ , どのアニメーションから始めるか, ハンドルの数 , アイコン )
Npc::Npc(const std::string& name, const tnl::Vector3& pos, const int CHARASIZE, std::string ghdl_pass[], float scale , const NPC_ANIMATION animation_namber , const int ghdl_num , const std::string& npc_icon_image)
{
	//引数を代入して呼び出し元で使う
	npc_name_ = name;
	npc_pos_ = pos;
	scale_ = scale;
	npc_chara_chip.y = CHARASIZE;
	npc_chara_chip.x = CHARASIZE;
	anim_ctrl_dir = animation_namber;
	npc_icon_hdl = ResourceManager::getResourceManager()->LoadGraphEX(npc_icon_image.c_str());
	
	for (int i = 0; i < ghdl_num; i++)
	{
		LoadDivGraph(ghdl_pass[i].c_str(), 3, 3, 1, npc_chara_chip.x, npc_chara_chip.y, anim_hdls[i]);
	}
}

Npc::~Npc()
{
	//Npcの画像解放
	for (int i = 0; i < static_cast<int>(NPC_ANIMATION::NPC_DIR_MAX); i++) {
		for (int k = 0; k < 5; k++) {
			DeleteGraph(anim_hdls[i][k]);
			// 解放後のポインタの値をデバッグトレースで確認
			tnl::DebugTrace("Deleted Npcimage at [%d][%d]. Pointer value: %p\n", i, k, anim_hdls[i][k]);
		}
	}

}

//更新処理
void Npc::Update(float delta_time)
{
	//アニメーション
	anim_time_count += delta_time;
	if (anim_time_count > ChangeAnimationTime) {
		anim_ctrl_frame++;
		anim_ctrl_frame %= animationFlame;

		anim_time_count = 0;
	}

}

//描画
void Npc::Draw(const KonCamera& camera)
{
	tnl::Vector3 draw_pos = npc_pos_ - camera.getTargetPos() + tnl::Vector3(DXE_WINDOW_WIDTH >> 1, DXE_WINDOW_HEIGHT >> 1, 0);
	DrawRotaGraph(static_cast<int>(draw_pos.x), static_cast<int>(draw_pos.y), scale_, COLOR_BRACK, anim_hdls[static_cast<int>(anim_ctrl_dir)][anim_ctrl_frame], true);
}

//---------------------------------------------------------------------------------------------------------
//セーブを行うNPC





