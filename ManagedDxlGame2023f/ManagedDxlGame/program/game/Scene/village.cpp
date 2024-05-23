#include "village.h"
#include"../Manager/SceneManager.h"
#include"battleScene.h"
#include "../../dxlib_ext/dxlib_ext.h"
#include"mapScene.h"
#include<iostream>

//------------------------------------------------------------------------------------------------------------------------
//�������@&  ���

//�R���X�g���N�^
VillageScene::VillageScene(tnl::Vector3 firstPos ): village_Pos(firstPos)
{

	//����������
	InitVillageScene();

	//�}�b�v�`�b�v�̃��[�h
	mapChip_Loding();

	//�f�o�b�N�p
	/*player->getPlayerStatusSave().SetPlayerCurentHp(player->getPlayerStatusSave().getcurentHp() - 3);*/

}

VillageScene::~VillageScene()
{
	//�T�E���h����������
	SoundManager::getSoundManager()->daleteSound("sound/SoundEffect/decision.mp3");
	SoundManager::getSoundManager()->daleteSound("sound/SoundEffect/idou.mp3");
	SoundManager::getSoundManager()->daleteSound("sound/BGM/tasogare.mp3");

	//�}�b�v�`�b�v�̉��(1992 = �}�b�v�`�b�v�̑���)
	for (int i = 0; i < 1992; i++) {
		DeleteGraph(gpc_map_chip_hdls_[i]);
		tnl::DebugTrace("Deleted  MapChipimage at [%d]. Pointer value: %p\n", i, gpc_map_chip_hdls_[i]);
	}
}

//������
void VillageScene::InitVillageScene()
{
	
	//Player�̏����ʒu�����߂�
	GameManager::getGameManager()->getPlayer()->SetPlayerPosition(village_Pos);

	//����֘A�̏�����
	weapon_shop = std::make_shared<WeaponShop>();
	GameManager::getGameManager()->getInventory()->SetPlayer(GameManager::getGameManager()->getPlayer());

	//�C���x���g���̏�����
	weapon_shop->setInventory(GameManager::getGameManager()->getInventory());


	//NPC�̏�����
	herdman_npc = std::make_shared<Npc>("����", tnl::Vector3(1375, 760, 0), herdman_hdl_pass, 1.5f, 0, 4);
	Armsdealer_npc = std::make_shared<Npc>("���폤�l", tnl::Vector3(1150, 140, 0), Armsdealer_hdl_pass, 1.5f, 0, 1);
	Priest_npc = std::make_shared<Npc>("�_��", tnl::Vector3(239, 740, 0), Priest_hdl_pass, 1.5f, 0, 3);

	//���j���[�̏�����
	InitMenuWindow();

	//soundManager�̏�����
	SoundManager::getSoundManager()->sound_Play("sound/BGM/tasogare.mp3", DX_PLAYTYPE_LOOP);

	//�}�b�v�`�b�v�̏�����
	mapchip = std::make_shared<MapChip>();

}


//---���j���[�̏�����---//
void VillageScene::InitMenuWindow()
{

	//�ʏ�̃��j���[�E�B���h�E
	menuWindow = std::make_shared<Menu>(50, 50, 300, 270, "graphics/WindowBase_01.png");

	//���j���[�̑I���E�B���h�E
	menuSelectWindow = std::make_shared<Menu>(50, 50, 250, 200, "graphics/WindowBase_01.png");

	//�R�����g�\���̍ۂ̃��j���[�E�B���h�E
	menuComentWindiow = std::make_shared<Menu>(50, 500, 700, 200, "graphics/WindowBase_01.png");

}

//------------------------------------------------------------------------------------------------------------------------
//�X�V����

void VillageScene::Update(float delta_time)
{
	//Player
	GameManager::getGameManager()->getPlayer()->player_Move(delta_time, PlayerVelocity);
	 
	//�V�[�P���X
	sequence_.update(delta_time);

	//camera
	GameManager::getGameManager()->getCamera()->update(GameManager::getGameManager()->getPlayer()->getPlayerPos(), MAP_WIDTH, MAP_HEIGHT);

	//npc
	herdman_npc->Update(delta_time);
	Armsdealer_npc->Update(delta_time);
	Priest_npc->Update(delta_time);


	//Inventory
	/*GameManager::getGameManager()->UpdateCommon(delta_time);*/
	GameManager::getGameManager()->getInventory()->Update(delta_time);
		
	//�����蔻��
	mapChip_collision();

}

//------------------------------------------------------------------------------------------------------------------------
//�`��

void VillageScene::Draw()
{

	//---mapChip�̕`��---//
	for (auto mapchip : MapChips) {
		mapchip->Draw(*GameManager::getGameManager()->getCamera());
	}

	//---plyer�̕`��---//
	GameManager::getGameManager()->getPlayer()->player_draw(*GameManager::getGameManager()->getCamera(), PlyerScale);

	//---Npc�̕`��---//
	herdman_npc->Draw(*GameManager::getGameManager()->getCamera());
	Armsdealer_npc->Draw(*GameManager::getGameManager()->getCamera());
	Priest_npc->Draw(*GameManager::getGameManager()->getCamera());

	//���ꂼ��̃V�[�P���X�ł̕`��
	switchSequenceDraw();

	//�C���x���g���̕`��
	GameManager::getGameManager()->getInventory()->draw();

	////�J�����^�[�Q�b�g�̍��W
	//DrawStringEx(100, 100, -1, "%f", GameManager::getGameManager()->getCamera()->getTargetPos().x);
	//DrawStringEx(100, 150, -1, "%f", GameManager::getGameManager()->getCamera()->getTargetPos().y);
} 

//------------------------------------------------------------------------------------------------------------------------
//�V�[�P���X�֘A

bool VillageScene::seqIdle(float delta_time)
{
	if (sequence_.isStart()) {

		GameManager::getGameManager()->getPlayer()->setPlayerControl(PlyControlTrue);
		select_num = Sequence_num::Idle;
	}

	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_ESCAPE)) {
		//���艹��炷
		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
		//�C���x���g����\������
		GameManager::getGameManager()->getInventory()->SetSelect_num(first_menu);
	}

	//�v���C���[�̍��W���}�b�v�O�ɏo����V�[����؂�ւ���
	if (GameManager::getGameManager()->getPlayer()->getPlayerPos().x >= MAP_WIDTH || GameManager::getGameManager()->getPlayer()->getPlayerPos().x < 0 || GameManager::getGameManager()->getPlayer()->getPlayerPos().y >= MAP_HEIGHT /*tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)*/)
	{
		TimeCount = true;
		//���y��Stop����
		SoundManager::getSoundManager()->StopSound("sound/BGM/tasogare.mp3");
		//�ړ���SE�𗬂�
		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/idou.mp3", DX_PLAYTYPE_BACK);
	}
	//�؂�ւ���ۂ�SE�𗬂��؂��Ă���J�ڂ���
	if (TimeCount) {
		//0.2�bSE�𗬂��Ă���J�ڂ�����
		if (GameManager::getGameManager()->TimeCount(delta_time, TimeSE)) {
			GameManager::getGameManager()->getPlayer()->SetPlayerPosition(first_world_pos);
			sequence_.change(&VillageScene::seqChangeScene);
			auto mgr = SceneManager::GetInstance();
			mgr->changeScene(new MapScene(), 1.0f, 3.0f);
		}
	}

	return true;
}

//���퉮
bool VillageScene::seqWeaponShop(float delta_time)
{
	if (sequence_.isStart()) {
		select_num = Sequence_num::WeaponShop;
		weapon_shop->buyComentClear();
	}

	//���퉮�̏���
	weapon_shop->BuyWeapon(GameManager::getGameManager()->getPlayer());
	weapon_shop->WeaponShopCursorMove();
	
	//0����������V�[�P���X��ς���
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_0)) {
		//�v���C���[�𓮂���悤�ɂ���
		GameManager::getGameManager()->getPlayer()->setPlayerControl(PlyControlTrue);
		//�V�[����؂�ւ���
		sequence_.change(&VillageScene::seqIdle);
	}
	return true;
}

//�V�[����ς��鎞�̃t�F�[�h�A�E�g�������
bool VillageScene::seqChangeScene(float delta_time)
{
	return true;
}

//���j���[
bool VillageScene::seqMenuOpen(float delta_time)
{
	if (sequence_.isStart())
	{
		//�v���C���[�𓮂��Ȃ�����
		GameManager::getGameManager()->getPlayer()->setPlayerControl(PlyControlFalse);
	}

	if (EventManager::getEventManager()->isNpcMenuOpen()) {
		if(tnl::Input::IsKeyDownTrigger(eKeys::KB_1)){
			sequence_.change(&VillageScene::seqNpc);
		}
	}

	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_0)) {
		sequence_.change(&VillageScene::seqIdle);
		//�ŏ��̑Ԑ��ƈ���Ă�����߂�
		if (herdman_npc->getAnimationSelect() != 0) {
			herdman_npc->NpcAnimationReset();
			herdman_npc->ChengeAnimation();
		}
		//�v���C���[�𓮂���悤�ɂ���
		GameManager::getGameManager()->getPlayer()->setPlayerControl(PlyControlTrue);
	}

	return false;
}

//Npc�̏���
bool VillageScene::seqNpc(float delta_time)
{
	if (sequence_.isStart()) {
		select_num = Sequence_num::MenuNpc;
	}

	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_0)) {
		sequence_.change(&VillageScene::seqIdle);
		//�v���C���[�𓮂���悤�ɂ���
		GameManager::getGameManager()->getPlayer()->setPlayerControl(PlyControlTrue);
		//�R�����g��\���ł���悤�ɂ���
		NpcComntTrue = true;
	}

	return false;
}

//�V�[�P���X���̕`��
void VillageScene::switchSequenceDraw()
{
	switch (select_num)
	{
		//�ʏ�V�[�P���X
	case VillageScene::Sequence_num::Idle:
		
		//DrawStringEx(menuWindow->menu_x + 50, menuWindow->menu_y + 350, -1, "seqIdle");

		break;

		//���퉮�̃V�[�P���X�̎�
	case VillageScene::Sequence_num::WeaponShop:

		menuComentWindiow->Menu_draw();

		//�R�����g�ƃA�C�R��
		ResourceManager::getResourceManager()->DrawRotaGraphEx("graphics/Npc/Armsdealer_icon.png", 90, 600, 1.5f, 0, true);

		//���퉮�̃R�����g��\������
	    EventManager::getEventManager()->armsdealerComentDraw(armsdealer_comet);

		//���퉮�̕`��
		weapon_shop->Draw();

		//�v���C���[�̏�������`��
		DrawStringEx(135, 100, -1, "������: %d", GameManager::getGameManager()->getPlayer()->getPlayerMoney());

		/*DrawStringEx(menuWindow->menu_x + 350, menuWindow->menu_y + 350, -1, "seqWeaponShop");*/

		break;

		//���j���[�V�[�P���X�̎�
	case VillageScene::Sequence_num::MenuOpen:

		//Npc�̃R�����g�\��
		EventManager::getEventManager()->NpcComentDraw();

		break;

		//���퉮�V�[�P���X�̎�
	case VillageScene::Sequence_num::MenuShopOpen:

		//���퉮�̃R�����g�\��
		EventManager::getEventManager()->NpcComentDraw();

		break;

		//Npc�V�[�P���X�̎�(Npc�ɂ���ē����ς���)
	case VillageScene::Sequence_num::MenuNpc:

		EventManager::getEventManager()->NpcSwitchComent();

		break;


	default:
		break;
	}
}

//------------------------------------------------------------------------------------------------------------------------
//�}�b�v�`�b�v�֘A

//�}�b�v�`�b�v�̃��[�h
void VillageScene::mapChip_Loding()
{
	// �}�b�v�`�b�v�̉摜�̃��[�h
	gpc_map_chip_hdls_pass_ = "graphics/BaseChip_pipo.png";

	map_chip_width_ = 32;
	map_chip_height_ = 32;
	map_chip_x_size_ = 8;
	map_chip_y_size_ = 249;
	map_chip_all_size_ = map_chip_x_size_ * map_chip_y_size_;

	LoadDivGraph(gpc_map_chip_hdls_pass_.c_str(),
		map_chip_all_size_,
		map_chip_x_size_,
		map_chip_y_size_,
		map_chip_width_,
		map_chip_height_,
		gpc_map_chip_hdls_);

	// �}�b�v�f�[�^�̃��[�h
	map_data_csv_pass_ = "csv/map_chip_first_village.csv";
	map_csv = tnl::LoadCsv<int>(map_data_csv_pass_);

	//�}�b�v�`�b�v�ǂݍ���
	for (int i = 0; i < map_csv.size(); ++i) {
		for (int k = 0; k < map_csv[i].size(); ++k) {
			if (-1 == map_csv[i][k]) continue;
			tnl::Vector3 pos;
			int gpc_hdl;
			pos.x = { mapchip->getChipSize() / 2 + k * mapchip->getChipSize()};
			pos.y = { i * mapchip->getChipSize()};
			gpc_hdl = gpc_map_chip_hdls_[map_csv[i][k]];
			/*DrawGraph(pos.x, pos.y, gpc_hdl, true);*/
			MapChips.emplace_back(std::make_shared<MapChip>(pos, gpc_hdl));

		}
	}
}

//�����蔻��̏���
void VillageScene::mapChip_collision()
{
	 //�u���b�N�ƏՓ˔��� & �␳
	 //�Q�[�����[�v���ł̓����蔻��̃`�F�b�N]
	for (auto mapChip : MapChips) {
		int mapChipValue = map_csv[mapChip->MapChipPos().y / mapChip->getChipSize()][mapChip->MapChipPos().x / mapChip->getChipSize()];
		if(colisionValues.count(mapChipValue) > 0)
		{
			int collision = ResourceManager::getResourceManager()->IsIntersectRectToCorrectPosition(GameManager::getGameManager()->getPlayer()->getPlayerPos(), GameManager::getGameManager()->getPlayer()->getPlayerPrevPos(), GameManager::getGameManager()->getPlayer()->getPlayerSize(PlyerWidth), GameManager::getGameManager()->getPlayer()->getPlayerSize(PlyerHeight), mapChip->MapChipPos(), map_chip_width_, map_chip_height_);
			//�����蔻�肪��p�����ۂɂ��̔ԍ���510�Ԃ������ꍇ�Awindow��\������
			if (collision) {
					//���퉮�Ƙb�����ꍇ
					if (mapChipValue == map_Chip_WeaponShop) {
						if (select_num == Sequence_num::Idle && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
							//�V�[�P���X��؂�ւ���
							EventManager::getEventManager()->SetMenuNpc(2);
							EventManager::getEventManager()->SetNpcComent(0);
							select_num = Sequence_num::MenuShopOpen;
							sequence_.change(&VillageScene::seqMenuOpen);
							
						}
						if (select_num == Sequence_num::MenuShopOpen && tnl::Input::IsKeyDownTrigger(eKeys::KB_1)) {
							//�V�[�P���X��؂�ւ���
							sequence_.change(&VillageScene::seqWeaponShop);
						}
					}

					//�����Ƙb������
					if (mapChipValue == map_Chip_Herdman) {
						if (NpcComntTrue) {
							if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
								select_num = Sequence_num::MenuOpen;
								EventManager::getEventManager()->SetMenuNpc(0);
								EventManager::getEventManager()->SetNpcComent(1);
								sequence_.change(&VillageScene::seqMenuOpen);
								NpcComntTrue = false;
							}
						}
					}
					//�I�u�W�F�N�g�ɐG�ꂽ�Ƃ�
					if (mapChipValue == map_Chip_Object) {
						if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
							select_num = Sequence_num::MenuOpen;
							EventManager::getEventManager()->SetMenuNpc(3);
							sequence_.change(&VillageScene::seqMenuOpen);
					}
				}
			}
		}
	}

	//---Npc�̓����蔻��---//

	//����
	int h_colition = ResourceManager::getResourceManager()->IsIntersectRectToCorrectPosition(
					GameManager::getGameManager()->getPlayer()->getPlayerPos(),
					GameManager::getGameManager()->getPlayer()->getPlayerPrevPos(),
					GameManager::getGameManager()->getPlayer()->getPlayerSize(PlyerWidth),
					GameManager::getGameManager()->getPlayer()->getPlayerSize(PlyerHeight),
					herdman_npc->getNpcPos(),
					herdman_npc->getNpcSize(npcWidth),
					herdman_npc->getNpcSize(npcHeight)
				);


	if (h_colition) {

		//�����瓖��������
		bool playerOnBottom = GameManager::getGameManager()->getPlayer()->getPlayerPos().y >= herdman_npc->getNpcPos().y + herdman_npc->getNpcSize(npcHeight);

		if (playerOnBottom) {
			if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
				//�A�j���[�V���������ɂ���
				herdman_npc->SetNpcAnimation(1);
				herdman_npc->ChengeAnimation();
				select_num = Sequence_num::MenuOpen;
				EventManager::getEventManager()->SetMenuNpc(0);
				EventManager::getEventManager()->SetNpcComent(1);
				sequence_.change(&VillageScene::seqMenuOpen);
			}
		}
	}

	//�_��
	int P_collision = ResourceManager::getResourceManager()->IsIntersectRectToCorrectPosition
	(
		GameManager::getGameManager()->getPlayer()->getPlayerPos(),
		GameManager::getGameManager()->getPlayer()->getPlayerPrevPos(),
		GameManager::getGameManager()->getPlayer()->getPlayerSize(PlyerWidth),
		GameManager::getGameManager()->getPlayer()->getPlayerSize(PlyerHeight),
		Priest_npc->getNpcPos(),
		Priest_npc->getNpcSize(npcWidth),
		Priest_npc->getNpcSize(npcHeight)
	);


	if (P_collision) {
		//�����瓖��������
		bool playerOnBottom = GameManager::getGameManager()->getPlayer()->getPlayerPos().y >= Priest_npc->getNpcPos().y + Priest_npc->getNpcSize(npcHeight);
		
		if (playerOnBottom) {
			if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
				Priest_npc->NpcAnimationReset();
				Priest_npc->ChengeAnimation();
				select_num = Sequence_num::MenuOpen;
				EventManager::getEventManager()->SetMenuNpc(1);
				EventManager::getEventManager()->SetNpcComent(2);
				sequence_.change(&VillageScene::seqMenuOpen);
			}
		}
	}

}
