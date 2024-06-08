#include "InMapScene.h"
#include"../Manager/SceneManager.h"
#include"battleScene.h"
#include "../../dxlib_ext/dxlib_ext.h"
#include"mapScene.h"
#include<iostream>
#include"../Manager/UiManager.h"
#include"../Manager/CsvManager.h"

//------------------------------------------------------------------------------------------------------------------------
//�������@&  ���

//�R���X�g���N�^
//���� : ���ɓ��������̍ŏ��̍��W , ���݂̃V�[��
InMapScene::InMapScene(tnl::Vector3 firstPos, InMapState inmap_state): first_pos(firstPos)
{
	//������
	InitInMapScene(inmap_state);
}

InMapScene::~InMapScene()
{
	//�T�E���h���~�߂�
	SoundManager::getSoundManager()->StopSound("sound/SoundEffect/decision.mp3");
	SoundManager::getSoundManager()->StopSound("sound/SoundEffect/idou.mp3");
	SoundManager::getSoundManager()->StopSound("sound/BGM/tasogare.mp3");

	//�}�b�v�`�b�v�̉��(1992 = �}�b�v�`�b�v�̑���)
	for (int i = 0; i < 1992; i++) {
		DeleteGraph(gpc_map_chip_hdls_[i]);
		tnl::DebugTrace("Deleted  MapChipimage at [%d]. Pointer value: %p\n", i, gpc_map_chip_hdls_[i]);
	}
}

//������
void InMapScene::InitVillageScene()
{
	
	//����֘A�̏�����
	weapon_shop = std::make_shared<WeaponShop>();

	//NPC�̏�����
	herdman_npc = std::make_shared<Npc>("����", tnl::Vector3(1375, 760, 0),48 , herdman_hdl_pass, 1.5f, 0, 4);
	armsdealer_npc = std::make_shared<Npc>("���폤�l", tnl::Vector3(1150, 140, 0), 48 , Armsdealer_hdl_pass, 1.5f, 0, 1);
	priest_npc = std::make_shared<Npc>("�_��", tnl::Vector3(239, 740, 0),48 , Priest_hdl_pass, 1.5f, 0, 3);

	//�A�N�^�[���X�g��NPC��ǉ�����
	actor_list.emplace_back(herdman_npc);
	actor_list.emplace_back(armsdealer_npc);
	actor_list.emplace_back(priest_npc);

	//soundManager�̏�����
	SoundManager::getSoundManager()->sound_Play("sound/BGM/tasogare.mp3", DX_PLAYTYPE_LOOP);

	//�}�b�v�`�b�v�̃��[�h
	//���p�̃}�b�v�`�b�v�����[�h����
	mapChip_Loding(CsvManager::getCsvManager()->GetVillagMapCsv());

}

//�_���W�����V�[��(�����̏�)�̏�����
void InMapScene::InitDungeonScene()
{
	//�{�X
	boss_npc = std::make_shared<Npc>("Zerahkiel", tnl::Vector3(801, 153, 0), 128 , boss_hdl_pass, 1, 0, 1);
	//�A�N�^�[���X�g��NPC��ǉ�����
	actor_list.emplace_back(boss_npc);

	//�}�b�v�`�b�v�̃��[�h
	//�_���W�����p�̃}�b�v�`�b�v�����[�h����
	mapChip_Loding(CsvManager::getCsvManager()->GetBossCastleFloorCsv());

	//�����̏���ł̉��y
	SoundManager::getSoundManager()->sound_Play("sound/BGM/maou_bgm_castle.mp3",DX_PLAYTYPE_LOOP);
}


void InMapScene::InitInMapScene(InMapState inmap_state)
{
	//Player�̏����ʒu�����߂�
	GameManager::getGameManager()->getPlayer()->SetPlayerPosition(first_pos);

	//���j���[�̏�����
	InitMenuWindow();

	//�}�b�v�`�b�v�̏�����
	mapchip = std::make_shared<MapChip>();

	switch (inmap_state)
	{
	//��
	case InMapScene::InMapState::VILLAGE:

		//���݂̃V�[���𑺂ɐݒ�
		curent_inmap_scene = inmap_state;

		//����������
		InitVillageScene();

		//�}�b�v�̍����ƃ}�b�v�̕������߂�
		map_height = 945;																					
		map_width = 1600;																					

		break;

	//��
	case InMapScene::InMapState::TOWN:

		//���݂̃V�[���𒬂ɐݒ�
		curent_inmap_scene = inmap_state;

		break;

	//��
	case InMapScene::InMapState::CASTLE:

		//���݂̃V�[������ɐݒ�
		curent_inmap_scene = inmap_state;

		break;

	//�_���W����
	case InMapScene::InMapState::BOSSCASTLE:

		//���݂̃V�[�����_���W�����ɐݒ�
		curent_inmap_scene = inmap_state;

		//�}�b�v�̍����ƃ}�b�v�̕������߂�
		map_height = 945;
		map_width = 1600;

		//�_���W�����V�[���̏�����
		InitDungeonScene();

		break;


	default:
		break;
	}
}

//---���j���[�̏�����---//
void InMapScene::InitMenuWindow()
{

	//�ʏ�̃��j���[�E�B���h�E
	menuWindow = UIManager::getUIManager()->getMenu("menu_window");

}

//------------------------------------------------------------------------------------------------------------------------
//�X�V����

void InMapScene::Update(float delta_time)
{
	InMapSceneUpdate(delta_time);
}

//���ꂼ��̍X�V����
void InMapScene::InMapSceneUpdate(float delta_time)
{
	//Player
	GameManager::getGameManager()->getPlayer()->player_Move(delta_time, PlayerVelocity);

	//�V�[�P���X
	sequence_.update(delta_time);

	//camera
	GameManager::getGameManager()->getCamera()->update(GameManager::getGameManager()->getPlayer()->getPlayerPos(), map_width, map_height);

	//Inventory
	GameManager::getGameManager()->getInventory()->Update(delta_time);


	switch (curent_inmap_scene)
	{
	//���V�[��
	case InMapScene::InMapState::VILLAGE:

		//npc
		for (auto& actor : actor_list) {
			actor->Update(delta_time);
		}


		break;

	//���V�[��
	case InMapScene::InMapState::TOWN:


		break;

	//��V�[��
	case InMapScene::InMapState::CASTLE:


		break;

	//�{�X�̏�V�[��
	case InMapScene::InMapState::BOSSCASTLE:

		//npc
		for (auto& actor : actor_list) {
			actor->Update(delta_time);
		}

		break;

	default:
		break;
	}
}

//------------------------------------------------------------------------------------------------------------------------
//�`��

void InMapScene::Draw()
{
	InMapSceneDraw();
}

//���ꂼ��̕`�揈��
void InMapScene::InMapSceneDraw()
{
	switch (curent_inmap_scene)
	{
	case InMapScene::InMapState::VILLAGE:

		//---mapChip�̕`��---//
		for (auto& mapchip : MapChips) {
			mapchip->Draw(*GameManager::getGameManager()->getCamera());
		}

		//---Npc�̕`��---//
		for (auto& actor : actor_list) {
			actor->Draw(*GameManager::getGameManager()->getCamera());
		}

		break;

	//���V�[��
	case InMapScene::InMapState::TOWN:


		break;

	//��V�[��
	case InMapScene::InMapState::CASTLE:


		break;

	//�{�X�̏�
	case InMapScene::InMapState::BOSSCASTLE:

		//---mapChip�̕`��---//
		for (auto& mapchip : MapChips) {
			mapchip->Draw(*GameManager::getGameManager()->getCamera());
		}

		for (auto& mapchip : MapChip_object) {
			mapchip->Draw(*GameManager::getGameManager()->getCamera());
		}

		//---Npc�̕`��---//
		for (auto& actor : actor_list) {
			actor->Draw(*GameManager::getGameManager()->getCamera());
		}

		DrawStringEx(100, 100, -1, "%f", GameManager::getGameManager()->getPlayer()->getPlayerPos().x);
		DrawStringEx(100, 150, -1, "%f", GameManager::getGameManager()->getPlayer()->getPlayerPos().y);


		break;


	default:


		break;
	}

	//---plyer�̕`��---//
	GameManager::getGameManager()->getPlayer()->player_draw(*GameManager::getGameManager()->getCamera(), PlyerScale);

	//�C���x���g���̕`��
	GameManager::getGameManager()->getInventory()->draw();

	//���ꂼ��̃V�[�P���X�ł̕`��
	switchSequenceDraw();
}

//------------------------------------------------------------------------------------------------------------------------
//�V�[�P���X�֘A

bool InMapScene::seqIdle(float delta_time)
{
	if (sequence_.isStart()) {

		if (!GameManager::getGameManager()->getPlayer()->getPlayerControl()) {
			GameManager::getGameManager()->getPlayer()->setPlayerControl();
		}
		select_num = Sequence_num::Idle;
	}

	if (GameManager::getGameManager()->getInventory()->GetSelectMenuNum() == Inventory::MenuWindow_I::EMPTY && tnl::Input::IsKeyDownTrigger(eKeys::KB_ESCAPE)) {

		//���艹��炷
		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
		//�C���x���g����\������
		GameManager::getGameManager()->getInventory()->InventoryMenuChange(Inventory::MenuWindow_I::FIRSTMENU);
		//plyer�𓮂��Ȃ��悤�ɂ���
		GameManager::getGameManager()->getPlayer()->setPlayerControl();
	}

	//�v���C���[�̍��W���}�b�v�O�ɏo����V�[����؂�ւ���
	if (GameManager::getGameManager()->getPlayer()->getPlayerPos().x >= map_width || GameManager::getGameManager()->getPlayer()->getPlayerPos().x < 0 || GameManager::getGameManager()->getPlayer()->getPlayerPos().y >= map_height /*tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)*/)
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
			sequence_.change(&InMapScene::seqChangeScene);
			auto mgr = SceneManager::GetInstance();
			mgr->changeScene(new MapScene(),0.3);
		}
	}

	//�����蔻��
	mapChip_collision(CsvManager::getCsvManager()->GetVillagMapCsv());

	return true;
}

//���퉮
bool InMapScene::seqWeaponShop(float delta_time)
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
		//�V�[����؂�ւ���
		sequence_.change(&InMapScene::seqIdle);
	}
	return true;
}

//�V�[����ς��鎞�̃t�F�[�h�A�E�g������׃V�[�P���X
bool InMapScene::seqChangeScene(float delta_time)
{
	if (curent_inmap_scene == InMapState::VILLAGE) {
		GameManager::getGameManager()->getPlayer()->SetPlayerPosition(FIRST_WORLD_POS);
	}
	else if (curent_inmap_scene == InMapState::BOSSCASTLE) {
		GameManager::getGameManager()->getPlayer()->SetPlayerPosition(BOSS_EXIT_POS);
	}
	GameManager::getGameManager()->getCamera()->SavePosition(GameManager::getGameManager()->getPlayer()->getPlayerPos());

	return true;
}

//���j���[
bool InMapScene::seqMenuOpen(float delta_time)
{
	if (sequence_.isStart())
	{
		//�v���C���[�𓮂��Ȃ�����
		GameManager::getGameManager()->getPlayer()->setPlayerControl();
		select_num = Sequence_num::MenuOpen;
	}

	if (EventManager::getEventManager()->GetSelectMenu() != EventManager::MenuOpen::EMPTY && EventManager::getEventManager()->GetNpcMove() == EventManager::NpcMove::WEAPONSHOP) {
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_1)) {
			sequence_.change(&InMapScene::seqWeaponShop);
		}
	}
	else if (EventManager::getEventManager()->GetSelectMenu() == EventManager::MenuOpen::EMPTY || EventManager::getEventManager()->GetSelectMenu() == EventManager::MenuOpen::BOSS) {
		if(tnl::Input::IsKeyDownTrigger(eKeys::KB_1)){
			sequence_.change(&InMapScene::seqNpc);
		}
	}
	

	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_0)) {

		//�V�[�P���X��؂�ւ���
		sequence_.change(&InMapScene::seqIdle);

		//���V�[���̂�
		if (curent_inmap_scene == InMapState::VILLAGE) {
			//�ŏ��̑Ԑ��ƈ���Ă�����߂�
			if (herdman_npc->GetAnimationSelect() != 0) {
				herdman_npc->NpcAnimationReset();
				herdman_npc->ChengeAnimation();
			}
		}
	}

	return false;
}

//Npc�̏���
bool InMapScene::seqNpc(float delta_time)
{
	if (sequence_.isStart()) {
		select_num = Sequence_num::MenuNpc;
	}

	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_0)) {
		sequence_.change(&InMapScene::seqIdle);
		////�R�����g��\���ł���悤�ɂ���
		//NpcComntTrue = true;
	}

	return false;
}

//�V�[�P���X���̕`��
void InMapScene::switchSequenceDraw()
{
	switch (select_num)
	{
		//�ʏ�V�[�P���X
	case InMapScene::Sequence_num::Idle:
		
		//DrawStringEx(menuWindow->menu_x + 50, menuWindow->menu_y + 350, -1, "seqIdle");

		break;

		//���퉮�̃V�[�P���X�̎�
	case InMapScene::Sequence_num::WeaponShop:

		if (auto menu_coment_window = menuWindow.lock()) {
			menu_coment_window->Menu_draw(50, 500, 700, 200);
		}

		//�R�����g�ƃA�C�R��
		ResourceManager::getResourceManager()->DrawRotaGraphEx("graphics/Npc/Armsdealer_icon.png", 90, 600, 1.5f, 0, true);

		//���퉮�̃R�����g��\������
	    UIManager::getUIManager()->armsdealerComentDraw(armsdealer_comet);

		//���퉮�̕`��
		weapon_shop->Draw();

		//�v���C���[�̏�������`��
		DrawStringEx(135, 100, -1, "������: %d", GameManager::getGameManager()->getPlayer()->getPlayerMoney());

		/*DrawStringEx(menuWindow->menu_x + 350, menuWindow->menu_y + 350, -1, "seqWeaponShop");*/

		break;

		//���j���[�V�[�P���X�̎�
	case InMapScene::Sequence_num::MenuOpen:

		//Npc�̃R�����g�\��
		EventManager::getEventManager()->EventMenuWindow();

		break;

		//Npc�V�[�P���X�̎�(Npc�ɂ���ē����ς���)
	case InMapScene::Sequence_num::MenuNpc:

		EventManager::getEventManager()->NpcSwitchComent();

		/*if (EventManager::getEventManager()->GetNpcMove() == EventManager::NpcMove::BOSS) {
			sequence_.change(&InMapScene::seqChangeScene);
		}*/

		break;


	default:
		break;
	}
}

//------------------------------------------------------------------------------------------------------------------------
//�}�b�v�`�b�v�֘A

//�}�b�v�`�b�v�̃��[�h
void InMapScene::mapChip_Loding(std::vector<std::vector<int>>map_csv)
{
	// �}�b�v�`�b�v�̉摜�̃��[�h
	gpc_map_chip_hdls_pass_ = "graphics/image_(2024_0115_0231).png";

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

	//�}�b�v�`�b�v�ǂݍ���
	for (int i = 0; i < map_csv.size(); ++i) {
		for (int k = 0; k < map_csv[i].size(); ++k) {
			if (-1 == map_csv[i][k]) continue;
			tnl::Vector3 pos;
			int gpc_hdl;
			pos.x = { mapchip->GetChipSize() / 2 + k * mapchip->GetChipSize()};
			pos.y = { i * mapchip->GetChipSize()};
			gpc_hdl = gpc_map_chip_hdls_[map_csv[i][k]];
			/*DrawGraph(pos.x, pos.y, gpc_hdl, true);*/
			MapChips.emplace_back(std::make_shared<MapChip>(pos, gpc_hdl));

		}
	}

	//�V�[�����_���W������������
	if (curent_inmap_scene == InMapState::BOSSCASTLE) {

		//�I�u�W�F�N�g
		auto world_object_csv = CsvManager::getCsvManager()->GetBossCastleObjectCsv();

		//�I�u�W�F�N�g�̃}�b�v�`�b�v�̓ǂݍ���
		for (int i = 0; i < world_object_csv.size(); ++i) {
			for (int k = 0; k < world_object_csv[i].size(); ++k) {
				if (-1 == world_object_csv[i][k])continue;
				tnl::Vector3 pos;
				int gpc_hdl;
				pos.x = { mapchip->GetChipSize() / 2 + k * mapchip->GetChipSize() };
				pos.y = { i * mapchip->GetChipSize() };
				gpc_hdl = gpc_map_chip_hdls_[world_object_csv[i][k]];
				MapChip_object.emplace_back(std::make_shared<MapChip>(pos, gpc_hdl));
			}
		}
	}
}

//�����蔻��̏���
void InMapScene::mapChip_collision(std::vector<std::vector<int>>map_csv)
{
	//���݂̃V�[��������������
	if (curent_inmap_scene == InMapState::VILLAGE) {
		//���̓����蔻��
		VillageCollision(map_csv);
	}
	else if (curent_inmap_scene == InMapState::BOSSCASTLE) {
		//�_���W�����V�[���̓����蔻��
		DungeonCollison();
	}
}

//���V�[���ł̓����蔻��
void InMapScene::VillageCollision(std::vector<std::vector<int>>map_csv)
{
	//�u���b�N�ƏՓ˔��� & �␳
	//�Q�[�����[�v���ł̓����蔻��̃`�F�b�N]
	for (auto& mapChip : MapChips) {

		int mapChipValue = map_csv[static_cast<unsigned>(mapChip->MapChipPos().y / mapChip->GetChipSize())][static_cast<unsigned>(mapChip->MapChipPos().x / mapChip->GetChipSize())];
		if (village_colision_values.count(mapChipValue) > 0)
		{
			int collision = ResourceManager::getResourceManager()->IsIntersectRectToCorrectPosition(GameManager::getGameManager()->getPlayer()->getPlayerPos(), GameManager::getGameManager()->getPlayer()->getPlayerPrevPos(), GameManager::getGameManager()->getPlayer()->GetPlayerWidth(), GameManager::getGameManager()->getPlayer()->GetPlayerHight(), mapChip->MapChipPos(), map_chip_width_, map_chip_height_);
			//�����蔻�肪��p�����ۂɂ��̔ԍ���510�Ԃ������ꍇ�Awindow��\������
			if (collision) {
				//���퉮�Ƙb�����ꍇ
				if (mapChipValue == map_Chip_WeaponShop) {
					if (select_num == Sequence_num::Idle && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
						//�V�[�P���X��؂�ւ���
						EventManager::getEventManager()->SetMenuNpc(EventManager::MenuOpen::WEAPONSHOP);
						EventManager::getEventManager()->SetNpcComent(EventManager::NpcMove::WEAPONSHOP);
						sequence_.change(&InMapScene::seqMenuOpen);

					}
				}

				//�����Ƙb������
				else if (mapChipValue == map_Chip_Herdman) {
					if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

						//��Ԃ�؂�ւ���
						EventManager::getEventManager()->SetMenuNpc(EventManager::MenuOpen::EMPTY);
						EventManager::getEventManager()->SetNpcComent(EventManager::NpcMove::HERDMAN);

						//1����R�����g�𗬂�
						UIManager::getUIManager()->ComentClear();

						//�R�����g�p�̔z�������������
						UIManager::getUIManager()->ComentLoad(9, herdman_npc->GetNpcName());

						sequence_.change(&InMapScene::seqMenuOpen);
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
		GameManager::getGameManager()->getPlayer()->GetPlayerWidth(),
		GameManager::getGameManager()->getPlayer()->GetPlayerHight(),
		herdman_npc->GetNpcPos(),
		herdman_npc->GetNpcSizeWidth(),
		herdman_npc->GetNpcSizeHegiht()
	);


	if (h_colition) {

		//�����瓖��������
		bool playerOnBottom = GameManager::getGameManager()->getPlayer()->getPlayerPos().y >= herdman_npc->GetNpcPos().y + herdman_npc->GetNpcSizeHegiht();

		if (playerOnBottom) {
			if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
				//�A�j���[�V���������ɂ���
				herdman_npc->SetNpcAnimation(1);
				herdman_npc->ChengeAnimation();

				//1����R�����g�𗬂�
				UIManager::getUIManager()->ComentClear();

				//�R�����g�p�̔z�������������
				UIManager::getUIManager()->ComentLoad(9, herdman_npc->GetNpcName());

				//��Ԃ�؂�ւ���
				EventManager::getEventManager()->SetMenuNpc(EventManager::MenuOpen::EMPTY);
				EventManager::getEventManager()->SetNpcComent(EventManager::NpcMove::HERDMAN);

				//�V�[�P���X��؂�ւ���
				sequence_.change(&InMapScene::seqMenuOpen);
			}
		}
	}

	//�_��
	int P_collision = ResourceManager::getResourceManager()->IsIntersectRectToCorrectPosition
	(
		GameManager::getGameManager()->getPlayer()->getPlayerPos(),
		GameManager::getGameManager()->getPlayer()->getPlayerPrevPos(),
		GameManager::getGameManager()->getPlayer()->GetPlayerWidth(),
		GameManager::getGameManager()->getPlayer()->GetPlayerHight(),
		priest_npc->GetNpcPos(),
		priest_npc->GetNpcSizeWidth(),
		priest_npc->GetNpcSizeHegiht()
	);


	if (P_collision) {
		//�����瓖��������
		bool playerOnBottom = GameManager::getGameManager()->getPlayer()->getPlayerPos().y >= priest_npc->GetNpcPos().y + priest_npc->GetNpcSizeHegiht();

		if (playerOnBottom) {
			if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
				priest_npc->NpcAnimationReset();
				priest_npc->ChengeAnimation();
				select_num = Sequence_num::MenuOpen;
				EventManager::getEventManager()->SetMenuNpc(EventManager::MenuOpen::EMPTY);
				EventManager::getEventManager()->SetNpcComent(EventManager::NpcMove::PRIEST);
				sequence_.change(&InMapScene::seqMenuOpen);
			}
		}
	}
}

//�_���W�����̃V�[���ł̓����蔻��
void InMapScene::DungeonCollison()
{
	//---Npc�̓����蔻��---//

	//�{�X
	int boss_colition = ResourceManager::getResourceManager()->IsIntersectRectToCorrectPosition(
		GameManager::getGameManager()->getPlayer()->getPlayerPos(),
		GameManager::getGameManager()->getPlayer()->getPlayerPrevPos(),
		GameManager::getGameManager()->getPlayer()->GetPlayerWidth(),
		GameManager::getGameManager()->getPlayer()->GetPlayerHight(),
		boss_npc->GetNpcPos(),
		boss_npc->GetNpcSizeWidth(),
		boss_npc->GetNpcSizeHegiht()
	);

	//�������Ă���Ƃ���Enter�L�[����������o�g���V�[���ɑJ�ڂ�����
	//����Z���t���ǉ��\��
	if (boss_colition && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

		const int BOSS_COMENT_MAX = 6;

		//�C�x���g�}�l�[�W���[�̏�Ԃ��X�V����
		EventManager::getEventManager()->SetMenuNpc(EventManager::MenuOpen::BOSS);
		EventManager::getEventManager()->SetNpcComent(EventManager::NpcMove::BOSS);

		//1����R�����g�𗬂�
		UIManager::getUIManager()->ComentClear();

		//�R�����g�p�̔z�������������
		UIManager::getUIManager()->ComentLoad(BOSS_COMENT_MAX, boss_npc->GetNpcName());

		//�V�[�P���X��؂�ւ���
		sequence_.change(&InMapScene::seqMenuOpen);
	}
}
