#include "GlobalParamLoader.h"



GlobalParamLoader* GlobalParamLoader::Instance()
{
	static GlobalParamLoader instance;

	return &instance;
}