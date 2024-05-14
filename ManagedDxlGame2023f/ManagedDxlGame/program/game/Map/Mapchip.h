///
///MapChipクラス
///

#pragma once
#include "../../dxlib_ext/dxlib_ext.h"
#include"../Manager/Camera.h"


class MapChip {
public:
	MapChip() {};
	MapChip(const tnl::Vector3& pos , int gfx_hdl);
	void Update(float delta_time);
	void Draw(const KonCamera& camera);

	//マップチップサイズの取得
	const float& getChipSize(){
		return CHIP_SIZE;
	}
	//マップチップの座標取得
	tnl::Vector3& MapChipPos() {
		return pos_;
	}

private:

	const float CHIP_SIZE = 32;							//マップチップのサイズ
	int chip_gpc_hdl = 0;								//画像のhdl
	tnl::Vector3 pos_ = { 0, 0, 0 };					//マップチップのpos

};