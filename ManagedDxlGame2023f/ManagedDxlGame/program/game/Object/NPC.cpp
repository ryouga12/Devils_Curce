#include "NPC.h"
#include"../Manager/ResourceManager.h"
#include"../Manager/GameManager.h"
#include"../Manager/UiManager.h"
#include"../Manager/CsvManager.h"


//Npcのコンストラクタ 
Npc::Npc(const std::string& name, const tnl::Vector3& pos,const int& CHIP_SIZE, const float& scale , const NPC_ANIMATION& animation_namber , const std::string& npc_icon_image)
{
	//引数を代入して呼び出し元で使う
	npc_name_ = name;
	npc_pos_ = pos;
	scale_ = scale;
	npc_chara_chip.x = CHIP_SIZE;
	npc_chara_chip.y = CHIP_SIZE;
	anim_ctrl_dir = animation_namber;
	npc_icon_hdl = ResourceManager::GetResourceManager()->LoadGraphEX(npc_icon_image.c_str());
}

Npc::Npc(const std::string& name, const std::string& icon_hdl)
{
	npc_name_ = name;
	npc_icon_hdl = ResourceManager::GetResourceManager()->LoadGraphEX(icon_hdl.c_str());
}

Npc::~Npc()
{
	//Npcの画像解放
	for (int i = 0; i < static_cast<int>(NPC_ANIMATION::NPC_DIR_MAX); i++) {
		for (int k = 0; k < 5; k++) {
			DeleteGraph(anim_hdls[i][k]);
			// 解放後のポインタの値をデバッグトレースで確認
			tnl::DebugTrace("\nDeleted Npcimage at [%d][%d]. Pointer value: %p\n", i, k, anim_hdls[i][k]);
		}
	}

	tnl::DebugTrace("\n------------------------------------------------------------");
	tnl::DebugTrace("\n %sが解放されました" , npc_name_.c_str());
	tnl::DebugTrace("\n------------------------------------------------------------");

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
	tnl::Vector3 draw_pos = npc_pos_ - camera.GetTargetPos() + tnl::Vector3(DXE_WINDOW_WIDTH >> 1, DXE_WINDOW_HEIGHT >> 1, 0);
	DrawRotaGraph(static_cast<int>(draw_pos.x), static_cast<int>(draw_pos.y), scale_, koni::Color::BLACK, anim_hdls[static_cast<int>(anim_ctrl_dir)][anim_ctrl_frame], true);
}

//Npcの画像を格納する
void Npc::SetNpcImage(const std::string& npc_name, const int& inmap_state)
{
	//アニメーション画像を格納配列
	std::vector<std::string> npc_image;

	auto npc_image_size = CsvManager::GetCsvManager()->GetNpcImageCsv().size();

	for (int i = 1; i < npc_image_size; i++) {

		//npcの名前とどこのマップに今いるかによってその時の画像のロードを行う
		if (npc_name == CsvManager::GetCsvManager()->GetNpcImageCsv()[i][0].c_str() &&  inmap_state == std::stoi(CsvManager::GetCsvManager()->GetNpcImageCsv()[i][1].c_str())) {
			//2列目までは名前とシーンの数値が入っている為＋2して3列目から数える
			for (int k = 2; k < static_cast<int>(NPC_ANIMATION::NPC_DIR_MAX) + 2; k++) {
				npc_image.emplace_back(CsvManager::GetCsvManager()->GetNpcImageCsv()[i][k]);
			}
		}
	}

	for (int i = 0; i < static_cast<int>(NPC_ANIMATION::NPC_DIR_MAX); i++) {

		LoadDivGraph(
			npc_image[i].c_str(),							// 画像のパス
			TOTAL_FRAME,									// 総フレーム数
			HORIZONTAL_FRAME,								// 横フレーム数
			VERTICAL_FRAME,									// 縦フレーム数
			npc_chara_chip.x,								// 横方向へ 1フレームあたりの幅
			npc_chara_chip.y,								// 縦方向へ 1フレームあたりの幅
			anim_hdls[i]);									// 画像ハンドルを格納する配列名);
	}
}





