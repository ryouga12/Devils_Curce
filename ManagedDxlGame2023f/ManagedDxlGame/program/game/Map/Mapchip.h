///
///MapChipクラス
///

#pragma once
#include "../../dxlib_ext/dxlib_ext.h"
#include"../System/Camera.h"
#include"../../koni_name_space/common/common_value.h"


class MapChip {
public:
	MapChip() = default;
	MapChip(const tnl::Vector3& pos , const int gfx_hdl);
	void Update(float delta_time);
	void Draw(const KonCamera& camera);

	//マップチップサイズの取得
	tnl::Vector2i GetChipSize()const {
		return CHIP_SIZE;
	}

	//マップチップの座標取得
	tnl::Vector3& MapChipPos() {
		return pos_;
	}

private:

	tnl::Vector2i CHIP_SIZE = { 32, 32 };				//マップチップのサイズ
	int chip_gpc_hdl = 0;								//画像のhdl
	tnl::Vector3 pos_ = { 0, 0, 0 };					//マップチップのpos
};