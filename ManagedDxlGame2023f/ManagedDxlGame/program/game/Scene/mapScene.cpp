#include "mapScene.h"
#include"../Manager/SceneManager.h"
#include"resultScene.h"
#include"../Object/Player.h"
#include"../Manager/CsvManager.h"
#include"battleScene.h"
#include"../Manager/UiManager.h"

//------------------------------------------------------------------------------------------------------------------------
//�������@&  ���

MapScene::MapScene(){

	//���݂̃V�[����MapScene�ɐݒ�
	curent_scene = SceneState::WORLDMAP;

	//csv�̏�����
	CsvManager::GetCsvManager()->LoadCsv(curent_scene);

	//�}�b�v�`�b�v�̏�����
	mapchip = std::make_shared<MapChip>();

	//MapChip�̓ǂݍ���
	worldMapLoad();

	//�����J�E���g��150����300�̒l�̃����_���̐��l�����Ċm����ς���
	encounterThreshold = (rand() % 150) + 150;

	//---�}�b�v�̍����ƕ������߂�---//
	
	//�}�b�v�̍���
	map_height = 1550;		
	//�}�b�v�̕�
	map_width = 3200;					
	

	//�C�x���g�֘A�̏�����
	EventManager::GetEventManager()->InitEventRelated(npc_list);

	//BGM�𗬂�
	SoundManager::GetSoundManager()->Sound_Play("sound/BGM/sfc-harukanaru-daichi.mp3", DX_PLAYTYPE_LOOP);
}

MapScene::~MapScene()
{
	//�}�b�v�`�b�v�̉��(2096 = �}�b�v�`�b�v�̑���)
	for (int i = 0; i < 2096; i++) {
		DeleteGraph(gpc_map_chip_hdls_[i]);
		tnl::DebugTrace("Deleted  MapChipimage at [%d]. Pointer value: %p\n", i, gpc_map_chip_hdls_[i]);
	}

	//BGM���~�߂�
	SoundManager::GetSoundManager()->StopSound("sound/BGM/sfc-harukanaru-daichi.mp3");

}

//------------------------------------------------------------------------------------------------------------------------
//�X�V����

//�f�o�b�N�p
void MapScene::DebugMapScene()
{
	//�f�o�b�N����
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_1)) {

		auto mgr = SceneManager::GetSceneManager();

		//�t���O��false�̏ꍇtrue�ɕύX����(�V�[����J�ڂ������)
		if (!mgr->GetSceneFlag()) { mgr->SceneFlagChange(); }

		mgr->changeScene(new ResultScene());
	}
	else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_3)) {

		//���V�[���ɐ؂�ւ���
		auto mgr = SceneManager::GetSceneManager();

		//�t���O��false�̏ꍇtrue�ɕύX����(�V�[����J�ڂ������)
		if (!mgr->GetSceneFlag()) { mgr->SceneFlagChange(); }

		mgr->changeScene(new InMapScene(TOWN_POS, InMapScene::InMapState::TOWN), 0.1f);
		GameManager::GetGameManager()->GetPlayer()->SetPlayerPosition(VILLAGE_POS);
		sequence_.change(&MapScene::seqChangeScene);

	}
	else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_4)) {
		SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/idou.mp3", DX_PLAYTYPE_BACK);
		//�_���W�����V�[���ɐ؂�ւ���
		auto mgr = SceneManager::GetSceneManager();

		//�t���O��false�̏ꍇtrue�ɕύX����(�V�[����J�ڂ������)
		if (!mgr->GetSceneFlag()) { mgr->SceneFlagChange(); }

		mgr->changeScene(new InMapScene(BOSS_CASTLE_POS, InMapScene::InMapState::BOSSCASTLE), 0.1f, 0.8f);

		//���W���{�X�̏ꏊ�ɃZ�b�g����
		GameManager::GetGameManager()->GetPlayer()->SetPlayerPosition(BOSS_CASTLE_POS);
		sequence_.change(&MapScene::seqChangeScene);
	}
	else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_5)) {

		//�鉺���V�[���ɐ؂�ւ���
		auto mgr = SceneManager::GetSceneManager();

		//�t���O��false�̏ꍇtrue�ɕύX����(�V�[����J�ڂ������)
		if (!mgr->GetSceneFlag()) { mgr->SceneFlagChange(); }

		mgr->changeScene(new InMapScene(CASTLE_TOWN_POS, InMapScene::InMapState::CASTLETOWN), 0.1f);
		GameManager::GetGameManager()->GetPlayer()->SetPlayerPosition(CASTLE_TOWN_POS);
		sequence_.change(&MapScene::seqChangeScene);

	}
	else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_6)) {

		//�E�҂̕�V�[���ɐ؂�ւ���
		auto mgr = SceneManager::GetSceneManager();

		//�t���O��false�̏ꍇtrue�ɕύX����(�V�[����J�ڂ������)
		if (!mgr->GetSceneFlag()) { mgr->SceneFlagChange(); }

		mgr->changeScene(new InMapScene(GRAVE_POS, InMapScene::InMapState::GRAVE), 0.1f);
		GameManager::GetGameManager()->GetPlayer()->SetPlayerPosition(GRAVE_POS);
		sequence_.change(&MapScene::seqChangeScene);

	}
}


void MapScene::Update(float delta_time)
{
	//�V�[�P���X�̍X�V����
	sequence_.update(delta_time);

	//�v���C���[�̃A�j���[�V�����̏���
	GameManager::GetGameManager()->GetPlayer()->PlayerAnimation(delta_time);

	//�����𐔂���
	//�G���J�E���g����
	if (ChackEncount(GameManager::GetGameManager()->GetPlayer()->GetStep())) {

		//BGM���~�߂�
		SoundManager::GetSoundManager()->StopSound("sound/BGM/sfc-harukanaru-daichi.mp3");
		//�o�g���p��BGM�𗬂�
		SoundManager::GetSoundManager()->Sound_Play("sound/BGM/sentou.mp3", DX_PLAYTYPE_LOOP);

		//0.5�b�o�߂�����V�[����J�ڂ�����
		if (GameManager::GetGameManager()->TimeCount(delta_time, koni::Numeric::SECONDS_0_5_F)) {

			auto mgr = SceneManager::GetSceneManager();

			//�t���O��false�̏ꍇtrue�ɕύX����(�V�[����J�ڂ������)
			if (!mgr->GetSceneFlag()) { mgr->SceneFlagChange(); }

			//�V�[����J�ڂ�����(�v���C���[�̍��W��n��,�G��ID,�G�̃|�C���^��n��)
			mgr->changeScene(new BattleScene(GameManager::GetGameManager()->GetPlayer()->GetPlayerPos(), background_hdl, std::make_shared<MobMonster>(enemy_id)));
			//������0�ɂ���
			GameManager::GetGameManager()->GetPlayer()->StepReset();
		}
	}

	//�f�o�b�N�p�̍X�V����(��ɒ��ȂǂɑJ�ڂ�����)
	DebugMapScene();

	//Inventory�̍X�V����
	GameManager::GetGameManager()->GetInventory()->Update(delta_time);

	//���ʏ���
	__super::Update(delta_time);

}

//------------------------------------------------------------------------------------------------------------------------
//�`��

void MapScene::Draw()
{
	//MapChip�̕`��
	for (auto& world_map : map_chips) {
		world_map->Draw(*GameManager::GetGameManager()->GetCamera());
	}

	for (auto& world_object : map_chip_object) {
		world_object->Draw(*GameManager::GetGameManager()->GetCamera());
	}

	//�v���C���[�̕`��
	GameManager::GetGameManager()->GetPlayer()->Player_draw(*GameManager::GetGameManager()->GetCamera(), koni::Numeric::SCALE_ONE_F);

	//�C���x���g���̕`��
	GameManager::GetGameManager()->GetInventory()->draw();

	//���ʕ`��
	__super::Draw();
}

//------------------------------------------------------------------------------------------------------------------------
//�V�[�P���X�֘A

bool MapScene::seqMove(float delta_time)
{
	//�A�j���[�V�����ƕ�����0�ɂ���
	if (sequence_.isStart()) {
		GameManager::GetGameManager()->GetPlayer()->AnimationChange(Animation_default);
		GameManager::GetGameManager()->GetPlayer()->StepReset();
	}

	//�v���C���[�̍X�V����
	GameManager::GetGameManager()->GetPlayer()->Player_Move(delta_time, player_velocity, map_height);

	//�J�����̍X�V����
	GameManager::GetGameManager()->GetCamera()->update(GameManager::GetGameManager()->GetPlayer()->GetPlayerPos(), map_width, map_height);


	//ESC����������A�C���x���g����\������
	if (GameManager::GetGameManager()->GetInventory()->GetSelectMenuNum() == Inventory::MenuWindow_I::EMPTY && tnl::Input::IsKeyDownTrigger(eKeys::KB_ESCAPE)
		&& GameManager::GetGameManager()->GetPlayer()->GetPlayerControl()) {
			
			//SE�𗬂�
			SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
			//�C���x���g����\������
			GameManager::GetGameManager()->GetInventory()->InventoryMenuChange(Inventory::MenuWindow_I::FIRSTMENU);
			//plyer�𓮂��Ȃ��悤�ɂ���
			GameManager::GetGameManager()->GetPlayer()->PlayerControlChangeFlag();
	}

	//�����蔻��
	WorldMapCollision();

	return false;
}

//�C�x���g���N�����V�[�P���X
bool MapScene::seqEvent(float delta_time)
{
	if (sequence_.isStart()) {

		//�v���C���[�̓����𐧌�����
		GameManager::GetGameManager()->GetPlayer()->PlayerControlChangeFlag();

		//�C�x���g�t���O��false�̏ꍇ�t���O��؂�ւ���
		if (!EventManager::GetEventManager()->GetEventFlag()) {
			EventManager::GetEventManager()->EventFlagChange();
		}

	}

	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_0)) {

		if (!GameManager::GetGameManager()->GetPlayer()->GetPlayerControl()) {
			//�v���C���[�𓮂���悤�ɂ���
			GameManager::GetGameManager()->GetPlayer()->PlayerControlChangeFlag();
		}

		sequence_.change(&MapScene::seqMove);
	}

	//�C�x���g��������Npc�Ƃ̍X�V����
	EventManager::GetEventManager()->NpcEventUpdate(delta_time, npc_list,this);

	return false;
}

bool MapScene::seqChangeScene(float delta_time)
{
	//�J������ۊǍ��W�ɓ����
	GameManager::GetGameManager()->GetCamera()->SavePosition(GameManager::GetGameManager()->GetPlayer()->GetPlayerPos());

	return true;
}

//�V�[�P���X���O���Ő؂�ւ���
void MapScene::SequenceChangeMove()
{
	//��ɃC�x���g�}�l�[�W���[�ȂǂŃC�x���g��؂�ւ��鎞�Ɏg�p
	//�ʏ�ɖ߂�
	sequence_.change(&MapScene::seqMove);
}

//------------------------------------------------------------------------------------------------------------------------
//�}�b�v�`�b�v�֘A

void MapScene::WorldMapCollision()
{
	//���[���h�}�b�v��csv
	auto world_map_csv = CsvManager::GetCsvManager()->GetWorldMapCsv();

	//���[���h�}�b�v�ɂ���I�u�W�F�N�g��csv
	auto world_object_csv = CsvManager::GetCsvManager()->GetWorldMapObjectCsv();

	//�I�u�W�F�N�g�̓����蔻��
	for (auto& MapChip : map_chip_object) {

		int mapChipValue = static_cast<int>(world_object_csv[static_cast<unsigned>(MapChip->MapChipPos().y / MapChip->GetChipSize().x)][static_cast<unsigned>(MapChip->MapChipPos().x / MapChip->GetChipSize().x)]);
		if (colisionObjectValues.count(mapChipValue) > 0 && tnl::IsIntersectRect(GameManager::GetGameManager()->GetPlayer()->GetPlayerPos(), GameManager::GetGameManager()->GetPlayer()->GetPlayerSize().x, GameManager::GetGameManager()->GetPlayer()->GetPlayerSize().y, MapChip->MapChipPos(), map_chip_width_, map_chip_height_))
		{
			//���W�␳
			tnl::eCorrResRect Object_hit = tnl::CorrectPositionRect(
				GameManager::GetGameManager()->GetPlayer()->GetPlayerPrevPos(),
				MapChip->MapChipPos(),
				GameManager::GetGameManager()->GetPlayer()->GetPlayerSize(),
				MapChip->GetChipSize(),
				GameManager::GetGameManager()->GetPlayer()->GetPlayerPos(),
				MapChip->MapChipPos(),
				tnl::eCorrTypeRect::PWRFL_B,
				tnl::eCorrTypeRect::PWRFL_B , 0);

		}

		//���̃}�b�v�`�b�v
		const int MAP_CHIP_VILLAGE = 2035;

		//�{�X�̏�̃}�b�v�`�b�v
		const int MAP_CHIP_BOSS_CASTLE = 2090;

		//�ړ��K�i�̃}�b�v�`�b�v
		const int MAP_CHIP_STAIRS = 334;
		//��ڂ̈ړ��K�i�̃}�b�v�`�b�v
		const int MAP_CHIP_SECOND_STAIRS = 335;

		//���̃}�b�v�`�b�v
		const int MAP_CHIP_TOWN = 2032;

		//�鉺���̃}�b�v�`�b�v
		const int MAP_CHIP_CASTLE_TOWN = 2069;

		//�E�҂̕�̓���
		const int MAP_CHIP_GRAVE_ENTRANCE = 1247;

		if (ObjectValues.count(mapChipValue) > 0) {
			int Collision = tnl::IsIntersectRect(GameManager::GetGameManager()->GetPlayer()->GetPlayerPos(), GameManager::GetGameManager()->GetPlayer()->GetPlayerSize().x, GameManager::GetGameManager()->GetPlayer()->GetPlayerSize().y, MapChip->MapChipPos(), map_chip_width_, map_chip_height_);
			if (Collision) {

				//�I�u�W�F�N�g�ɓ���������Ԃ�Enter�L�[����������J�ڂ�����
				//��ɑ��⒬�A��A�{�X�̏�ȂǂɑJ�ڂ�����
				if (mapChipValue == MAP_CHIP_VILLAGE) {

						SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/idou.mp3", DX_PLAYTYPE_BACK);

						//���V�[���ɐ؂�ւ���
						auto mgr = SceneManager::GetSceneManager();

						//�t���O��false�̏ꍇtrue�ɕύX����(�V�[����J�ڂ������)
						if (!mgr->GetSceneFlag()) { mgr->SceneFlagChange(); }

						//�v���C���[���ꎞ�I�Ɍ����Ȃ�����
						GameManager::GetGameManager()->GetPlayer()->PlayerDisplayChange();

						mgr->changeScene(new InMapScene(VILLAGE_POS, InMapScene::InMapState::VILLAGE), 0.1f);
						GameManager::GetGameManager()->GetPlayer()->SetPlayerPosition(VILLAGE_POS);
						sequence_.change(&MapScene::seqChangeScene);

						break;
					
				}
				//�{�X�̏邾������
				else if (mapChipValue == MAP_CHIP_BOSS_CASTLE) {
						SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/idou.mp3", DX_PLAYTYPE_BACK);
						//�_���W�����V�[���ɐ؂�ւ���
						auto mgr = SceneManager::GetSceneManager();

						//�t���O��false�̏ꍇtrue�ɕύX����(�V�[����J�ڂ������)
						if (!mgr->GetSceneFlag()) { mgr->SceneFlagChange(); }

						//�v���C���[���ꎞ�I�Ɍ����Ȃ�����
						GameManager::GetGameManager()->GetPlayer()->PlayerDisplayChange();

						mgr->changeScene(new InMapScene(BOSS_CASTLE_POS, InMapScene::InMapState::BOSSCASTLE), 0.1f ,0.8f);

						//���W���{�X�̏ꏊ�ɃZ�b�g����
						GameManager::GetGameManager()->GetPlayer()->SetPlayerPosition(BOSS_CASTLE_POS);
						sequence_.change(&MapScene::seqChangeScene);

						break;
				}
				//�ړ��K�i��������
				else if (mapChipValue == MAP_CHIP_STAIRS || mapChipValue == MAP_CHIP_SECOND_STAIRS) {

						auto mgr = SceneManager::GetSceneManager();

						if (mgr->GetSceneFlag()) {
							//�t���O��؂�ւ��ăV�[����J�ڂ����Ȃ��悤�ɂ���
							mgr->SceneFlagChange();
						}

						//�t�F�[�h������
						mgr->FadeScene(FADE_OUT_TIME, FADE_IN_TIME);

						//�K�i�̍��W
						tnl::Vector3 STAIRS_POS;

						//��ڂ̊K�i��������
						if (mapChipValue == 334) {

							STAIRS_POS = { 1476, 172, 0 };
						}
						//��ڂ̊K�i��������
						else if (mapChipValue == 335) {

							STAIRS_POS = { 909, 202, 0 };
						}

						//���W���K�i�̏ꏊ�ɃZ�b�g����
						GameManager::GetGameManager()->GetPlayer()->SetPlayerPosition(STAIRS_POS);

						//���W���J�����̕ۊǍ��W�ɃZ�b�g����
						GameManager::GetGameManager()->GetCamera()->SavePosition(STAIRS_POS);

						break;
					
				}
				//���V�[���ւ̑J��
				else if (mapChipValue == MAP_CHIP_TOWN) {

					//���V�[���ɐ؂�ւ���
					auto mgr = SceneManager::GetSceneManager();

					//�t���O��false�̏ꍇtrue�ɕύX����(�V�[����J�ڂ������)
					if (!mgr->GetSceneFlag()) { mgr->SceneFlagChange(); }

					//�v���C���[���ꎞ�I�Ɍ����Ȃ�����
					GameManager::GetGameManager()->GetPlayer()->PlayerDisplayChange();

					mgr->changeScene(new InMapScene(TOWN_POS, InMapScene::InMapState::TOWN), 0.1f);
					GameManager::GetGameManager()->GetPlayer()->SetPlayerPosition(TOWN_POS);
					sequence_.change(&MapScene::seqChangeScene);

					break;
				}
				//�鉺���V�[���ւ̑J��
				else if (mapChipValue == MAP_CHIP_CASTLE_TOWN) {

					//�鉺���V�[���ɐ؂�ւ���
					auto mgr = SceneManager::GetSceneManager();

					//�t���O��false�̏ꍇtrue�ɕύX����(�V�[����J�ڂ������)
					if (!mgr->GetSceneFlag()) { mgr->SceneFlagChange(); }

					//�v���C���[���ꎞ�I�Ɍ����Ȃ�����
					GameManager::GetGameManager()->GetPlayer()->PlayerDisplayChange();

					mgr->changeScene(new InMapScene(CASTLE_TOWN_POS, InMapScene::InMapState::CASTLETOWN), 0.1f);
					GameManager::GetGameManager()->GetPlayer()->SetPlayerPosition(CASTLE_TOWN_POS);
					sequence_.change(&MapScene::seqChangeScene);

					break;
				}
				//�E�҂̕�V�[���ֈڍs����
				else if (mapChipValue == MAP_CHIP_GRAVE_ENTRANCE) {

					//�E�҂̕�V�[���ɐ؂�ւ���
					auto mgr = SceneManager::GetSceneManager();

					//�t���O��false�̏ꍇtrue�ɕύX����(�V�[����J�ڂ������)
					if (!mgr->GetSceneFlag()) { mgr->SceneFlagChange(); }

					//�v���C���[���ꎞ�I�Ɍ����Ȃ�����
					GameManager::GetGameManager()->GetPlayer()->PlayerDisplayChange();

					mgr->changeScene(new InMapScene(GRAVE_POS, InMapScene::InMapState::GRAVE), 0.1f);
					GameManager::GetGameManager()->GetPlayer()->SetPlayerPosition(GRAVE_POS);
					sequence_.change(&MapScene::seqChangeScene);

					break;

				}

			}
		}

	}
	//�n�`�̓����蔻��
	for (auto& worldMap_C : map_chips) {

		int mapChipValue = world_map_csv[static_cast<unsigned>(worldMap_C->MapChipPos().y / worldMap_C->GetChipSize().x)][static_cast<unsigned>(worldMap_C->MapChipPos().x / worldMap_C->GetChipSize().x)];
	
		//�G���J�E���g�p�̓����蔻��
		if (encount_kind.count(mapChipValue)&& tnl::IsIntersectRect(GameManager::GetGameManager()->GetPlayer()->GetPlayerPos(), GameManager::GetGameManager()->GetPlayer()->GetPlayerSize().x, GameManager::GetGameManager()->GetPlayer()->GetPlayerSize().y, worldMap_C->MapChipPos(), map_chip_width_, map_chip_height_)) {

			//���邢����
			const int LUMINOUS_GRASSLAND = 2;

			//�����Â�����
			const int TWILIGHT_GRASSLAND = 0;

			//�Â�����
			const int SHADOW_GRASSLAND = 3;

			//�ጴ
			const int SNOW_FIELD = 1320;

			//�r��
			const int WILDERNESS[2] = { 1 , 6 };

			//�{�X�G���A
			const int BOSS_AREA = 5;

			//map��̂ǂ��ɋ��邩�ɂ���ēG��ς���
			//�����ɂ���ꍇ
			if (mapChipValue == LUMINOUS_GRASSLAND) {

				//�ŏ��̓G��ID��n��
				enemy_id = 1;

				//mapChip�ɉ����Ĕw�i��ς���
				background_hdl = ResourceManager::GetResourceManager()->LoadGraphEX("graphics/haikei/battle_field_01.jpg");
			}
			else if (mapChipValue == TWILIGHT_GRASSLAND || mapChipValue == SHADOW_GRASSLAND) {

				//�����̓G��ID��n��
				enemy_id = 2;

				//mapChip�ɉ����Ĕw�i��ς���
				background_hdl = ResourceManager::GetResourceManager()->LoadGraphEX("graphics/haikei/battle_field_02.jpg");
			}
			//�r���ɂ���ꍇ
			else if (mapChipValue == WILDERNESS[0] || mapChipValue == WILDERNESS[1]) {

				//�r���ɂ���G��ID��n��
				enemy_id = 3;
 
				//mapChip�ɉ����Ĕw�i��ς���
				background_hdl = ResourceManager::GetResourceManager()->LoadGraphEX("graphics/haikei/battle_field_03.jpg");
			}
			//�ጴ�ɂ���ꍇ
			else if (mapChipValue == SNOW_FIELD) {

				//�r���ɂ���G��ID��n��
				enemy_id = 4;

				//mapChip�ɉ����Ĕw�i��ς���
				background_hdl = ResourceManager::GetResourceManager()->LoadGraphEX("graphics/haikei/pipo-battlebg005b.jpg");

			}
			//�{�X�G���A�̏ꍇ
			else if (mapChipValue == BOSS_AREA) {

				//�{�X�G���A�ɂ���G��ID��n��
				enemy_id = 5;

				//mapChip�ɉ����Ĕw�i��ς���
				background_hdl = ResourceManager::GetResourceManager()->LoadGraphEX("graphics/haikei/pipo-battlebg014b.png");
			}
		}
	}

}

//�G���J�E���g
bool MapScene::ChackEncount(const int& step)
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
	auto world_map_csv = CsvManager::GetCsvManager()->GetWorldMapCsv();

	//���[���h�}�b�v�ɂ���I�u�W�F�N�g��csv
	auto world_object_csv = CsvManager::GetCsvManager()->GetWorldMapObjectCsv();

	map_chip_all_size_ = map_chip_x_size_ * map_chip_y_size_;

	LoadDivGraph(gpc_map_chip_hdls_pass_.c_str(),
		map_chip_all_size_,
		map_chip_x_size_,
		map_chip_y_size_,
		map_chip_width_,
		map_chip_height_,
		gpc_map_chip_hdls_);

	//�����̃}�b�v�`�b�v�̓ǂݍ���
	for (int i = 0; i < world_map_csv.size(); ++i) {
		for (int k = 0; k < world_map_csv[i].size(); ++k) {
			if (-1 == world_map_csv[i][k]) continue;
			tnl::Vector3 pos;
			int gpc_hdl;
			pos.x = { static_cast<float>(mapchip->GetChipSize().x / koni::Numeric::DIVIDER_TWO + k * mapchip->GetChipSize().x) };
			pos.y = { static_cast<float>(i * mapchip->GetChipSize().x) };
			gpc_hdl = gpc_map_chip_hdls_[world_map_csv[i][k]];

			map_chips.emplace_back(std::make_shared<MapChip>(pos, gpc_hdl));

		}
	}

	//�I�u�W�F�N�g�̃}�b�v�`�b�v�̓ǂݍ���
	for (int i = 0; i < world_object_csv.size(); ++i) {
		for (int k = 0; k < world_object_csv[i].size(); ++k) {
			if (-1 == world_object_csv[i][k])continue;
			tnl::Vector3 pos;
			int gpc_hdl;
			pos.x = { static_cast<float>(mapchip->GetChipSize().x / koni::Numeric::DIVIDER_TWO + k * mapchip->GetChipSize().x) };
			pos.y = { static_cast<float>(i * mapchip->GetChipSize().x) };
			gpc_hdl = gpc_map_chip_hdls_[world_object_csv[i][k]];

			map_chip_object.emplace_back(std::make_shared<MapChip>(pos, gpc_hdl));
		}
	}

}