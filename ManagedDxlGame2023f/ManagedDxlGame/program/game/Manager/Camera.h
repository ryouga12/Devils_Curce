//
//カメラクラス
//


#pragma once
#include "../../dxlib_ext/dxlib_ext.h"


class KonCamera {
public:
	KonCamera();

	void update(tnl::Vector3 target_pos, int MAP_WIDTH, int MAP_HEIGHT);
	void draw();

	//ターゲットの座標を取得する
	tnl::Vector3 getTargetPos()const{
		return target_;
	}

	//ターゲットの座標をセットする
	void SetTargetPos(tnl::Vector3 newpos) {
		target_ = newpos;
	}

private:

	//プレイヤーをターゲットにする
	tnl::Vector3 target_;

};

