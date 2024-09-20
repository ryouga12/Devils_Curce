//------------------------------------------------------------------------------------------------------------
//
//�o�g�����O�N���X(�o�g�����O��ݒ肵�ė���)
//
//-------------------------------------------------------------------------------------------------------------

#pragma once
#include "../../dxlib_ext/dxlib_ext.h"
#include"../../koni_name_space/common/common_value.h"

class BattleLog final{
public:

	//���O��ǉ�����
	void AddLog(const std::string& log);

	//�A�C�e�����g�p�������̃��O
	void AddRecoveryLog(const std::string& player, const std::string& Item, int recovery);

	//�A�C�e������肵�����̏���
	void AddItemDrop(const std::string& Player, const std::string& ItemName);

	//�o�t�n�̃��O
	void AddBuffLog(const std::string& player, const std::string& Item,const std::string& type, int buff);

	//�`�揈��
	void DrawLogs()const;

	//�o�g�����O�̃��Z�b�g
	void LogReset() {
		logs.clear();
	}

private:

	//�o�g�����O���i�[����z��
	std::list<std::string>logs;

};