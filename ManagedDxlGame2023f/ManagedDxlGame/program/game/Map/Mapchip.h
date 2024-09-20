//------------------------------------------------------------------------------------------------------------
//
//MapChip�N���X
//
//-------------------------------------------------------------------------------------------------------------

#pragma once
#include "../../dxlib_ext/dxlib_ext.h"
#include"../System/Camera.h"
#include"../../koni_name_space/common/common_value.h"


class MapChip final{
public:

	MapChip() = default;

	//arg_1 : �}�b�v�`�b�v�̍��W
	//arg_2 : �}�b�v�`�b�v�̉摜
	//�}�b�v�`�b�v�̍��W�Ɖ摜��ݒ肷��
	MapChip(const tnl::Vector3& pos , const int gfx_hdl);
	
	//�`��
	void Draw(const KonCamera& camera);

	//�}�b�v�`�b�v�T�C�Y�̎擾
	tnl::Vector2i GetChipSize()const {
		return CHIP_SIZE;
	}

	//�}�b�v�`�b�v�̍��W�擾
	tnl::Vector3& MapChipPos() {
		return pos_;
	}

private:

	//�}�b�v�`�b�v�̃T�C�Y
	tnl::Vector2i CHIP_SIZE = { 32, 32 };				
	//�摜��hdl
	int chip_gpc_hdl = 0;								
	//�}�b�v�`�b�v��pos
	tnl::Vector3 pos_ = { 0, 0, 0 };					
};