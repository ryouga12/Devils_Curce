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

//�A�C�e���̃h���b�v�����߂�
void Enemy::InitEnemyItemDrop(int EnemyID)
{
	Enemy_Drop_Item.clear();

	//�z���ID���r���ĊY������ID���C���x���g���Ɋi�[����
	for (int i = 0; i < Enemy_Csv_Array.size(); i++) {

		//�z���1�s�ڂ͐����̈�+1���Ċm�F���s��
		if (EnemyID == std::stoi(Enemy_Csv_Array[i + 1][1].c_str())) {

			//�C���x���g���ɂ��ꂼ��ɑΉ������A�C�e�����i�[
			Enemy_Drop_Item.emplace_back(item->GetItemById(std::stoi(Enemy_Csv_Array[i + 1][12].c_str())));
 			Enemy_Drop_Item.emplace_back(item->GetItemById(std::stoi(Enemy_Csv_Array[i + 1][13].c_str())));
			break;
		}
	}

}

//�G�̔z�������������
//������ID�͂ǂ̃}�b�v�`�b�v�ɋ��邩�Ŕz��ɓ����G�̏������߂�
void Enemy::InitEnemyArray(int id)
{
	//�G�̔z����N���A����
	//�G�̔z�����x�N���A���ē��������擾�ł��Ȃ��悤�ɂ���
	Enemy_Array.clear();

	if (id == 1) {
		for (int i = 0; i < Enemy_num; i++) {
			Enemy_Array.emplace_back(GetEnemyStatus(i));
		}
	}
	else if (id == 2) {

		for (int i = 6; i < Enemy_Csv_Array.size(); i++) {

			//�G�ɂ����ID���擾����
			if (Enemy_num > Enemy_Array.size()) {
				auto enmeyid = GetEnemyStatus(std::stoi(Enemy_Csv_Array[i][1].c_str()));

				//�G��z��Ɋi�[����
				Enemy_Array.emplace_back(GetEnemyStatus(enmeyid.getEnemyId()));
			}
			//�G�̍ő�̐����������珈���𔲂���
			else {
				break;
			}
		}
	}
	else if (id == 3) {
		for (int i = 10; i < Enemy_num; i++) {
			Enemy_Array.emplace_back(GetEnemyStatus(i));
		}
	}
	else if (id == 4) {
		for (int i = 15; i < Enemy_num; i++) {
			Enemy_Array.emplace_back(GetEnemyStatus(i));
		}
	}
}
