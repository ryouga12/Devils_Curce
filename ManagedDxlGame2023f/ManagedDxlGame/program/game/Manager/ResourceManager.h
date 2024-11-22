//------------------------------------------------------------------------------------------------------------
//
//�摜�Ȃǂ��Ǘ�����}�l�[�W���[
//
//-------------------------------------------------------------------------------------------------------------


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

	static ResourceManager* GetResourceManager();
	static void DelategetResourceManager();

private:

	ResourceManager() = default;
	~ResourceManager();

public:

	//����������摜�����[�h���Ȃ��ׂ̊֐�
	int LoadGraphEX(const std::string&  gh);

	//���[�h�����摜����������
	bool DeleteGraphEx(const std::string& ghpath);

	//�z����̉摜���������
	//arg_1 : �n���h���̔z��
	//arg_2 : �n���h���̐�
	void DeleteGraphAryEx(int gh_ary[], const int& max_num);

	//���[�h�����摜��`�悷��
	//arg_1 : �摜��path
	//arg_2 : X���W
	//arg_3 : Y���W
	//arg_4 : �摜�̑傫��
	//arg_5 : �p�x
	//arg_6 : �������s������
	void DrawRotaGraphEx(const std::string& ghpath , const int& x , const int& y , const double& Exlate, const double& Angle, const int& TransFlag);


//------------------------------------------------------------------------------------------------------------------------	
private:
	//��x�ǂݍ���gh��ۑ�����map
	std::unordered_map<std::string, int> ghmap;

};