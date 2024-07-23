#include"BaseScene.h"
#include"../Manager/UiManager.h"
//#include"InMapScene.h"
//#include"mapScene.h"
//#include"battleScene.h"

void BaseScene::Update(float delta_time)
{
    //�^�u�L�[����������
    if (tnl::Input::IsKeyDownTrigger(eKeys::KB_TAB)) {

        //�����̃E�B���h�E������
        //�v���C���[�ɂ���ď�����悤�ɂ����
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

    //�������
    UIManager::getUIManager()->PlayerMoveDetail(player_detail_text);
}
