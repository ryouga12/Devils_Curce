#include"BaseScene.h"
#include"../Manager/UiManager.h"
#include"../Manager/GameManager.h"
#include"../Item/Inventory.h"


void BaseScene::Update(float delta_time)
{
    //タブキーを押したら
    if (tnl::Input::IsKeyDownTrigger(eKeys::KB_TAB)) {

        //説明のウィンドウを消す
        //プレイヤーによって消せるようにする為
        UIManager::GetUIManager()->PlayerDetailSwitchDisplay();
    }
    //0を押したらスキルを全て追加する(デバック用)
    else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_0)) {
        GameManager::GetGameManager()->GetPlayer()->DebugAddSkill();
    }
   
}

void BaseScene::Draw()
{
    auto player_detail_text = UIManager::GetUIManager()->GetPlayerMoveDetailText();

    if (curent_scene == SceneState::INMAP|| curent_scene == SceneState::WORLDMAP) {
        player_detail_text = UIManager::GetUIManager()->GetPlayerMoveDetailText();
    }
    else if (curent_scene == SceneState::BATTLE) {
        player_detail_text = UIManager::GetUIManager()->GetBattlePlayerMoveDetailText();
    }

    //イベント通知用のウィンドウメッセージ
    UIManager::GetUIManager()->DisplayEventMessage();

    //半透明にする
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, koni::Numeric::ALPHA_80_PERCENT);

    //操作説明
    UIManager::GetUIManager()->PlayerMoveDetail(player_detail_text);

    //デバック用の文字を表示する
    UIManager::GetUIManager()->DebugDetailWindow(curent_scene);

    //アルファ値を戻す
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, koni::Numeric::ALPHA_OPAQUE);

    
}
