
///
///�����Ȃǂ̗v�f
/// 

#pragma once
#include "Item.h"
#include<unordered_map>
#include "../../dxlib_ext/dxlib_ext.h"
#include"../Item/ItemBase.h"

class Actor;

class Weapon : public ItemBase{
public:

	Weapon();

	void draw();

private:

	//������i�[����z��
	std::vector<ItemBase>weponList;

public:

	//����̒l�𑼂̃N���X�Ŏg��
	std::vector<ItemBase>& GetWeaponValue()
	{
		return weponList;
	}

	//���탊�X�g�̐����擾����
	std::vector<ItemBase>& GetWeaponList() {
		return weponList;
	}

	//����̒l���Z�b�g����֐�
	void WeaponInint();

//------------------------------------------------------------------------------------------------------------------------
//---�|�C���^---//
private:
	Shared<Item>item_ = nullptr;

};