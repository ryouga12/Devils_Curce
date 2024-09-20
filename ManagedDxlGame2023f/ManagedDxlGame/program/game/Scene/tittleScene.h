//------------------------------------------------------------------------------------------------------------
//
//�^�C�g���V�[��(�ŏ��Ƀv���C���[�̖��O�����߂�L�������C�N�V�[���ƃv�����[�O�𗬂��V�[��������)
//
//-------------------------------------------------------------------------------------------------------------


#pragma once

#include "../../dxlib_ext/dxlib_ext.h"
#include "../Manager/SceneManager.h"
#include "../Manager/GameManager.h"
#include "../Manager/SoundManager.h"

class UIManager;

class TittleScene final: public BaseScene {
public:

    TittleScene();
    ~TittleScene() override;

    void Update(float delta_time) override;
    void Draw() override;

    // �^�C�g�����Ŏg�������S�ʂ𕪂���
    enum class TittleState {

        // �ŏ��̑I�����
        TITTLE,
        //�L�����̉摜�̑I�����
        CHARAIMAGE,
        // �`�����n�߂����̃L�������C�N
        CHARANAME,
        //�v�����[�O
        PROLOGUE
    };

    void SetTittleState(TittleState new_tittle) {
        curent_tittle = new_tittle;
    }

private:

    //�v���C���[�̃L�������C�N�őI���ł���C���[�W�̗񋓑�
    enum PlayerImageSelect {
        //�j�����
        PLAYER_IMAGE_MAN_0,
        //�j�����
        PLAYER_IMAGE_MAN_1,
        //�������
        PLAYER_IMAGE_WOMAN_0,
        //�������
        PLAYER_IMAGE_WOMAN_1,
        //�ő吔
        PLAYER_IMAGE_MAX
    };

    //�L�������C�N�Ŏg�p������W
    const tnl::Vector2i PlAYER_IMAGE_SELECT_POS[PLAYER_IMAGE_MAX] = {
        {300 ,300},
        {500, 300},
        {700, 300},
        {900 ,300},
    };

    //�v���C���[�̉摜��\������ׂ̊i�[���Ă����z��
    int player_image_hdl[PLAYER_IMAGE_MAX];

    //�v���C���[�̉摜�̃n���h��
    const std::string PLAYER_HDL[PLAYER_IMAGE_MAX] =
    {
     "graphics/Player/player_image_0/Player_Icon.png",
     "graphics/Player/player_image_01/Player_Icon.png",
     "graphics/Player/player_girl_image_02/Player_Icon.png",
     "graphics/Player/player_girl_image_03/Player_Icon.png"
    };

    //�J�[�\��
    int cousour = 0;

    //���݂̃J�[�\���̈ʒu
    int curent_player_image_hdl = PlayerImageSelect::PLAYER_IMAGE_MAN_0;

    //�v���C���[����̖��O�̎�t�������������̊m�F�t���O
    bool player_input_name_flag = false;

    // ���O�̓��� (���̏��A10�����ŌŒ�)
    char temp[8] = { 0 };

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

    //�^�C�g���̕����̍��W
    const tnl::Vector2i TITTLE_STRING_POS = { 380, 200 };

    // �^�C�g�����̏��
    TittleState curent_tittle = TittleState::TITTLE;

    // �V�[�P���X�֘A
    tnl::Sequence<TittleScene> sequence_ = tnl::Sequence<TittleScene>(this, &TittleScene::seqInit);

    // �w�i�摜
    int tittle_ghdl = 0;

    // �G���^�[�L�[�̉摜
    int enter_key_hdl = 0;

    // �ŏ��ɑJ�ڂ������̍��W
    const tnl::Vector3 FIRST_POS_ = { 195, 312, 0 };

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

    //�v�����[�O�̍X�V����
    bool seqPrologue(float delta_time);
    

};
