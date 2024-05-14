#include "GameManager.h"


//�V���O���g��
GameManager* GameManager::getGameManager()
{
	static GameManager* p_instance = nullptr;

	if (!p_instance)p_instance = new GameManager();
	return p_instance;
}



GameManager::GameManager()
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
	//�X�L���̃|�C���^
	if (skill == nullptr) {
		skill = std::make_shared<Skill>();
	}
	else {
		tnl::DebugTrace("�|�C���^�͂��łɑ��݂��܂�");
	}
}

GameManager::~GameManager()
{
	

}

void GameManager::draw()
{

	
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

void GameManager::displayDialogue()
{
	DrawStringEx(80, 100, -1, "(1 : �b��)");
	DrawStringEx(80, 150, -1, "(0 : ���j���[�����)");
}

bool GameManager::TimeCount(float delta_time, float Time)
{
	count_Time += delta_time;

	if (count_Time > Time) {
		count_Time = 0;
		return true;
	}

	return false;
}




