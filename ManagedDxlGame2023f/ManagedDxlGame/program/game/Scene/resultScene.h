//------------------------------------------------------------------------------------------------------------
//
//エピローグを流すシーン
//
//-------------------------------------------------------------------------------------------------------------

#pragma once
#include"../Manager/SoundManager.h"
#include"../Menu/MenuWindow.h"
#include "../../dxlib_ext/dxlib_ext.h"
#include"../Manager/SceneManager.h"
#include"../../koni_name_space/common/common_value.h"

class ResultScene final : public BaseScene {
public :

	ResultScene();
	~ResultScene()override;

	void Update(float delta_time)override;
	void Draw()override;
		
private:

	tnl::Sequence<ResultScene> sequence_ = tnl::Sequence<ResultScene>(this, &ResultScene::seqInit);
	bool seqIdle(float delta_time);
	bool seqInit(float delta_time);
	
	//リザルト画面の背景
	int result_background = 0;

};
