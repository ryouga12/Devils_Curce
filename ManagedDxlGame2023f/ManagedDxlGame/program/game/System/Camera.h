//------------------------------------------------------------------------------------------------------------
//
//カメラクラス
//
//-------------------------------------------------------------------------------------------------------------


#pragma once
#include "../../dxlib_ext/dxlib_ext.h"

class Player;

class KonCamera final{
public:
	KonCamera();

	//カメラの動き処理
	//arg_1 : 追従する座標
	//arg_2 : マップの幅 
	//arg_3 : マップの高さ 
	void update(const tnl::Vector3& target_pos, const int& MAP_WIDTH, const int& MAP_HEIGHT);
	void draw();

	//ターゲットの座標を取得する
	tnl::Vector3 GetTargetPos()const{
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

	//---主にマップで使うカメラ---//

	//プレイヤーをターゲットにする
	tnl::Vector3 target_;

	//カメラの座標を保管しておく
	tnl::Vector3 camera_trarget_savepos_;

	//プレイヤーの追跡するスピード
	const float TRACKING_SPPED = 0.05f;

};
