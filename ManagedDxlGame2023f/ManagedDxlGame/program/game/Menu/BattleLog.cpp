#include "BattleLog.h"

void BattleLog::addLog(const std::string& log)
{
	// ���O����8�𒴂��Ă���ꍇ�A�ŏ��̃��O���폜����
	/*if (logs.size() >= 8) {
		logs.erase(logs.begin());
	}*/
	logs.push_back(log);
}

//�_���[�W��^�������̃��O
void BattleLog::addDamageLog(const std::string& attacker, const std::string& target, int damage)
{
	std::string log = attacker + "��" + target + "��" + std::to_string(damage) + "�̃_���[�W��^����";
	addLog(log);
}

//�_���[�W������������̃��O
void BattleLog::addDamagePlayerLog(const std::string& attacker, const std::string& target, int damage)
{
	std::string log = target + "��" + attacker + "����" + std::to_string(damage) + "�̃_���[�W���������";
	addLog(log);
}

//�A�C�e�����g�p�������̃��O
void BattleLog::addRecoveryLog(const std::string& player, const std::string& Item, int recovery)
{
	std::string log = player + "��" + Item + "���g�p���܂����B" + std::to_string(recovery) + "�񕜂��܂����B";
	addLog(log);
}

//���Z���g�������̃��O
void BattleLog::addSkillUseLog(const std::string& name, const std::string& Skillname, const std::string& target ,int damage)
{
	std::string log = name + "��" + Skillname + "���g�p���܂����B" + target + "��" +  std::to_string(damage) + "�̃_���[�W��^�����B";
	addLog(log);
}

//���Z���o�������̃��O
void BattleLog::addSkillLog(const std::string& skillName)
{
	std::string log = skillName + "���o����!!";
	addLog(log);

}

//�S�[���h����ɓ��ꂽ���̃��O
void BattleLog::addMoney(const std::string& Player, int money)
{
	std::string log = Player + "��" + std::to_string(money) + "�S�[���h����ɓ��ꂽ�I";
	addLog(log);
}

void BattleLog::addItemDrop(const std::string& Player, const std::string& ItemName)
{
	std::string log = Player + "��" + ItemName + "����ɓ��ꂽ�I";
	addLog(log);
}

void BattleLog::addBuffLog(const std::string& player, const std::string& Item, const std::string& type, int buff)
{
	std::string log = player + "��" + Item + "���g�p���܂����B" + type + "��"+ std::to_string(buff) + "�オ�����B";
	addLog(log);
}

//�`�揈��
void BattleLog::drawLogs() const
{
	//���O�̕`��ʒu
	int yPos = 50;
	// ���O���ォ�珇�ɕ`��
	for (const auto& log : logs) {
		DrawStringEx(690, yPos, -1, log.c_str());
		yPos += 20; // ���񃍃O�̈ʒu�𒲐�
	}
}
