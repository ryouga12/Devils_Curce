#include "mapScene.h"
#include"../Manager/SceneManager.h"
#include"resultScene.h"
#include"../Object/Player.h"
#include"../Manager/CsvManager.h"

//------------------------------------------------------------------------------------------------------------------------
//�������@&  ���

MapScene::MapScene(){

	mapchip = std::make_shared<MapChip>();

	//MapChip�̓ǂݍ���
	worldMapLoad();

	//�����J�E���g��150����300�̒l�̃����_���̐��l�����Ċm����ς���
	encounterThreshold = rand() % 201 + 150;

	//BGM�𗬂�
	SoundManager::getSoundManager()->sound_Play("sound/BGM/sfc-harukanaru-daichi.mp3", DX_PLAYTYPE_LOOP);
}

MapScene::~MapScene()
{
	//�}�b�v�`�b�v�̉��(2096 = �}�b�v�`�b�v�̑���)
	for (int i = 0; i < 2096; i++) {
		DeleteGraph(gpc_map_chip_hdls_[i]);
		tnl::DebugTrace("Deleted  MapChipimage at [%d]. Pointer value: %p\n", i, gpc_map_chip_hdls_[i]);
	}

	//BGM���~�߂�
	SoundManager::getSoundManager()->StopSound("sound/BGM/sfc-harukanaru-daichi.mp3");

}

//------------------------------------------------------------------------------------------------------------------------
//�X�V����

void MapScene::Update(float delta_time)
{
	//�V�[�P���X�̍X�V����
	sequence_.update(delta_time);

	//�v���C���[�̍X�V����
	GameManager::getGameManager()->getPlayer()->player_Move(delta_time, player_velocity);

	//�J�����̍X�V����
	GameManager::getGameManager()->getCamera()->update(GameManager::getGameManager()->getPlayer()->getPlayerPos(), MAP_WIDTH, MAP_HEIGHT);

	//�����𐔂���
	//�G���J�E���g����
	if (ChackEncount(GameManager::getGameManager()->getPlayer()->GetStep())) {
		//BGM���~�߂�
		SoundManager::getSoundManager()->StopSound("sound/BGM/sfc-harukanaru-daichi.mp3");
		//�o�g���p��BGM�𗬂�
		SoundManager::getSoundManager()->sound_Play("sound/BGM/sentou.mp3", DX_PLAYTYPE_LOOP);

		//0.5�b�o�߂�����V�[����J�ڂ�����
		if (GameManager::getGameManager()->TimeCount(delta_time, time)) {

			auto mgr = SceneManager::GetInstance();
			//�V�[����J�ڂ�����(�v���C���[�̍��W��n��,�G��ID,�G�̃|�C���^��n��)
			mgr->changeScene(new BattleScene(GameManager::getGameManager()->getPlayer()->getPlayerPos(), background_hdl, std::make_shared<MobMonster>(enemy_id)));
			//������0�ɂ���
			GameManager::getGameManager()->getPlayer()->StepReset();
		}
	}

	//Inventory
	GameManager::getGameManager()->getInventory()->Update(delta_time);
	
	//�����蔻��
	WorldMapCollision();

}

//------------------------------------------------------------------------------------------------------------------------
//�`��

void MapScene::Draw()
{
	//MapChip�̕`��
	for (auto& world_map : MapChip_continent) {
		world_map->Draw(*GameManager::getGameManager()->getCamera());
	}

	for (auto& world_object : MapChips_object) {
		world_object->Draw(*GameManager::getGameManager()->getCamera());
	}

	//�v���C���[�̕`��
	GameManager::getGameManager()->getPlayer()->player_draw(*GameManager::getGameManager()->getCamera(), player_size);

	//�C���x���g���̕`��
	GameManager::getGameManager()->getInventory()->draw();

}

//------------------------------------------------------------------------------------------------------------------------
//�V�[�P���X�֘A

bool MapScene::seqIdle(float delta_time)
{
	//�A�j���[�V�����ƕ�����0�ɂ���
	if (sequence_.isStart()) {
		GameManager::getGameManager()->getPlayer()->AnimationChange(Animation_default);
		GameManager::getGameManager()->getPlayer()->StepReset();
		
	}
	//�f�o�b�N����
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_1)) {
		auto mgr = SceneManager::GetInstance();
		mgr->changeScene(new ResultScene());
	}

	//ESC����������A�C���x���g����\������
	if (GameManager::getGameManager()->getInventory()->GetSelectMenuNum() == Inventory::MenuWindow_I::EMPTY && tnl::Input::IsKeyDownTrigger(eKeys::KB_ESCAPE)) {
			
			//SE�𗬂�
			SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
			//�C���x���g����\������
			GameManager::getGameManager()->getInventory()->InventoryMenuChange(Inventory::MenuWindow_I::FIRSTMENU);
			//plyer�𓮂��Ȃ��悤�ɂ���
			GameManager::getGameManager()->getPlayer()->setPlayerControl();
	}

	return false;
}

bool MapScene::seqChangeScene(float delta_time)
{
	//�J������ۊǍ��W�ɓ����
	GameManager::getGameManager()->getCamera()->SavePosition(GameManager::getGameManager()->getPlayer()->getPlayerPos());

	return true;
}

//------------------------------------------------------------------------------------------------------------------------
//�}�b�v�`�b�v�֘A

void MapScene::WorldMapCollision()
{
	//���[���h�}�b�v��csv
	auto world_map_csv = CsvManager::getCsvManager()->GetWorldMapCsv();

	//���[���h�}�b�v�ɂ���I�u�W�F�N�g��csv
	auto world_object_csv = CsvManager::getCsvManager()->GetWorldMapObjectCsv();

	//�I�u�W�F�N�g�̓����蔻��
	for (auto& MapChip : MapChips_object) {
		int mapChipValue = static_cast<int>(world_object_csv[static_cast<unsigned>(MapChip->MapChipPos().y / MapChip->GetChipSize())][static_cast<unsigned>(MapChip->MapChipPos().x / MapChip->GetChipSize())]);
		if (colisionObjectValues.count(mapChipValue) > 0)
		{
			ResourceManager::getResourceManager()->IsIntersectRectToCorrectPosition(GameManager::getGameManager()->getPlayer()->getPlayerPos(), GameManager::getGameManager()->getPlayer()->getPlayerPrevPos(), GameManager::getGameManager()->getPlayer()->GetPlayerWidth(), GameManager::getGameManager()->getPlayer()->GetPlayerHight(), MapChip->MapChipPos(), map_chip_width_, map_chip_height_);
		}
		if (villageValues.count(mapChipValue) > 0) {
			int Collision = tnl::IsIntersectRect(GameManager::getGameManager()->getPlayer()->getPlayerPos(), GameManager::getGameManager()->getPlayer()->GetPlayerWidth(), GameManager::getGameManager()->getPlayer()->GetPlayerHight(), MapChip->MapChipPos(), map_chip_width_, map_chip_height_);
			if (Collision) {

				//�I�u�W�F�N�g�ɓ���������Ԃ�Enter�L�[����������J�ڂ�����
				//��ɑ��⒬�A��A�{�X�̏�ȂǂɑJ�ڂ�����
				if (mapChipValue == map_chip_village) {
					if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
						SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/idou.mp3", DX_PLAYTYPE_BACK);
						//���V�[���ɐ؂�ւ���
						auto mgr = SceneManager::GetInstance();
						mgr->changeScene(new InMapScene(village_pos, InMapScene::InMapState::VILLAGE), 0.1);
						GameManager::getGameManager()->getPlayer()->SetPlayerPosition(village_pos);
						sequence_.change(&MapScene::seqChangeScene);
					}
				}
				//�{�X�̏邾������
				else if (mapChipValue == map_chip_boss_castle) {
					if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
						SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/idou.mp3", DX_PLAYTYPE_BACK);
						//�_���W�����V�[���ɐ؂�ւ���
						auto mgr = SceneManager::GetInstance();
						mgr->changeScene(new InMapScene(boss_castle_pos, InMapScene::InMapState::BOSSCASTLE), 0.1 ,0.8);
						//���W���{�X�̏ꏊ�ɃZ�b�g����
						GameManager::getGameManager()->getPlayer()->SetPlayerPosition(boss_castle_pos);
						sequence_.change(&MapScene::seqChangeScene);
					}
				}

			}
		}

	}
	//�n�`�̓����蔻��
	for (auto& worldMap_C : MapChip_continent) {
		int mapChipValue = world_map_csv[static_cast<unsigned>(worldMap_C->MapChipPos().y / worldMap_C->GetChipSize())][static_cast<unsigned>(worldMap_C->MapChipPos().x / worldMap_C->GetChipSize())];
		if (worldCollisionValues.count(mapChipValue)) {
			int collison = ResourceManager::getResourceManager()->IsIntersectRectToCorrectPosition(GameManager::getGameManager()->getPlayer()->getPlayerPos(), GameManager::getGameManager()->getPlayer()->getPlayerPrevPos(), GameManager::getGameManager()->getPlayer()->GetPlayerWidth(), GameManager::getGameManager()->getPlayer()->GetPlayerHight(), worldMap_C->MapChipPos(), map_chip_width_, map_chip_height_);
		}
		//�G���J�E���g�p�̓����蔻��
		if (encount_kind.count(mapChipValue)) {
			int encount = tnl::IsIntersectRect(GameManager::getGameManager()->getPlayer()->getPlayerPos(), GameManager::getGameManager()->getPlayer()->GetPlayerWidth(), GameManager::getGameManager()->getPlayer()->GetPlayerHight(), worldMap_C->MapChipPos(), map_chip_width_, map_chip_height_);

				//map��̂ǂ��ɋ��邩�ɂ���ēG��ς���
			if (encount) {
				//�����ɂ���ꍇ
				if (mapChipValue == plain) {

					//�ŏ��̓G��ID��n��
					enemy_id = 1;

					//mapChip�ɉ����Ĕw�i��ς���
					background_hdl = ResourceManager::getResourceManager()->LoadGraphEX("graphics/haikei/battle_field_01.jpg");
				}
				else if (mapChipValue == plain_second) {

					//�����̓G��ID��n��
					enemy_id = 2;

					//mapChip�ɉ����Ĕw�i��ς���
					background_hdl = ResourceManager::getResourceManager()->LoadGraphEX("graphics/haikei/battle_field_02.jpg");;
				}
				//�r���ɂ���ꍇ
				else if (mapChipValue == wilderness[0] || mapChipValue == wilderness[1]) {

					//�r���ɂ���G��ID��n��
					enemy_id = 3;

					//mapChip�ɉ����Ĕw�i��ς���
					background_hdl = ResourceManager::getResourceManager()->LoadGraphEX("graphics/haikei/battle_field_03.jpg");;
				}
				//�{�X�G���A�̏ꍇ
				else if (mapChipValue == boss_area) {

					//�{�X�G���A�ɂ���G��ID��n��
					enemy_id = 4;

					//mapChip�ɉ����Ĕw�i��ς���
					background_hdl = ResourceManager::getResourceManager()->LoadGraphEX("graphics/haikei/pipo-battlebg014b.jpg");
				}
			}
		}
	}

}

//�G���J�E���g
bool MapScene::ChackEncount(int step)
{
	if (step >= encounterThreshold) {
		return true;
	}

	return false;
}

//�}�b�v�`�b�v�̃��[�h
void MapScene::worldMapLoad()
{
	//���[���h�}�b�v��csv
	auto world_map_csv = CsvManager::getCsvManager()->GetWorldMapCsv();

	//���[���h�}�b�v�ɂ���I�u�W�F�N�g��csv
	auto world_object_csv = CsvManager::getCsvManager()->GetWorldMapObjectCsv();

	// �}�b�v�`�b�v�̉摜�̃��[�h
	map_chip_ghdl_pass = "graphics/image_(2024_0115_0231).png";

	map_chip_width_ = 32;
	map_chip_height_ = 32;
	map_chip_x_size_ = 8;
	map_chip_y_size_ = 262;
	map_chip_all_size_ = map_chip_x_size_ * map_chip_y_size_;

	LoadDivGraph(map_chip_ghdl_pass.c_str(),
		map_chip_all_size_,
		map_chip_x_size_,
		map_chip_y_size_,
		map_chip_width_,
		map_chip_height_,
		gpc_map_chip_hdls_);

	//�����̃}�b�v�`�b�v�̓ǂݍ���
	for (int i = 0; i < world_map_csv.size(); ++i) {
		for (int k = 0; k < world_map_csv[i].size(); ++k) {
			if (-1 == world_map_csv[i][k])continue;
			tnl::Vector3 pos;
			int gpc_hdl;
			pos.x = { mapchip->GetChipSize() / 2 + k * mapchip->GetChipSize()};
			pos.y = { i * mapchip->GetChipSize() };
			gpc_hdl = gpc_map_chip_hdls_[world_map_csv[i][k]];
			MapChip_continent.emplace_back(std::make_shared<MapChip>(pos, gpc_hdl));
		}
	}

	//�I�u�W�F�N�g�̃}�b�v�`�b�v�̓ǂݍ���
	for (int i = 0; i < world_object_csv.size(); ++i) {
		for (int k = 0; k <  world_object_csv[i].size(); ++k) {
			if (-1 == world_object_csv[i][k])continue;
			tnl::Vector3 pos;
			int gpc_hdl;
			pos.x = { mapchip->GetChipSize() / 2 + k * mapchip->GetChipSize()};
			pos.y = { i * mapchip->GetChipSize()};
			gpc_hdl = gpc_map_chip_hdls_[world_object_csv[i][k]];
			MapChips_object.emplace_back(std::make_shared<MapChip>(pos, gpc_hdl));
		}
	}

}