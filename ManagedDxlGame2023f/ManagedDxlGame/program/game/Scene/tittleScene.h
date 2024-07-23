#pragma once

#include "../../dxlib_ext/dxlib_ext.h"
#include "../Manager/SceneManager.h"
#include "../Manager/GameManager.h"
#include "../Manager/SoundManager.h"

class UIManager;

class TittleScene : public BaseScene {
public:
    TittleScene();
    ~TittleScene() override;

    void Update(float delta_time) override;
    void Draw() override;

    // �^�C�g�����Ŏg�������S�ʂ𕪂���
    enum class TittleState {
        // �ŏ��̑I�����
        TITTLE,
        // �`�����n�߂����̃L�������C�N
        CHARAMIKE,
    };

    void SetTittleState(TittleState new_tittle) {
        curent_tittle = new_tittle;
    }

private:

    // ���j���[�֘A�̒萔
    const int MENU_Y = 450;
    const int MENU_X = 500;
    const int MENU_WIDTH = 250;
    const int MENU_HEIGHT = 200;

    // �^�C�g���֘A�̒萔
    const int TITLE_TOP = 200;
    const int TITLE_LEFT = 450;
    const int FONT_SIZE_TITLE = 60;
    const int FONT_SIZE_DEFAULT = 16;

    // ���j���[�I�����֘A�̒萔
    const int SELECTION_NUM = 2;
    const int SELECTION_START_X = 550;
    const int SELECTION_START_Y = 500;
    const int SELECTION_OFFSET_Y = 50;

    // �^�C�g�����̏��
    TittleState curent_tittle = TittleState::TITTLE;

    // �V�[�P���X�֘A
    tnl::Sequence<TittleScene> sequence_ = tnl::Sequence<TittleScene>(this, &TittleScene::seqInit);

    // �w�i�摜
    int tittle_ghdl = 0;

    // �G���^�[�L�[�̉摜
    int enter_key_hdl = 0;

    // �ŏ��ɑJ�ڂ������̍��W
    tnl::Vector3 first_pos_ = { 195, 312, 0 };

    // �x��������b��
    float Time = 1.0f;

    // �|�C���^
    Shared<Menu> menu_window = nullptr;
    Shared<Menu> menu_window_white = nullptr;

    Shared<MenuWindow> title_selection = nullptr;
    MenuWindow::MenuElement_t* tittle_selection_coment = nullptr;

    // �V�[����؂�ւ���
    bool SceneChange = false;
    bool seqChangeScene(float delta_time) { return true; }

    // �������V�[�P���X
    bool seqInit(float delta_time);

    // �^�C�g�����̍X�V����
    bool seqTittle(float delta_time);

    // �L�������C�N���̍X�V����
    bool seqCharaMike(float delta_time);
};
