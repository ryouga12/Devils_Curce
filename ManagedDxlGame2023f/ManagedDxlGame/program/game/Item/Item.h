#pragma once
///
///アイテムを管理するクラス
/// 
#include<string>
#include "../../dxlib_ext/dxlib_ext.h"
#include"../Manager/ResourceManager.h"
#include"../Item/ItemBase.h"



class Item : public  ItemBase{
public:
	
	Item();
	~Item();

	void update(float delta_time);
	void draw();

	//アイテムの配列を取得する
	const std::list<ItemBase>&getItemArray()const {
		return m_Item_Status_Type;
	}

private:

	//読み込み関数
	void ItemLoadTypeInfo();
	
	//modelを入れる変数
	ItemBase item_Status_ary;

	//構造体の総数
	int m_item_Status_Total_num;

	//Csv読み取り用の配列
	std::vector<std::vector<tnl::CsvCell>>m_itemStatus_Csv_Info;

	//構造体を格納する配列
	std::list<ItemBase>m_Item_Status_Type;

	//アイテムなどの説明文などを入れる配列
	std::string ItemDisc = {};

	//itemの座標
	tnl::Vector3 item_pos = {};


public:

	//IDを指定してアイテムを取得する関数
	ItemBase GetItemById(int id) const;

};