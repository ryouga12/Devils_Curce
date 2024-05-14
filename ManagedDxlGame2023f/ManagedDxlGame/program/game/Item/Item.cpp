#include "Item.h"
#include <vector>
#include <algorithm>


Item::Item()
{
	if (m_itemStatus_Csv_Info.size() == 0) {
		m_itemStatus_Csv_Info = tnl::LoadCsv<tnl::CsvCell>("csv/Item.csv");
		ItemLoadTypeInfo();
	}

	
}

Item::~Item()
{

}

void Item::update(float delta_time)
{

}



void Item::ItemLoadTypeInfo()
{
	int max_num = m_itemStatus_Csv_Info.size();

	m_item_Status_Total_num = max_num;

	for (int y = 1; y < max_num; y++) {
		ItemBase itemStatus_info;

		//csvからnameの取得
		itemStatus_info.SetItemName(m_itemStatus_Csv_Info[y][0].getString());
		//csvからidの取得
		itemStatus_info.SetItemId(m_itemStatus_Csv_Info[y][1].getInt());
		//csvからtypeの取得
		itemStatus_info.SetItemType(m_itemStatus_Csv_Info[y][2].getInt());
		//csvからDamageの取得
		itemStatus_info.SetItemDamage(m_itemStatus_Csv_Info[y][3].getInt());
		//csvからdefanceの取得
		itemStatus_info.SetItemDefance(m_itemStatus_Csv_Info[y][5].getInt());
		//csvからpriceを取得
		itemStatus_info.SetItemPrice(m_itemStatus_Csv_Info[y][6].getInt());
		//csvからWeaponTypeを取得する
		itemStatus_info.SetWeaponType(m_itemStatus_Csv_Info[y][7].getInt());
		//csvからItemDetailを取得する
		itemStatus_info.SetItemDetail(m_itemStatus_Csv_Info[y][8].getString());

		m_Item_Status_Type.emplace_back(itemStatus_info);
	}

	//std::sort(m_Item_Status_Type.begin(), m_Item_Status_Type.end(), ItemStatus{}.id);

	//ダメージ順にソート
	m_Item_Status_Type.sort([](const Item::ItemBase& left, const Item::ItemBase& right){
		return left.getItemDamage() > right.getItemDamage();
	});

}

ItemBase Item::GetItemById(int id) const
{
	auto it = std::find_if(m_Item_Status_Type.begin(), m_Item_Status_Type.end(),
		[id](const ItemBase& item) { return item.getItemId() == id; });

	if (it != m_Item_Status_Type.end()) {
		return *it;
	}
	else {
		// IDに対応するアイテムが見つからない場合は、適当なデフォルト値を返すかエラー処理を行うなど
		return ItemBase{};
	}
}

void Item::draw()
{
	
}

