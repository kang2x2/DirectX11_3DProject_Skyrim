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

	//for (size_t i = 0; i < mVecNode.size(); ++i)
	//{
	//	Safe_Release(mVecNode[i].mName);
	//}
}
