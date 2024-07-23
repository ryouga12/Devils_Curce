#include "InMapScene.h"
#include"../Manager/SceneManager.h"
#include"battleScene.h"
#include "../../dxlib_ext/dxlib_ext.h"
#include"mapScene.h"
#include<iostream>
#include"../Manager/UiManager.h"
#include"../Manager/CsvManager.h"
#include"../Item/WeaponShop.h"

//------------------------------------------------------------------------------------------------------------------------
//�������@&  ���

//�R���X�g���N�^
//���� : ���ɓ��������̍ŏ��̍��W , ���݂̃V�[��
InMapScene::InMapScene(tnl::Vector3 firstPos, InMapState inmap_state): first_pos(firstPos), curent_inmap_scene(inmap_state)
{
	//���݂̃V�[����InMap�ɐݒ�
	curent_scene = SceneState::INMAP;
}

InMapScene::~InMapScene()
{

	//�}�b�v�`�b�v�̉��(2096 = �}�b�v�`�b�v�̑���)
	for (int i = 0; i < 2096; i++) {
		DeleteGraph(gpc_map_chip_hdls_[i]);
		tnl::DebugTrace("Deleted  MapChipimage at [%d]. Pointer value: %p\n", i, gpc_map_chip_hdls_[i]);
	}
}

//������
void InMapScene::InitVillageScene()
{
	//����֘A�̏�����
	weapon_shop = std::make_shared<WeaponShop>(curent_inmap_scene);

	//����
	std::vector<std::string> herdman_npc_ghdl = { "graphics/Npc/herdman.png"  ,"graphics/Npc/herdman_right.png" ,"graphics/Npc/herdman_up.png" , "graphics/Npc/herdman_idle.png"};

	for (int i = 0; i < herdman_npc_ghdl.size(); i++) {

		herdman_hdl_pass[i] = herdman_npc_ghdl[i];
	}

	//���폤�l
	std::vector<std::string> armsdealer_npc_ghdl = {"graphics/Npc/Armsdealer_left.png"  ,"graphics/Npc/Armsdealer_right.png" ,"graphics/Npc/Armsdealer_up.png"  ,"graphics/Npc/Armsdealer.png"};

	for (int i = 0; i < armsdealer_npc_ghdl.size(); i++) {

		Armsdealer_hdl_pass[i] = armsdealer_npc_ghdl[i];
	}

	//�������l
	std::vector<std::string> female_villagers_npc_ghdl = { "graphics/Npc/female_villagers_left.png"  ,"graphics/Npc/female_villagers_right.png" ,"graphics/Npc/female_villagers_up.png"  ,"graphics/Npc/female_villagers_idle.png" };

	for (int i = 0; i < armsdealer_npc_ghdl.size(); i++) {

		female_resident_hdl_pass[i] = female_villagers_npc_ghdl[i];
	}

	//�j�����l
	std::vector<std::string> male_villagers_npc_ghdl = { "graphics/Npc/male_villagers_left.png"  ,"graphics/Npc/male_villagers_right.png" ,"graphics/Npc/male_villagers_up.png"  ,"graphics/Npc/male_villagers_idle.png" };

	for (int i = 0; i < armsdealer_npc_ghdl.size(); i++) {

		resident_male_hdl_pass[i] = male_villagers_npc_ghdl[i];
	}


	//NPC�̏�����
	herdman_npc = std::make_shared<Npc>("����", tnl::Vector3(1375, 760, 0),48 , herdman_hdl_pass, NPC_SCALE, Npc::NPC_ANIMATION::NPC_DIR_LEFT, 4 ,"graphics/Npc/herdman_icon.png" );
	armsdealer_npc = std::make_shared<Npc>("���폤�l", tnl::Vector3(1150, 140, 0), 48 , Armsdealer_hdl_pass, NPC_SCALE, Npc::NPC_ANIMATION::NPC_DIR_DOWN, 4 ,"graphics/Npc/Armsdealer_icon.png" );
	priest_npc = std::make_shared<Npc>("�_��", tnl::Vector3(239, 740, 0),48 , Priest_hdl_pass, NPC_SCALE, Npc::NPC_ANIMATION::NPC_DIR_DOWN, 4 , "graphics/Npc/Priest_icon.png");
	female_resident_npc = std::make_shared<Npc>("�������l", tnl::Vector3(585, 667, 0), 48, female_resident_hdl_pass, NPC_SCALE, Npc::NPC_ANIMATION::NPC_DIR_RIGHT, 4, "graphics/Npc/female_villagers_icon.png");
	resident_male_npc = std::make_shared<Npc>("�j�����l", tnl::Vector3(1475, 360, 0), 48, resident_male_hdl_pass, NPC_SCALE, Npc::NPC_ANIMATION::NPC_DIR_DOWN, 4, "graphics/Npc/male_villagers_icon.png");


	//�A�N�^�[���X�g��NPC��ǉ�����
	actor_list.emplace_back(armsdealer_npc);
	actor_list.emplace_back(herdman_npc);
	actor_list.emplace_back(priest_npc);
	actor_list.emplace_back(female_resident_npc);
	actor_list.emplace_back(resident_male_npc);

	//soundManager�̏�����
	SoundManager::getSoundManager()->sound_Play("sound/BGM/tasogare.mp3", DX_PLAYTYPE_LOOP);

	//�}�b�v�`�b�v�̃��[�h
	//���p�̃}�b�v�`�b�v�����[�h����
	mapChip_Loding(CsvManager::getCsvManager()->GetVillagMapCsv(), CsvManager::getCsvManager()->GetVillagMapObjectCsv());

}

//�_���W�����V�[��(�����̏�)�̏�����
void InMapScene::InitDungeonScene()
{
	//�{�X
	boss_npc = std::make_shared<Npc>("����", tnl::Vector3(801, 153, 0), 128 , boss_hdl_pass, 1, Npc::NPC_ANIMATION::NPC_DIR_DOWN, 4, "graphics/Npc/herdman_icon.png");
	//�A�N�^�[���X�g��NPC��ǉ�����
	actor_list.emplace_back(boss_npc);

	//�}�b�v�`�b�v�̃��[�h
	//�_���W�����p�̃}�b�v�`�b�v�����[�h����
	mapChip_Loding(CsvManager::getCsvManager()->GetBossCastleFloorCsv() , CsvManager::getCsvManager()->GetBossCastleObjectCsv());

	//�����̏���ł̉��y
	SoundManager::getSoundManager()->sound_Play("sound/BGM/maou_bgm_castle.mp3",DX_PLAYTYPE_LOOP);
}

//���V�[���̏�����
void InMapScene::InitTownScene()
{
	//����֘A�̏�����
	weapon_shop = std::make_shared<WeaponShop>(curent_inmap_scene);

	//���폤�l�̉摜������������
	std::vector<std::string> armsdealer_npc_ghdl = { "graphics/Npc/Armsdealer_man_town_left.png"  ,"graphics/Npc/Armsdealer_man_town_right.png","graphics/Npc/Armsdealer_man_town_up.png", "graphics/Npc/Armsdealer_man_town.png" };

	for (int i = 0; i < armsdealer_npc_ghdl.size(); i++) {

		Armsdealer_hdl_pass[i] = armsdealer_npc_ghdl[i];
	}

	//�����̉摜������������
	std::vector<std::string> npc_ghdl = {"graphics/Npc/town_herdman_left.png"  ,"graphics/Npc/town_herdman_right.png","graphics/Npc/town_herdman_up.png","graphics/Npc/town_herdman_idle.png" };
	
	for (int i = 0; i < npc_ghdl.size(); i++) {

		herdman_hdl_pass[i] = npc_ghdl[i];
	}

	//�h���̉摜������������
	std::vector<std::string> inn_npc_ghdl = {"graphics/Npc/inn_left.png" ,"graphics/Npc/inn_right.png","graphics/Npc/inn_up.png","graphics/Npc/town_inn_.png" };

	for (int i = 0; i < inn_npc_ghdl.size(); i++) {

		inn_hdl_pass[i] = inn_npc_ghdl[i];
	}

	//�����̉摜������������
	std::vector<std::string> enemy_npc_ghdl = { "graphics/Npc/enemy_1_left.png" ,"graphics/Npc/enemy_1_right.png","graphics/Npc/enemy_1_up.png","graphics/Npc/enemy_1_idle.png" };

	for (int i = 0; i < enemy_npc_ghdl.size(); i++) {

		enemy_npc_pass[i] = enemy_npc_ghdl[i];
	}

	//---npc�̏�����---//

	//���폤�l
	armsdealer_npc = std::make_shared<Npc>("���폤�l", tnl::Vector3(1730, 580, 0), 48, Armsdealer_hdl_pass, 1.5f, Npc::NPC_ANIMATION::NPC_DIR_DOWN, 4 ,"graphics/Npc/Armsdealer_man_icon_.png" );

	//����
	herdman_npc = std::make_shared<Npc>("����", tnl::Vector3(220, 266, 0), 48, herdman_hdl_pass, 1.5f, Npc::NPC_ANIMATION::NPC_DIR_UP, 4 ,"graphics/Npc/town_herdman_icon.png");

	//�h��
	inn_npc = std::make_shared<Npc>("�h��", tnl::Vector3(780, 1062, 0), 48, inn_hdl_pass, 1.5f, Npc::NPC_ANIMATION::NPC_DIR_DOWN, 4 ,"graphics/Npc/inn_icon.png");

	//�G
	//�C�x���g�t���O��true�������琶��������
	if (EventManager::getEventManager()->GetEnemyEventFlag()) {
		enemy_npc = std::make_shared<Npc>("����", tnl::Vector3(2032, 205, 0), 48, enemy_npc_pass, 1.5f, Npc::NPC_ANIMATION::NPC_DIR_UP, 4, "graphics/Npc/enemy_1_icon.png");

		actor_list.emplace_back(enemy_npc);
	}

	//�A�N�^�[���X�g��NPC��ǉ�����
	actor_list.emplace_back(armsdealer_npc);
	actor_list.emplace_back(herdman_npc);
	actor_list.emplace_back(inn_npc);


	//soundManager�̏�����
	//����BGM�𗬂�
	SoundManager::getSoundManager()->sound_Play("sound/BGM/town_bgm.mp3", DX_PLAYTYPE_LOOP);

	//�}�b�v�`�b�v�̃��[�h
	mapChip_Loding(CsvManager::getCsvManager()->GetTownMapCsv(), CsvManager::getCsvManager()->GetTownObjectMapCsv());
}

//�鉺���V�[���̏�����
void InMapScene::InitCastleTownScene()
{
	auto mgr = SceneManager::GetInstance();

	//�V�[���}�l�[�W���[�̃V�[���t���O��false�̏ꍇ�̂�
	if(!mgr->GetSceneFlag()){

		//��V�[������ȍ~�������Ɉ�x�}�b�v�`�b�v����ɂ���
		MapChips.clear();
		MapChip_object.clear();

		//�v���C���[�̈ʒu��ς���
		GameManager::getGameManager()->getPlayer()->SetPlayerPosition(CASTLE_EXIT_POS);

		//�J�����ʒu���ύX����
		GameManager::getGameManager()->getCamera()->SavePosition(GameManager::getGameManager()->getPlayer()->getPlayerPos());
		GameManager::getGameManager()->getCamera()->SetTargetPos();
	}

	//soundManager�̏�����
	//�鉺����BGM�𗬂�
	SoundManager::getSoundManager()->sound_Play("sound/BGM/castle_town_bgm.mp3", DX_PLAYTYPE_LOOP);

	//�}�b�v�`�b�v�̃��[�h
	mapChip_Loding(CsvManager::getCsvManager()->GetCastleTownMapCsv(), CsvManager::getCsvManager()->GetCastleTownObjectMapCsv());
}

//��V�[���̏�����
void InMapScene::InitCastleScene()
{
	MapChips.clear();
	MapChip_object.clear();
	actor_list.clear();

	//�v���C���[�̈ʒu��ς���
	GameManager::getGameManager()->getPlayer()->SetPlayerPosition(CASTLE_FIRST_POS);

	//�J�����ʒu���ύX����
	GameManager::getGameManager()->getCamera()->SavePosition(GameManager::getGameManager()->getPlayer()->getPlayerPos());
	GameManager::getGameManager()->getCamera()->SetTargetPos();

	//��
	if (!king_npc) {
		king_npc = std::make_shared<Npc>("��", tnl::Vector3(944, 315, 0), 48, king_hdl_pass, 1.5f, Npc::NPC_ANIMATION::NPC_DIR_DOWN, 4, "graphics/Npc/king_icon.png");
	}
	//����
	if (!queen_npc) {
		queen_npc = std::make_shared<Npc>("����", tnl::Vector3(1261, 315, 0), 48, queen_hdl_pass, 1.5f, Npc::NPC_ANIMATION::NPC_DIR_DOWN, 4, "graphics/Npc/queen_icon.png");
	}

	//�A�N�^�[���X�g��NPC��ǉ�����
	actor_list.emplace_back(king_npc);
	actor_list.emplace_back(queen_npc);

	//�}�b�v�`�b�v�̃��[�h
	mapChip_Loding(CsvManager::getCsvManager()->GetCastleMapCsv(), CsvManager::getCsvManager()->GetCastleObjectMapCsv());
}


void InMapScene::InitInMapScene()
{
	//Player�̏����ʒu�����߂�
	GameManager::getGameManager()->getPlayer()->SetPlayerPosition(first_pos);

	//���j���[�̏�����
	InitMenuWindow();

	//�}�b�v�`�b�v�̏�����
	if (!mapchip) {
		mapchip = std::make_shared<MapChip>();
	}

	switch (curent_inmap_scene)
	{
	//��
	case InMapScene::InMapState::VILLAGE:


		//����������
		InitVillageScene();

		//�}�b�v�̍����ƃ}�b�v�̕������߂�
		map_height = 945;																					
		map_width = 1600;																					

		break;

	//��
	case InMapScene::InMapState::TOWN:

		//�}�b�v�̍����ƃ}�b�v�̕������߂�
		map_height = 1400;
		map_width = 2240;

		//���V�[���̏�����
		InitTownScene();

		break;

	//�鉺��
	case InMapScene::InMapState::CASTLETOWN:

		//�}�b�v�̍����ƃ}�b�v�̕������߂�
		map_height = 1008;
		map_width = 2240;

		//�鉺���V�[���̏�����
		InitCastleTownScene();

		break;

	//��
	case InMapScene::InMapState::CASTLE:

		//�}�b�v�̍����ƃ}�b�v�̕������߂�
		map_height = 1740;
		map_width = 2240;

		//��V�[���̏�����
		InitCastleScene();

		break;

	//�����̏�
	case InMapScene::InMapState::BOSSCASTLE:

		//�}�b�v�̍����ƃ}�b�v�̕������߂�
		map_height = 940;
		map_width = 1600;

		//�_���W�����V�[���̏�����
		InitDungeonScene();

		break;


	default:
		break;
	}

	//�C�x���g�֘A�̏�����
	EventManager::getEventManager()->InitEventRelated(actor_list);

}

//---���j���[�̏�����---//
void InMapScene::InitMenuWindow()
{

	//�ʏ�̃��j���[�E�B���h�E
	menuWindow = UIManager::getUIManager()->getMenu("menu_window");

	//�b���R�}���h
	if (!dia_log_coment) {

		dia_log_coment = new MenuWindow::MenuElement_t[]{
			{100 , 100 , "�b��" , 0},
			{100 , 150 , "����" , 1}
		};
	}

	if (!dia_log_coment_select) {
		dia_log_coment_select = std::make_shared<MenuWindow>("graphics/WindowBase_01.png", dia_log_coment, 2, 1);
		dia_log_coment_select->Open();
	}

}

//------------------------------------------------------------------------------------------------------------------------
//�X�V����

void InMapScene::Update(float delta_time)
{
	InMapSceneUpdate(delta_time);

	__super::Update(delta_time);
}

//���ꂼ��̍X�V����
void InMapScene::InMapSceneUpdate(float delta_time)
{
	//�V�[�P���X
	sequence_.update(delta_time);

	//Player
	GameManager::getGameManager()->getPlayer()->player_Move(delta_time, PlayerVelocity, map_height);

	//Inventory
	GameManager::getGameManager()->getInventory()->Update(delta_time);

	//UIManager�̍X�V����
	//UIManager::getUIManager()->Update(delta_time);

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

		//npc
		for (auto& actor : actor_list) {
			actor->Update(delta_time);
		}

		break;

	//�鉺��
	case InMapScene::InMapState::CASTLETOWN:


		break;

	//��V�[��
	case InMapScene::InMapState::CASTLE:

		//npc
		for (auto& actor : actor_list) {
			actor->Update(delta_time);
		}

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

	__super::Draw();
}

//���ꂼ��̕`�揈��
void InMapScene::InMapSceneDraw()
{

	//---mapChip�̕`��---//
	for (auto& mapchip : MapChips) {
		mapchip->Draw(*GameManager::getGameManager()->getCamera());
	}

	//---mapChip�̕`��---//
	for (auto& mapchip : MapChip_object) {
		mapchip->Draw(*GameManager::getGameManager()->getCamera());
	}

	switch (curent_inmap_scene)
	{
	case InMapScene::InMapState::VILLAGE:

		//---Npc�̕`��---//
		for (auto& actor : actor_list) {
			actor->Draw(*GameManager::getGameManager()->getCamera());
		}

		break;

	//���V�[��
	case InMapScene::InMapState::TOWN:

		//---Npc�̕`��---//
		for (auto& actor : actor_list) {
			actor->Draw(*GameManager::getGameManager()->getCamera());
		}

		break;

	//�鉺��
	case InMapScene::InMapState::CASTLETOWN:


		break;

	//��V�[��
	case InMapScene::InMapState::CASTLE:

		//---Npc�̕`��---//
		for (auto& actor : actor_list) {
			actor->Draw(*GameManager::getGameManager()->getCamera());
		}

		break;

	//�{�X�̏�
	case InMapScene::InMapState::BOSSCASTLE:


		//---Npc�̕`��---//
		for (auto& actor : actor_list) {
			actor->Draw(*GameManager::getGameManager()->getCamera());
		}

		/*DrawStringEx(100, 100, -1, "%f", GameManager::getGameManager()->getPlayer()->getPlayerPos().x);
		DrawStringEx(100, 150, -1, "%f", GameManager::getGameManager()->getPlayer()->getPlayerPos().y);*/


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

	//UIManager�̕`��(��ɃA�C�R���Ȃ�)
	//UIManager::getUIManager()->Draw();

	/*DrawStringEx(100, 100, -1, "%f", GameManager::getGameManager()->getPlayer()->getPlayerPos().x);
	DrawStringEx(100, 150, -1, "%f", GameManager::getGameManager()->getPlayer()->getPlayerPos().y);*/

}

//------------------------------------------------------------------------------------------------------------------------
//�V�[�P���X�֘A

//�V�[�P���X��؂�ւ���
void InMapScene::SequenceChange(Sequence_num secect_sequence)
{
	//�I��������V�[�P���X�ɂ���đ��̏ꏊ�ŃV�[�P���X��؂�ւ���
	//��ɃC�x���g�}�l�[�W���[�ȂǂŃC�x���g��؂�ւ��鎞�Ɏg�p
	switch (secect_sequence)
	{

	//�V�[�P���X��؂�ւ���
	//�v���C���[���s����
	case InMapScene::Sequence_num::MOVE:

		//�ʏ�ɖ߂�
		sequence_.change(&InMapScene::seqMove);

		break;

	//���퉮
	case InMapScene::Sequence_num::WEAPONSHOP:

		sequence_.change(&InMapScene::seqWeaponShop);

		break;

	//���j���[���J��
	case InMapScene::Sequence_num::MENUOPEN:

		sequence_.change(&InMapScene::seqMenuOpen);

		break;


	//npc�Ƙb��
	case InMapScene::Sequence_num::MENUNPC:

		//�ʏ�ɖ߂�
		sequence_.change(&InMapScene::seqNpc);

		break;

	default:

		break;
	}
}

//������
bool InMapScene::seqInit(float delta_time)
{
	if (sequence_.isStart()) {

		select_num = Sequence_num::INIT;

		//������
		InitInMapScene();

		//�V�[�P���X��؂�ւ���
		sequence_.change(&InMapScene::seqMove);
	}

	return false;
}

bool InMapScene::seqMove(float delta_time)
{
	if (sequence_.isStart()) {

		if (!GameManager::getGameManager()->getPlayer()->getPlayerControl()) {
			GameManager::getGameManager()->getPlayer()->setPlayerControl();
		}
		select_num = Sequence_num::MOVE;
	}

	//�J�����̍X�V����
	GameManager::getGameManager()->getCamera()->update(GameManager::getGameManager()->getPlayer()->getPlayerPos(), map_width, map_height);

	if (GameManager::getGameManager()->getInventory()->GetSelectMenuNum() == Inventory::MenuWindow_I::EMPTY && tnl::Input::IsKeyDownTrigger(eKeys::KB_ESCAPE)) {

		//���艹��炷
		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
		//�C���x���g����\������
		GameManager::getGameManager()->getInventory()->InventoryMenuChange(Inventory::MenuWindow_I::FIRSTMENU);
		//plyer�𓮂��Ȃ��悤�ɂ���
		GameManager::getGameManager()->getPlayer()->setPlayerControl();
	}

	//�����蔻��
	//�C���x���g�����J���Ă��鎞�͓����蔻�������
	if (GameManager::getGameManager()->getInventory()->GetSelectMenuNum() == Inventory::MenuWindow_I::EMPTY) {
		mapChip_collision();
	}

	//�v���C���[�̍��W���}�b�v�O�ɏo����V�[����؂�ւ���
	//���V�[���̂ݕ����Ȃ��ׁA��ɂ����Ȃ��悤�ɂ���
	if (curent_inmap_scene == InMapState::VILLAGE) {

		//�v���C���[�̃T�C�Y
		auto player_size = GameManager::getGameManager()->getPlayer()->GetPlayerSize();

		//�v���C���[�̈ړ�����
		if (GameManager::getGameManager()->getPlayer()->getPlayerPos().y < static_cast<float>(player_size.y / 2)) {

			tnl::Vector3 player_pos = { GameManager::getGameManager()->getPlayer()->getPlayerPos().x ,static_cast<float>(player_size.y / 2) , 0 };

			GameManager::getGameManager()->getPlayer()->SetPlayerPosition(player_pos);
		}
	}
	//�鉺���V�[���̏ꍇ��ɍs�������Ɉړ�������
	else if (curent_inmap_scene == InMapState::CASTLETOWN && GameManager::getGameManager()->getPlayer()->getPlayerPos().y < 0) {

		curent_inmap_scene = InMapState::CASTLE;

		//�����������ď�V�[���Ɉڍs������
		sequence_.change(&InMapScene::seqInit);

		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/idou.mp3", DX_PLAYTYPE_BACK);

		auto mgr = SceneManager::GetInstance();

		if (mgr->GetSceneFlag()) {
			//�t���O��؂�ւ��ăV�[����J�ڂ����Ȃ��悤�ɂ���
			mgr->SceneFlagChange();
		}

		//�t�F�[�h������
		mgr->FadeScene(0.1f, 1.0f);

		//�T�E���h���~�߂�
		SoundManager::getSoundManager()->StopSound("sound/BGM/castle_town_bgm.mp3");

		return false;

	}
	//��V�[���̏ꍇ�A�}�b�v�O�ɏo����鉺���ɑJ�ڂ�����
	else if (GameManager::getGameManager()->getPlayer()->getPlayerPos().y >= map_height && curent_inmap_scene == InMapState::CASTLE)
	{
		curent_inmap_scene = InMapState::CASTLETOWN;

		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/idou.mp3", DX_PLAYTYPE_BACK);

		auto mgr = SceneManager::GetInstance();

		if (mgr->GetSceneFlag()) {
			//�t���O��؂�ւ��ăV�[����J�ڂ����Ȃ��悤�ɂ���
			mgr->SceneFlagChange();
		}

		//�t�F�[�h������
		mgr->FadeScene(0.1f, 1.0f);

		//�����������ď�V�[���Ɉڍs������
		sequence_.change(&InMapScene::seqInit);

		return false;
	}
	if (GameManager::getGameManager()->getPlayer()->getPlayerPos().x >= map_width || GameManager::getGameManager()->getPlayer()->getPlayerPos().x < 0 || GameManager::getGameManager()->getPlayer()->getPlayerPos().y >= map_height)
	{
		TimeCount = true;

		//���̏ꍇ
		if (curent_inmap_scene == InMapState::VILLAGE) {
			//���y��Stop����
			SoundManager::getSoundManager()->StopSound("sound/BGM/tasogare.mp3");
		}
		//���̏ꍇ
		else if (curent_inmap_scene == InMapState::TOWN) {
			//���y��Stop����
			SoundManager::getSoundManager()->StopSound("sound/BGM/town_bgm.mp3");
		}
		//�����̏�̏ꍇ
		else if (curent_inmap_scene == InMapState::BOSSCASTLE) {
			//���y��Stop����
			SoundManager::getSoundManager()->StopSound("sound/BGM/maou_bgm_castle.mp3");
		}
		//�鉺���̏ꍇ
		else if (curent_inmap_scene == InMapState::CASTLETOWN) {
			//���y��Stop����
			SoundManager::getSoundManager()->StopSound("sound/BGM/castle_town_bgm.mp3");
		}

		//�ړ���SE�𗬂�
		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/idou.mp3", DX_PLAYTYPE_BACK);
	}
	//�؂�ւ���ۂ�SE�𗬂��؂��Ă���J�ڂ���
	if (TimeCount) {

		//0.2�bSE�𗬂��Ă���J�ڂ�����
		if (GameManager::getGameManager()->TimeCount(delta_time, TimeSE)) {

			sequence_.change(&InMapScene::seqChangeScene);

			//�V�[���}�l�[�W���[�̃|�C���^���擾����
			auto mgr = SceneManager::GetInstance();

			//�t���O��false�̏ꍇtrue�ɕύX����(�V�[����J�ڂ������)
			if (!mgr->GetSceneFlag()) { mgr->SceneFlagChange(); }

			//�V�[����J�ڂ�����
			mgr->changeScene(new MapScene(),0.3);
		}
	}
	
	
	return false;
}

//���퉮
bool InMapScene::seqWeaponShop(float delta_time)
{
	if (sequence_.isStart()) {

		select_num = Sequence_num::WEAPONSHOP;
		//����R�����g������������
		//�R�����g�����Ԃ�Ȃ��悤�ɂ����
		weapon_shop->buyComentClear();

		//Npc�̃R�����g��؂�ւ���
		EventManager::getEventManager()->SetNpcComent(EventManager::NpcMove::WEAPONSHOP);
	}

	//���퉮�̏���
	weapon_shop->BuyWeapon(GameManager::getGameManager()->getPlayer());
	weapon_shop->WeaponShopCursorMove();
	
	//0����������V�[�P���X��ς���
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_0)) {
		//�V�[����؂�ւ���
		sequence_.change(&InMapScene::seqMove);
	}

	return false;
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
	else if (curent_inmap_scene == InMapState::TOWN) {
		GameManager::getGameManager()->getPlayer()->SetPlayerPosition(TOWN_EXIT_POS);
	}
	else if (curent_inmap_scene == InMapState::CASTLETOWN) {
		GameManager::getGameManager()->getPlayer()->SetPlayerPosition(CASTLE_TOWN_EXIT_POS);
	}

	return true;
}

//���j���[
bool InMapScene::seqMenuOpen(float delta_time)
{
	if (sequence_.isStart())
	{
		//�v���C���[�𓮂��Ȃ�����
		GameManager::getGameManager()->getPlayer()->setPlayerControl();
		select_num = Sequence_num::MENUOPEN;
	}

	if (EventManager::getEventManager()->GetSelectMenu() != EventManager::MenuOpen::EMPTY && EventManager::getEventManager()->GetNpcMove() == EventManager::NpcMove::WEAPONSHOP
		&& dia_log_coment_select->getSelectNum() == 0) {
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			sequence_.change(&InMapScene::seqWeaponShop);
		}
	}
	else if (EventManager::getEventManager()->GetSelectMenu() == EventManager::MenuOpen::EMPTY && dia_log_coment_select->getSelectNum() == 0 || EventManager::getEventManager()->GetSelectMenu() == EventManager::MenuOpen::BOSS
		&& dia_log_coment_select->getSelectNum() == 0) {
		if(tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)){
			sequence_.change(&InMapScene::seqNpc);
		}
	}
	

	if (dia_log_coment_select->getSelectNum() == 1 && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

		//�V�[�P���X��؂�ւ���
		sequence_.change(&InMapScene::seqMove);
	}

	return false;
}

//Npc�̏���
bool InMapScene::seqNpc(float delta_time)
{
	if (sequence_.isStart()) {
		select_num = Sequence_num::MENUNPC;

		//�t���[�������Z�b�g���ăZ�[�u�R�����g�����Z�b�g����
		UIManager::getUIManager()->FrameReset();
	}

	//�r���ŉ�b�𒆒f����ׂ̃L�[
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_0)) {
		sequence_.change(&InMapScene::seqMove);
	}

	//�C�x���g��������Npc�Ƃ̍X�V����
	EventManager::getEventManager()->NpcEventUpdate(delta_time, this, static_cast<int>(curent_inmap_scene) , actor_list);

	return false;
}

//�V�[�P���X���̕`��
void InMapScene::switchSequenceDraw()
{
	switch (select_num)
	{
		//�ʏ�V�[�P���X
	case InMapScene::Sequence_num::MOVE:
		
		//DrawStringEx(menuWindow->menu_x + 50, menuWindow->menu_y + 350, -1, "seqIdle");

		break;

		//���j���[�V�[�P���X�̎�
	case InMapScene::Sequence_num::MENUOPEN:

		//Npc�̃R�����g�\��
		EventManager::getEventManager()->EventMenuWindow();

		//�b���R�����g
		dia_log_coment_select->All(50, 50, 250, 200);

		break;

		//Npc�V�[�P���X�̎�(Npc�ɂ���ē����ς���)
	case InMapScene::Sequence_num::MENUNPC:

		EventManager::getEventManager()->NpcSwitchComent(weapon_shop);

		break;

	case InMapScene::Sequence_num::WEAPONSHOP:

		EventManager::getEventManager()->NpcSwitchComent(weapon_shop);

		break;
	}
}


//------------------------------------------------------------------------------------------------------------------------
//�}�b�v�`�b�v�֘A

//�}�b�v�`�b�v�̃��[�h
void InMapScene::mapChip_Loding(std::vector<std::vector<int>> map_csv ,std::vector<std::vector<int>> map_object_csv)
{
	//csv������������
	map_csv_ = map_csv;

	map_csv_objct_ = map_object_csv;

	map_chip_all_size_ = map_chip_x_size_ * map_chip_y_size_;

	LoadDivGraph(gpc_map_chip_hdls_pass_.c_str(),
		map_chip_all_size_,
		map_chip_x_size_,
		map_chip_y_size_,
		map_chip_width_,
		map_chip_height_,
		gpc_map_chip_hdls_);

	//�}�b�v�`�b�v�ǂݍ���
	for (int i = 0; i < map_csv_.size(); ++i) {
		for (int k = 0; k < map_csv_[i].size(); ++k) {
			if (-1 == map_csv_[i][k]) continue;
			tnl::Vector3 pos;
			int gpc_hdl;
			pos.x = { static_cast<float>(mapchip->GetChipSize().x / 2 + k * mapchip->GetChipSize().x)};
			pos.y = { static_cast<float>(i * mapchip->GetChipSize().x)};
			gpc_hdl = gpc_map_chip_hdls_[map_csv_[i][k]];

			MapChips.emplace_back(std::make_shared<MapChip>(pos, gpc_hdl));

		}
	}

	//�I�u�W�F�N�g�̃}�b�v�`�b�v�̓ǂݍ���
	for (int i = 0; i < map_csv_objct_.size(); ++i) {
		for (int k = 0; k < map_csv_objct_[i].size(); ++k) {
			if (-1 == map_csv_objct_[i][k])continue;
			tnl::Vector3 pos;
			int gpc_hdl;
			pos.x = { static_cast<float>(mapchip->GetChipSize().x / 2 + k * mapchip->GetChipSize().x) };
			pos.y = { static_cast<float>(i * mapchip->GetChipSize().x) };
			gpc_hdl = gpc_map_chip_hdls_[map_csv_objct_[i][k]];

			MapChip_object.emplace_back(std::make_shared<MapChip>(pos, gpc_hdl));
		}
	}
	
}

//�����蔻��̏���
void InMapScene::mapChip_collision()
{
	//���݂̃V�[��������������
	if (curent_inmap_scene == InMapState::VILLAGE) {
		//���̓����蔻��
		VillageCollision();
	}
	//���݂̃V�[�����{�X�̏邾������
	else if (curent_inmap_scene == InMapState::BOSSCASTLE) {
		//�{�X�̏�V�[���̓����蔻��
		DungeonCollison();
	}
	//���݂̃V�[��������������
	else if (curent_inmap_scene == InMapState::TOWN) {
		TownCollision();
	}
	//���݂̃V�[�����鉺����������
	else if (curent_inmap_scene == InMapState::CASTLETOWN) {
		CastleTownCollision();
	}
	//���݂̃V�[������V�[����������
	else if (curent_inmap_scene == InMapState::CASTLE) {
		CastleCollision();
	}
}

//���V�[���ł̓����蔻��
void InMapScene::VillageCollision()
{
	//�v���C���[���擾����
	auto& player = GameManager::getGameManager()->getPlayer();

	//�u���b�N�ƏՓ˔��� & �␳
	//�Q�[�����[�v���ł̓����蔻��̃`�F�b�N]
	for (auto& mapChip : MapChip_object) {

		int mapChipValue = map_csv_objct_[static_cast<unsigned>(mapChip->MapChipPos().y / mapChip->GetChipSize().y)][static_cast<unsigned>(mapChip->MapChipPos().x / mapChip->GetChipSize().x)];


		//�����蔻��
		if (tnl::IsIntersectRect(player->getPlayerPos(), player->GetPlayerWidth(), player->GetPlayerHight(), mapChip->MapChipPos(), map_chip_width_, map_chip_height_)) {

			//���W�␳
			tnl::eCorrResRect collision = tnl::CorrectPositionRect(
				player->getPlayerPrevPos(),
				mapChip->MapChipPos(),
				player->GetPlayerSize(),
				mapChip->GetChipSize(),
				player->getPlayerPos(),
				mapChip->MapChipPos(),
				tnl::eCorrTypeRect::PWRFL_B,
				tnl::eCorrTypeRect::PWRFL_B, 1);


			//���퉮�Ƙb�����ꍇ
			if (mapChipValue == map_Chip_WeaponShop) {

				//�A�C�R���̃A�j���[�V�����𗬂�
				UIManager::getUIManager()->IconAnimation();

				if (select_num == Sequence_num::MOVE && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

					//�A�C�R���̃A�j���[�V�������~�߂�
					UIManager::getUIManager()->IconAnimationStop();

					//�V�[�P���X��؂�ւ���
					EventManager::getEventManager()->SetMenuNpc(EventManager::MenuOpen::WEAPONSHOP);
					EventManager::getEventManager()->SetNpcComent(EventManager::NpcMove::WEAPONSHOP);
					sequence_.change(&InMapScene::seqMenuOpen);

				}
			}

			//�����̑O�̊�����b������
			else if (mapChipValue == map_Chip_Herdman) {

				//�A�C�R���̃A�j���[�V�����𗬂�
				UIManager::getUIManager()->IconAnimation();

				if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

					//�A�C�R���̃A�j���[�V�������~�߂�
					UIManager::getUIManager()->IconAnimationStop();

					//��Ԃ�؂�ւ���
					EventManager::getEventManager()->SetMenuNpc(EventManager::MenuOpen::EMPTY);
					EventManager::getEventManager()->SetNpcComent(EventManager::NpcMove::HERDMAN);

					//1����R�����g�𗬂�
					UIManager::getUIManager()->ComentClear();

					//�J�E���g�����Z�b�g����
					UIManager::getUIManager()->CountReset();

					//�R�����g�p�̔z�������������
					UIManager::getUIManager()->ComentLoad(9, herdman_npc->GetNpcName());

					sequence_.change(&InMapScene::seqMenuOpen);
				}
			}
		}



		//---Npc�̓����蔻��---//

		//����
		else if (tnl::IsIntersectRect(player->getPlayerPos(), player->GetPlayerWidth(), player->GetPlayerHight(), herdman_npc->GetNpcPos(), herdman_npc->GetNpcSizeWidth(), herdman_npc->GetNpcSizeHegiht())) {

			//���W�␳
			tnl::eCorrResRect herdman = tnl::CorrectPositionRect(
				player->getPlayerPrevPos(),
				herdman_npc->GetNpcPos(),
				player->GetPlayerSize(),
				herdman_npc->GetNpcCharaChip(),
				player->getPlayerPos(),
				herdman_npc->GetNpcPos(),
				tnl::eCorrTypeRect::PWRFL_B,
				tnl::eCorrTypeRect::PWRFL_B, 0);

			//�����瓖��������
			//bool playerOnBottom = GameManager::getGameManager()->getPlayer()->getPlayerPos().y >= herdman_npc->GetNpcPos().y + herdman_npc->GetNpcSizeHegiht();

			if (static_cast<int>(herdman) != 0) {

				//�A�C�R���̃A�j���[�V�����𗬂�
				UIManager::getUIManager()->IconAnimation();

				if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

					//�A�C�R���̃A�j���[�V�������~�߂�
					UIManager::getUIManager()->IconAnimationStop();

					//���݂̃A�j���[�V������ۊǂ���
					herdman_npc->SetNpcAnimationSave(herdman_npc->GetNpcAnimation());

					//�A�j���[�V���������ɂ���
					herdman_npc->SetNpcAnimation(static_cast<Npc::NPC_ANIMATION>(herdman));

					//1����R�����g�𗬂�
					UIManager::getUIManager()->ComentClear();

					//�J�E���g�����Z�b�g����
					UIManager::getUIManager()->CountReset();

					//���[�h����R�����g�̐�
					const int HERDMAN_COMENT_MAX = 9;

					//�R�����g�p�̔z�������������
					UIManager::getUIManager()->ComentLoad(HERDMAN_COMENT_MAX, herdman_npc->GetNpcName());

					//��Ԃ�؂�ւ���
					EventManager::getEventManager()->SetMenuNpc(EventManager::MenuOpen::EMPTY);
					EventManager::getEventManager()->SetNpcComent(EventManager::NpcMove::HERDMAN);

					//�V�[�P���X��؂�ւ���
					sequence_.change(&InMapScene::seqMenuOpen);
				}
			}
		}

		//�_��
		else if (tnl::IsIntersectRect(player->getPlayerPos(), player->GetPlayerWidth(), player->GetPlayerHight(), priest_npc->GetNpcPos(), priest_npc->GetNpcSizeWidth(), priest_npc->GetNpcSizeHegiht())) {

			//���W�␳
			tnl::eCorrResRect priest_hit = tnl::CorrectPositionRect(
				player->getPlayerPrevPos(),
				priest_npc->GetNpcPos(),
				player->GetPlayerSize(),
				priest_npc->GetNpcCharaChip(),
				player->getPlayerPos(),
				priest_npc->GetNpcPos(),
				tnl::eCorrTypeRect::PWRFL_B,
				tnl::eCorrTypeRect::PWRFL_B, 0);

			//�����瓖��������
			//bool playerOnBottom = GameManager::getGameManager()->getPlayer()->getPlayerPos().y >= priest_npc->GetNpcPos().y + priest_npc->GetNpcSizeHegiht();

			if (priest_hit != tnl::eCorrResRect::UP) {

				//�A�C�R���̃A�j���[�V�����𗬂�
				UIManager::getUIManager()->IconAnimation();

				if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

					//�A�C�R���̃A�j���[�V�������~�߂�
					UIManager::getUIManager()->IconAnimationStop();

					//�A�j���[�V������؂�ւ���
					priest_npc->SetNpcAnimation(static_cast<Npc::NPC_ANIMATION>(priest_hit));

					//1����R�����g�𗬂�
					UIManager::getUIManager()->ComentClear();

					//�J�E���g�����Z�b�g����
					UIManager::getUIManager()->CountReset();

					//�C�x���g�t���O��false�̏ꍇ�t���O��؂�ւ���
					if (!EventManager::getEventManager()->GetEventFlag()) {
						EventManager::getEventManager()->EventFlagChange();
					}

					//���[�h����R�����g�̐�
					const int PRIEST_COMENT_MAX = 3;

					//�R�����g�p�̔z�������������
					UIManager::getUIManager()->ComentLoad(PRIEST_COMENT_MAX, priest_npc->GetNpcName());

					//��Ԃ�؂�ւ���
					EventManager::getEventManager()->SetMenuNpc(EventManager::MenuOpen::EMPTY);
					EventManager::getEventManager()->SetNpcComent(EventManager::NpcMove::PRIEST);
					sequence_.change(&InMapScene::seqMenuOpen);
				}
			}
		}
		//�������l
		else if (tnl::IsIntersectRect(player->getPlayerPos(), player->GetPlayerWidth(), player->GetPlayerHight(), female_resident_npc->GetNpcPos(), female_resident_npc->GetNpcSizeWidth(), female_resident_npc->GetNpcSizeHegiht())) {

			//���W�␳
			tnl::eCorrResRect female_resident_hit = tnl::CorrectPositionRect(
				player->getPlayerPrevPos(),
				female_resident_npc->GetNpcPos(),
				player->GetPlayerSize(),
				female_resident_npc->GetNpcCharaChip(),
				player->getPlayerPos(),
				female_resident_npc->GetNpcPos(),
				tnl::eCorrTypeRect::PWRFL_B,
				tnl::eCorrTypeRect::PWRFL_B, 0);

			if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {


				//�A�j���[�V������؂�ւ���
				female_resident_npc->SetNpcAnimation(static_cast<Npc::NPC_ANIMATION>(female_resident_hit));

				//1����R�����g�𗬂�
				UIManager::getUIManager()->ComentClear();

				//�J�E���g�����Z�b�g����
				UIManager::getUIManager()->CountReset();

				//�C�x���g�t���O��false�̏ꍇ�t���O��؂�ւ���
				if (!EventManager::getEventManager()->GetEventFlag()) {
					EventManager::getEventManager()->EventFlagChange();
				}

				//���[�h����R�����g�̐�
				const int FEMALE_RESIDENT_COMENT_MAX = 3;

				//�R�����g�p�̔z�������������
				UIManager::getUIManager()->ComentLoad(FEMALE_RESIDENT_COMENT_MAX, female_resident_npc->GetNpcName());

				//��Ԃ�؂�ւ���
				EventManager::getEventManager()->SetMenuNpc(EventManager::MenuOpen::EMPTY);
				EventManager::getEventManager()->SetNpcComent(EventManager::NpcMove::FEMALE_RESIDENT);
				sequence_.change(&InMapScene::seqMenuOpen);
			}
		}
		//�j�����l
		else if (tnl::IsIntersectRect(player->getPlayerPos(), player->GetPlayerWidth(), player->GetPlayerHight(), resident_male_npc->GetNpcPos(), resident_male_npc->GetNpcSizeWidth(), resident_male_npc->GetNpcSizeHegiht())) {

			//���W�␳
			tnl::eCorrResRect resident_male_hit = tnl::CorrectPositionRect(
				player->getPlayerPrevPos(),
				resident_male_npc->GetNpcPos(),
				player->GetPlayerSize(),
				resident_male_npc->GetNpcCharaChip(),
				player->getPlayerPos(),
				resident_male_npc->GetNpcPos(),
				tnl::eCorrTypeRect::PWRFL_B,
				tnl::eCorrTypeRect::PWRFL_B, 0);

			if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {


				//�A�j���[�V������؂�ւ���
				resident_male_npc->SetNpcAnimation(static_cast<Npc::NPC_ANIMATION>(resident_male_hit));

				//1����R�����g�𗬂�
				UIManager::getUIManager()->ComentClear();

				//�J�E���g�����Z�b�g����
				UIManager::getUIManager()->CountReset();

				//�C�x���g�t���O��false�̏ꍇ�t���O��؂�ւ���
				if (!EventManager::getEventManager()->GetEventFlag()) {
					EventManager::getEventManager()->EventFlagChange();
				}

				//���[�h����R�����g�̐�
				const int RESIDENT_MALE_COMENT_MAX = 3;

				//�R�����g�p�̔z�������������
				UIManager::getUIManager()->ComentLoad(RESIDENT_MALE_COMENT_MAX, resident_male_npc->GetNpcName());

				//��Ԃ�؂�ւ���
				EventManager::getEventManager()->SetMenuNpc(EventManager::MenuOpen::EMPTY);
				EventManager::getEventManager()->SetNpcComent(EventManager::NpcMove::RESIDENT_MALE);
				sequence_.change(&InMapScene::seqMenuOpen);
			}
		}

	}

}
//���V�[���ł̓����蔻��
void InMapScene::TownCollision()
{

	auto& player = GameManager::getGameManager()->getPlayer();

	for (auto& map_chip_collision : MapChip_object) {

		int mapChipValue = map_csv_objct_[static_cast<unsigned>(map_chip_collision->MapChipPos().y / map_chip_collision->GetChipSize().x)][static_cast<unsigned>(map_chip_collision->MapChipPos().x / map_chip_collision->GetChipSize().x)];

		//�����蔻��
		if (tnl::IsIntersectRect(player->getPlayerPos(), player->GetPlayerWidth(), player->GetPlayerHight(), map_chip_collision->MapChipPos(), map_chip_width_, map_chip_height_)) {

			//���W�␳
			tnl::eCorrResRect n = tnl::CorrectPositionRect(
				player->getPlayerPrevPos(),
				map_chip_collision->MapChipPos(),
				player->GetPlayerSize(),
				map_chip_collision->GetChipSize(),
				player->getPlayerPos(),
				map_chip_collision->MapChipPos(),
				tnl::eCorrTypeRect::PWRFL_B,
				tnl::eCorrTypeRect::PWRFL_B, 0);


			//���퉮�Ƙb�����ꍇ
			if (mapChipValue == 708 && select_num == Sequence_num::MOVE && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

				////�A�C�R���̃A�j���[�V�����𗬂�
				//UIManager::getUIManager()->IconAnimation();
				//�V�[�P���X��؂�ւ���
				EventManager::getEventManager()->SetMenuNpc(EventManager::MenuOpen::WEAPONSHOP);
				EventManager::getEventManager()->SetNpcComent(EventManager::NpcMove::WEAPONSHOP);
				sequence_.change(&InMapScene::seqMenuOpen);
			}

			//�h���ɘb��������
			else if (mapChipValue == 673 && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

				//��Ԃ�؂�ւ���
				EventManager::getEventManager()->SetMenuNpc(EventManager::MenuOpen::EMPTY);
				EventManager::getEventManager()->SetNpcComent(EventManager::NpcMove::INN);

				//1����R�����g�𗬂�
				UIManager::getUIManager()->ComentClear();

				//�J�E���g�����Z�b�g����
				UIManager::getUIManager()->CountReset();

				//�C�x���g�t���O��false�̏ꍇ�t���O��؂�ւ���
				if (!EventManager::getEventManager()->GetEventFlag()) {
					EventManager::getEventManager()->EventFlagChange();
				}

				//�R�����g�p�̔z�������������
				UIManager::getUIManager()->ComentLoad(3, inn_npc->GetNpcName());

				sequence_.change(&InMapScene::seqMenuOpen);

			}

		}
		//����
		else if (tnl::IsIntersectRect(player->getPlayerPos(), player->GetPlayerWidth(), player->GetPlayerHight(), herdman_npc->GetNpcPos(), herdman_npc->GetNpcSizeWidth(), herdman_npc->GetNpcSizeHegiht())) {

			//���W�␳
			tnl::eCorrResRect herdman_hit = tnl::CorrectPositionRect(
				player->getPlayerPrevPos(),
				herdman_npc->GetNpcPos(),
				player->GetPlayerSize(),
				herdman_npc->GetNpcCharaChip(),
				player->getPlayerPos(),
				herdman_npc->GetNpcPos(),
				tnl::eCorrTypeRect::PWRFL_B,
				tnl::eCorrTypeRect::PWRFL_B, 0);

			//�����瓖��������
			//bool playerOnBottom = GameManager::getGameManager()->getPlayer()->getPlayerPos().y >= priest_npc->GetNpcPos().y + priest_npc->GetNpcSizeHegiht();

			if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

				//�A�j���[�V���������ɂ���
				herdman_npc->SetNpcAnimation(static_cast<Npc::NPC_ANIMATION>(herdman_hit));

				//1����R�����g�𗬂�
				UIManager::getUIManager()->ComentClear();

				//�J�E���g�����Z�b�g����
				UIManager::getUIManager()->CountReset();

				//�C�x���g�t���O��false�̏ꍇ�t���O��؂�ւ���
				if (!EventManager::getEventManager()->GetEventFlag()) {
					EventManager::getEventManager()->EventFlagChange();
				}

				//���[�h����R�����g�̐�
				const int HEDMAN_COMENT_MAX = 6;

				//�R�����g�p�̔z�������������
				UIManager::getUIManager()->ComentLoad(HEDMAN_COMENT_MAX, herdman_npc->GetNpcName());

				//��Ԃ�؂�ւ���
				EventManager::getEventManager()->SetMenuNpc(EventManager::MenuOpen::EMPTY);
				EventManager::getEventManager()->SetNpcComent(EventManager::NpcMove::TOWNHERDMAN);
				sequence_.change(&InMapScene::seqMenuOpen);
			}
		}
	}
		//�G�̃|�C���^�����݂��Ȃ��ꍇ�����蔻�������
		if (!enemy_npc) { return; }
		//����
		if (tnl::IsIntersectRect(player->getPlayerPos(), player->GetPlayerWidth(), player->GetPlayerHight(), enemy_npc->GetNpcPos(), enemy_npc->GetNpcSizeWidth(), enemy_npc->GetNpcSizeHegiht())) {

			//���W�␳
			tnl::eCorrResRect enemy_hit = tnl::CorrectPositionRect(
				player->getPlayerPrevPos(),
				enemy_npc->GetNpcPos(),
				player->GetPlayerSize(),
				enemy_npc->GetNpcCharaChip(),
				player->getPlayerPos(),
				enemy_npc->GetNpcPos(),
				tnl::eCorrTypeRect::PWRFL_B,
				tnl::eCorrTypeRect::PWRFL_B, 0);

			if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

				//�A�j���[�V������ύX����
				enemy_npc->SetNpcAnimation(static_cast<Npc::NPC_ANIMATION>(enemy_hit));

				//1����R�����g�𗬂�
				UIManager::getUIManager()->ComentClear();

				//�J�E���g�����Z�b�g����
				UIManager::getUIManager()->CountReset();

				//�C�x���g�t���O��false�̏ꍇ�t���O��؂�ւ���
				if (!EventManager::getEventManager()->GetEventFlag()) {
					EventManager::getEventManager()->EventFlagChange();
				}

				//���[�h����R�����g�̐�
				const int ENEMY_COMENT_MAX = 5;

				//�R�����g�p�̔z�������������
				UIManager::getUIManager()->ComentLoad(ENEMY_COMENT_MAX, enemy_npc->GetNpcName());

				//��Ԃ�؂�ւ���
				EventManager::getEventManager()->SetMenuNpc(EventManager::MenuOpen::EMPTY);
				EventManager::getEventManager()->SetNpcComent(EventManager::NpcMove::ENEMY);
				sequence_.change(&InMapScene::seqMenuOpen);
			}
		}
		
}

//�{�X�̏�V�[���ł̓����蔻��
void InMapScene::DungeonCollison()
{
	auto& player = GameManager::getGameManager()->getPlayer();

	//---Npc�̓����蔻��---//

	
	if (tnl::IsIntersectRect(player->getPlayerPos(), player->GetPlayerWidth(), player->GetPlayerHight(), boss_npc->GetNpcPos(), boss_npc->GetNpcSizeWidth(), boss_npc->GetNpcSizeHegiht())) {

		//���W�␳
		tnl::eCorrResRect boss = tnl::CorrectPositionRect(
			player->getPlayerPrevPos(),
			boss_npc->GetNpcPos(),
			player->GetPlayerSize(),
			boss_npc->GetNpcCharaChip(),
			player->getPlayerPos(),
			boss_npc->GetNpcPos(),
			tnl::eCorrTypeRect::PWRFL_B,
			tnl::eCorrTypeRect::PWRFL_B, 0);

		//�������Ă���Ƃ���Enter�L�[����������o�g���V�[���ɑJ�ڂ�����
		//����Z���t���ǉ��\��
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

			const int BOSS_COMENT_MAX = 7;

			//�C�x���g�}�l�[�W���[�̏�Ԃ��X�V����
			EventManager::getEventManager()->SetMenuNpc(EventManager::MenuOpen::BOSS);
			EventManager::getEventManager()->SetNpcComent(EventManager::NpcMove::BOSS);

			//�J�E���g�����Z�b�g����
			UIManager::getUIManager()->CountReset();

			//1����R�����g�𗬂�
			UIManager::getUIManager()->ComentClear();

			//�C�x���g�t���O��false�̏ꍇ�t���O��؂�ւ���
			if (!EventManager::getEventManager()->GetEventFlag()) {
				EventManager::getEventManager()->EventFlagChange();
			}

			//�R�����g�p�̔z�������������
			UIManager::getUIManager()->ComentLoad(BOSS_COMENT_MAX, boss_npc->GetNpcName());

			//�V�[�P���X��؂�ւ���
			sequence_.change(&InMapScene::seqMenuOpen);
		}
	}
}

//�鉺���V�[���ł̓����蔻��
void InMapScene::CastleTownCollision()
{
	auto& player = GameManager::getGameManager()->getPlayer();

	for (auto& map_chip_collision : MapChip_object) {

		int mapChipValue = map_csv_objct_[static_cast<unsigned>(map_chip_collision->MapChipPos().y / map_chip_collision->GetChipSize().x)][static_cast<unsigned>(map_chip_collision->MapChipPos().x / map_chip_collision->GetChipSize().x)];

		//�}�b�v�`�b�v�̂����蔻��
		if (tnl::IsIntersectRect(player->getPlayerPos(), player->GetPlayerWidth(), player->GetPlayerHight(), map_chip_collision->MapChipPos(), map_chip_width_, map_chip_height_)) {

			//���W�␳
			tnl::eCorrResRect map_chip_hit = tnl::CorrectPositionRect(
				player->getPlayerPrevPos(),
				map_chip_collision->MapChipPos(),
				player->GetPlayerSize(),
				map_chip_collision->GetChipSize(),
				player->getPlayerPos(),
				map_chip_collision->MapChipPos(),
				tnl::eCorrTypeRect::PWRFL_B,
				tnl::eCorrTypeRect::PWRFL_B, 0);
		}
	}
}

//��V�[���̓����蔻��
void InMapScene::CastleCollision()
{
	auto& player = GameManager::getGameManager()->getPlayer();

	for (auto& map_chip_collision : MapChip_object) {

		int mapChipValue = map_csv_objct_[static_cast<unsigned>(map_chip_collision->MapChipPos().y / map_chip_collision->GetChipSize().x)][static_cast<unsigned>(map_chip_collision->MapChipPos().x / map_chip_collision->GetChipSize().x)];

		//�}�b�v�`�b�v�̂����蔻��
		if (tnl::IsIntersectRect(player->getPlayerPos(), player->GetPlayerWidth(), player->GetPlayerHight(), map_chip_collision->MapChipPos(), map_chip_width_, map_chip_height_)) {

			//���W�␳
			tnl::eCorrResRect map_chip_hit = tnl::CorrectPositionRect(
				player->getPlayerPrevPos(),
				map_chip_collision->MapChipPos(),
				player->GetPlayerSize(),
				map_chip_collision->GetChipSize(),
				player->getPlayerPos(),
				map_chip_collision->MapChipPos(),
				tnl::eCorrTypeRect::PWRFL_B,
				tnl::eCorrTypeRect::PWRFL_B, 0);
		}
	}

	//---Npc�̓����蔻��---//

	//��
	if (tnl::IsIntersectRect(player->getPlayerPos(), player->GetPlayerWidth(), player->GetPlayerHight(), king_npc->GetNpcPos(), king_npc->GetNpcSizeWidth(), king_npc->GetNpcSizeHegiht())) {

		//���W�␳
		tnl::eCorrResRect king_hit = tnl::CorrectPositionRect(
			player->getPlayerPrevPos(),
			king_npc->GetNpcPos(),
			player->GetPlayerSize(),
			king_npc->GetNpcCharaChip(),
			player->getPlayerPos(),
			king_npc->GetNpcPos(),
			tnl::eCorrTypeRect::PWRFL_B,
			tnl::eCorrTypeRect::PWRFL_B, 0);

			if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

				//�A�j���[�V������ύX����
				king_npc->SetNpcAnimation(static_cast<Npc::NPC_ANIMATION>(king_hit));

				//1����R�����g�𗬂�
				UIManager::getUIManager()->ComentClear();

				//�J�E���g�����Z�b�g����
				UIManager::getUIManager()->CountReset();

				//���[�h����R�����g�̐�
				const int KING_COMENT_MAX = 6;

				//�R�����g�p�̔z�������������
				UIManager::getUIManager()->ComentLoad(KING_COMENT_MAX, king_npc->GetNpcName());

				//��Ԃ�؂�ւ���
				EventManager::getEventManager()->SetMenuNpc(EventManager::MenuOpen::EMPTY);
				EventManager::getEventManager()->SetNpcComent(EventManager::NpcMove::KING);

				//�V�[�P���X��؂�ւ���
				sequence_.change(&InMapScene::seqMenuOpen);
			}
	}
	//����
	else if (tnl::IsIntersectRect(player->getPlayerPos(), player->GetPlayerWidth(), player->GetPlayerHight(), queen_npc->GetNpcPos(), queen_npc->GetNpcSizeWidth(), queen_npc->GetNpcSizeHegiht())) {

		//���W�␳
		tnl::eCorrResRect queen_hit = tnl::CorrectPositionRect(
			player->getPlayerPrevPos(),
			queen_npc->GetNpcPos(),
			player->GetPlayerSize(),
			queen_npc->GetNpcCharaChip(),
			player->getPlayerPos(),
			queen_npc->GetNpcPos(),
			tnl::eCorrTypeRect::PWRFL_B,
			tnl::eCorrTypeRect::PWRFL_B, 0);


		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

			//�A�j���[�V������ύX����
			queen_npc->SetNpcAnimation(static_cast<Npc::NPC_ANIMATION>(queen_hit));

			//1����R�����g�𗬂�
			UIManager::getUIManager()->ComentClear();

			//�J�E���g�����Z�b�g����
			UIManager::getUIManager()->CountReset();

			//���[�h����R�����g�̐�
			const int QUEEN_COMENT_MAX = 3;

			//�R�����g�p�̔z�������������
			UIManager::getUIManager()->ComentLoad(QUEEN_COMENT_MAX, queen_npc->GetNpcName());

			//��Ԃ�؂�ւ���
			EventManager::getEventManager()->SetMenuNpc(EventManager::MenuOpen::EMPTY);
			EventManager::getEventManager()->SetNpcComent(EventManager::NpcMove::QUEEN);

			//�V�[�P���X��؂�ւ���
			sequence_.change(&InMapScene::seqMenuOpen);
		}
	}
}
