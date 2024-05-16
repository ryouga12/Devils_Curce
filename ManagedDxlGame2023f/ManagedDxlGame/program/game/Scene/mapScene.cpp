#include "mapScene.h"
#include"../Manager/SceneManager.h"
#include"resultScene.h"
#include"../Object/Player.h"

//------------------------------------------------------------------------------------------------------------------------
//�������@&  ���

MapScene::MapScene(tnl::Vector3 PlyerPos) : pos(PlyerPos){
	
	//Player�̏����ʒu�����߂�
	GameManager::getGameManager()->getPlayer()->SetPlayerPosition(pos);

	//�J�������v���C���[�ɍ��킹��
	GameManager::getGameManager()->getCamera()->SetTargetPos(pos);

	mapchip = std::make_shared<MapChip>();
	enemy = std::make_shared<Enemy>();


	//MapChip�̓ǂݍ���
	worldMapLoad();

	//�����J�E���g��150����300�̒l�̃����_���̐��l�����Ċm����ς���
	encounterThreshold = rand() % 201 + 150;

	//BGM�𗬂�
	SoundManager::getSoundManager()->sound_Play("sound/BGM/sfc-harukanaru-daichi.mp3", DX_PLAYTYPE_LOOP);

}

MapScene::~MapScene()
{
	//�T�E���h��delete����
	SoundManager::getSoundManager()->daleteSound("sound/SoundEffect/idou.mp3");
	SoundManager::getSoundManager()->daleteSound("sound/BGM/sfc-harukanaru-daichi.mp3");

	//�}�b�v�`�b�v�̉��(2096 = �}�b�v�`�b�v�̑���)
	for (int i = 0; i < 2096; i++) {
		DeleteGraph(gpc_map_chip_hdls_[i]);
		tnl::DebugTrace("Deleted  MapChipimage at [%d]. Pointer value: %p\n", i, gpc_map_chip_hdls_[i]);
	}
}

//------------------------------------------------------------------------------------------------------------------------
//�X�V����

void MapScene::Update(float delta_time)
{
	//�V�[�P���X�̍X�V����
	sequence_.update(delta_time);

	//�v���C���[�̍X�V����
	GameManager::getGameManager()->getPlayer()->player_Move(delta_time, PlayerVelocity);

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
		if (GameManager::getGameManager()->TimeCount(delta_time, Time)) {
			auto mgr = SceneManager::GetInstance();
			//�V�[����J�ڂ�����(�v���C���[�̍��W��n��,�C���x���g����n��,�v���C���[��n��,�G�̔z���n��,�w�i��n��)
			mgr->changeScene(new BattleScene(GameManager::getGameManager()->getPlayer()->getPlayerPos(), background_hdl , enemy_id));
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
	for (auto world_map : MapChip_continent) {
		world_map->Draw(*GameManager::getGameManager()->getCamera());
	}

	for (auto world_object : MapChips_object) {
		world_object->Draw(*GameManager::getGameManager()->getCamera());
	}

	//�v���C���[�̕`��
	GameManager::getGameManager()->getPlayer()->player_draw(*GameManager::getGameManager()->getCamera(), PlayerSize);

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

	//SE�𗬂��Ă���ڍs����
	if (TimeFlag) {
		if (GameManager::getGameManager()->TimeCount(delta_time, TimeSE)) {
			//���V�[���ɐ؂�ւ���
			auto mgr = SceneManager::GetInstance();
			mgr->changeScene(new VillageScene(village_Pos));
			sequence_.change(&MapScene::seqChangeScene);
			TimeFlag = false;
		}
	}

	//ESC����������A�C���x���g����\������
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_ESCAPE)) {
		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
		//�C���x���g����\������
		GameManager::getGameManager()->getInventory()->SetSelect_num(first_menu);
	}

	return true;
}

//------------------------------------------------------------------------------------------------------------------------
//�}�b�v�`�b�v�֘A

void MapScene::WorldMapCollision()
{
	//�I�u�W�F�N�g�̓����蔻��
	for (auto MapChip : MapChips_object) {
		int mapChipValue = world_object_csv[MapChip->MapChipPos().y / MapChip->getChipSize()][MapChip->MapChipPos().x / MapChip->getChipSize()];
		if (colisionObjectValues.count(mapChipValue) > 0)
		{
			int collision = ResourceManager::getResourceManager()->IsIntersectRectToCorrectPosition(GameManager::getGameManager()->getPlayer()->getPlayerPos(), GameManager::getGameManager()->getPlayer()->getPlayerPrevPos(), GameManager::getGameManager()->getPlayer()->getPlayerSize(PlyerWidth), GameManager::getGameManager()->getPlayer()->getPlayerSize(PlyerHeight), MapChip->MapChipPos(), map_chip_width_, map_chip_height_);

		}
		if (villageValues.count(mapChipValue) > 0) {
			int villageCollision = tnl::IsIntersectRect(GameManager::getGameManager()->getPlayer()->getPlayerPos(), GameManager::getGameManager()->getPlayer()->getPlayerSize(PlyerWidth), GameManager::getGameManager()->getPlayer()->getPlayerSize(PlyerHeight), MapChip->MapChipPos(), map_chip_width_, map_chip_height_);
			if (villageCollision) {
				//���̂������ꍇ
				if (mapChipValue == mapChip_village) {
					if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
						SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/idou.mp3", DX_PLAYTYPE_BACK);
						/*SoundManager::getSoundManager()->StopSound("sound/BGM/sfc-harukanaru-daichi.mp3");*/
						TimeFlag = true;
					}
				}
			}
		}

	}
	//�n�`�̓����蔻��
	for (auto worldMap_C : MapChip_continent) {
		int mapChipValue = world_map_csv[worldMap_C->MapChipPos().y / worldMap_C->getChipSize()][worldMap_C->MapChipPos().x / worldMap_C->getChipSize()];
		if (worldCollisionValues.count(mapChipValue)) {
			int collison = ResourceManager::getResourceManager()->IsIntersectRectToCorrectPosition(GameManager::getGameManager()->getPlayer()->getPlayerPos(), GameManager::getGameManager()->getPlayer()->getPlayerPrevPos(), GameManager::getGameManager()->getPlayer()->getPlayerSize(PlyerWidth), GameManager::getGameManager()->getPlayer()->getPlayerSize(PlyerHeight), worldMap_C->MapChipPos(), map_chip_width_, map_chip_height_);
		}
		//�G���J�E���g�p�̓����蔻��
		if (encount_kind.count(mapChipValue)) {
			int encount = tnl::IsIntersectRect(GameManager::getGameManager()->getPlayer()->getPlayerPos(), GameManager::getGameManager()->getPlayer()->getPlayerSize(PlyerWidth), GameManager::getGameManager()->getPlayer()->getPlayerSize(PlyerHeight), worldMap_C->MapChipPos(), map_chip_width_, map_chip_height_);

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

	//�����}�b�v�f�[�^�̃��[�h
	map_chip_csv_pass = "csv/world_map_plain.csv";
	world_map_csv = tnl::LoadCsv<int>(map_chip_csv_pass);

	//�I�u�W�F�N�g�f�[�^�̃��[�h
	map_chip_csv_object_pass = "csv/world_map_object.csv";
	world_object_csv = tnl::LoadCsv<int>(map_chip_csv_object_pass);

	//�����̃}�b�v�`�b�v�̓ǂݍ���
	for (int i = 0; i < world_map_csv.size(); ++i) {
		for (int k = 0; k < world_map_csv[i].size(); ++k) {
			if (-1 == world_map_csv[i][k])continue;
			tnl::Vector3 pos;
			int gpc_hdl;
			pos.x = { mapchip->getChipSize() / 2 + k * mapchip->getChipSize()};
			pos.y = { i * mapchip->getChipSize() };
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
			pos.x = { mapchip->getChipSize() / 2 + k * mapchip->getChipSize()};
			pos.y = { i * mapchip->getChipSize()};
			gpc_hdl = gpc_map_chip_hdls_[world_object_csv[i][k]];
			MapChips_object.emplace_back(std::make_shared<MapChip>(pos, gpc_hdl));
		}
	}

}