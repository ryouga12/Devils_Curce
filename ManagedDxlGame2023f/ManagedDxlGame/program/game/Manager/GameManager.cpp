#include "GameManager.h"


//�V���O���g��
GameManager* GameManager::GetGameManager()
{
	static GameManager* p_instance = nullptr;

	if (!p_instance)p_instance = new GameManager();
	return p_instance;
}

void GameManager::DelateGameManager()
{
	delete GetGameManager();
}

//���ꂼ��̃|�C���^�[�𐶐�����
void GameManager::PoiterCreate()
{
	//�C���x���g���̃|�C���^

	if (inventory == nullptr) {
		inventory = std::make_shared<Inventory>();
	}
	else {
		tnl::DebugTrace("�|�C���^�͂��łɑ��݂��܂�");
	}
	//�v���C���[�̃|�C���^
	if (player == nullptr) {
		player = std::make_shared<Player>();
	}
	else {
		tnl::DebugTrace("�|�C���^�͂��łɑ��݂��܂�");
	}
	//�J�����̃|�C���^
	if (camera == nullptr) {
		camera = std::make_shared<KonCamera>();
	}
	else {
		tnl::DebugTrace("�|�C���^�͂��łɑ��݂��܂�");
	}

}

//�|�C���^�����Z�b�g����
void GameManager::Reset()
{
	if (inventory != nullptr) {
		inventory.reset();
	}
	else {
		tnl::DebugTrace("�|�C���^�͑��݂��܂���");
	}
	if (player != nullptr) {
		player.reset();
	}
	else {
		tnl::DebugTrace("�|�C���^�͑��݂��܂���");
	}
	if (camera != nullptr) {
		camera.reset();
	}
	else {
		tnl::DebugTrace("�|�C���^�͑��݂��܂���");
	}
}

int GameManager::GetRandEx(int a, int b)
{

	if (a > b) {
		int diff = static_cast<int>(a - b);
		int random = static_cast<int>(GetRand(diff) + b);

		return random;
	}
	else {
		int diff = static_cast<int>(b - a);
		int random = static_cast<int>(GetRand(diff) + a);

		return random;
	}

	return 0;
}

bool GameManager::TimeCount(float delta_time, float Time)
{
	//�w�肵�����Ԃ܂Ńv���X����
	count_time += delta_time;

	//�w�莞�Ԃ��߂�����True�ɂ���
	if (count_time > Time) {
		count_time = 0;
		return true;
	}
	return false;
}



