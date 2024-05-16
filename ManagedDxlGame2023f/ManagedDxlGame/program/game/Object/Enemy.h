#pragma once
#include "../../dxlib_ext/dxlib_ext.h"
#include"../Item/ItemBase.h"
#include"../Item/Item.h"

class ItemBase;

class Enemy {
public:

	Enemy();

	//Enemy�̃X�e�[�^�X
	struct EnemyStatus
	{
	public:

		//name��Ԃ��Q�b�^�[
		std::string getEnemyString() const {
			return name;
		}
		//ID��Ԃ��Q�b�^�[
		int getEnemyId() const {
			return id;
		}
		//Hp��Ԃ��Q�b�^�[
		int getEnemyHp() const{
			return hp;
		}
		//Attack��Ԃ��Q�b�^�[
		int getEnemyAttack() const {
			return Attack;
		}
		//Defance��Ԃ��Q�b�^�[
		int getEnemyDefance() const {
			return Defance;
		}
		//Speed��Ԃ��Q�b�^�[
		int getEnemySpeed() const {
			return Speed;
		}
		//�o���l��Ԃ��Q�b�^�[
		int getEnemyExpoint() const {
			return ex_point;
		}
		//money��Ԃ��Q�b�^�[
		int getEnemyMoney() const {
			return money;
		}
		//ghdl��Ԃ��Q�b�^�[
		const std::string& getEnemyGhdl()const {
			return gh;
		}
		//���ꂼ��̑ϐ���Ԃ��Q�b�^�[
		float getFireResist() const {
			return FireResist;
		}
		float getIceResist() const {
			return IceResist;
		}
		float getThunderResist() const {
			return ThunderResist;
		}
		//�ʏ�h���b�v�̃A�C�e����ID��Ԃ�
		int getNomalDrop()const {
			return Nomal_Drop;
		}
		//���A�h���b�v�̃A�C�e��ID��Ԃ�
		int getRareDrop()const {
			return RareDrop;
		}

		//---�Z�b�^�[---//

		//�G�̖��O���Z�b�g����
		void SetEnemyName(std::string newname) {
			name = newname;
		}
		//�G��ID���Z�b�g����
		void SetEnemyId(int newid){
			id = newid;
		}
		//�G��Hp���Z�b�g����
		void SetEnemyHp(int newhp) {
			hp = newhp;
		}
		//�G��Attack���Z�b�g����
		void SetEnemyAttack(int newattack) {
			Attack = newattack;
		}
		//�G��Defance���Z�b�g����
		void SetEnemyDefance(int newdefance) {
			Defance = newdefance;
		}
		//�G��Speed���Z�b�g����
		void SetEnemySpeed(int newspeed) {
			Speed = newspeed;
		}
		//�G�̌o���l���Z�b�g����
		void SetEnemyExpoint(int newexpoint) {
			ex_point = newexpoint;
		}
		//�G��money���Z�b�g����
		void SetEnemyMoney(int newmoney) {
			money = newmoney;
		}
		//�G�̃n���h�����Z�b�g����
		void SetGhdl(std::string newgh) {
			gh = newgh;
		}
		//�G�̑ϐ����Z�b�g����
		void SetFireResist(float newresist) {
			FireResist = newresist;
		}
		void SetIceResist(float newresist) {
			IceResist = newresist;
		}
		void SetThunderResist(float newresist) {
			ThunderResist = newresist;
		}
		//�G�̒ʏ�h���b�v��ID���Z�b�g����
		void SetNomalDropID(int newID) {
			Nomal_Drop = newID;
		}
		//�G�̃��A�h���b�v��ID���Z�b�g����
		void SetRareDoropID(int newID) {
			RareDrop = newID;
		}

	private:

		std::string name;
		int id;
		int hp;
		int Attack;
		int Defance;
		int Speed;
		int ex_point;
		int money;
		std::string gh;
		float FireResist;
		float IceResist;
		float ThunderResist;
		int Nomal_Drop;
		int RareDrop;

	};

	//Enemy�̃X�e�[�^�X��ǂݍ��ވׂ̊֐�
	void EnemyLoadTypeInfo();

	//�G�̏�����
	void EnemyInit();

	//�G�̂̃X�e�[�^�X��id���擾����
	EnemyStatus GetEnemyStatus(int id) const;
	
	//�G�̔z����󂯎��ׂ̃Q�b�^�[
	std::vector<EnemyStatus> GetEnemyArray(int id); 

	//�G��ID�ɂ���ăh���b�v�̃A�C�e����؂�ւ���
	void InitEnemyItemDrop(int EnemyID);

	//�G���������Ă���A�C�e���̔z����擾����
	std::vector<ItemBase>& getItemDropArray() {
		return Enemy_Drop_Item;
	}

private:

	//Enemy�̃X�e�[�^�X�����Ă����ϐ�
	EnemyStatus Enemy_Status_Type;

	//�\���̂̑���
	int EnemyStatus_Total_Num;

	//�\���̂��i�[����z��
	std::vector<EnemyStatus>Enemy_Type_Array;

	//csv�����Ă����z��
	std::vector<std::vector<std::string>>Enemy_Csv_Array;

	//---�G�����Ă����z��---//

	//�ŏ��̃G���A�ŏo�Ă���G
	std::vector<EnemyStatus>First_Area_Enemy;
	//���̃G���A�ŏo�Ă���G
	std::vector<EnemyStatus>Second_Area_Enemy;
	//����Ɏ��̃G���A�ŏo�Ă���G
	std::vector<EnemyStatus>Third_Area_Enemy;
	//�{�X�̃G���A�ŏo�ė���G
	std::vector<EnemyStatus>Boss_Area_Enemy;
	//�G���[���ɕԂ��p�̔z��
	std::vector<EnemyStatus>Null_Array;

	//---�G�̃h���b�v�A�C�e�����i�[�������z��---//

	std::vector<ItemBase>Enemy_Drop_Item;

	//�G��ID�̗񋓑�
	enum {
		SLIM,
		BAT,
		SNAKE,
		ARMORBEETLE,
		MONSTERDOG,
		BLOODBAT,
		TYRANTWORM,
		MONSTERPLANT,
		GOBLIN,
		GHOSTMUSH,
		SALAMANDER,
		MOKUJIN,
		CATKNIGHT,
		PUMOKINGHOST,
		NEOSLIM,
		WOODWALKER,
		BIGGOBLIN,
		GOLDREAPER,
		BLOODHOUND,
		LITTLEDEMON
	};

	//�A�C�e���̃|�C���^
	Shared<Item>item = nullptr;

};