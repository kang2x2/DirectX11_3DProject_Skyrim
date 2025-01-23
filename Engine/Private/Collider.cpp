#include "Collider.h"

#include "PipeLine.h"

#include "Bounding_AABB.h"
#include "Bounding_OBB.h"
#include "Bounding_Sphere.h"

_uint CCollider::m_iIDIndex = 0;

CCollider::CCollider(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CComponent(_pDevice, _pContext)
{
}

CCollider::CCollider(const CCollider& rhs)
	: CComponent(rhs)
#ifdef _DEBUG
	, m_pBatch(rhs.m_pBatch)
	, m_pEffect(rhs.m_pEffect)
	, m_pInputLayout(rhs.m_pInputLayout)
	, m_pDSState(rhs.m_pDSState)
#endif
	, m_eColliderType(rhs.m_eColliderType)

{
#ifdef _DEBUG
	Safe_AddRef(m_pInputLayout);
	Safe_AddRef(m_pDSState);
#endif
}

HRESULT CCollider::Initialize_ProtoType(COLLIDER_TYPE _eType)
{
	m_eColliderType = _eType;

#ifdef _DEBUG
	/* 디버깅에 대한 준비 */
	m_pBatch = new PrimitiveBatch<VertexPositionColor>(m_pContext);

	m_pEffect = new BasicEffect(m_pDevice);
	m_pEffect->SetVertexColorEnabled(true);

	const void* pShaderByteCode = nullptr;
	size_t		iShaderByteCodeLength = 0;

	m_pEffect->GetVertexShaderBytecode(&pShaderByteCode, &iShaderByteCodeLength);

	if (FAILED(m_pDevice->CreateInputLayout(VertexPositionColor::InputElements, VertexPositionColor::InputElementCount, pShaderByteCode, iShaderByteCodeLength, &m_pInputLayout)))
		return E_FAIL;

	D3D11_DEPTH_STENCIL_DESC	DSStateDesc = {};
	DSStateDesc.DepthEnable = true;
	DSStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	DSStateDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	DSStateDesc.StencilEnable = false;

	if (FAILED(m_pDevice->CreateDepthStencilState(&DSStateDesc, &m_pDSState)))
		return E_FAIL;

#endif

	return S_OK;
}

HRESULT CCollider::Initialize_Clone(void* _pArg)
{
	/*  ProtoType 생성 때에는 타입만 정해준 상태. (AABB, OBB 등등) 
		여기에서 사본을 생성할 때에 구조체를 받아
		실제 충돌을 위한 충돌체를 만든다. */

	const CBounding::BOUNDING_DESC* pBoundingDesc =
					(const CBounding::BOUNDING_DESC*)_pArg;

	switch (m_eColliderType)
	{
	case TYPE_AABB:
		m_pBounding = CBounding_AABB::Create(pBoundingDesc);
		break;
	case TYPE_OBB:
		m_pBounding = CBounding_OBB::Create(pBoundingDesc);
		break;
	case TYPE_SPHERE:
		m_pBounding = CBounding_Sphere::Create(pBoundingDesc);
		break;
	}

	if (m_pBounding == nullptr)
		return E_FAIL;

	m_iColliderID = m_iIDIndex++;

	return S_OK;
}

void CCollider::Update(_fmatrix _TransformMat)
{
	m_pBounding->Update(_TransformMat);

	//if (m_mapHadCol.size() > 0)
	//{
	//	auto iter = m_mapHadCol.begin();
	//
	//	while (iter != m_mapHadCol.end())
	//	{
	//		if (!IsCollision(iter->second))
	//			iter = m_mapHadCol.erase(iter);
	//		else
	//			++iter;
	//	}
	//}
}

void CCollider::Late_Update(_float _fTimeDelta)
{
}

_bool CCollider::IsCollision(CCollider* _pTargetCollider)
{
	if (m_pBounding->IsCollision(_pTargetCollider->m_eColliderType, _pTargetCollider->m_pBounding))
	{
		return true;
	}

	return false;
}

void CCollider::Set_ColliderDesc(_float _fExtents)
{
	m_pBounding->Set_ColliderDesc(_fExtents);
}

_vector CCollider::Get_WorldCenter()
{
	return m_pBounding->Get_WorldCenter();
}

#ifdef _DEBUG

HRESULT CCollider::Render()
{
	m_pContext->OMSetDepthStencilState(m_pDSState, 0);

	CPipeLine* pPipeLine = CPipeLine::GetInstance();
	Safe_AddRef(pPipeLine);

	m_pEffect->SetWorld(XMMatrixIdentity());
	m_pEffect->SetView(pPipeLine->Get_Transform_Matrix(CPipeLine::D3DTS_VIEW));
	m_pEffect->SetProjection(pPipeLine->Get_Transform_Matrix(CPipeLine::D3DTS_PROJ));

	Safe_Release(pPipeLine);

	m_pContext->IASetInputLayout(m_pInputLayout);
	m_pEffect->Apply(m_pContext);

	m_pBatch->Begin();

	m_pBounding->Render(m_pBatch);

	m_pBatch->End();

	return S_OK;
}

#endif

void CCollider::Set_OwnerObj(CGameObject* _pObj)
{
	m_pOwnerObj = _pObj;
}

void CCollider::Set_bISCol(_bool _bIsCol)
{
	m_pBounding->Set_bISCol(_bIsCol);
}

CCollider* CCollider::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext, COLLIDER_TYPE _eType)
{
	CCollider* pInstance = new CCollider(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize_ProtoType(_eType)))
	{
		MSG_BOX("Fail Create : CCollider");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CCollider::Clone(void* _pArg)
{
	CCollider* pInstance = new CCollider(*this);

	if (FAILED(pInstance->Initialize_Clone(_pArg)))
	{
		MSG_BOX("Fail Clone : CCollider");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCollider::Free()
{
	__super::Free();

	Safe_Release(m_pBounding);

#ifdef _DEBUG
	Safe_Release(m_pDSState);
	Safe_Release(m_pInputLayout);

	if (!m_isCloned)
	{
		Safe_Delete(m_pBatch);
		Safe_Delete(m_pEffect);
	}
#endif
}
