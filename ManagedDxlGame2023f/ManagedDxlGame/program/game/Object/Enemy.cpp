#include "Enemy.h"

Enemy::Enemy()
{
	//csv�̓ǂݍ���	
	Enemy_Csv_Array = tnl::LoadCsv<std::string>("csv/Enemy.csv");

	//Enemy�̃X�e�[�^�X�����[�h����֐�
	EnemyLoadTypeInfo();
	
	//Enemy�̏�����
	EnemyInit();

	item = std::make_shared<Item>();
}

void Enemy::EnemyLoadTypeInfo()
{

	int max_num = Enemy_Csv_Array.size();

	EnemyStatus_Total_Num = max_num;

	for (int y = 1; y < max_num; y++) {
		
		//csv���疼�O�̎擾
		Enemy_Status_Type.SetEnemyName(Enemy_Csv_Array[y][0].c_str());
		//csv����id�̎擾
		Enemy_Status_Type.SetEnemyId(std::stoi(Enemy_Csv_Array[y][1].c_str()));
		//csv����hp�̎擾
		Enemy_Status_Type.SetEnemyHp(std::stoi(Enemy_Csv_Array[y][2].c_str()));
		//csv����U���͂̎擾
		Enemy_Status_Type.SetEnemyAttack(std::stoi(Enemy_Csv_Array[y][3].c_str()));
		//csv����h��͂̎擾
		Enemy_Status_Type.SetEnemyDefance(std::stoi(Enemy_Csv_Array[y][4].c_str()));
		//csv���炷�΂₳�̎擾
		Enemy_Status_Type.SetEnemySpeed(std::stoi(Enemy_Csv_Array[y][5].c_str()));
		//csv����o���l�̎擾
		Enemy_Status_Type.SetEnemyExpoint(std::stoi(Enemy_Csv_Array[y][6].c_str()));
		//csv���炨���̎擾
		Enemy_Status_Type.SetEnemyMoney(std::stoi(Enemy_Csv_Array[y][7].c_str()));
		//csv����ghpath�̎擾
		Enemy_Status_Type.SetGhdl(Enemy_Csv_Array[y][8].c_str());
		//csv���牊�ϐ��l���擾
		Enemy_Status_Type.SetFireResist(std::stoi(Enemy_Csv_Array[y][9].c_str()));
		//csv����X�ϐ��l�̎擾
		Enemy_Status_Type.SetIceResist(std::stoi(Enemy_Csv_Array[y][10].c_str()));
		//csv���痋�ϐ��l�̎擾
		Enemy_Status_Type.SetThunderResist(std::stoi( Enemy_Csv_Array[y][11].c_str()));
		//csv����ʏ�h���b�v��ID���擾
		Enemy_Status_Type.SetNomalDropID(std::stoi(Enemy_Csv_Array[y][12].c_str()));
		//csv���烌�A�h���b�v��ID���擾
		Enemy_Status_Type.SetRareDoropID(std::stoi(Enemy_Csv_Array[y][13].c_str()));

		Enemy_Type_Array.emplace_back(Enemy_Status_Type);


	}



}

void Enemy::EnemyInit()
{
	//---�G�̔z�������---//
	First_Area_Enemy.emplace_back(GetEnemyStatus(0));
	First_Area_Enemy.emplace_back(GetEnemyStatus(1));
	First_Area_Enemy.emplace_back(GetEnemyStatus(2));
	First_Area_Enemy.emplace_back(GetEnemyStatus(3));
	First_Area_Enemy.emplace_back(GetEnemyStatus(4));

	Second_Area_Enemy.emplace_back(GetEnemyStatus(5));
	Second_Area_Enemy.emplace_back(GetEnemyStatus(6));
	Second_Area_Enemy.emplace_back(GetEnemyStatus(7));
	Second_Area_Enemy.emplace_back(GetEnemyStatus(8));
	Second_Area_Enemy.emplace_back(GetEnemyStatus(9));

	Third_Area_Enemy.emplace_back(GetEnemyStatus(10));
	Third_Area_Enemy.emplace_back(GetEnemyStatus(11));
	Third_Area_Enemy.emplace_back(GetEnemyStatus(12));
	Third_Area_Enemy.emplace_back(GetEnemyStatus(13));
	Third_Area_Enemy.emplace_back(GetEnemyStatus(14));

	Boss_Area_Enemy.emplace_back(GetEnemyStatus(15));
	Boss_Area_Enemy.emplace_back(GetEnemyStatus(16));
	Boss_Area_Enemy.emplace_back(GetEnemyStatus(17));
	Boss_Area_Enemy.emplace_back(GetEnemyStatus(18));
	Boss_Area_Enemy.emplace_back(GetEnemyStatus(19));

	//�G���[�p�̔z����N���A���ď���������
	Null_Array.clear();
}

Enemy::EnemyStatus Enemy::GetEnemyStatus(int id) const
{
	auto it = std::find_if(Enemy_Type_Array.begin(), Enemy_Type_Array.end(), [id]
	(const EnemyStatus& status) {return status.getEnemyId() == id; });

	if (it != Enemy_Type_Array.end()) {
		return *it;
	}
	else {
		// ID�ɑΉ�����A�C�e����������Ȃ��ꍇ�́A�K���ȃf�t�H���g�l��Ԃ����G���[�������s���Ȃ�
		return EnemyStatus{};
	}
}

std::vector<Enemy::EnemyStatus> Enemy::GetEnemyArray(int id)
{
	if (id == 1) {
		return First_Area_Enemy;
	}
	else if (id == 2) {
		return Second_Area_Enemy;
	}
	else if (id == 3) {
		return Third_Area_Enemy;
	}
	else if (id == 4) {
		return Boss_Area_Enemy;
	}
	//�G���[�p�̔z��
	else {
		return Null_Array;
	}
}

//�A�C�e���̃h���b�v�����߂�
void Enemy::InitEnemyItemDrop(int EnemyID)
{
	Enemy_Drop_Item.clear();

	//�z���ID���r���ĊY������ID���C���x���g���Ɋi�[����
	for (int i = 0; i < Enemy_Csv_Array.size(); i++) {
		if (EnemyID == std::stoi(Enemy_Csv_Array[i + 1][1].c_str())) {

			//�C���x���g���ɂ��ꂼ��ɑΉ������A�C�e�����i�[
			Enemy_Drop_Item.emplace_back(item->GetItemById(std::stoi(Enemy_Csv_Array[i + 1][12].c_str())));
			Enemy_Drop_Item.emplace_back(item->GetItemById(std::stoi(Enemy_Csv_Array[i + 1][13].c_str())));
			break;
		}
	}
}