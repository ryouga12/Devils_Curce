#pragma once
#include "../../dxlib_ext/dxlib_ext.h"

class BattleLog {
public:

	//���O��ǉ�����
	void addLog(const std::string& log);

	//�_���[�W��^�������̃��O
	void addDamageLog(const std::string& attacker, const std::string& target, int damage);

	//�_���[�W������������̃��O
	void addDamagePlayerLog(const std::string& attacker, const std::string& target, int damage);

	//�A�C�e�����g�p�������̃��O
	void addRecoveryLog(const std::string& player, const std::string& Item, int recovery);

	//���Z���g�������̃��O
	void addSkillUseLog(const std::string& name, const std::string& Skillname, const std::string& target ,int damage);

	//�X�L�����o�������̃��O
	void addSkillLog(const std::string& skillName);

	//�S�[���h����肵�����̃��O
	void addMoney(const std::string& Player, int money);

	//�A�C�e������肵�����̏���
	void addItemDrop(const std::string& Player, const std::string& ItemName);

	//�o�t�n�̃��O
	void addBuffLog(const std::string& player, const std::string& Item,const std::string& type, int buff);

	//�`�揈��
	void drawLogs()const;

	//�o�g�����O�̃��Z�b�g
	void LogReset() {
		logs.clear();
	}

private:

	//�o�g�����O���i�[����z��
	std::list<std::string>logs;

};