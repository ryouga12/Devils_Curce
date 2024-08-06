#pragma once
#include "../../dxlib_ext/dxlib_ext.h"


class ItemBase {
public:

	//�A�C�e���̖��O���擾����
	const std::string& GetItemName()const {
		return name;
	}
	//�A�C�e����id���擾����
	int GetItemId()const {
		return id;
	}
	//�A�C�e���̃^�C�v�̎擾����
	int GetItemType()const {
		return type;
	}
	//�A�C�e����EPIC���擾����
	int GetEpic()const {
		return epic;
	}
	//�A�C�e���̍U���͂��擾����
	int GetItemDamage()const {
		return damage;
	}
	//�A�C�e���̖h��͂��擾����
	int GetItemDefance()const {
		return defance;
	}
	//�A�C�e���̂������擾����
	int GetItemPrice()const {
		return price;
	}

	//�A�C�e���̏ڍׂ��擾����
	const std::string& GetItemDetail()const {
		return ItemDetail;
	}
	//���ꕐ��̕␳�����郂���X�^�[�̖��O�̎擾
	const std::string& GetEpicCorrection()const {
		return epic_correction;
	}

	
	//����̃^�C�v���Z�b�g����
	int GetItemWeapontype()const {
		return weaponType;
	}

	//�A�C�e���̖��O���Z�b�g����
	void SetItemName(const std::string& newname) {
		name = newname;
	}
	//�A�C�e����id���Z�b�g����
	void SetItemId(const int& newid) {
		id = newid;
	}
	//�A�C�e���̃^�C�v���Z�b�g����
	void SetItemType(const int& newtype) {
		type = newtype;
	}
	//�A�C�e����Epic���Z�b�g����
	void SetItemEpic(const int& new_epic) {
		epic = new_epic;
	}
	//�A�C�e���̍U���͂��Z�b�g����
	void SetItemDamage(const int& newdamage) {
		damage = newdamage;
	}
	//�A�C�e���̖h��͂��Z�b�g����
	void SetItemDefance(const int& newdefance) {
		defance = newdefance;
	}
	//�A�C�e���̂������Z�b�g����
	void SetItemPrice(const int& newprice) {
		price = newprice;
	}
	//����̃^�C�v���Z�b�g����
	void SetWeaponType(const int& newweapontype) {
		weaponType = newweapontype;
	}
	//�A�C�e���̏ڍׂ��Z�b�g����
	void SetItemDetail(const std::string& newdetail) {
		ItemDetail = newdetail;
	}
	//�G�̃_���[�W�ɕ␳�p�̎푰���擾����
	void SetEpicCorrection(const std::string& new_correction) {
		epic_correction = new_correction;
	}


private:

	//�A�C�e���̖��O
	std::string name;
	//�A�C�e����id
	int id = 0;
	//�A�C�e���̃^�C�v
	int type = 0;
	//����A�C�e��
	int epic = 0;
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
	//���ꕐ��̂ǂ̃����X�^�[�̃_���[�W�ɕ␳�������邩
	std::string epic_correction;

};