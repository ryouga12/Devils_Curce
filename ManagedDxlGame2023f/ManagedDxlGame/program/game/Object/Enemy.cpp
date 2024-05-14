#include "Enemy.h"

Enemy::Enemy()
{
	//csvの読み込み	
	Enemy_Csv_Array = tnl::LoadCsv<tnl::CsvCell>("csv/Enemy.csv");

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
		Enemy_Status_Type.SetEnemyName(Enemy_Csv_Array[y][0].getString());
		//csvからidの取得
		Enemy_Status_Type.SetEnemyId(Enemy_Csv_Array[y][1].getInt());
		//csvからhpの取得
		Enemy_Status_Type.SetEnemyHp(Enemy_Csv_Array[y][2].getInt());
		//csvから攻撃力の取得
		Enemy_Status_Type.SetEnemyAttack(Enemy_Csv_Array[y][3].getInt());
		//csvから防御力の取得
		Enemy_Status_Type.SetEnemyDefance(Enemy_Csv_Array[y][4].getInt());
		//csvからすばやさの取得
		Enemy_Status_Type.SetEnemySpeed(Enemy_Csv_Array[y][5].getInt());
		//csvから経験値の取得
		Enemy_Status_Type.SetEnemyExpoint(Enemy_Csv_Array[y][6].getInt());
		//csvからお金の取得
		Enemy_Status_Type.SetEnemyMoney(Enemy_Csv_Array[y][7].getInt());
		//csvからghpathの取得
		Enemy_Status_Type.SetGhdl(Enemy_Csv_Array[y][8].getString());
		//csvから炎耐性値を取得
		Enemy_Status_Type.SetFireResist(Enemy_Csv_Array[y][9].getFloat());
		//csvから氷耐性値の取得
		Enemy_Status_Type.SetIceResist(Enemy_Csv_Array[y][10].getFloat());
		//csvから雷耐性値の取得
		Enemy_Status_Type.SetThunderResist(Enemy_Csv_Array[y][11].getFloat());

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

//敵のIDによってドロップアイテムを変える
void Enemy::InitEnemyItemDrop(int EnemyID)
{
	//最初に配列を初期化する
	Enemy_Drop_Item.clear();

	switch (EnemyID) {

	case SLIM:

	//通常ドロップはポーション
	Enemy_Drop_Item.emplace_back(item->GetItemById(3));

	//レアドロップはスライムの鈴
	Enemy_Drop_Item.emplace_back(item->GetItemById(20));

	break;

	case BAT:

	//通常ドロップはポーション
	Enemy_Drop_Item.emplace_back(item->GetItemById(3));

	//レアドロップは革の盾
	Enemy_Drop_Item.emplace_back(item->GetItemById(2));

	break;

	case SNAKE:

	//通常ドロップはポーション
	Enemy_Drop_Item.emplace_back(item->GetItemById(3));

	//レアドロップはスネークの鈴
	Enemy_Drop_Item.emplace_back(item->GetItemById(21));
	
	break;

	case ARMORBEETLE:

	//通常ドロップはポーション
	Enemy_Drop_Item.emplace_back(item->GetItemById(4));

	//レアドロップはノービスブライドソード
	Enemy_Drop_Item.emplace_back(item->GetItemById(15));

	break;
		
	case MONSTERDOG:

	//通常ドロップはマジックポーション
	Enemy_Drop_Item.emplace_back(item->GetItemById(14));

	//レアドロップはハイポーション
	Enemy_Drop_Item.emplace_back(item->GetItemById(8));

	break;

	case BLOODBAT:

	//通常ドロップはマジックポーション
	Enemy_Drop_Item.emplace_back(item->GetItemById(14));
	
	//レアドロップは木のハンマー
	Enemy_Drop_Item.emplace_back(item->GetItemById(5));

	break;
		
	case TYRANTWORM:

	//通常ドロップはマジックポーション
	Enemy_Drop_Item.emplace_back(item->GetItemById(14));

	//通常ドロップはアタックポーション
	Enemy_Drop_Item.emplace_back(item->GetItemById(16));

	break;

	case MONSTERPLANT:

	//通常ドロップは木のハンマー
	Enemy_Drop_Item.emplace_back(item->GetItemById(5));

	//レアドロップはプラントエキス(回復アイテム)
	Enemy_Drop_Item.emplace_back(item->GetItemById(17));

	break;

	case GOBLIN:
	
	//通常ドロップはポーション
	Enemy_Drop_Item.emplace_back(item->GetItemById(3));

	//レアドロップはゴブリンの鈴
	Enemy_Drop_Item.emplace_back(item->GetItemById(22));

	break;

	case GHOSTMUSH:

	//通常ドロップはポーション
	Enemy_Drop_Item.emplace_back(item->GetItemById(3));

	//レアドロップはプラントエキス(回復アイテム)
	Enemy_Drop_Item.emplace_back(item->GetItemById(17));

	break;

	case SALAMANDER:

	//通常ドロップはアタックポーション
	Enemy_Drop_Item.emplace_back(item->GetItemById(16));

	//レアドロップは鱗の鎧
	Enemy_Drop_Item.emplace_back(item->GetItemById(19));

	break;

	case MOKUJIN:

	//通常ドロップは木の剣
	Enemy_Drop_Item.emplace_back(item->GetItemById(4));

	//レアドロップはポーション
	Enemy_Drop_Item.emplace_back(item->GetItemById(3));

	break;

	case CATKNIGHT:

	//通常ドロップは銅の剣
	Enemy_Drop_Item.emplace_back(item->GetItemById(0));

	//レアドロップは冒険者の刀
	Enemy_Drop_Item.emplace_back(item->GetItemById(10));

	break;

	case PUMOKINGHOST:

	//通常ドロップはかぼちゃの盾
	Enemy_Drop_Item.emplace_back(item->GetItemById(23));

	//レアドロップはゴーストマッシュの鈴
	Enemy_Drop_Item.emplace_back(item->GetItemById(24));

	break;

	case NEOSLIM:

	//通常ドロップはポーション
	Enemy_Drop_Item.emplace_back(item->GetItemById(3));

	//レアドロップはハイポーション
	Enemy_Drop_Item.emplace_back(item->GetItemById(8));

	break;

	case WOODWALKER:

	//通常ドロップは木の盾
	Enemy_Drop_Item.emplace_back(item->GetItemById(25));

	//レアドロップはプラントエキス(回復アイテム)
	Enemy_Drop_Item.emplace_back(item->GetItemById(17));

	break;

	case BIGGOBLIN:

	//通常ドロップは棍棒
	Enemy_Drop_Item.emplace_back(item->GetItemById(18));

	//レアドロップは鉄の棍棒
	Enemy_Drop_Item.emplace_back(item->GetItemById(25));

	break;

	case GOLDREAPER:

	//通常ドロップはアタックポーション
	Enemy_Drop_Item.emplace_back(item->GetItemById(18));

	//レアドロップはリーパーの鈴
	Enemy_Drop_Item.emplace_back(item->GetItemById(27));

	break;

	case BLOODHOUND:

	//通常ドロップはハイポーション
	Enemy_Drop_Item.emplace_back(item->GetItemById(8));

	//レアドロップは毛皮のコート
	Enemy_Drop_Item.emplace_back(item->GetItemById(28));

	break;

	case LITTLEDEMON:

	//通常ドロップは木の棒
	Enemy_Drop_Item.emplace_back(item->GetItemById(6));

	//レアドロップはデーモンの角
	Enemy_Drop_Item.emplace_back(item->GetItemById(29));

	break;

	}

}
