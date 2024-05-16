#include "Enemy.h"

Enemy::Enemy()
{
	//csvの読み込み	
	Enemy_Csv_Array = tnl::LoadCsv<std::string>("csv/Enemy.csv");

	//Enemyのステータスをロードする関数
	EnemyLoadTypeInfo();
	
	//Enemyの初期化
	EnemyInit();

	item = std::make_shared<Item>();
}

void Enemy::EnemyLoadTypeInfo()
{

	int max_num = Enemy_Csv_Array.size();

	EnemyStatus_Total_Num = max_num;

	for (int y = 1; y < max_num; y++) {
		
		//csvから名前の取得
		Enemy_Status_Type.SetEnemyName(Enemy_Csv_Array[y][0].c_str());
		//csvからidの取得
		Enemy_Status_Type.SetEnemyId(std::stoi(Enemy_Csv_Array[y][1].c_str()));
		//csvからhpの取得
		Enemy_Status_Type.SetEnemyHp(std::stoi(Enemy_Csv_Array[y][2].c_str()));
		//csvから攻撃力の取得
		Enemy_Status_Type.SetEnemyAttack(std::stoi(Enemy_Csv_Array[y][3].c_str()));
		//csvから防御力の取得
		Enemy_Status_Type.SetEnemyDefance(std::stoi(Enemy_Csv_Array[y][4].c_str()));
		//csvからすばやさの取得
		Enemy_Status_Type.SetEnemySpeed(std::stoi(Enemy_Csv_Array[y][5].c_str()));
		//csvから経験値の取得
		Enemy_Status_Type.SetEnemyExpoint(std::stoi(Enemy_Csv_Array[y][6].c_str()));
		//csvからお金の取得
		Enemy_Status_Type.SetEnemyMoney(std::stoi(Enemy_Csv_Array[y][7].c_str()));
		//csvからghpathの取得
		Enemy_Status_Type.SetGhdl(Enemy_Csv_Array[y][8].c_str());
		//csvから炎耐性値を取得
		Enemy_Status_Type.SetFireResist(std::stoi(Enemy_Csv_Array[y][9].c_str()));
		//csvから氷耐性値の取得
		Enemy_Status_Type.SetIceResist(std::stoi(Enemy_Csv_Array[y][10].c_str()));
		//csvから雷耐性値の取得
		Enemy_Status_Type.SetThunderResist(std::stoi( Enemy_Csv_Array[y][11].c_str()));
		//csvから通常ドロップのIDを取得
		Enemy_Status_Type.SetNomalDropID(std::stoi(Enemy_Csv_Array[y][12].c_str()));
		//csvからレアドロップのIDを取得
		Enemy_Status_Type.SetRareDoropID(std::stoi(Enemy_Csv_Array[y][13].c_str()));

		Enemy_Type_Array.emplace_back(Enemy_Status_Type);


	}



}

void Enemy::EnemyInit()
{
	//---敵の配列を入れる---//
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

	//エラー用の配列をクリアして初期化する
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
		// IDに対応するアイテムが見つからない場合は、適当なデフォルト値を返すかエラー処理を行うなど
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
	//エラー用の配列
	else {
		return Null_Array;
	}
}

//アイテムのドロップを決める
void Enemy::InitEnemyItemDrop(int EnemyID)
{
	Enemy_Drop_Item.clear();

	//配列のIDを比較して該当したIDをインベントリに格納する
	for (int i = 0; i < Enemy_Csv_Array.size(); i++) {
		if (EnemyID == std::stoi(Enemy_Csv_Array[i + 1][1].c_str())) {

			//インベントリにそれぞれに対応したアイテムを格納
			Enemy_Drop_Item.emplace_back(item->GetItemById(std::stoi(Enemy_Csv_Array[i + 1][12].c_str())));
			Enemy_Drop_Item.emplace_back(item->GetItemById(std::stoi(Enemy_Csv_Array[i + 1][13].c_str())));
			break;
		}
	}
}