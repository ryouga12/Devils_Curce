#include"BaseScene.h"
#include"../Manager/UiManager.h"
#include"../Manager/GameManager.h"


void BaseScene::Update(float delta_time)
{
    //�^�u�L�[����������
    if (tnl::Input::IsKeyDownTrigger(eKeys::KB_TAB)) {

        //�����̃E�B���h�E������
        //�v���C���[�ɂ���ď�����悤�ɂ����
        UIManager::GetUIManager()->PlayerDetailSwitchDisplay();
    }
    //0����������X�L����S�Ēǉ�����(�f�o�b�N�p)
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

    //�������ɂ���
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, koni::Numeric::ALPHA_80_PERCENT);
    //�������
    UIManager::GetUIManager()->PlayerMoveDetail(player_detail_text);
    //�A���t�@�l��߂�
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, koni::Numeric::ALPHA_OPAQUE);
}
