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
InMapScene::InMapScene(const tnl::Vector3& firstPos, const InMapState& inmap_state): first_pos(firstPos), curent_inmap_scene(inmap_state)
{
	//���݂̃V�[����InMap�ɐݒ�
	curent_scene = SceneState::INMAP;

	//csv�̏�����
	CsvManager::GetCsvManager()->LoadCsv(curent_scene);
}

InMapScene::~InMapScene()
{
	//�}�b�v�`�b�v�̉��(2096 = �}�b�v�`�b�v�̑���)
	for (int i = 0; i < map_chip_all_size_; i++) {
		DeleteGraph(gpc_map_chip_hdls_[i]);
		tnl::DebugTrace("\n Deleted  MapChipimage at [%d]. Pointer value: %p\n", i, gpc_map_chip_hdls_[i]);
	}
}

//������
void InMapScene::InitVillageScene()
{
	//����֘A�̏�����
	weapon_shop = std::make_shared<WeaponShop>(curent_inmap_scene);


	//NPC�̏�����
	if (!herdman_npc) {
		herdman_npc = std::make_shared<Npc>("����", HERDMAN_POS, NPC_CHIP_SIZE, koni::Numeric::SCALE_ONE_AND_HALF, Npc::NPC_ANIMATION::NPC_DIR_LEFT, "graphics/Npc/herdman_icon.png");
	}
	if (!armsdealer_npc) {
		armsdealer_npc = std::make_shared<Npc>("���폤�l", ARMSDEALER_VILLAGE_POS, NPC_CHIP_SIZE, koni::Numeric::SCALE_ONE_AND_HALF, Npc::NPC_ANIMATION::NPC_DIR_DOWN, "graphics/Npc/Armsdealer_icon.png");
	}
	if (!priest_npc) {
		priest_npc = std::make_shared<Npc>("�_��", PRIEST_VILLAGE_POS, NPC_CHIP_SIZE, koni::Numeric::SCALE_ONE_AND_HALF, Npc::NPC_ANIMATION::NPC_DIR_DOWN, "graphics/Npc/Priest_icon.png");
	}
	if (!female_resident_npc) {
		female_resident_npc = std::make_shared<Npc>("�������l", FEMALE_VILLAGERS_POS, NPC_CHIP_SIZE, koni::Numeric::SCALE_ONE_AND_HALF, Npc::NPC_ANIMATION::NPC_DIR_RIGHT, "graphics/Npc/female_villagers_icon.png");
	}
	if (!resident_male_npc) {
		resident_male_npc = std::make_shared<Npc>("�j�����l", MALE_VILLAGERS_POS, NPC_CHIP_SIZE, koni::Numeric::SCALE_ONE_AND_HALF, Npc::NPC_ANIMATION::NPC_DIR_DOWN, "graphics/Npc/male_villagers_icon.png");
	}


	//�A�N�^�[���X�g��NPC��ǉ�����
	npc_list.emplace_back(armsdealer_npc);
	npc_list.emplace_back(herdman_npc);
	npc_list.emplace_back(priest_npc);
	npc_list.emplace_back(female_resident_npc);
	npc_list.emplace_back(resident_male_npc);

	//soundManager�̏�����
	SoundManager::GetSoundManager()->Sound_Play("sound/BGM/tasogare.mp3", DX_PLAYTYPE_LOOP);

	//�}�b�v�`�b�v�̃��[�h
	//���p�̃}�b�v�`�b�v�����[�h����
	mapChip_Loding(CsvManager::GetCsvManager()->GetVillagMapCsv(), CsvManager::GetCsvManager()->GetVillagMapObjectCsv());

}

//�{�X�̏�V�[���̏�����
void InMapScene::InitBossCastleScene()
{
	auto mgr = SceneManager::GetSceneManager();

	//�V�[���}�l�[�W���[�̃V�[���t���O��false�̏ꍇ�̂�
	if (!mgr->GetSceneFlag()) {

		//�{�X�̊ԃV�[������ȍ~�������Ɉ�x�}�b�v�`�b�v����ɂ���
		map_chips.clear();
		map_chip_object.clear();

		//�v���C���[�̈ʒu��ς���
		GameManager::GetGameManager()->GetPlayer()->SetPlayerPosition(BOSS_ROOM_EXIT_POS);

		//�J�����ʒu���ύX����
		GameManager::GetGameManager()->GetCamera()->SavePosition(GameManager::GetGameManager()->GetPlayer()->GetPlayerPos());
		GameManager::GetGameManager()->GetCamera()->SetTargetPos();
	}

	//�e�̔Ԑl
	if (!shadow_npc && EventManager::GetEventManager()->GetEnemyEventFlag_3()) {

		shadow_npc = std::make_shared<Npc>("�e�̔Ԑl", SHADOW_ENEMY_POS, NPC_CHIP_SIZE, koni::Numeric::SCALE_ONE_AND_HALF, Npc::NPC_ANIMATION::NPC_DIR_DOWN, "graphics/Npc/shadow_monster_icon.png");

		//�A�N�^�[���X�g��NPC��ǉ�����
		npc_list.emplace_back(shadow_npc);
	}

	//�}�b�v�`�b�v�̃��[�h
	//�_���W�����p�̃}�b�v�`�b�v�����[�h����
	mapChip_Loding(CsvManager::GetCsvManager()->GetBossCastleFloorCsv(), CsvManager::GetCsvManager()->GetBossCastleObjectCsv());

	//�����̏���ł̉��y
	SoundManager::GetSoundManager()->Sound_Play("sound/BGM/maou_bgm_castle.mp3", DX_PLAYTYPE_LOOP);
}

//�{�X�̊ԃV�[���̏�����
void InMapScene::InitBossRoomScene()
{
	map_chips.clear();
	map_chip_object.clear();

	//�v���C���[�̈ʒu��ς���
	GameManager::GetGameManager()->GetPlayer()->SetPlayerPosition(BOSS_ROOM_FIRS_POS);

	//�J�����ʒu���ύX����
	GameManager::GetGameManager()->GetCamera()->SavePosition(GameManager::GetGameManager()->GetPlayer()->GetPlayerPos());
	GameManager::GetGameManager()->GetCamera()->SetTargetPos();

	//�{�X
	if (!boss_npc) {

		boss_npc = std::make_shared<Npc>("����", LAST_BOS_POS, NPC_CHIP_SIZE, koni::Numeric::SCALE_ONE_AND_HALF, Npc::NPC_ANIMATION::NPC_DIR_DOWN, "graphics/Npc/maou_icon.png");
	}

	//�A�N�^�[���X�g��NPC��ǉ�����
	npc_list.emplace_back(boss_npc);

	//�}�b�v�`�b�v�̃��[�h
	//�_���W�����p�̃}�b�v�`�b�v�����[�h����
	mapChip_Loding(CsvManager::GetCsvManager()->GetBossRoomFloorCsv() , CsvManager::GetCsvManager()->GetBossRoomObjectCsv());

}

//���V�[���̏�����
void InMapScene::InitTownScene()
{
	//����֘A�̏�����
	weapon_shop = std::make_shared<WeaponShop>(curent_inmap_scene);

	//---npc�̏�����---//

	//���폤�l
	if (!armsdealer_npc) {

		armsdealer_npc = std::make_shared<Npc>("���폤�l", ARMSDEALER_TOWN_POS, NPC_CHIP_SIZE, koni::Numeric::SCALE_ONE_AND_HALF, Npc::NPC_ANIMATION::NPC_DIR_DOWN, "graphics/Npc/Armsdealer_man_icon_.png");

	}
	//����
	if (!herdman_npc) {

		herdman_npc = std::make_shared<Npc>("����", TOWN_HERDMAN_POS, NPC_CHIP_SIZE, koni::Numeric::SCALE_ONE_AND_HALF, Npc::NPC_ANIMATION::NPC_DIR_UP, "graphics/Npc/town_herdman_icon.png");
	}

	//�h��
	if (!inn_npc) {

		//�h��
		inn_npc = std::make_shared<Npc>("�h��", INN_TOWN_POS, NPC_CHIP_SIZE, koni::Numeric::SCALE_ONE_AND_HALF, Npc::NPC_ANIMATION::NPC_DIR_DOWN, "graphics/Npc/inn_icon.png");
	}

	//�G
	//�C�x���g�t���O��true�������琶��������
	if (EventManager::GetEventManager()->GetEnemyEventFlag_1()&& !enemy_npc) {

		enemy_npc = std::make_shared<Npc>("����", ENEMY_POS , NPC_CHIP_SIZE, koni::Numeric::SCALE_ONE_AND_HALF, Npc::NPC_ANIMATION::NPC_DIR_UP, "graphics/Npc/enemy_1_icon.png");

		npc_list.emplace_back(enemy_npc);
	}

	//�j������
	if (!resident_male_npc) {

		//�j������
		resident_male_npc = std::make_shared<Npc>("�j������", TOWNS_PEOPLE_POS, NPC_CHIP_SIZE, koni::Numeric::SCALE_ONE_AND_HALF, Npc::NPC_ANIMATION::NPC_DIR_DOWN, "graphics/Npc/male_villagers_icon.png");
	}


	//�A�N�^�[���X�g��NPC��ǉ�����
	npc_list.emplace_back(armsdealer_npc);
	npc_list.emplace_back(herdman_npc);
	npc_list.emplace_back(inn_npc);
	npc_list.emplace_back(resident_male_npc);


	//soundManager�̏�����
	//����BGM�𗬂�
	SoundManager::GetSoundManager()->Sound_Play("sound/BGM/town_bgm.mp3", DX_PLAYTYPE_LOOP);

	//�}�b�v�`�b�v�̃��[�h
	mapChip_Loding(CsvManager::GetCsvManager()->GetTownMapCsv(), CsvManager::GetCsvManager()->GetTownObjectMapCsv());
}

//�鉺���V�[���̏�����
void InMapScene::InitCastleTownScene()
{
	auto mgr = SceneManager::GetSceneManager();

	//�V�[���}�l�[�W���[�̃V�[���t���O��false�̏ꍇ�̂�
	if(!mgr->GetSceneFlag()){

		//��V�[������ȍ~�������Ɉ�x�}�b�v�`�b�v����ɂ���
		map_chips.clear();
		map_chip_object.clear();

		//�j���Z�l�̃|�C���^���������
		//�����|�C���^�ňႤ�Z�l����Ə鉺���ŏo��������׈�x�������
		if (resident_male_npc) { resident_male_npc.reset(); }

		//�v���C���[�̈ʒu��ς���
		GameManager::GetGameManager()->GetPlayer()->SetPlayerPosition(CASTLE_EXIT_POS);

		//�J�����ʒu���ύX����
		GameManager::GetGameManager()->GetCamera()->SavePosition(GameManager::GetGameManager()->GetPlayer()->GetPlayerPos());
		GameManager::GetGameManager()->GetCamera()->SetTargetPos();
	}

	//����֘A�̏�����
	weapon_shop = std::make_shared<WeaponShop>(curent_inmap_scene);

	//���폤�l
	if (!armsdealer_npc) {

		//�|�C���^�̐���
		armsdealer_npc = std::make_shared<Npc>("���폤�l", ARMSDEALER_CASTLE_TOWN_POS, NPC_CHIP_SIZE, koni::Numeric::SCALE_ONE_AND_HALF, Npc::NPC_ANIMATION::NPC_DIR_DOWN,  "graphics/Npc/Armsdealer_icon.png");

	}
	//�h��
	if (!inn_npc) {

		//�h��
		inn_npc = std::make_shared<Npc>("�h��", INN_CASTLE_TOWN_POS, NPC_CHIP_SIZE, koni::Numeric::SCALE_ONE_AND_HALF, Npc::NPC_ANIMATION::NPC_DIR_DOWN, "graphics/Npc/inn_icon.png");
	}
	//�_��
	if (!priest_npc) {

		priest_npc = std::make_shared<Npc>("�_��", PRIEST_CASTLE_TOWN_POS, NPC_CHIP_SIZE, koni::Numeric::SCALE_ONE_AND_HALF, Npc::NPC_ANIMATION::NPC_DIR_DOWN, "graphics/Npc/Priest_icon.png");

	}
	//�j���Z��
	if (!resident_male_npc) {

		//�j���Z��
		resident_male_npc = std::make_shared<Npc>("�j���Z��", MALE_RESIDENT_POS, NPC_CHIP_SIZE, koni::Numeric::SCALE_ONE_AND_HALF, Npc::NPC_ANIMATION::NPC_DIR_RIGHT,"graphics/Npc/male_resident_icon.png");
	}

	//�A�N�^�[���X�g��NPC��ǉ�����
	npc_list.emplace_back(armsdealer_npc);
	npc_list.emplace_back(inn_npc);
	npc_list.emplace_back(priest_npc);
	npc_list.emplace_back(resident_male_npc);

	//soundManager�̏�����
	//�鉺����BGM�𗬂�
	SoundManager::GetSoundManager()->Sound_Play("sound/BGM/castle_town_bgm.mp3", DX_PLAYTYPE_LOOP);

	//�}�b�v�`�b�v�̃��[�h
	mapChip_Loding(CsvManager::GetCsvManager()->GetCastleTownMapCsv(), CsvManager::GetCsvManager()->GetCastleTownObjectMapCsv());
}

//��V�[���̏�����
void InMapScene::InitCastleScene()
{
	map_chips.clear();
	map_chip_object.clear();

	//�j���Z�l�̃|�C���^���������
	//�����|�C���^�ňႤ�Z�l����Ə鉺���ŏo��������׈�x�������
	if (resident_male_npc) { resident_male_npc.reset(); }

	//�v���C���[�̈ʒu��ς���
	GameManager::GetGameManager()->GetPlayer()->SetPlayerPosition(CASTLE_FIRST_POS);

	//�J�����ʒu���ύX����
	GameManager::GetGameManager()->GetCamera()->SavePosition(GameManager::GetGameManager()->GetPlayer()->GetPlayerPos());
	GameManager::GetGameManager()->GetCamera()->SetTargetPos();

	//��
	if (!king_npc) {
		king_npc = std::make_shared<Npc>("��", KING_POS, NPC_CHIP_SIZE, koni::Numeric::SCALE_ONE_AND_HALF, Npc::NPC_ANIMATION::NPC_DIR_DOWN, "graphics/Npc/king_icon.png");
	}
	//����
	if (!queen_npc) {
		queen_npc = std::make_shared<Npc>("����", QUEEN_POS, NPC_CHIP_SIZE, koni::Numeric::SCALE_ONE_AND_HALF, Npc::NPC_ANIMATION::NPC_DIR_DOWN, "graphics/Npc/queen_icon.png");
	}
	//�w��
	if (!resident_male_npc) {
		resident_male_npc = std::make_shared<Npc>("�w��", SCHOLAR_POS, NPC_CHIP_SIZE, koni::Numeric::SCALE_ONE_AND_HALF, Npc::NPC_ANIMATION::NPC_DIR_UP, "graphics/Npc/scholar_icon.png");
	}
	//���m
	if (!soldier_npc) {
		soldier_npc = std::make_shared<Npc>("���m", SOLDIER_POS, NPC_CHIP_SIZE, koni::Numeric::SCALE_ONE_AND_HALF, Npc::NPC_ANIMATION::NPC_DIR_DOWN, "graphics/Npc/soldier_icon.png");
	}
	if (!second_soldier_npc) {
		second_soldier_npc = std::make_shared<Npc>("���m", SECOND_SOLDIER_POS, NPC_CHIP_SIZE, koni::Numeric::SCALE_ONE_AND_HALF, Npc::NPC_ANIMATION::NPC_DIR_DOWN, "graphics/Npc/soldier_icon.png");
	}

	//�A�N�^�[���X�g��NPC��ǉ�����
	npc_list.emplace_back(king_npc);
	npc_list.emplace_back(queen_npc);
	npc_list.emplace_back(resident_male_npc);
	npc_list.emplace_back(soldier_npc);
	npc_list.emplace_back(second_soldier_npc);


	//soundManager�̏�����
	//���BGM�𗬂�
	SoundManager::GetSoundManager()->Sound_Play("sound/BGM/castle.mp3", DX_PLAYTYPE_LOOP);

	//�}�b�v�`�b�v�̃��[�h
	mapChip_Loding(CsvManager::GetCsvManager()->GetCastleMapCsv(), CsvManager::GetCsvManager()->GetCastleObjectMapCsv());
}

//��V�[���̏�����
void InMapScene::InitGraveScene()
{
	//�C�x���g�t���O���o���Ă���΃|�C���^�𐶐�������
	if (EventManager::GetEventManager()->GetEnemyEventFlag_2()) {

		if (!ordeal_npc) {
			//�����̔Ԑl�|�C���^�𐶐�����
			ordeal_npc = std::make_shared<Npc>("�����̔Ԑl", "graphics/Npc/ordeal_icon.png");
		}

		npc_list.emplace_back(ordeal_npc);
	}

	//soundManager�̏�����
	//�E�҂̕��BGM�𗬂�
	SoundManager::GetSoundManager()->Sound_Play("sound/BGM/grave.mp3", DX_PLAYTYPE_LOOP);

	//�}�b�v�`�b�v�̃��[�h
	mapChip_Loding(CsvManager::GetCsvManager()->GetGraveFloorCsv(), CsvManager::GetCsvManager()->GetGraveObjectCsv());

}


void InMapScene::InitInMapScene()
{
	//Player�̏����ʒu�����߂�
	GameManager::GetGameManager()->GetPlayer()->SetPlayerPosition(first_pos);

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
	case InMapState::BOSSCASTLE:

		//�}�b�v�̍����ƃ}�b�v�̕������߂�
		map_height = 945;
		map_width = 1600;

		InitBossCastleScene();

		break;


	//�����̊�
	case InMapScene::InMapState::BOSSROOM:

		//�}�b�v�̍����ƃ}�b�v�̕������߂�
		map_height = 940;
		map_width = 1600;

		//�_���W�����V�[���̏�����
		InitBossRoomScene();

		break;

	//��
	case InMapScene::InMapState::GRAVE:

		//�}�b�v�̍����ƃ}�b�v�̕������߂�
		map_height = 945;
		map_width = 1600;

		//�E�҂̕�V�[���̏�����
		InitGraveScene();

		break;

	default:
		break;
	}

	//Npc�̉摜�̏�����
	//��V�[���ł͉摜�̂���NPC���g��Ȃ��ׁA����ȊO�̃V�[���ł̂�
	if (!npc_list.empty() && curent_inmap_scene != InMapState::GRAVE) {

		for (auto& npc_list : npc_list) {

			//�摜���Z�b�g����
			npc_list->SetNpcImage(npc_list->GetNpcName(),static_cast<int>(curent_inmap_scene));
		}
	}

	//�C�x���g�֘A�̏�����
	EventManager::GetEventManager()->InitEventRelated(npc_list);

}

//---���j���[�̏�����---//
void InMapScene::InitMenuWindow()
{

	//�b���R�}���h
	if (!dia_log_coment) {

		//---���W---//

		//�b��
		const tnl::Vector2i TALK_STRING_POS = {100 , 100};

		//����
		const tnl::Vector2i CLOASE_STRING_POS = { 100 , 150 };

		dia_log_coment = new MenuWindow::MenuElement_t[]{
			{TALK_STRING_POS.x ,   TALK_STRING_POS.y ,   "�b��" ,	MenuWindow::Elements::FIRST_ELEMENT},
			{CLOASE_STRING_POS.x , CLOASE_STRING_POS.y , "����" , MenuWindow::Elements::SECOND_ELEMENT}
		};
	}

	if (!dia_log_coment_select) {

		//�b���R�}���h�̗v�f��
		const int DIA_LOG_COMENT_NUM = 2;

		dia_log_coment_select = std::make_shared<MenuWindow>("graphics/WindowBase_01.png", dia_log_coment, DIA_LOG_COMENT_NUM);
		dia_log_coment_select->Open();
	}

	//���퉮�̍w���R�}���h
	if (!weapon_shop_coment) {

		//---���W---//

		//�w��
		const tnl::Vector2i BUY_STRING_POS = { 100 , 100 };

		//����
		const tnl::Vector2i SELL_STRING_POS = { 100 , 150 };

		//����
		const tnl::Vector2i CLOASE_STRING_POS = { 100 , 200 };

		weapon_shop_coment = new MenuWindow::MenuElement_t[]{
			{BUY_STRING_POS.x ,    BUY_STRING_POS.y ,     "�w������" , MenuWindow::Elements::FIRST_ELEMENT},
			{SELL_STRING_POS.x ,   SELL_STRING_POS.y ,    "����"     , MenuWindow::Elements::SECOND_ELEMENT},
			{CLOASE_STRING_POS.x , CLOASE_STRING_POS.y,   "����"   , MenuWindow::Elements::THERD_ELEMENT}
		};
	}

	if (!weapon_shop_coment_select) {

		//�b���R�}���h�̗v�f��
		const int WEAPON_SHOP_COMENT_NUM = 3;

		weapon_shop_coment_select = std::make_shared<MenuWindow>("graphics/WindowBase_01.png", weapon_shop_coment, WEAPON_SHOP_COMENT_NUM);
		weapon_shop_coment_select->Open();
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
	GameManager::GetGameManager()->GetPlayer()->PlayerAnimation(delta_time);;

	//Inventory
	GameManager::GetGameManager()->GetInventory()->Update(delta_time);

	//npc�̍X�V����
	for (auto& actor : npc_list) {
		actor->Update(delta_time);
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
	for (auto& mapchip : map_chips) {
		mapchip->Draw(*GameManager::GetGameManager()->GetCamera());
	}

	//---mapChip�̕`��---//
	for (auto& mapchip : map_chip_object) {
		mapchip->Draw(*GameManager::GetGameManager()->GetCamera());
	}

	//---Npc�̕`��---//
	for (auto& actor : npc_list) {
		actor->Draw(*GameManager::GetGameManager()->GetCamera());
	}

	//---plyer�̕`��---//
	GameManager::GetGameManager()->GetPlayer()->Player_draw(*GameManager::GetGameManager()->GetCamera(), koni::Numeric::SCALE_ONE_AND_HALF);

	//�C���x���g���̕`��
	GameManager::GetGameManager()->GetInventory()->draw();

	//���ꂼ��̃V�[�P���X�ł̕`��
	switchSequenceDraw();

	/*DrawStringEx(100, 100, koni::Color::WHITE, "%f", GameManager::getGameManager()->getPlayer()->getPlayerPos().x);
	DrawStringEx(100, 150, -1, "%f", GameManager::getGameManager()->getPlayer()->getPlayerPos().y);*/

}

//------------------------------------------------------------------------------------------------------------------------
//�V�[�P���X�֘A

//�V�[�P���X��؂�ւ���
void InMapScene::SequenceChangeMove()
{
	//��ɃC�x���g�}�l�[�W���[�ȂǂŃC�x���g��؂�ւ��鎞�Ɏg�p
	//�ʏ�ɖ߂�
	sequence_.change(&InMapScene::seqMove);
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

//�}�b�v�O�ɏo���ۂ̏���
void InMapScene::InMapSceneExitProcess(const float delta_time)
{
	//�v���C���[�̍��W���}�b�v�O�ɏo����V�[����؂�ւ���
	//���V�[���̂ݕ����Ȃ��ׁA��ɂ����Ȃ��悤�ɂ���
	if (curent_inmap_scene == InMapState::VILLAGE) {

		//�v���C���[�̃T�C�Y
		auto player_size = GameManager::GetGameManager()->GetPlayer()->GetPlayerSize();

		//�v���C���[�̈ړ�����
		if (GameManager::GetGameManager()->GetPlayer()->GetPlayerPos().y < static_cast<float>(player_size.y / 2)) {

			tnl::Vector3 player_pos = { GameManager::GetGameManager()->GetPlayer()->GetPlayerPos().x ,static_cast<float>(player_size.y / koni::Numeric::DIVIDER_TWO) , 0 };

			GameManager::GetGameManager()->GetPlayer()->SetPlayerPosition(player_pos);
		}
	}
	//�鉺���V�[���̏ꍇ��ɍs�������Ɉړ�������
	//�܂��̓{�X�̏�̓��V�[���̏ꍇ��ɍs�������ɑJ�ڂ�����
	else if (curent_inmap_scene == InMapState::CASTLETOWN && GameManager::GetGameManager()->GetPlayer()->GetPlayerPos().y < 0
			|| curent_inmap_scene == InMapState::BOSSCASTLE && GameManager::GetGameManager()->GetPlayer()->GetPlayerPos().y < 0) {

		//�鉺���̏ꍇ��Ɉړ�������
		if (curent_inmap_scene == InMapState::CASTLETOWN)
		{
			curent_inmap_scene = InMapState::CASTLE;

			//�T�E���h���~�߂�
			SoundManager::GetSoundManager()->StopSound("sound/BGM/castle_town_bgm.mp3");
		}
		//�{�X�̏�̏ꍇ�A�{�X�̊ԂɈړ�������
		else {
			curent_inmap_scene = InMapState::BOSSROOM;
		}

		//�����������ď�V�[���Ɉڍs������
		sequence_.change(&InMapScene::seqInit);

		SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/idou.mp3", DX_PLAYTYPE_BACK);

		auto mgr = SceneManager::GetSceneManager();

		//�t���O��؂�ւ��ăV�[����J�ڂ����Ȃ��悤�ɂ���
		if (mgr->GetSceneFlag()) { mgr->SceneFlagChange(); }

		//�t�F�[�h������
		mgr->FadeScene(FADE_OUT_TIME, FADE_IN_TIME);

		//NPC����x����������
		npc_list.clear();

		return ;

	}
	//��V�[���̏ꍇ�A�}�b�v�O�ɏo����鉺���ɑJ�ڂ�����
	//�܂��̓{�X�̏�̃{�X�̊Ԃ�������K�i���Ɉړ�������
	else if (GameManager::GetGameManager()->GetPlayer()->GetPlayerPos().y >= map_height && curent_inmap_scene == InMapState::CASTLE
			|| GameManager::GetGameManager()->GetPlayer()->GetPlayerPos().y >= map_height && curent_inmap_scene == InMapState::BOSSROOM)
	{
		//��̏ꍇ�A�鉺���ɑJ�ڂ�����
		if (curent_inmap_scene == InMapState::CASTLE) {
			curent_inmap_scene = InMapState::CASTLETOWN;
		}
		//�{�X�̊Ԃ̏ꍇ�A�{�X�̏�̓��Ɉړ�������(�K�i��)
		else {
			curent_inmap_scene = InMapState::BOSSCASTLE;
		}
			
		SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/idou.mp3", DX_PLAYTYPE_BACK);

		auto mgr = SceneManager::GetSceneManager();

		//�t���O��؂�ւ��ăV�[����J�ڂ����Ȃ��悤�ɂ���
		if (mgr->GetSceneFlag()) { mgr->SceneFlagChange(); }

		//�t�F�[�h������
		mgr->FadeScene(FADE_OUT_TIME, FADE_IN_TIME);

		//�T�E���h���~�߂�
		SoundManager::GetSoundManager()->StopSound("sound/BGM/castle.mp3");

		//NPC����x����������
		npc_list.clear();

		//�����������ď�V�[���Ɉڍs������
		sequence_.change(&InMapScene::seqInit);

		return ;
	}
	if (GameManager::GetGameManager()->GetPlayer()->GetPlayerPos().x >= map_width || GameManager::GetGameManager()->GetPlayer()->GetPlayerPos().x < 0 || GameManager::GetGameManager()->GetPlayer()->GetPlayerPos().y >= map_height)
	{
		TimeCount = true;

		//���̏ꍇ
		if (curent_inmap_scene == InMapState::VILLAGE) {
			//���y��Stop����
			SoundManager::GetSoundManager()->StopSound("sound/BGM/tasogare.mp3");
		}
		//���̏ꍇ
		else if (curent_inmap_scene == InMapState::TOWN) {
			//���y��Stop����
			SoundManager::GetSoundManager()->StopSound("sound/BGM/town_bgm.mp3");
		}
		//�����̏�̏ꍇ
		else if (curent_inmap_scene == InMapState::BOSSCASTLE) {
			//���y��Stop����
			SoundManager::GetSoundManager()->StopSound("sound/BGM/maou_bgm_castle.mp3");
		}
		//�鉺���̏ꍇ
		else if (curent_inmap_scene == InMapState::CASTLETOWN) {
			//���y��Stop����
			SoundManager::GetSoundManager()->StopSound("sound/BGM/castle_town_bgm.mp3");
		}
		//�E�҂̕�̏ꍇ
		else if (curent_inmap_scene == InMapState::GRAVE) {
			SoundManager::GetSoundManager()->StopSound("sound/BGM/grave.mp3");
		}

		//�ړ���SE�𗬂�
		SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/idou.mp3", DX_PLAYTYPE_BACK);
	}

	//�؂�ւ���ۂ�SE�𗬂��؂��Ă���J�ڂ���
	if (TimeCount) {

		//0.2�bSE�𗬂��Ă���J�ڂ�����
		if (GameManager::GetGameManager()->TimeCount(delta_time, TimeSE)) {

			//�v���C���[���ꎞ�I�Ɍ����Ȃ�����
			GameManager::GetGameManager()->GetPlayer()->PlayerDisplayChange();

			sequence_.change(&InMapScene::seqChangeScene);

			//�V�[���}�l�[�W���[�̃|�C���^���擾����
			auto mgr = SceneManager::GetSceneManager();

			//�t���O��false�̏ꍇtrue�ɕύX����(�V�[����J�ڂ������)
			if (!mgr->GetSceneFlag()) { mgr->SceneFlagChange(); }

			//�V�[����J�ڂ�����
			mgr->changeScene(new MapScene());
		}
	}
}

bool InMapScene::seqMove(float delta_time)
{
	if (sequence_.isStart()) {

		if (!GameManager::GetGameManager()->GetPlayer()->GetPlayerControl()) {
			GameManager::GetGameManager()->GetPlayer()->PlayerControlChangeFlag();
		}

		select_num = Sequence_num::MOVE;
	}

	//�v���C���[�̓���
	GameManager::GetGameManager()->GetPlayer()->Player_Move(delta_time, PLAYER_VELOCITY, map_height);

	//�J�����̍X�V����
	GameManager::GetGameManager()->GetCamera()->update(GameManager::GetGameManager()->GetPlayer()->GetPlayerPos(), map_width, map_height);

	if (GameManager::GetGameManager()->GetInventory()->GetSelectMenuNum() == Inventory::MenuWindow_I::EMPTY && tnl::Input::IsKeyDownTrigger(eKeys::KB_ESCAPE)) {

		//���艹��炷
		SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
		//�C���x���g����\������
		GameManager::GetGameManager()->GetInventory()->InventoryMenuChange(Inventory::MenuWindow_I::FIRSTMENU);
		//player�𓮂��Ȃ��悤�ɂ���
		GameManager::GetGameManager()->GetPlayer()->PlayerControlChangeFlag();
	}

	//�����蔻��
	//�C���x���g�����J���Ă��鎞�͓����蔻�������
	if (GameManager::GetGameManager()->GetInventory()->GetSelectMenuNum() == Inventory::MenuWindow_I::EMPTY) {
		mapChip_collision();
	}

	//�}�b�v�O�ɏo���Ƃ��̏���
	InMapSceneExitProcess(delta_time);
	
	
	return false;
}

//���퉮
bool InMapScene::seqWeaponShop(float delta_time)
{
	if (sequence_.isStart()) {

		select_num = Sequence_num::WEAPONSHOP;
		
		//����R�����g������������
		//�R�����g�����Ԃ�Ȃ��悤�ɂ����
		weapon_shop->BuyComentClear();

		//Npc�̃R�����g��؂�ւ���
		EventManager::GetEventManager()->SetNpcComent(EventManager::NpcMove::WEAPONSHOP);
	}

	//���퉮�̏���
	weapon_shop->Update(delta_time);
	
	//0����������V�[�P���X��ς���
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_BACK)) {
		//�V�[����؂�ւ���
		sequence_.change(&InMapScene::seqMove);
		//���퉮�V�[�P���X��ҋ@��Ԃɖ߂�
		weapon_shop->WeaponShopChangeIdle();
	}

	return false;
}

//�V�[����ς��鎞�̃t�F�[�h�A�E�g������׃V�[�P���X
//�V�[���ɂ���č��W���؂�ւ���
bool InMapScene::seqChangeScene(float delta_time)
{
	//��
	if (curent_inmap_scene == InMapState::VILLAGE) {
		GameManager::GetGameManager()->GetPlayer()->SetPlayerPosition(FIRST_WORLD_POS);
	}
	//�{�X�̏�
	else if (curent_inmap_scene == InMapState::BOSSCASTLE) {
		GameManager::GetGameManager()->GetPlayer()->SetPlayerPosition(BOSS_EXIT_POS);
	}
	//��
	else if (curent_inmap_scene == InMapState::TOWN) {
		GameManager::GetGameManager()->GetPlayer()->SetPlayerPosition(TOWN_EXIT_POS);
	}
	//�鉺��
	else if (curent_inmap_scene == InMapState::CASTLETOWN) {
		GameManager::GetGameManager()->GetPlayer()->SetPlayerPosition(CASTLE_TOWN_EXIT_POS);
	}
	//�E�҂̕�
	else if (curent_inmap_scene == InMapState::GRAVE) {
		GameManager::GetGameManager()->GetPlayer()->SetPlayerPosition(GRAVE_EXIT_POS);
	}

	return false;
}

//���j���[
bool InMapScene::seqMenuOpen(float delta_time)
{
	if (sequence_.isStart())
	{
		//�v���C���[�𓮂��Ȃ�����
		GameManager::GetGameManager()->GetPlayer()->PlayerControlChangeFlag();
		select_num = Sequence_num::MENUOPEN;
	}

	//����̍w���R�}���h�������������I�������ꍇ�A���퉮�V�[�P���X�Ɉڍs����
	//������I�������ꍇ
	if (EventManager::GetEventManager()->GetSelectMenu() == EventManager::MenuOpen::WEAPONSHOP && weapon_shop_coment_select->getSelectNum() == MenuWindow::Elements::FIRST_ELEMENT
		&& tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

		//�V�[�P���X�𕐊퉮�V�[�P���X�ɐ؂�ւ���
		sequence_.change(&InMapScene::seqWeaponShop);

		//���퉮�̏�Ԃ�؂�ւ���
		weapon_shop->SetWeaponShopTransaction(WeaponShop::TransactionType::BUY);

		return false;
	}
	//�����I�������ꍇ
	else if (EventManager::GetEventManager()->GetSelectMenu() == EventManager::MenuOpen::WEAPONSHOP && weapon_shop_coment_select->getSelectNum() == MenuWindow::Elements::SECOND_ELEMENT
		&& tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

		//�V�[�P���X�𕐊퉮�V�[�P���X�ɐ؂�ւ���
		sequence_.change(&InMapScene::seqWeaponShop);

		//���퉮�̏�Ԃ�؂�ւ���
		weapon_shop->SetWeaponShopTransaction(WeaponShop::TransactionType::SELL);

		return false;
	}
	//������������瓮����悤�ɂ���
	else if (EventManager::GetEventManager()->GetSelectMenu() == EventManager::MenuOpen::WEAPONSHOP && weapon_shop_coment_select->getSelectNum() == MenuWindow::Elements::THERD_ELEMENT) {

		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

			sequence_.change(&InMapScene::seqMove);

			return false;
		}
	}

	//�b���R�}���h
	if (EventManager::GetEventManager()->GetSelectMenu() == EventManager::MenuOpen::EMPTY && dia_log_coment_select->getSelectNum() == 0 || EventManager::GetEventManager()->GetSelectMenu() == EventManager::MenuOpen::BOSS
		&& dia_log_coment_select->getSelectNum() == MenuWindow::Elements::FIRST_ELEMENT) {
		if(tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)){
			sequence_.change(&InMapScene::seqNpc);
		}
	}
	else if (dia_log_coment_select->getSelectNum() == MenuWindow::Elements::SECOND_ELEMENT && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

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
		UIManager::GetUIManager()->FrameReset();
	}

	//�r���ŉ�b�𒆒f����ׂ̃L�[
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_BACK)) {
		sequence_.change(&InMapScene::seqMove);
	}

	//�C�x���g��������Npc�Ƃ̍X�V����
	EventManager::GetEventManager()->NpcEventUpdate(delta_time, npc_list , this , static_cast<int>(curent_inmap_scene));

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
		
		if (EventManager::GetEventManager()->GetSelectMenu() == EventManager::MenuOpen::WEAPONSHOP) {

			weapon_shop_coment_select->All(DIALOG_POS.x, DIALOG_POS.y, WEAPON_SHOP_WINDOW_SIZE.x, WEAPON_SHOP_WINDOW_SIZE.y);
		}
		else {
			//�b���R�����g
			dia_log_coment_select->All(DIALOG_POS.x, DIALOG_POS.y, DIALOG_SIZE.x, DIALOG_SIZE.y);

		}
	
		break;

		//Npc�V�[�P���X�̎�(Npc�ɂ���ē����ς���)
	case InMapScene::Sequence_num::MENUNPC:

		EventManager::GetEventManager()->NpcSwitchComent(weapon_shop);

		break;

	case InMapScene::Sequence_num::WEAPONSHOP:

		EventManager::GetEventManager()->NpcSwitchComent(weapon_shop);

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
			pos.x = { static_cast<float>(mapchip->GetChipSize().x / koni::Numeric::DIVIDER_TWO + k * mapchip->GetChipSize().x)};
			pos.y = { static_cast<float>(i * mapchip->GetChipSize().x)};
			gpc_hdl = gpc_map_chip_hdls_[map_csv_[i][k]];

			map_chips.emplace_back(std::make_shared<MapChip>(pos, gpc_hdl));

		}
	}

	//�I�u�W�F�N�g�̃}�b�v�`�b�v�̓ǂݍ���
	for (int i = 0; i < map_csv_objct_.size(); ++i) {
		for (int k = 0; k < map_csv_objct_[i].size(); ++k) {
			if (-1 == map_csv_objct_[i][k])continue;
			tnl::Vector3 pos;
			int gpc_hdl;
			pos.x = { static_cast<float>(mapchip->GetChipSize().x / koni::Numeric::DIVIDER_TWO + k * mapchip->GetChipSize().x) };
			pos.y = { static_cast<float>(i * mapchip->GetChipSize().x) };
			gpc_hdl = gpc_map_chip_hdls_[map_csv_objct_[i][k]];

			map_chip_object.emplace_back(std::make_shared<MapChip>(pos, gpc_hdl));
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
		BossCastleCollision();
	}
	//���݂̃V�[�����{�X�̊Ԃ�������
	else if (curent_inmap_scene == InMapState::BOSSROOM) {
		//�{�X�̏�V�[���̓����蔻��
		BossRoomCollision();
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
	//���݂̃V�[�����E�҂̕�V�[����������
	else if (curent_inmap_scene == InMapState::GRAVE) {
		GraveSceneCollision();
	}
}

//���V�[���ł̓����蔻��
void InMapScene::VillageCollision()
{
	//�v���C���[���擾����
	auto& player = GameManager::GetGameManager()->GetPlayer();

	//�u���b�N�ƏՓ˔��� & �␳
	//�Q�[�����[�v���ł̓����蔻��̃`�F�b�N]
	for (auto& mapChip : map_chip_object) {

		int mapChipValue = map_csv_objct_[static_cast<unsigned>(mapChip->MapChipPos().y / mapChip->GetChipSize().y)][static_cast<unsigned>(mapChip->MapChipPos().x / mapChip->GetChipSize().x)];


		//�����蔻��
		if (tnl::IsIntersectRect(player->GetPlayerPos(), player->GetPlayerSize().x, player->GetPlayerSize().y, mapChip->MapChipPos(), map_chip_width_, map_chip_height_)) {

			//���W�␳
			tnl::eCorrResRect collision = tnl::CorrectPositionRect(
				player->GetPlayerPrevPos(),
				mapChip->MapChipPos(),
				player->GetPlayerSize(),
				mapChip->GetChipSize(),
				player->GetPlayerPos(),
				mapChip->MapChipPos(),
				tnl::eCorrTypeRect::PWRFL_B,
				tnl::eCorrTypeRect::PWRFL_B , 0);

			//���퉮
			const int MAP_CHIP_WEAPONSHOP = 510;

			//���퉮�Ƙb�����ꍇ
			if (mapChipValue == MAP_CHIP_WEAPONSHOP) {

				if (select_num == Sequence_num::MOVE && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

					//�V�[�P���X��؂�ւ���
					EventManager::GetEventManager()->SetMenuNpc(EventManager::MenuOpen::WEAPONSHOP);
					EventManager::GetEventManager()->SetNpcComent(EventManager::NpcMove::WEAPONSHOP);
					sequence_.change(&InMapScene::seqMenuOpen);

				}
			}
		}
	}


	//---Npc�̓����蔻��---//

	for (auto& npc_ : npc_list) {

		if (tnl::IsIntersectRect(player->GetPlayerPos(), player->GetPlayerSize().x, player->GetPlayerSize().y, npc_->GetNpcPos(), npc_->GetNpcSizeWidth(), npc_->GetNpcSizeHegiht())) {

			//���W�␳
			tnl::eCorrResRect collison = tnl::CorrectPositionRect(
				player->GetPlayerPrevPos(),
				npc_->GetNpcPos(),
				player->GetPlayerSize(),
				npc_->GetNpcCharaChip(),
				player->GetPlayerPos(),
				npc_->GetNpcPos(),
				tnl::eCorrTypeRect::PWRFL_B,
				tnl::eCorrTypeRect::PWRFL_B, 0);

			//����
			if (collison != tnl::eCorrResRect::LEFT && npc_->GetNpcName() == "����") {

				if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

					//�A�j���[�V���������ɂ���
					herdman_npc->SetNpcAnimation(static_cast<Npc::NPC_ANIMATION>(collison));

					//1����R�����g�𗬂�
					UIManager::GetUIManager()->ComentClear();

					//�J�E���g�����Z�b�g����
					UIManager::GetUIManager()->CountReset();

					//���[�h����R�����g�̐�
					const int HERDMAN_COMENT_MAX = 9;

					//�R�����g�p�̔z�������������
					UIManager::GetUIManager()->ComentLoad(HERDMAN_COMENT_MAX, herdman_npc->GetNpcName());

					//��Ԃ�؂�ւ���
					EventManager::GetEventManager()->SetMenuNpc(EventManager::MenuOpen::EMPTY);
					EventManager::GetEventManager()->SetNpcComent(EventManager::NpcMove::HERDMAN);

					//�V�[�P���X��؂�ւ���
					sequence_.change(&InMapScene::seqMenuOpen);
				}
			}

			//�_��
			else if (collison != tnl::eCorrResRect::UP && npc_->GetNpcName() == "�_��") {

				if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

					//�A�j���[�V������؂�ւ���
					priest_npc->SetNpcAnimation(static_cast<Npc::NPC_ANIMATION>(collison));

					//1����R�����g�𗬂�
					UIManager::GetUIManager()->ComentClear();

					//�J�E���g�����Z�b�g����
					UIManager::GetUIManager()->CountReset();

					//�C�x���g�t���O��false�̏ꍇ�t���O��؂�ւ���
					if (!EventManager::GetEventManager()->GetEventFlag()) {
						EventManager::GetEventManager()->EventFlagChange();
					}

					//���[�h����R�����g�̐�
					const int PRIEST_COMENT_MAX = 3;

					//�R�����g�p�̔z�������������
					UIManager::GetUIManager()->ComentLoad(PRIEST_COMENT_MAX, priest_npc->GetNpcName());

					//��Ԃ�؂�ւ���
					EventManager::GetEventManager()->SetMenuNpc(EventManager::MenuOpen::EMPTY);
					EventManager::GetEventManager()->SetNpcComent(EventManager::NpcMove::PRIEST);
					sequence_.change(&InMapScene::seqMenuOpen);
				}
			}
			//�������l
			else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN) && npc_->GetNpcName() == "�������l") {

				//�A�j���[�V������؂�ւ���
				female_resident_npc->SetNpcAnimation(static_cast<Npc::NPC_ANIMATION>(collison));

				//1����R�����g�𗬂�
				UIManager::GetUIManager()->ComentClear();

				//�J�E���g�����Z�b�g����
				UIManager::GetUIManager()->CountReset();

				//�C�x���g�t���O��false�̏ꍇ�t���O��؂�ւ���
				if (!EventManager::GetEventManager()->GetEventFlag()) {
					EventManager::GetEventManager()->EventFlagChange();
				}

				//���[�h����R�����g�̐�
				const int FEMALE_RESIDENT_COMENT_MAX = 3;

				//�R�����g�p�̔z�������������
				UIManager::GetUIManager()->ComentLoad(FEMALE_RESIDENT_COMENT_MAX, female_resident_npc->GetNpcName());

				//��Ԃ�؂�ւ���
				EventManager::GetEventManager()->SetMenuNpc(EventManager::MenuOpen::EMPTY);
				EventManager::GetEventManager()->SetNpcComent(EventManager::NpcMove::FEMALE_RESIDENT);
				sequence_.change(&InMapScene::seqMenuOpen);
			}
			//�j�����l
			else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN) && npc_->GetNpcName() == "�j�����l") {


				//�A�j���[�V������؂�ւ���
				resident_male_npc->SetNpcAnimation(static_cast<Npc::NPC_ANIMATION>(collison));

				//1����R�����g�𗬂�
				UIManager::GetUIManager()->ComentClear();

				//�J�E���g�����Z�b�g����
				UIManager::GetUIManager()->CountReset();

				//�C�x���g�t���O��false�̏ꍇ�t���O��؂�ւ���
				if (!EventManager::GetEventManager()->GetEventFlag()) {
					EventManager::GetEventManager()->EventFlagChange();
				}

				//���[�h����R�����g�̐�
				const int RESIDENT_MALE_COMENT_MAX = 3;

				//�R�����g�p�̔z�������������
				UIManager::GetUIManager()->ComentLoad(RESIDENT_MALE_COMENT_MAX, resident_male_npc->GetNpcName());

				//��Ԃ�؂�ւ���
				EventManager::GetEventManager()->SetMenuNpc(EventManager::MenuOpen::EMPTY);
				EventManager::GetEventManager()->SetNpcComent(EventManager::NpcMove::RESIDENT_MALE);
				sequence_.change(&InMapScene::seqMenuOpen);
			}
		}
	}
}
//���V�[���ł̓����蔻��
void InMapScene::TownCollision()
{

	auto& player = GameManager::GetGameManager()->GetPlayer();

	//�}�b�v�`�b�v�̓����蔻��
	for (auto& map_chip_collision : map_chip_object) {

		int mapChipValue = map_csv_objct_[static_cast<unsigned>(map_chip_collision->MapChipPos().y / map_chip_collision->GetChipSize().x)][static_cast<unsigned>(map_chip_collision->MapChipPos().x / map_chip_collision->GetChipSize().x)];

		//�����蔻��
		if (tnl::IsIntersectRect(player->GetPlayerPos(), player->GetPlayerSize().x, player->GetPlayerSize().y, map_chip_collision->MapChipPos(), map_chip_width_, map_chip_height_)) {

			//���W�␳
			tnl::eCorrResRect n = tnl::CorrectPositionRect(
				player->GetPlayerPrevPos(),
				map_chip_collision->MapChipPos(),
				player->GetPlayerSize(),
				map_chip_collision->GetChipSize(),
				player->GetPlayerPos(),
				map_chip_collision->MapChipPos(),
				tnl::eCorrTypeRect::PWRFL_B,
				tnl::eCorrTypeRect::PWRFL_B, 0);


			//���퉮�̃}�b�v�`�b�v
			const int TOWN_MAPCHIP_WEAPON_SHOP = 708;

			//�h���̃}�b�v�`�b�v
			const int TOWN_MAPCHIP_INN = 673;

			//���퉮�Ƙb�����ꍇ
			if (mapChipValue == TOWN_MAPCHIP_WEAPON_SHOP && select_num == Sequence_num::MOVE && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

				//�V�[�P���X��؂�ւ���
				EventManager::GetEventManager()->SetMenuNpc(EventManager::MenuOpen::WEAPONSHOP);
				EventManager::GetEventManager()->SetNpcComent(EventManager::NpcMove::WEAPONSHOP);
				sequence_.change(&InMapScene::seqMenuOpen);
			}

			//�h���ɘb��������
			else if (mapChipValue == TOWN_MAPCHIP_INN && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

				//��Ԃ�؂�ւ���
				EventManager::GetEventManager()->SetMenuNpc(EventManager::MenuOpen::EMPTY);
				EventManager::GetEventManager()->SetNpcComent(EventManager::NpcMove::INN);

				//1����R�����g�𗬂�
				UIManager::GetUIManager()->ComentClear();

				//�J�E���g�����Z�b�g����
				UIManager::GetUIManager()->CountReset();

				//�C�x���g�t���O��false�̏ꍇ�t���O��؂�ւ���
				if (!EventManager::GetEventManager()->GetEventFlag()) {
					EventManager::GetEventManager()->EventFlagChange();
				}

				//�R�����g�p�̔z�������������
				UIManager::GetUIManager()->ComentLoad(3, inn_npc->GetNpcName());

				sequence_.change(&InMapScene::seqMenuOpen);

			}

		}
	}

	//npc�̓����蔻��
	for (auto& npc_ : npc_list) {

		//�����蔻��
		if (tnl::IsIntersectRect(player->GetPlayerPos(), player->GetPlayerSize().x, player->GetPlayerSize().y, npc_->GetNpcPos(), npc_->GetNpcSizeWidth(), npc_->GetNpcSizeHegiht())) {

			//���W�␳
			tnl::eCorrResRect collison = tnl::CorrectPositionRect(
				player->GetPlayerPrevPos(),
				npc_->GetNpcPos(),
				player->GetPlayerSize(),
				npc_->GetNpcCharaChip(),
				player->GetPlayerPos(),
				npc_->GetNpcPos(),
				tnl::eCorrTypeRect::PWRFL_B,
				tnl::eCorrTypeRect::PWRFL_B, 0);

			//����
			if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN) && npc_->GetNpcName() == "����") {

				//�A�j���[�V���������ɂ���
				herdman_npc->SetNpcAnimation(static_cast<Npc::NPC_ANIMATION>(collison));

				//1����R�����g�𗬂�
				UIManager::GetUIManager()->ComentClear();

				//�J�E���g�����Z�b�g����
				UIManager::GetUIManager()->CountReset();

				//�C�x���g�t���O��false�̏ꍇ�t���O��؂�ւ���
				if (!EventManager::GetEventManager()->GetEventFlag()) {
					EventManager::GetEventManager()->EventFlagChange();
				}

				//���[�h����R�����g�̐�
				const int HEDMAN_COMENT_MAX = 6;

				//�R�����g�p�̔z�������������
				UIManager::GetUIManager()->ComentLoad(HEDMAN_COMENT_MAX, herdman_npc->GetNpcName());

				//��Ԃ�؂�ւ���
				EventManager::GetEventManager()->SetMenuNpc(EventManager::MenuOpen::EMPTY);
				EventManager::GetEventManager()->SetNpcComent(EventManager::NpcMove::TOWNHERDMAN);
				sequence_.change(&InMapScene::seqMenuOpen);
			}
			//�j�����l
			else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN) && npc_->GetNpcName() == "�j������") {


				//�A�j���[�V������؂�ւ���
				resident_male_npc->SetNpcAnimation(static_cast<Npc::NPC_ANIMATION>(collison));

				//1����R�����g�𗬂�
				UIManager::GetUIManager()->ComentClear();

				//�J�E���g�����Z�b�g����
				UIManager::GetUIManager()->CountReset();

				//�C�x���g�t���O��false�̏ꍇ�t���O��؂�ւ���
				if (!EventManager::GetEventManager()->GetEventFlag()) {
					EventManager::GetEventManager()->EventFlagChange();
				}

				//���[�h����R�����g�̐�
				const int RESIDENT_MALE_COMENT_MAX = 3;

				//�R�����g�p�̔z�������������
				UIManager::GetUIManager()->ComentLoad(RESIDENT_MALE_COMENT_MAX, resident_male_npc->GetNpcName());

				//��Ԃ�؂�ւ���
				EventManager::GetEventManager()->SetMenuNpc(EventManager::MenuOpen::EMPTY);
				EventManager::GetEventManager()->SetNpcComent(EventManager::NpcMove::RESIDENT_MALE);
				sequence_.change(&InMapScene::seqMenuOpen);
			}
			//����
			else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN) && npc_->GetNpcName() == "����"
				&& enemy_npc) {

				//�A�j���[�V������ύX����
				enemy_npc->SetNpcAnimation(static_cast<Npc::NPC_ANIMATION>(collison));

				//1����R�����g�𗬂�
				UIManager::GetUIManager()->ComentClear();

				//�J�E���g�����Z�b�g����
				UIManager::GetUIManager()->CountReset();

				//�C�x���g�t���O��false�̏ꍇ�t���O��؂�ւ���
				if (!EventManager::GetEventManager()->GetEventFlag()) {
					EventManager::GetEventManager()->EventFlagChange();
				}

				//���[�h����R�����g�̐�
				const int ENEMY_COMENT_MAX = 5;

				//�R�����g�p�̔z�������������
				UIManager::GetUIManager()->ComentLoad(ENEMY_COMENT_MAX, enemy_npc->GetNpcName());

				//��Ԃ�؂�ւ���
				EventManager::GetEventManager()->SetMenuNpc(EventManager::MenuOpen::EMPTY);
				EventManager::GetEventManager()->SetNpcComent(EventManager::NpcMove::ENEMY);
				sequence_.change(&InMapScene::seqMenuOpen);
			}
		}
	}	
}

//�{�X�̏�V�[���̓����蔻��
void InMapScene::BossCastleCollision()
{
	auto& player = GameManager::GetGameManager()->GetPlayer();

	//�}�b�v�`�b�v�̓����蔻��
	for (auto& map_chip_collision : map_chip_object) {

		int mapChipValue = map_csv_objct_[static_cast<unsigned>(map_chip_collision->MapChipPos().y / map_chip_collision->GetChipSize().x)][static_cast<unsigned>(map_chip_collision->MapChipPos().x / map_chip_collision->GetChipSize().x)];

		//�����蔻��
		if (tnl::IsIntersectRect(player->GetPlayerPos(), player->GetPlayerSize().x, player->GetPlayerSize().y, map_chip_collision->MapChipPos(), map_chip_width_, map_chip_height_)) {

			//���W�␳
			tnl::eCorrResRect n = tnl::CorrectPositionRect(
				player->GetPlayerPrevPos(),
				map_chip_collision->MapChipPos(),
				player->GetPlayerSize(),
				map_chip_collision->GetChipSize(),
				player->GetPlayerPos(),
				map_chip_collision->MapChipPos(),
				tnl::eCorrTypeRect::PWRFL_B,
				tnl::eCorrTypeRect::PWRFL_B, 0);


		}
	}

	if (!shadow_npc) { return; }
	//npc�̓����蔻��
	//�����蔻��
	if (tnl::IsIntersectRect(player->GetPlayerPos(), player->GetPlayerSize().x, player->GetPlayerSize().y, shadow_npc->GetNpcPos(), shadow_npc->GetNpcSizeWidth(), shadow_npc->GetNpcSizeHegiht())) {

		//���W�␳
		tnl::eCorrResRect shadow_collison = tnl::CorrectPositionRect(
			player->GetPlayerPrevPos(),
			shadow_npc->GetNpcPos(),
			player->GetPlayerSize(),
			shadow_npc->GetNpcCharaChip(),
			player->GetPlayerPos(),
			shadow_npc->GetNpcPos(),
			tnl::eCorrTypeRect::PWRFL_B,
			tnl::eCorrTypeRect::PWRFL_B, 0);

		//�e�̔Ԑl
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN) && shadow_npc) {

			//�A�j���[�V������ύX����
			shadow_npc->SetNpcAnimation(static_cast<Npc::NPC_ANIMATION>(shadow_collison));

			//1����R�����g�𗬂�
			UIManager::GetUIManager()->ComentClear();

			//�J�E���g�����Z�b�g����
			UIManager::GetUIManager()->CountReset();

			//�C�x���g�t���O��false�̏ꍇ�t���O��؂�ւ���
			if (!EventManager::GetEventManager()->GetEventFlag()) {
				EventManager::GetEventManager()->EventFlagChange();
			}

			//���[�h����R�����g�̐�
			const int SHADOW_ENEMY_COMENT_MAX = 4;

			//�R�����g�p�̔z�������������
			UIManager::GetUIManager()->ComentLoad(SHADOW_ENEMY_COMENT_MAX, shadow_npc->GetNpcName());

			//��Ԃ�؂�ւ���
			EventManager::GetEventManager()->SetMenuNpc(EventManager::MenuOpen::EMPTY);
			EventManager::GetEventManager()->SetNpcComent(EventManager::NpcMove::SHADOWENEMY);
			sequence_.change(&InMapScene::seqMenuOpen);
		}
	}
}

//�{�X�̊ԃV�[���ł̓����蔻��
void InMapScene::BossRoomCollision()
{
	auto& player = GameManager::GetGameManager()->GetPlayer();

	//�}�b�v�`�b�v�̓����蔻��
	for (auto& map_chip_collision : map_chip_object) {

		int mapChipValue = map_csv_objct_[static_cast<unsigned>(map_chip_collision->MapChipPos().y / map_chip_collision->GetChipSize().x)][static_cast<unsigned>(map_chip_collision->MapChipPos().x / map_chip_collision->GetChipSize().x)];

		//�����蔻��
		if (tnl::IsIntersectRect(player->GetPlayerPos(), player->GetPlayerSize().x, player->GetPlayerSize().y, map_chip_collision->MapChipPos(), map_chip_width_, map_chip_height_)) {

			//���W�␳
			tnl::eCorrResRect n = tnl::CorrectPositionRect(
				player->GetPlayerPrevPos(),
				map_chip_collision->MapChipPos(),
				player->GetPlayerSize(),
				map_chip_collision->GetChipSize(),
				player->GetPlayerPos(),
				map_chip_collision->MapChipPos(),
				tnl::eCorrTypeRect::PWRFL_B,
				tnl::eCorrTypeRect::PWRFL_B, 0);
		}
	}

	//---Npc�̓����蔻��---//

	if (tnl::IsIntersectRect(player->GetPlayerPos(), player->GetPlayerSize().x, player->GetPlayerSize().y, boss_npc->GetNpcPos(), boss_npc->GetNpcSizeWidth(), boss_npc->GetNpcSizeHegiht())) {

		//���W�␳
		tnl::eCorrResRect boss = tnl::CorrectPositionRect(
			player->GetPlayerPrevPos(),
			boss_npc->GetNpcPos(),
			player->GetPlayerSize(),
			boss_npc->GetNpcCharaChip(),
			player->GetPlayerPos(),
			boss_npc->GetNpcPos(),
			tnl::eCorrTypeRect::PWRFL_B,
			tnl::eCorrTypeRect::PWRFL_B, 0);

		//�������Ă���Ƃ���Enter�L�[����������o�g���V�[���ɑJ�ڂ�����
		//����Z���t���ǉ��\��
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

			const int BOSS_COMENT_MAX = 7;

			//�C�x���g�}�l�[�W���[�̏�Ԃ��X�V����
			EventManager::GetEventManager()->SetMenuNpc(EventManager::MenuOpen::BOSS);
			EventManager::GetEventManager()->SetNpcComent(EventManager::NpcMove::BOSS);

			//�J�E���g�����Z�b�g����
			UIManager::GetUIManager()->CountReset();

			//1����R�����g�𗬂�
			UIManager::GetUIManager()->ComentClear();

			//�C�x���g�t���O��false�̏ꍇ�t���O��؂�ւ���
			if (!EventManager::GetEventManager()->GetEventFlag()) {
				EventManager::GetEventManager()->EventFlagChange();
			}

			//�R�����g�p�̔z�������������
			UIManager::GetUIManager()->ComentLoad(BOSS_COMENT_MAX, boss_npc->GetNpcName());

			//�V�[�P���X��؂�ւ���
			sequence_.change(&InMapScene::seqMenuOpen);
		}
	}
}

//�鉺���V�[���ł̓����蔻��
void InMapScene::CastleTownCollision()
{
	auto& player = GameManager::GetGameManager()->GetPlayer();

	for (auto& map_chip_collision : map_chip_object) {

		int mapChipValue = map_csv_objct_[static_cast<unsigned>(map_chip_collision->MapChipPos().y / map_chip_collision->GetChipSize().x)][static_cast<unsigned>(map_chip_collision->MapChipPos().x / map_chip_collision->GetChipSize().x)];

		//�}�b�v�`�b�v�̂����蔻��
		if (tnl::IsIntersectRect(player->GetPlayerPos(), player->GetPlayerSize().x, player->GetPlayerSize().y, map_chip_collision->MapChipPos(), map_chip_width_, map_chip_height_)) {

			//���W�␳
			tnl::eCorrResRect map_chip_hit = tnl::CorrectPositionRect(
				player->GetPlayerPrevPos(),
				map_chip_collision->MapChipPos(),
				player->GetPlayerSize(),
				map_chip_collision->GetChipSize(),
				player->GetPlayerPos(),
				map_chip_collision->MapChipPos(),
				tnl::eCorrTypeRect::PWRFL_B,
				tnl::eCorrTypeRect::PWRFL_B, 0);

			//���퉮�̃}�b�v�`�b�v
			const int CASTLE_TOWN_MAPCHIP_WEAPON_SHOP = 674;

			//�h���̃}�b�v�`�b�v
			const int CASTLE_TOWN_MAPCHIP_INN = 699;

			//���퉮�Ƙb�����ꍇ
			if (mapChipValue == CASTLE_TOWN_MAPCHIP_WEAPON_SHOP && select_num == Sequence_num::MOVE && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

				//�V�[�P���X��؂�ւ���
				EventManager::GetEventManager()->SetMenuNpc(EventManager::MenuOpen::WEAPONSHOP);
				EventManager::GetEventManager()->SetNpcComent(EventManager::NpcMove::WEAPONSHOP);
				sequence_.change(&InMapScene::seqMenuOpen);
			}

			//�h���ɘb��������
			else if (mapChipValue == CASTLE_TOWN_MAPCHIP_INN && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

				//��Ԃ�؂�ւ���
				EventManager::GetEventManager()->SetMenuNpc(EventManager::MenuOpen::EMPTY);
				EventManager::GetEventManager()->SetNpcComent(EventManager::NpcMove::INN);

				//1����R�����g�𗬂�
				UIManager::GetUIManager()->ComentClear();

				//�J�E���g�����Z�b�g����
				UIManager::GetUIManager()->CountReset();

				//�C�x���g�t���O��false�̏ꍇ�t���O��؂�ւ���
				if (!EventManager::GetEventManager()->GetEventFlag()) {
					EventManager::GetEventManager()->EventFlagChange();
				}

				//�R�����g�p�̔z�������������
				UIManager::GetUIManager()->ComentLoad(3, inn_npc->GetNpcName());

				sequence_.change(&InMapScene::seqMenuOpen);

			}
		}
	}

	for (auto& npc_ : npc_list) {

		//�_��
		if (tnl::IsIntersectRect(player->GetPlayerPos(), player->GetPlayerSize().x, player->GetPlayerSize().y, npc_->GetNpcPos(), npc_->GetNpcSizeWidth(), npc_->GetNpcSizeHegiht())) {

			//���W�␳
			tnl::eCorrResRect collision = tnl::CorrectPositionRect(
				player->GetPlayerPrevPos(),
				npc_->GetNpcPos(),
				player->GetPlayerSize(),
				npc_->GetNpcCharaChip(),
				player->GetPlayerPos(),
				npc_->GetNpcPos(),
				tnl::eCorrTypeRect::PWRFL_B,
				tnl::eCorrTypeRect::PWRFL_B, 0);

			if (collision != tnl::eCorrResRect::UP && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)
				&&npc_->GetNpcName() == "�_��") {

				//�A�j���[�V������؂�ւ���
				priest_npc->SetNpcAnimation(static_cast<Npc::NPC_ANIMATION>(collision));

				//1����R�����g�𗬂�
				UIManager::GetUIManager()->ComentClear();

				//�J�E���g�����Z�b�g����
				UIManager::GetUIManager()->CountReset();

				//�C�x���g�t���O��false�̏ꍇ�t���O��؂�ւ���
				if (!EventManager::GetEventManager()->GetEventFlag()) {
					EventManager::GetEventManager()->EventFlagChange();
				}

				//���[�h����R�����g�̐�
				const int PRIEST_COMENT_MAX = 3;

				//�R�����g�p�̔z�������������
				UIManager::GetUIManager()->ComentLoad(PRIEST_COMENT_MAX, priest_npc->GetNpcName());

				//��Ԃ�؂�ւ���
				EventManager::GetEventManager()->SetMenuNpc(EventManager::MenuOpen::EMPTY);
				EventManager::GetEventManager()->SetNpcComent(EventManager::NpcMove::PRIEST);
				sequence_.change(&InMapScene::seqMenuOpen);
				
			}
			//�j���Z��
			else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)&& npc_->GetNpcName() == "�j���Z��") {


				//�A�j���[�V������؂�ւ���
				resident_male_npc->SetNpcAnimation(static_cast<Npc::NPC_ANIMATION>(collision));

				//1����R�����g�𗬂�
				UIManager::GetUIManager()->ComentClear();

				//�J�E���g�����Z�b�g����
				UIManager::GetUIManager()->CountReset();

				//�C�x���g�t���O��false�̏ꍇ�t���O��؂�ւ���
				if (!EventManager::GetEventManager()->GetEventFlag()) {
					EventManager::GetEventManager()->EventFlagChange();
				}

				//���[�h����R�����g�̐�
				const int RESIDENT_MALE_COMENT_MAX = 3;

				//�R�����g�p�̔z�������������
				UIManager::GetUIManager()->ComentLoad(RESIDENT_MALE_COMENT_MAX, resident_male_npc->GetNpcName());

				//��Ԃ�؂�ւ���
				EventManager::GetEventManager()->SetMenuNpc(EventManager::MenuOpen::EMPTY);
				EventManager::GetEventManager()->SetNpcComent(EventManager::NpcMove::RESIDENT_MALE);
				sequence_.change(&InMapScene::seqMenuOpen);
			}
		}
	}
}

//��V�[���̓����蔻��
void InMapScene::CastleCollision()
{
	auto& player = GameManager::GetGameManager()->GetPlayer();

	for (auto& map_chip_collision : map_chip_object) {

		int mapChipValue = map_csv_objct_[static_cast<unsigned>(map_chip_collision->MapChipPos().y / map_chip_collision->GetChipSize().x)][static_cast<unsigned>(map_chip_collision->MapChipPos().x / map_chip_collision->GetChipSize().x)];

		//�}�b�v�`�b�v�̂����蔻��
		if (tnl::IsIntersectRect(player->GetPlayerPos(), player->GetPlayerSize().x, player->GetPlayerSize().y, map_chip_collision->MapChipPos(), map_chip_width_, map_chip_height_)) {

			//���W�␳
			tnl::eCorrResRect map_chip_hit = tnl::CorrectPositionRect(
				player->GetPlayerPrevPos(),
				map_chip_collision->MapChipPos(),
				player->GetPlayerSize(),
				map_chip_collision->GetChipSize(),
				player->GetPlayerPos(),
				map_chip_collision->MapChipPos(),
				tnl::eCorrTypeRect::PWRFL_B,
				tnl::eCorrTypeRect::PWRFL_B, 0);
		}
	}

	//---Npc�̓����蔻��---//

	for (auto& npc_ : npc_list) {

		if (tnl::IsIntersectRect(player->GetPlayerPos(), player->GetPlayerSize().x, player->GetPlayerSize().y, npc_->GetNpcPos(), npc_->GetNpcSizeWidth(), npc_->GetNpcSizeHegiht())) {

			//���W�␳
			tnl::eCorrResRect collision = tnl::CorrectPositionRect(
				player->GetPlayerPrevPos(),
				npc_->GetNpcPos(),
				player->GetPlayerSize(),
				npc_->GetNpcCharaChip(),
				player->GetPlayerPos(),
				npc_->GetNpcPos(),
				tnl::eCorrTypeRect::PWRFL_B,
				tnl::eCorrTypeRect::PWRFL_B, 0);

			//��
			if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN) && npc_->GetNpcName() == "��") {

			//�A�j���[�V������ύX����
			king_npc->SetNpcAnimation(static_cast<Npc::NPC_ANIMATION>(collision));

			//1����R�����g�𗬂�
			UIManager::GetUIManager()->ComentClear();

			//�J�E���g�����Z�b�g����
			UIManager::GetUIManager()->CountReset();

			//���[�h����R�����g�̐�
			const int KING_COMENT_MAX = 6;

			//�R�����g�p�̔z�������������
			UIManager::GetUIManager()->ComentLoad(KING_COMENT_MAX, king_npc->GetNpcName());

			//��Ԃ�؂�ւ���
			EventManager::GetEventManager()->SetMenuNpc(EventManager::MenuOpen::EMPTY);
			EventManager::GetEventManager()->SetNpcComent(EventManager::NpcMove::KING);

			//�V�[�P���X��؂�ւ���
			sequence_.change(&InMapScene::seqMenuOpen);
			}
			//����
			else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN) && npc_->GetNpcName() == "����") {

			//�A�j���[�V������ύX����
			queen_npc->SetNpcAnimation(static_cast<Npc::NPC_ANIMATION>(collision));

			//1����R�����g�𗬂�
			UIManager::GetUIManager()->ComentClear();

			//�J�E���g�����Z�b�g����
			UIManager::GetUIManager()->CountReset();

			//���[�h����R�����g�̐�
			const int QUEEN_COMENT_MAX = 3;

			//�R�����g�p�̔z�������������
			UIManager::GetUIManager()->ComentLoad(QUEEN_COMENT_MAX, queen_npc->GetNpcName());

			//��Ԃ�؂�ւ���
			EventManager::GetEventManager()->SetMenuNpc(EventManager::MenuOpen::EMPTY);
			EventManager::GetEventManager()->SetNpcComent(EventManager::NpcMove::QUEEN);

			//�V�[�P���X��؂�ւ���
			sequence_.change(&InMapScene::seqMenuOpen);
			}
			//�w��
			else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN) && npc_->GetNpcName() == "�w��") {

			//�A�j���[�V������ύX����
			resident_male_npc->SetNpcAnimation(static_cast<Npc::NPC_ANIMATION>(collision));

			//1����R�����g�𗬂�
			UIManager::GetUIManager()->ComentClear();

			//�J�E���g�����Z�b�g����
			UIManager::GetUIManager()->CountReset();

			//���[�h����R�����g�̐�
			const int SCHOLAR_COMENT_MAX = 7;

			//�R�����g�p�̔z�������������
			UIManager::GetUIManager()->ComentLoad(SCHOLAR_COMENT_MAX, resident_male_npc->GetNpcName());

			//��Ԃ�؂�ւ���
			EventManager::GetEventManager()->SetMenuNpc(EventManager::MenuOpen::EMPTY);
			EventManager::GetEventManager()->SetNpcComent(EventManager::NpcMove::SCHOLAR);

			//�V�[�P���X��؂�ւ���
			sequence_.change(&InMapScene::seqMenuOpen);
			}
			//���m
			else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN) && npc_->GetNpcName() == "���m") {

			//�A�j���[�V������ύX����
			npc_->SetNpcAnimation(static_cast<Npc::NPC_ANIMATION>(collision));

			//1����R�����g�𗬂�
			UIManager::GetUIManager()->ComentClear();

			//�J�E���g�����Z�b�g����
			UIManager::GetUIManager()->CountReset();

			//���[�h����R�����g�̐�
			const int SOLDIER_COMENT_MAX = 3;

			//�R�����g�p�̔z�������������
			UIManager::GetUIManager()->ComentLoad(SOLDIER_COMENT_MAX, soldier_npc->GetNpcName());

			//��Ԃ�؂�ւ���
			EventManager::GetEventManager()->SetMenuNpc(EventManager::MenuOpen::EMPTY);
			EventManager::GetEventManager()->SetNpcComent(EventManager::NpcMove::RESIDENT_MALE);

			//�V�[�P���X��؂�ւ���
			sequence_.change(&InMapScene::seqMenuOpen);
			}
		}
	}
}

//�E�҂̕�V�[���̓����蔻��
void InMapScene::GraveSceneCollision()
{

	auto& player = GameManager::GetGameManager()->GetPlayer();

	//�u���b�N�ƏՓ˔��� & �␳
	//�Q�[�����[�v���ł̓����蔻��̃`�F�b�N]
	for (auto& mapChip : map_chip_object) {


		int mapChipValue = map_csv_objct_[static_cast<unsigned>(mapChip->MapChipPos().y / mapChip->GetChipSize().y)][static_cast<unsigned>(mapChip->MapChipPos().x / mapChip->GetChipSize().x)];

		//�����蔻��
		if (tnl::IsIntersectRect(player->GetPlayerPos(), player->GetPlayerSize().x, player->GetPlayerSize().y, mapChip->MapChipPos(), map_chip_width_, map_chip_height_)) {

			//���W�␳
			tnl::eCorrResRect collision = tnl::CorrectPositionRect(
				player->GetPlayerPrevPos(),
				mapChip->MapChipPos(),
				player->GetPlayerSize(),
				mapChip->GetChipSize(),
				player->GetPlayerPos(),
				mapChip->MapChipPos(),
				tnl::eCorrTypeRect::PWRFL_B,
				tnl::eCorrTypeRect::PWRFL_B);

			//��̃}�b�v�`�b�v
			const int MAP_CHIP_GRAVE = 1694;

			//�E�҂̕�ɐG�ꂽ��
			//�C�x���g���I�����ĂȂ����
			if (mapChipValue == MAP_CHIP_GRAVE && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)
				&& ordeal_npc) {

				//�����̔Ԑl�̃R�����g�t���O�����Z�b�g����
				EventManager::GetEventManager()->OrdealComentFlagReset();

				//��Ԃ�؂�ւ���
				EventManager::GetEventManager()->SetMenuNpc(EventManager::MenuOpen::EMPTY);
				EventManager::GetEventManager()->SetNpcComent(EventManager::NpcMove::ORDEAL);

				//1����R�����g�𗬂�
				UIManager::GetUIManager()->ComentClear();

				//�J�E���g�����Z�b�g����
				UIManager::GetUIManager()->CountReset();

				//�C�x���g�t���O��false�̏ꍇ�t���O��؂�ւ���
				if (!EventManager::GetEventManager()->GetEventFlag()) {
					EventManager::GetEventManager()->EventFlagChange();
				}

				//���[�h����R�����g�̐�
				const int ORDEAL_COMENT_MAX = 5;

				//�R�����g�p�̔z�������������
				UIManager::GetUIManager()->ComentLoad(ORDEAL_COMENT_MAX, ordeal_npc->GetNpcName());

				sequence_.change(&InMapScene::seqMenuOpen);

			}
		}
	}
}
