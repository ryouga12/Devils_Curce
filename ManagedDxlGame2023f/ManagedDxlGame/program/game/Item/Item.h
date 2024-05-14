#pragma once
///
///�A�C�e�����Ǘ�����N���X
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

	//�A�C�e���̔z����擾����
	const std::list<ItemBase>&getItemArray()const {
		return m_Item_Status_Type;
	}

private:

	//�ǂݍ��݊֐�
	void ItemLoadTypeInfo();
	
	//model������ϐ�
	ItemBase item_Status_ary;

	//�\���̂̑���
	int m_item_Status_Total_num;

	//Csv�ǂݎ��p�̔z��
	std::vector<std::vector<tnl::CsvCell>>m_itemStatus_Csv_Info;

	//�\���̂��i�[����z��
	std::list<ItemBase>m_Item_Status_Type;

	//�A�C�e���Ȃǂ̐������Ȃǂ�����z��
	std::string ItemDisc = {};

	//item�̍��W
	tnl::Vector3 item_pos = {};


public:

	//ID���w�肵�ăA�C�e�����擾����֐�
	ItemBase GetItemById(int id) const;

};