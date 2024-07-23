//
//カメラクラス
//


#pragma once
#include "../../dxlib_ext/dxlib_ext.h"

class Player;

class KonCamera {
public:
	KonCamera();

	//カメラの動き処理
	//引数 : 追従する座標 , マップの幅 , マップの高さ 
	void update(tnl::Vector3 target_pos = {0 , 0 , 0}, int MAP_WIDTH = 0, int MAP_HEIGHT = 0);
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

	//カメラの状態
	//カメラ状態を切り替えてカメラを操作する
	enum class CameraState {
		INPLAY,			//プレイ中
		DEBUG			//デバック
	};

	//カメラシェイクをスタートさせる
	void StartShake(const float duration, const float magnitude);

	// バトル用カメラ座標を取得する
	tnl::Vector3 getBattleCameraPos() const {
		return battle_camera_pos_;
	}

private:

	//---主にマップで使うカメラ---//

	//プレイヤーをターゲットにする
	tnl::Vector3 target_;

	//カメラの座標を保管しておく
	tnl::Vector3 camera_trarget_savepos_;

	//---バトル時に使うカメラ---//

	//バトル時のカメラ
	tnl::Vector3 battle_camera_pos_ = { DXE_WINDOW_WIDTH / 2, DXE_WINDOW_HEIGHT / 2 ,  0 };

	//保存用のカメラ
	tnl::Vector3 battle_camera_save_pos = { DXE_WINDOW_WIDTH / 2, DXE_WINDOW_HEIGHT / 2 ,  0 };

	//---カメラシェイク用のパラメータ---//

	// カメラがシェイク中かどうかを示すフラグ
	bool is_shaking_ = false;       
	// シェイクの継続時間（秒）
	float shake_duration_ = 0;  
	// シェイクの強度（揺れの幅）
	float shake_magnitude_ = 0; 
	// シェイクが開始してからの経過時間（秒）
	float shake_elapsed_ = 0;      

	//カメラシェイク
	void CameraShake();
};
