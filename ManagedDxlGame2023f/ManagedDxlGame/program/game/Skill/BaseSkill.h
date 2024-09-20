//------------------------------------------------------------------------------------------------------------
//
//�X�L���N���X�̃x�[�X�N���X
//
//-------------------------------------------------------------------------------------------------------------


#pragma once
#include "../../dxlib_ext/dxlib_ext.h"

class BaseSkill {

protected:

	//�X�L�����Ƃ�id
	int skill_id = 0;
	//���Z�̖��O
	std::string skill_name;
	//���Z�̈З�
	float power = 0;
	//���Z�̐���
	std::string description;
	//���Z���g�����ۂ̏���Mp
	int consume_mp = 0;
	//���̃X�L�����U���n���o�t�n�����f����ϐ�
	int skill_type = 0;
	

public:

	BaseSkill() {}

	//�R���X�g���N�^ : �����Ŗ��O�ƈЗ͂Ɠ��Z�̐����Ɠ��Z�g�p���̏���MP��ݒ肷��
	BaseSkill(const int& id ,const std::string& _Skillname, const float& _Power , const std::string& description_ ,const int& consumeMp , const int& skilltype) : skill_id(id), skill_name(_Skillname), power(_Power), description(description_), consume_mp(consumeMp),skill_type(skilltype){}

};