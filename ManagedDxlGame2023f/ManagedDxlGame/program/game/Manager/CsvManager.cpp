#include "CsvManager.h"

CsvManager* CsvManager::getCsvManager()
{
	static CsvManager* p_instance = nullptr;

	if (!p_instance)p_instance = new CsvManager();
	return p_instance;
}

CsvManager::CsvManager()
{

}
