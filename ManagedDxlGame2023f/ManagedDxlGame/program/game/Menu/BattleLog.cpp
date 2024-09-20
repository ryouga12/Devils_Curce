#include "BattleLog.h"
#include"../Manager/GameManager.h"

void BattleLog::AddLog(const std::string& log)
{
	// ���O����8�𒴂��Ă���ꍇ�A�ŏ��̃��O���폜����
	if (logs.size() >= 8) {
		auto erase_log = logs.begin();
		//�ŏ��̃��O�̓^�[���\�L�Ȃ̂�2�ڂ̗v�f����������
		logs.erase(erase_log++);
	}
	logs.push_back(log);
}

//�A�C�e�����g�p�������̃��O
void BattleLog::AddRecoveryLog(const std::string& player, const std::string& item, int recovery)
{
	std::string log = player + "��" + item + "���g�p���܂����B" + std::to_string(recovery) + "�񕜂��܂����B";
	AddLog(log);
}

void BattleLog::AddItemDrop(const std::string& Player, const std::string& ItemName)
{
	std::string log = Player + "��" + ItemName + "����ɓ��ꂽ�I";
	AddLog(log);
}

void BattleLog::AddBuffLog(const std::string& player, const std::string& Item, const std::string& type, int buff)
{
	std::string log = player + "��" + Item + "���g�p���܂����B" + type + "��"+ std::to_string(buff) + "�オ�����B";
	AddLog(log);
}

//�`�揈��
void BattleLog::DrawLogs() const
{
	//���O�̕`��ʒu
	int yPos = 50;
	const int LOG_POS_X = 690;

	// ���O���ォ�珇�ɕ`��
	for (const auto& log : logs) {
		DrawStringEx(LOG_POS_X, yPos, koni::Color::WHITE, log.c_str());
		yPos += 20; // ���񃍃O�̈ʒu�𒲐�
	}
}
