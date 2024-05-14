#pragma once
#include "../../dxlib_ext/dxlib_ext.h"


class ItemBase {
public:

	//�A�C�e���̖��O���擾����
	const std::string& getItemName()const {
		return name;
	}
	//�A�C�e����id���擾����
	int getItemId()const {
		return id;
	}
	//�A�C�e���̃^�C�v�̎擾����
	int getItemType()const {
		return type;
	}
	//�A�C�e���̍U���͂��擾����
	int getItemDamage()const {
		return damage;
	}
	//�A�C�e���̖h��͂��擾����
	int getItemDefance()const {
		return defance;
	}
	//�A�C�e���̂������擾����
	int getItemPrice()const {
		return price;
	}

	//�A�C�e���̏ڍׂ��擾����
	const std::string& getItemDetail()const {
		return ItemDetail;
	}
	
	//����̃^�C�v���Z�b�g����
	int getItemWeapontype()const {
		return weaponType;
	}

	//�A�C�e���̖��O���Z�b�g����
	void SetItemName(const std::string& newname) {
		name = newname;
	}
	//�A�C�e����id���Z�b�g����
	void SetItemId(int newid) {
		id = newid;
	}
	//�A�C�e���̃^�C�v���Z�b�g����
	void SetItemType(int newtype) {
		type = newtype;
	}
	//�A�C�e���̍U���͂��Z�b�g����
	void SetItemDamage(int newdamage) {
		damage = newdamage;
	}
	//�A�C�e���̖h��͂��Z�b�g����
	void SetItemDefance(int newdefance) {
		defance = newdefance;
	}
	//�A�C�e���̂������Z�b�g����
	void SetItemPrice(int newprice) {
		price = newprice;
	}
	//����̃^�C�v���Z�b�g����
	void SetWeaponType(int newweapontype) {
		weaponType = newweapontype;
	}
	//�A�C�e���̏ڍׂ��Z�b�g����
	void SetItemDetail(const std::string& newdetail) {
		ItemDetail = newdetail;
	}


private:

	//�A�C�e���̖��O
	std::string name;
	//�A�C�e����id
	int id = 0;
	//�A�C�e���̃^�C�v
	int type = 0;
	//�A�C�e���̍U����(�����A�C�e��)
	int damage = 0;
	//�A�C�e���̖h���(�����A�C�e��)
	int defance = 0;
	//�A�C�e���̂���
	int price = 0;
	//����̃^�C�v
	int weaponType = 0;
	//�A�C�e���̐���
	std::string ItemDetail;
	
};