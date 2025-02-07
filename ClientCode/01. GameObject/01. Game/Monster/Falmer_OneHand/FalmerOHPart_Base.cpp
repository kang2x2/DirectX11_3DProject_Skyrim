#include "framework.h"
#include "FalmerOHPart_Base.h"

#include "GameInstance.h"

CFalmerOHPart_Base::CFalmerOHPart_Base(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

CFalmerOHPart_Base::CFalmerOHPart_Base(const CFalmerOHPart_Base& rhs)
	: CGameObject(rhs)
{
}

CBone* CFalmerOHPart_Base::Get_SocketBonePtr(const char* _pBoneName)
{
	if (m_pModelCom == nullptr)
		return nullptr;

	return m_pModelCom->Get_BonePtr(_pBoneName);
}

_float4x4 CFalmerOHPart_Base::Get_SocketPivotMatrix()
{
	if (m_pModelCom == nullptr)
		return _float4x4();

	return m_pModelCom->Get_PivotMatrix();
}

HRESULT CFalmerOHPart_Base::Initialize_ProtoType()
{
	return S_OK;
}

HRESULT CFalmerOHPart_Base::Initialize_Clone(void* _pArg)
{
	if (_pArg != nullptr)
	{
		PART_DESC* pPartDesc = (PART_DESC*)_pArg;
		m_pParent = pPartDesc->pParent;
		m_pParentTransform = pPartDesc->pParentTransform;
		Safe_AddRef(m_pParentTransform);
	}

	return S_OK;
}

void CFalmerOHPart_Base::PriorityTick(_float _fTimeDelta)
{
}

void CFalmerOHPart_Base::Tick(_float _fTimeDelta)
{
}

void CFalmerOHPart_Base::LateTick(_float _fTimeDelta)
{
}

HRESULT CFalmerOHPart_Base::Render()
{
	return S_OK;
}

HRESULT CFalmerOHPart_Base::Compute_RenderMatrix(_fmatrix ChildMatrix)
{
	XMStoreFloat4x4(&m_matWorld, ChildMatrix * m_pParentTransform->Get_WorldMatrix());

	return S_OK;
}

CFalmerOHPart_Base* CFalmerOHPart_Base::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	return nullptr;
}

CGameObject* CFalmerOHPart_Base::Clone(void* _pArg)
{
	return nullptr;
}

void CFalmerOHPart_Base::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pParentTransform);

}
