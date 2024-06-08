#include "Weapon.h"
#include<iostream>
#include<algorithm>

Weapon::Weapon()
{
	item_ = std::make_shared<Item>();

	WeaponInint();
}

void Weapon::draw()
{

}

void Weapon::WeaponInint()
{
	
	//��ڂ̓X
	
	//�؂̖_
	weponList.emplace_back(item_->GetItemById(6));
	//�؂̌�
	weponList.emplace_back(item_->GetItemById(4));
	//�؂̃n���}�[
	weponList.emplace_back(item_->GetItemById(5));
	//���̌�
	weponList.emplace_back(item_->GetItemById(2));
	//�|�[�V����
	weponList.emplace_back(item_->GetItemById(3));
	

	//�l�i���Ⴂ���Ƀ\�[�g
	std::sort(weponList.begin(), weponList.end(), [](const ItemBase& left, const ItemBase& right) {
		return left.getItemPrice() < right.getItemPrice();
		});

}

