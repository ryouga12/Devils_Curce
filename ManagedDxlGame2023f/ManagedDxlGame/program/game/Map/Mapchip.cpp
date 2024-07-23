#include "Mapchip.h"
#include "../../dxlib_ext/dxlib_ext.h"

MapChip::MapChip(const tnl::Vector3& pos, int gfx_hdl)
{
	chip_gpc_hdl = gfx_hdl;
	pos_ = pos;
}

void MapChip::Update(float delta_time)
{

}

void MapChip::Draw(const KonCamera& camera)
{
	tnl::Vector3 draw_pos = pos_ - camera.getTargetPos() + tnl::Vector3(DXE_WINDOW_WIDTH >> 1, DXE_WINDOW_HEIGHT >> 1, 0);
	DrawRotaGraph(static_cast<int>(draw_pos.x), static_cast<int>(draw_pos.y), 1, 0, chip_gpc_hdl, true);
}
