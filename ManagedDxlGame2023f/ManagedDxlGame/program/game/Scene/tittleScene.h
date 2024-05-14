#pragma once
///
///タイトル画面の処理など
///


#include"../Scene/BaseScene.h"
#include "../../dxlib_ext/dxlib_ext.h"
#include"../Manager/SceneManager.h"
#include"village.h"
#include"../Manager/GameManager.h"
#include"../Menu/MenuWindow.h"
#include"../Item/Item.h"
#include"../Object/Enemy.h"
#include"../Manager/SoundManager.h"
#include"../../game/Skill/Skill.h"


class TittleScene : public BaseScene  {
public:

	TittleScene();
	~TittleScene()override;

	void Update(float delta_time) override;
	void Draw()override;

	


private:
	tnl::Sequence<TittleScene> sequence_ = tnl::Sequence<TittleScene>(this, &TittleScene::seqIdle);
	bool seqIdle(float delta_time);


	//背景画像
	int tittle_ghdl = 0;

	//カラー（黒）
	int String_Color_Black = 0;


	//最初に遷移した時の座標
	tnl::Vector3 first_pos_ = { 195, 312 , 0 };						

	//遅延させる秒数
	float Time = 1.0f;
	


//------------------------------------------------------------------------------------------------------------------------	
//ポインタ

	Shared<Menu> tittle_Menu = nullptr;
	Shared<Inventory>inventory_ = nullptr;
	Shared<Player>myplayer = nullptr;
	Shared<Skill>skill = nullptr;
	/*Shared<Enemy>enemy = nullptr;*/
	

	//シーンを切り替える
	bool SceneChange = false;
	bool seqChangeScene(float delta_time) { return true; }

};