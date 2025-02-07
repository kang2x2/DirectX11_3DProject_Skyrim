#include "framework.h"
#include "FalmerUEPart_Base.h"

#include "GameInstance.h"

CFalmerUEPart_Base::CFalmerUEPart_Base(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

CFalmerUEPart_Base::CFalmerUEPart_Base(const CFalmerUEPart_Base& rhs)
	: CGameObject(rhs)
{
}

CBone* CFalmerUEPart_Base::Get_SocketBonePtr(const char* _pBoneName)
{
	if (m_pModelCom == nullptr)
		return nullptr;

	return m_pModelCom->Get_BonePtr(_pBoneName);
}

_float4x4 CFalmerUEPart_Base::Get_SocketPivotMatrix()
{
	if (m_pModelCom == nullptr)
		return _float4x4();

	return m_pModelCom->Get_PivotMatrix();
}

HRESULT CFalmerUEPart_Base::Initialize_ProtoType()
{
	return S_OK;
}

HRESULT CFalmerUEPart_Base::Initialize_Clone(void* _pArg)
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

void CFalmerUEPart_Base::PriorityTick(_float _fTimeDelta)
{
}

void CFalmerUEPart_Base::Tick(_float _fTimeDelta)
{
}

void CFalmerUEPart_Base::LateTick(_float _fTimeDelta)
{
}

HRESULT CFalmerUEPart_Base::Render()
{
	return S_OK;
}

HRESULT CFalmerUEPart_Base::Compute_RenderMatrix(_fmatrix ChildMatrix)
{
	XMStoreFloat4x4(&m_matWorld, ChildMatrix * m_pParentTransform->Get_WorldMatrix());

	return S_OK;
}

CFalmerUEPart_Base* CFalmerUEPart_Base::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	return nullptr;
}

CGameObject* CFalmerUEPart_Base::Clone(void* _pArg)
{
	return nullptr;
}

void CFalmerUEPart_Base::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pParentTransform);
}
