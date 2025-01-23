#include "framework.h"
#include "CarlottaPart_Base.h"

#include "GameInstance.h"

CCarlottaPart_Base::CCarlottaPart_Base(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

CCarlottaPart_Base::CCarlottaPart_Base(const CCarlottaPart_Base& rhs)
	: CGameObject(rhs)
{
}

CBone* CCarlottaPart_Base::Get_SocketBonePtr(const char* _pBoneName)
{
	if (m_pModelCom == nullptr)
		return nullptr;

	return m_pModelCom->Get_BonePtr(_pBoneName);
}

_float4x4 CCarlottaPart_Base::Get_SocketPivotMatrix()
{
	if (m_pModelCom == nullptr)
		return _float4x4();

	return m_pModelCom->Get_PivotMatrix();
}

HRESULT CCarlottaPart_Base::Initialize_ProtoType()
{
	return S_OK;
}

HRESULT CCarlottaPart_Base::Initialize_Clone(void* _pArg)
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

void CCarlottaPart_Base::PriorityTick(_float _fTimeDelta)
{
}

void CCarlottaPart_Base::Tick(_float _fTimeDelta)
{
}

void CCarlottaPart_Base::LateTick(_float _fTimeDelta)
{
}

HRESULT CCarlottaPart_Base::Render()
{
	return S_OK;
}

HRESULT CCarlottaPart_Base::Compute_RenderMatrix(_fmatrix ChildMatrix)
{
	XMStoreFloat4x4(&m_matWorld, ChildMatrix * m_pParentTransform->Get_WorldMatrix());

	return S_OK;
}

CCarlottaPart_Base* CCarlottaPart_Base::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	return nullptr;
}

CGameObject* CCarlottaPart_Base::Clone(void* _pArg)
{
	return nullptr;
}

void CCarlottaPart_Base::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom); // 이걸 주석 하냐 마냐에 따라 릴리즈 가능.
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pParentTransform);

	Safe_Release(m_pArmor);
	Safe_Release(m_pWeapon);

}
