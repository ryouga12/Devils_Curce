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

		//csv����name�̎擾
		itemStatus_info.SetItemName(m_itemStatus_Csv_Info[y][0].getString());
		//csv����id�̎擾
		itemStatus_info.SetItemId(m_itemStatus_Csv_Info[y][1].getInt());
		//csv����type�̎擾
		itemStatus_info.SetItemType(m_itemStatus_Csv_Info[y][2].getInt());
		//csv����Damage�̎擾
		itemStatus_info.SetItemDamage(m_itemStatus_Csv_Info[y][3].getInt());
		//csv����defance�̎擾
		itemStatus_info.SetItemDefance(m_itemStatus_Csv_Info[y][5].getInt());
		//csv����price���擾
		itemStatus_info.SetItemPrice(m_itemStatus_Csv_Info[y][6].getInt());
		//csv����WeaponType���擾����
		itemStatus_info.SetWeaponType(m_itemStatus_Csv_Info[y][7].getInt());
		//csv����ItemDetail���擾����
		itemStatus_info.SetItemDetail(m_itemStatus_Csv_Info[y][8].getString());

		m_Item_Status_Type.emplace_back(itemStatus_info);
	}

	//std::sort(m_Item_Status_Type.begin(), m_Item_Status_Type.end(), ItemStatus{}.id);

	//�_���[�W���Ƀ\�[�g
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
		// ID�ɑΉ�����A�C�e����������Ȃ��ꍇ�́A�K���ȃf�t�H���g�l��Ԃ����G���[�������s���Ȃ�
		return ItemBase{};
	}
}

void Item::draw()
{
	
}

