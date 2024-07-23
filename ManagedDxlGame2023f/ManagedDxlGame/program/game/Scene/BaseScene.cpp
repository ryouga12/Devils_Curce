#include"BaseScene.h"
#include"../Manager/UiManager.h"
//#include"InMapScene.h"
//#include"mapScene.h"
//#include"battleScene.h"

void BaseScene::Update(float delta_time)
{
    //タブキーを押したら
    if (tnl::Input::IsKeyDownTrigger(eKeys::KB_TAB)) {

        //説明のウィンドウを消す
        //プレイヤーによって消せるようにする為
        UIManager::getUIManager()->PlayerDetailSwitchDisplay();
    }
}

void BaseScene::Draw()
{
    auto player_detail_text = UIManager::getUIManager()->GetPlayerMoveDetailText();

    if (curent_scene == SceneState::INMAP|| curent_scene == SceneState::WORLDMAP) {
        player_detail_text = UIManager::getUIManager()->GetPlayerMoveDetailText();
    }
    else if (curent_scene == SceneState::BATTLE) {
        player_detail_text = UIManager::getUIManager()->GetBattlePlayerMoveDetailText();
    }

    //操作説明
    UIManager::getUIManager()->PlayerMoveDetail(player_detail_text);
}
