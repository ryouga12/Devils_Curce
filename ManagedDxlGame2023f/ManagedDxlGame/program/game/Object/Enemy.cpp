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

//アイテムのドロップを決める
void Enemy::InitEnemyItemDrop(int EnemyID)
{
	Enemy_Drop_Item.clear();

	//配列のIDを比較して該当したIDをインベントリに格納する
	for (int i = 0; i < Enemy_Csv_Array.size(); i++) {

		//配列の1行目は説明の為+1して確認を行う
		if (EnemyID == std::stoi(Enemy_Csv_Array[i + 1][1].c_str())) {

			//インベントリにそれぞれに対応したアイテムを格納
			Enemy_Drop_Item.emplace_back(item->GetItemById(std::stoi(Enemy_Csv_Array[i + 1][12].c_str())));
 			Enemy_Drop_Item.emplace_back(item->GetItemById(std::stoi(Enemy_Csv_Array[i + 1][13].c_str())));
			break;
		}
	}

}

//敵の配列を初期化する
//引数のIDはどのマップチップに居るかで配列に入れる敵の情報を決める
void Enemy::InitEnemyArray(int id)
{
	//敵の配列をクリアする
	//敵の配列を一度クリアして同じ情報を取得できないようにする
	Enemy_Array.clear();

	if (id == 1) {
		for (int i = 0; i < Enemy_num; i++) {
			Enemy_Array.emplace_back(GetEnemyStatus(i));
		}
	}
	else if (id == 2) {

		for (int i = 6; i < Enemy_Csv_Array.size(); i++) {

			//敵によってIDを取得する
			if (Enemy_num > Enemy_Array.size()) {
				auto enmeyid = GetEnemyStatus(std::stoi(Enemy_Csv_Array[i][1].c_str()));

				//敵を配列に格納する
				Enemy_Array.emplace_back(GetEnemyStatus(enmeyid.getEnemyId()));
			}
			//敵の最大の数を上回ったら処理を抜ける
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
