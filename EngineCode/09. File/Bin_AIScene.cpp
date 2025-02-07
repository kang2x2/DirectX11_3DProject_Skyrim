#include "Bin_AIScene.h"

CBin_AIScene::CBin_AIScene()
{
}

CBin_AIScene* CBin_AIScene::Create()
{
	return new CBin_AIScene();
}

void CBin_AIScene::Free()
{
	__super::Free();
}
