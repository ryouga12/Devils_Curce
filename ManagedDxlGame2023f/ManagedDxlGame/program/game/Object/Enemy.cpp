#include "Enemy.h"

Enemy::Enemy()
{
	//csv�̓ǂݍ���	
	Enemy_Csv_Array = tnl::LoadCsv<tnl::CsvCell>("csv/Enemy.csv");

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
		Enemy_Status_Type.SetEnemyName(Enemy_Csv_Array[y][0].getString());
		//csv����id�̎擾
		Enemy_Status_Type.SetEnemyId(Enemy_Csv_Array[y][1].getInt());
		//csv����hp�̎擾
		Enemy_Status_Type.SetEnemyHp(Enemy_Csv_Array[y][2].getInt());
		//csv����U���͂̎擾
		Enemy_Status_Type.SetEnemyAttack(Enemy_Csv_Array[y][3].getInt());
		//csv����h��͂̎擾
		Enemy_Status_Type.SetEnemyDefance(Enemy_Csv_Array[y][4].getInt());
		//csv���炷�΂₳�̎擾
		Enemy_Status_Type.SetEnemySpeed(Enemy_Csv_Array[y][5].getInt());
		//csv����o���l�̎擾
		Enemy_Status_Type.SetEnemyExpoint(Enemy_Csv_Array[y][6].getInt());
		//csv���炨���̎擾
		Enemy_Status_Type.SetEnemyMoney(Enemy_Csv_Array[y][7].getInt());
		//csv����ghpath�̎擾
		Enemy_Status_Type.SetGhdl(Enemy_Csv_Array[y][8].getString());
		//csv���牊�ϐ��l���擾
		Enemy_Status_Type.SetFireResist(Enemy_Csv_Array[y][9].getFloat());
		//csv����X�ϐ��l�̎擾
		Enemy_Status_Type.SetIceResist(Enemy_Csv_Array[y][10].getFloat());
		//csv���痋�ϐ��l�̎擾
		Enemy_Status_Type.SetThunderResist(Enemy_Csv_Array[y][11].getFloat());

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

//�G��ID�ɂ���ăh���b�v�A�C�e����ς���
void Enemy::InitEnemyItemDrop(int EnemyID)
{
	//�ŏ��ɔz�������������
	Enemy_Drop_Item.clear();

	switch (EnemyID) {

	case SLIM:

	//�ʏ�h���b�v�̓|�[�V����
	Enemy_Drop_Item.emplace_back(item->GetItemById(3));

	//���A�h���b�v�̓X���C���̗�
	Enemy_Drop_Item.emplace_back(item->GetItemById(20));

	break;

	case BAT:

	//�ʏ�h���b�v�̓|�[�V����
	Enemy_Drop_Item.emplace_back(item->GetItemById(3));

	//���A�h���b�v�͊v�̏�
	Enemy_Drop_Item.emplace_back(item->GetItemById(2));

	break;

	case SNAKE:

	//�ʏ�h���b�v�̓|�[�V����
	Enemy_Drop_Item.emplace_back(item->GetItemById(3));

	//���A�h���b�v�̓X�l�[�N�̗�
	Enemy_Drop_Item.emplace_back(item->GetItemById(21));
	
	break;

	case ARMORBEETLE:

	//�ʏ�h���b�v�̓|�[�V����
	Enemy_Drop_Item.emplace_back(item->GetItemById(4));

	//���A�h���b�v�̓m�[�r�X�u���C�h�\�[�h
	Enemy_Drop_Item.emplace_back(item->GetItemById(15));

	break;
		
	case MONSTERDOG:

	//�ʏ�h���b�v�̓}�W�b�N�|�[�V����
	Enemy_Drop_Item.emplace_back(item->GetItemById(14));

	//���A�h���b�v�̓n�C�|�[�V����
	Enemy_Drop_Item.emplace_back(item->GetItemById(8));

	break;

	case BLOODBAT:

	//�ʏ�h���b�v�̓}�W�b�N�|�[�V����
	Enemy_Drop_Item.emplace_back(item->GetItemById(14));
	
	//���A�h���b�v�͖؂̃n���}�[
	Enemy_Drop_Item.emplace_back(item->GetItemById(5));

	break;
		
	case TYRANTWORM:

	//�ʏ�h���b�v�̓}�W�b�N�|�[�V����
	Enemy_Drop_Item.emplace_back(item->GetItemById(14));

	//�ʏ�h���b�v�̓A�^�b�N�|�[�V����
	Enemy_Drop_Item.emplace_back(item->GetItemById(16));

	break;

	case MONSTERPLANT:

	//�ʏ�h���b�v�͖؂̃n���}�[
	Enemy_Drop_Item.emplace_back(item->GetItemById(5));

	//���A�h���b�v�̓v�����g�G�L�X(�񕜃A�C�e��)
	Enemy_Drop_Item.emplace_back(item->GetItemById(17));

	break;

	case GOBLIN:
	
	//�ʏ�h���b�v�̓|�[�V����
	Enemy_Drop_Item.emplace_back(item->GetItemById(3));

	//���A�h���b�v�̓S�u�����̗�
	Enemy_Drop_Item.emplace_back(item->GetItemById(22));

	break;

	case GHOSTMUSH:

	//�ʏ�h���b�v�̓|�[�V����
	Enemy_Drop_Item.emplace_back(item->GetItemById(3));

	//���A�h���b�v�̓v�����g�G�L�X(�񕜃A�C�e��)
	Enemy_Drop_Item.emplace_back(item->GetItemById(17));

	break;

	case SALAMANDER:

	//�ʏ�h���b�v�̓A�^�b�N�|�[�V����
	Enemy_Drop_Item.emplace_back(item->GetItemById(16));

	//���A�h���b�v�͗؂̊Z
	Enemy_Drop_Item.emplace_back(item->GetItemById(19));

	break;

	case MOKUJIN:

	//�ʏ�h���b�v�͖؂̌�
	Enemy_Drop_Item.emplace_back(item->GetItemById(4));

	//���A�h���b�v�̓|�[�V����
	Enemy_Drop_Item.emplace_back(item->GetItemById(3));

	break;

	case CATKNIGHT:

	//�ʏ�h���b�v�͓��̌�
	Enemy_Drop_Item.emplace_back(item->GetItemById(0));

	//���A�h���b�v�͖`���҂̓�
	Enemy_Drop_Item.emplace_back(item->GetItemById(10));

	break;

	case PUMOKINGHOST:

	//�ʏ�h���b�v�͂��ڂ���̏�
	Enemy_Drop_Item.emplace_back(item->GetItemById(23));

	//���A�h���b�v�̓S�[�X�g�}�b�V���̗�
	Enemy_Drop_Item.emplace_back(item->GetItemById(24));

	break;

	case NEOSLIM:

	//�ʏ�h���b�v�̓|�[�V����
	Enemy_Drop_Item.emplace_back(item->GetItemById(3));

	//���A�h���b�v�̓n�C�|�[�V����
	Enemy_Drop_Item.emplace_back(item->GetItemById(8));

	break;

	case WOODWALKER:

	//�ʏ�h���b�v�͖؂̏�
	Enemy_Drop_Item.emplace_back(item->GetItemById(25));

	//���A�h���b�v�̓v�����g�G�L�X(�񕜃A�C�e��)
	Enemy_Drop_Item.emplace_back(item->GetItemById(17));

	break;

	case BIGGOBLIN:

	//�ʏ�h���b�v�͞��_
	Enemy_Drop_Item.emplace_back(item->GetItemById(18));

	//���A�h���b�v�͓S�̞��_
	Enemy_Drop_Item.emplace_back(item->GetItemById(25));

	break;

	case GOLDREAPER:

	//�ʏ�h���b�v�̓A�^�b�N�|�[�V����
	Enemy_Drop_Item.emplace_back(item->GetItemById(18));

	//���A�h���b�v�̓��[�p�[�̗�
	Enemy_Drop_Item.emplace_back(item->GetItemById(27));

	break;

	case BLOODHOUND:

	//�ʏ�h���b�v�̓n�C�|�[�V����
	Enemy_Drop_Item.emplace_back(item->GetItemById(8));

	//���A�h���b�v�͖є�̃R�[�g
	Enemy_Drop_Item.emplace_back(item->GetItemById(28));

	break;

	case LITTLEDEMON:

	//�ʏ�h���b�v�͖؂̖_
	Enemy_Drop_Item.emplace_back(item->GetItemById(6));

	//���A�h���b�v�̓f�[�����̊p
	Enemy_Drop_Item.emplace_back(item->GetItemById(29));

	break;

	}

}
