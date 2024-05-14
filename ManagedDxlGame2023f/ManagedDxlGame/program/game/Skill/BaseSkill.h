#pragma once
#include "../../dxlib_ext/dxlib_ext.h"



class BaseSkill {

protected:

	//�X�L�����Ƃ�id
	int Id = 0;
	//���Z�̖��O
	std::string SkillName;
	//���Z�̈З�
	float Power = 0;
	//���Z�̐���
	std::string description;
	//���Z���g�����ۂ̏���Mp
	int ConsumeMp = 0;
	//���̃X�L�����U���n���o�t�n�����f����ϐ�
	int SkillType = 0;
	

public:

	BaseSkill() {}

	//�R���X�g���N�^ : �����Ŗ��O�ƈЗ͂Ɠ��Z�̐����Ɠ��Z�g�p���̏���MP��ݒ肷��
	BaseSkill(int id ,const std::string& _Skillname, float _Power , const std::string& description_ ,int consumeMp , int skilltype) : Id(id), SkillName(_Skillname), Power(_Power), description(description_), ConsumeMp(consumeMp),SkillType(skilltype){}


};