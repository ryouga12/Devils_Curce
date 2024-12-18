#pragma once
#include "../../dxlib_ext/dxlib_ext.h"

//---------------------------------------------------------------------------------------------------------
//アイテムのベースクラス


class ItemBase {
public:

	//アイテムの名前を取得する
	const std::string& GetItemName()const {
		return name;
	}
	//アイテムのidを取得する
	int GetItemId()const {
		return id;
	}
	//アイテムのタイプの取得する
	int GetItemType()const {
		return type;
	}
	//アイテムのEPICを取得する
	int GetEpic()const {
		return epic;
	}
	//アイテムの攻撃力を取得する
	int GetItemDamage()const {
		return damage;
	}
	//アイテムの防御力を取得する
	int GetItemDefance()const {
		return defance;
	}
	//アイテムのお金を取得する
	int GetItemPrice()const {
		return price;
	}

	//アイテムの詳細を取得する
	const std::string& GetItemDetail()const {
		return item_detail;
	}
	//特殊武器の補正させるモンスターの名前の取得
	const std::string& GetEpicCorrection()const {
		return epic_correction;
	}
	//特別アイテムかのフラグを取得する
	bool GetEssentialItemsFlag()const {
		return essential_items_flag;
	}

	//アイコンを取得する
	const std::string& GetItemIcon()const {
		return itemes_icon;
	}

	
	//武器のタイプをセットする
	int GetItemWeapontype()const {
		return weaponType;
	}

	//アイテムの名前をセットする
	void SetItemName(const std::string& newname) {
		name = newname;
	}
	//アイテムのidをセットする
	void SetItemId(const int& newid) {
		id = newid;
	}
	//アイテムのタイプをセットする
	void SetItemType(const int& newtype) {
		type = newtype;
	}
	//アイテムのEpicをセットする
	void SetItemEpic(const int& new_epic) {
		epic = new_epic;
	}
	//アイテムの攻撃力をセットする
	void SetItemDamage(const int& newdamage) {
		damage = newdamage;
	}
	//アイテムの防御力をセットする
	void SetItemDefance(const int& newdefance) {
		defance = newdefance;
	}
	//アイテムのお金をセットする
	void SetItemPrice(const int& newprice) {
		price = newprice;
	}
	//武器のタイプをセットする
	void SetWeaponType(const int& newweapontype) {
		weaponType = newweapontype;
	}
	//アイテムの詳細をセットする
	void SetItemDetail(const std::string& newdetail) {
		item_detail = newdetail;
	}
	//敵のダメージに補正用の種族をセットする
	//主にドラゴンキラーなどの特殊武器の為の補正用の情報を取得する
	void SetEpicCorrection(const std::string& new_correction) {
		epic_correction = new_correction;
	}
	//特別アイテムか示すフラグをセットする
	void SetEssentialItemsFlag(const int& new_flag) {
		essential_items_flag = new_flag;
	}
	//アイコンをセットする
	void SetItemIcon(const std::string& new_icon) {
		itemes_icon = new_icon;
	}


private:

	//アイテムの名前
	std::string name;
	//アイテムのid
	int id = 0;
	//アイテムのタイプ
	int type = 0;
	//特殊アイテム
	int epic = 0;
	//アイテムの攻撃力(装備アイテム)
	int damage = 0;
	//アイテムの防御力(装備アイテム)
	int defance = 0;
	//アイテムのお金
	int price = 0;
	//武器のタイプ
	int weaponType = 0;
	//アイテムの説明
	std::string item_detail;
	//特殊武器のどのモンスターのダメージに補正をかけるか
	std::string epic_correction;
	//そのアイテムは特別なアイテムか(主にイベントアイテムなど売却できないアイテムなど)
	bool essential_items_flag = false;
	//アイテムのアイコン
	std::string itemes_icon;
};