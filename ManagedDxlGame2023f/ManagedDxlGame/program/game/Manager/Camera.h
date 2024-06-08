//
//カメラクラス
//


#pragma once
#include "../../dxlib_ext/dxlib_ext.h"

class Player;

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
	void SetTargetPos() {
		target_ = camera_trarget_savepos_;
	}

	//カメラの保管座標に現在のカメラ座標を保存する
	void SavePosition(const tnl::Vector3& target_pos) {
		camera_trarget_savepos_ = target_pos;
	}

private:

	//プレイヤーをターゲットにする
	tnl::Vector3 target_;

	//カメラの座標を保管しておく
	tnl::Vector3 camera_trarget_savepos_;
};
