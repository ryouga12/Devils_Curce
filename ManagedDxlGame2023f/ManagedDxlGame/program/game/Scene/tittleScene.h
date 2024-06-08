#pragma once
///
///タイトル画面の処理など
///

#include "../../dxlib_ext/dxlib_ext.h"
#include"../Manager/SceneManager.h"
#include"InMapScene.h"
#include"../Manager/GameManager.h"
#include"../Manager/SoundManager.h"

class UIManager;;


class TittleScene : public BaseScene  {
public:

	TittleScene();
	~TittleScene()override;

	void Update(float delta_time) override;
	void Draw()override;

	//タイトル内で使う処理全般を分ける
	enum class TittleState {
		//最初の選択画面
		TITTLE,
		//冒険を始めた時のキャラメイク
		CHARAMIKE,
	};

	


private:
	tnl::Sequence<TittleScene> sequence_ = tnl::Sequence<TittleScene>(this, &TittleScene::seqTittle);
	bool seqTittle(float delta_time);


	//背景画像
	int tittle_ghdl = 0;

	//エンターキーの画像
	int enter_key_hdl = 0;

	//カラー（黒）
	int String_Color_Black = 0;


	//最初に遷移した時の座標
	tnl::Vector3 first_pos_ = { 195, 312 , 0 };						

	//遅延させる秒数
	float Time = 1.0f;
	


//------------------------------------------------------------------------------------------------------------------------	
//ポインタ

	Shared<Menu> menu_window = nullptr;
	Shared<Menu>menu_window_white = nullptr;

	//シーンを切り替える
	bool SceneChange = false;
	bool seqChangeScene(float delta_time) { return true; }

};