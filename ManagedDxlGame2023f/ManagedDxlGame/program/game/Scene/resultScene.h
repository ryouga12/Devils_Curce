#pragma once
#include"../Manager/SoundManager.h"
#include"../Menu/MenuWindow.h"
#include "../../dxlib_ext/dxlib_ext.h"
#include"../Manager/SceneManager.h"
#include"tittleScene.h"

class UIManager;

class ResultScene : public BaseScene {
public :
	ResultScene();
	~ResultScene()override;

	void Update(float delta_time)override;
	void Draw()override;
		
private:

	tnl::Sequence<ResultScene> sequence_ = tnl::Sequence<ResultScene>(this, &ResultScene::seqIdle);
	bool seqIdle(float delta_time);

	Shared<Menu>risult_menu = nullptr;

	int enter_key = 0;
	

};
