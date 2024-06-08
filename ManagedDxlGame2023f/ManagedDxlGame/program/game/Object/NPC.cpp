#include "NPC.h"

//Npcのコンストラクタ : 引数 : ( 名前 , 座標 ,キャラのサイズ , ハンドルのPass , 画像の大きさ , どのアニメーションから始めるか, ハンドルの数 )
Npc::Npc(const std::string& name, const tnl::Vector3& pos, const int CHARASIZE, std::string ghdl_pass[], float scale , int animation_namber , int ghdl_num)
{
	//引数を代入して呼び出し元で使う
	npc_name_ = name;
	npc_pos_ = pos;
	scale_ = scale;
	npc_chara_chip_height = CHARASIZE;
	npc_chara_chip_width = CHARASIZE;
	animation_select = animation_namber;
	
	for (int i = 0; i < ghdl_num; i++)
	{
		LoadDivGraph(ghdl_pass[i].c_str(), 3, 3, 1, npc_chara_chip_width, npc_chara_chip_height, anim_hdls[i]);
	}
}

Npc::~Npc()
{
	//Npcの画像解放
	for (int i = 0; i < static_cast<int>(NPC_DIR_MAX); i++) {
		for (int k = 0; k < 5; k++) {
			DeleteGraph(anim_hdls[i][k]);
			// 解放後のポインタの値をデバッグトレースで確認
			tnl::DebugTrace("Deleted Npcimage at [%d][%d]. Pointer value: %p\n", i, k, anim_hdls[i][k]);
		}
	}

}

//Npcのアニメーションを切り替える
void Npc::ChengeAnimation()
{
	switch (animation_select) {

	case NPC_DIR_DOWN:

		anim_ctrl_dir = NPC_DIR_DOWN;

		break;

	case NPC_DIR_UP:

		anim_ctrl_dir = NPC_DIR_UP;

		break;

	case NPC_DIR_LEFT:

		anim_ctrl_dir = NPC_DIR_LEFT;

		break;

	case NPC_DIR_RIGHT:

		anim_ctrl_dir = NPC_DIR_RIGHT;

		break;

	default:

		anim_ctrl_dir = NPC_DIR_DOWN;

		break;

	}
}

//更新処理
void Npc::Update(float delta_time)
{
	//アニメーション
	anim_time_count += delta_time;
	if (anim_time_count > CHANGEANIMATIONTIME) {
		anim_ctrl_frame++;
		anim_ctrl_frame %= ANIMATIONFLAME;

		anim_time_count = 0;
	}

}

//描画
void Npc::Draw(const KonCamera& camera)
{
	tnl::Vector3 draw_pos = npc_pos_ - camera.getTargetPos() + tnl::Vector3(DXE_WINDOW_WIDTH >> 1, DXE_WINDOW_HEIGHT >> 1, 0);
	DrawRotaGraph(static_cast<int>(draw_pos.x), static_cast<int>(draw_pos.y), scale_, COLOR_BRACK, anim_hdls[anim_ctrl_dir][anim_ctrl_frame], true);
}
