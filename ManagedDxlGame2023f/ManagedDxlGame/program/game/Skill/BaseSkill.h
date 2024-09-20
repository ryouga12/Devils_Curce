//------------------------------------------------------------------------------------------------------------
//
//スキルクラスのベースクラス
//
//-------------------------------------------------------------------------------------------------------------


#pragma once
#include "../../dxlib_ext/dxlib_ext.h"

class BaseSkill {

protected:

	//スキルごとのid
	int skill_id = 0;
	//特技の名前
	std::string skill_name;
	//特技の威力
	float power = 0;
	//特技の説明
	std::string description;
	//特技を使った際の消費Mp
	int consume_mp = 0;
	//そのスキルが攻撃系かバフ系か判断する変数
	int skill_type = 0;
	

public:

	BaseSkill() {}

	//コンストラクタ : ここで名前と威力と特技の説明と特技使用時の消費MPを設定する
	BaseSkill(const int& id ,const std::string& _Skillname, const float& _Power , const std::string& description_ ,const int& consumeMp , const int& skilltype) : skill_id(id), skill_name(_Skillname), power(_Power), description(description_), consume_mp(consumeMp),skill_type(skilltype){}

};