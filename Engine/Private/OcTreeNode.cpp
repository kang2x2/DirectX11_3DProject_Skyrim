#include "DebugDraw.h"
#include "OcTreeNode.h"
#include "GameInstance.h"

COcTreeNode::COcTreeNode()
	: m_pTransformCom(nullptr)
{
}

HRESULT COcTreeNode::Initialize(_float3 _vCenter, _float3 _vRange)
{
	m_vCenter = _vCenter;
	m_vRange = _vRange;
	m_bIsCull = true;

	m_pBoundingBox = new BoundingBox(m_vCenter, m_vRange);

	Ready_Component();


	// m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_vCenter.x, m_vCenter.y, m_vCenter.z, 1.f));
	// m_pTransformCom->Set_Scaling(m_vRange);

	m_pBoundingBox->Transform(*m_pBoundingBox, CPipeLine::GetInstance()->Get_Transform_Matrix(CPipeLine::D3DTS_WORLD));

	return S_OK;
}

HRESULT COcTreeNode::AddChild(COcTreeNode* _pChild)
{
	if (_pChild == nullptr)
		return E_FAIL;

	m_vChilds.push_back(_pChild);

	return S_OK;
}

HRESULT COcTreeNode::AddObject(CGameObject* _pObject)
{
	if (_pObject == nullptr)
		return E_FAIL;

	m_vObjects.push_back(_pObject);

	return S_OK;
}

HRESULT COcTreeNode::Update()
{
	// _matrix		Matrix = m_pTransformCom->Get_WorldMatrix();

	m_pBoundingBox->Transform(*m_pBoundingBox, CPipeLine::GetInstance()->Get_Transform_Matrix(CPipeLine::D3DTS_WORLD));

	return S_OK;
}

#ifdef _DEBUG

HRESULT COcTreeNode::Render(PrimitiveBatch<VertexPositionColor>* _pBatch)
{
	DX::Draw(_pBatch, *m_pBoundingBox, XMVectorSet(0.f, 1.f, 0.f, 1.f));

	return S_OK;
}

#endif

HRESULT COcTreeNode::Ready_Component()
{
	dynamic_cast<CTransform*>(CGameInstance::GetInstance()->Clone_Component(0, TEXT("ProtoType_Component_Transform"), &m_pTransformCom));

	return S_OK;
}

void COcTreeNode::Free()
{
	for (int i = 0; i < m_vChilds.size(); ++i)
	{
		Safe_Release(m_vChilds[i]);
	}
	for (int i = 0; i < m_vObjects.size(); ++i)
	{
		Safe_Release(m_vObjects[i]);
	}

	m_vChilds.clear();
	m_vObjects.clear();

	Safe_Delete(m_pBoundingBox);
	Safe_Release(m_pTransformCom);
}
