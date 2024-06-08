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
	
	//木の棒
	weponList.emplace_back(item_->GetItemById(6));
	//木の剣
	weponList.emplace_back(item_->GetItemById(4));
	//木のハンマー
	weponList.emplace_back(item_->GetItemById(5));
	//銅の剣
	weponList.emplace_back(item_->GetItemById(2));
	//ポーション
	weponList.emplace_back(item_->GetItemById(3));
	

	//値段が低い順にソート
	std::sort(weponList.begin(), weponList.end(), [](const ItemBase& left, const ItemBase& right) {
		return left.getItemPrice() < right.getItemPrice();
		});

}

