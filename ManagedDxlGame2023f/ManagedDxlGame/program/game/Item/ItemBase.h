#pragma once
#include "../../dxlib_ext/dxlib_ext.h"


class ItemBase {
public:

	//アイテムの名前を取得する
	const std::string& getItemName()const {
		return name;
	}
	//アイテムのidを取得する
	int getItemId()const {
		return id;
	}
	//アイテムのタイプの取得する
	int getItemType()const {
		return type;
	}
	//アイテムの攻撃力を取得する
	int getItemDamage()const {
		return damage;
	}
	//アイテムの防御力を取得する
	int getItemDefance()const {
		return defance;
	}
	//アイテムのお金を取得する
	int getItemPrice()const {
		return price;
	}

	//アイテムの詳細を取得する
	const std::string& getItemDetail()const {
		return ItemDetail;
	}
	
	//武器のタイプをセットする
	int getItemWeapontype()const {
		return weaponType;
	}

	//アイテムの名前をセットする
	void SetItemName(const std::string& newname) {
		name = newname;
	}
	//アイテムのidをセットする
	void SetItemId(int newid) {
		id = newid;
	}
	//アイテムのタイプをセットする
	void SetItemType(int newtype) {
		type = newtype;
	}
	//アイテムの攻撃力をセットする
	void SetItemDamage(int newdamage) {
		damage = newdamage;
	}
	//アイテムの防御力をセットする
	void SetItemDefance(int newdefance) {
		defance = newdefance;
	}
	//アイテムのお金をセットする
	void SetItemPrice(int newprice) {
		price = newprice;
	}
	//武器のタイプをセットする
	void SetWeaponType(int newweapontype) {
		weaponType = newweapontype;
	}
	//アイテムの詳細をセットする
	void SetItemDetail(const std::string& newdetail) {
		ItemDetail = newdetail;
	}


private:

	//アイテムの名前
	std::string name;
	//アイテムのid
	int id = 0;
	//アイテムのタイプ
	int type = 0;
	//アイテムの攻撃力(装備アイテム)
	int damage = 0;
	//アイテムの防御力(装備アイテム)
	int defance = 0;
	//アイテムのお金
	int price = 0;
	//武器のタイプ
	int weaponType = 0;
	//アイテムの説明
	std::string ItemDetail;
	
};