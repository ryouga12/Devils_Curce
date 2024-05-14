#pragma once
#include "../../dxlib_ext/dxlib_ext.h"



class BaseSkill {

protected:

	//スキルごとのid
	int Id = 0;
	//特技の名前
	std::string SkillName;
	//特技の威力
	float Power = 0;
	//特技の説明
	std::string description;
	//特技を使った際の消費Mp
	int ConsumeMp = 0;
	//そのスキルが攻撃系かバフ系か判断する変数
	int SkillType = 0;
	

public:

	BaseSkill() {}

	//コンストラクタ : ここで名前と威力と特技の説明と特技使用時の消費MPを設定する
	BaseSkill(int id ,const std::string& _Skillname, float _Power , const std::string& description_ ,int consumeMp , int skilltype) : Id(id), SkillName(_Skillname), Power(_Power), description(description_), ConsumeMp(consumeMp),SkillType(skilltype){}


};