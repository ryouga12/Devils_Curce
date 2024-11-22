//------------------------------------------------------------------------------------------------------------
//
//MapChipクラス
//
//-------------------------------------------------------------------------------------------------------------

#pragma once
#include "../../dxlib_ext/dxlib_ext.h"
#include"../System/Camera.h"
#include"../../koni_name_space/common/common_value.h"


class MapChip final{
public:

	MapChip() = default;

	//arg_1 : マップチップの座標
	//arg_2 : マップチップの画像
	//arg_3 : マップチップのサイズ(デフォルトは32、変更する場合のみ引数に入力を行う)
	//マップチップの座標と画像を設定する
	MapChip(const tnl::Vector3& pos , const int& gfx_hdl , const int& chip_size = 32);
	
	//描画
	void Draw(const KonCamera& camera);

	//マップチップサイズの取得
	tnl::Vector2i GetChipSize()const {
		return map_chip_size;
	}

	//マップチップの座標取得
	tnl::Vector3& MapChipPos() {
		return pos_;
	}

private:

	//マップチップのサイズ
	tnl::Vector2i map_chip_size = {32 , 32};
	//画像のhdl
	int chip_gpc_hdl = 0;								
	//マップチップのpos
	tnl::Vector3 pos_ = { 0, 0, 0 };					
};