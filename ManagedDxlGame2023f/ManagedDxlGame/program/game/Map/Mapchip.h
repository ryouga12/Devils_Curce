///
///MapChip�N���X
///

#pragma once
#include "../../dxlib_ext/dxlib_ext.h"
#include"../Manager/Camera.h"


class MapChip {
public:
	MapChip() {};
	MapChip(const tnl::Vector3& pos , int gfx_hdl);
	void Update(float delta_time);
	void Draw(const KonCamera& camera);

	//�}�b�v�`�b�v�T�C�Y�̎擾
	const float& getChipSize(){
		return CHIP_SIZE;
	}
	//�}�b�v�`�b�v�̍��W�擾
	tnl::Vector3& MapChipPos() {
		return pos_;
	}

private:

	const float CHIP_SIZE = 32;							//�}�b�v�`�b�v�̃T�C�Y
	int chip_gpc_hdl = 0;								//�摜��hdl
	tnl::Vector3 pos_ = { 0, 0, 0 };					//�}�b�v�`�b�v��pos

};