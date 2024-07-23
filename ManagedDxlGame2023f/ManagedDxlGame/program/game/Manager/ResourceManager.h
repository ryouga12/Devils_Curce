//
//�܂Ƃ߂�N���X
//


#pragma once
#include "../../dxlib_ext/dxlib_ext.h"
#include"../Effect/animation.h"
#include<unordered_map>
#include<string>
#include<vector>
#include"../Map/Mapchip.h"

//------------------------------------------
// �V���O���g���݌v

class ResourceManager final {

public:

	static ResourceManager* getResourceManager();
	static void delategetResourceManager() { delete getResourceManager(); }

private:

	ResourceManager();
	~ResourceManager();

public:

	void Update();
	void Draw();

	//����������摜�����[�h���Ȃ��ׂ̊֐�
	int LoadGraphEX(std::string gh);

	//���[�h�����摜����������
	bool deleteGraphEx(std::string ghpath);

	//���[�h�����摜��`�悷��
	void DrawRotaGraphEx(const std::string& ghpath , int x , int y , double Exlate, double Angle, int TransFlag);


//------------------------------------------------------------------------------------------------------------------------	
private:
	//��x�ǂݍ���gh��ۑ�����map
	std::unordered_map<std::string, int> ghmap;

};