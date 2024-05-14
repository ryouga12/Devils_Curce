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
	
	//一個目の店
	weponList.emplace_back(item_->GetItemById(6));
	weponList.emplace_back(item_->GetItemById(4));
	weponList.emplace_back(item_->GetItemById(5));
	weponList.emplace_back(item_->GetItemById(2));
	weponList.emplace_back(item_->GetItemById(3));
	

	//値段が低い順にソート
	std::sort(weponList.begin(), weponList.end(), [](const ItemBase& left, const ItemBase& right) {
		return left.getItemPrice() < right.getItemPrice();
		});

}

