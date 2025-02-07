#include "Navigation.h"
#include "Cell.h"
#include "Shader.h"
#include "PipeLine.h"

_float4x4 CNavigation::m_matWorld = {};

CNavigation::CNavigation(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CComponent(_pDevice, _pContext)
{
}

CNavigation::CNavigation(const CNavigation& rhs)
	: CComponent(rhs)
	, m_iCurIndex(rhs.m_iCurIndex)
	, m_vecCell(rhs.m_vecCell)
#ifdef _DEBUG
	, m_pShaderCom(rhs.m_pShaderCom)
#endif
{
#ifdef _DEBUG
	Safe_AddRef(m_pShaderCom);
#endif

	for (auto& iter : m_vecCell)
		Safe_AddRef(iter);
}

HRESULT CNavigation::Initialize_ProtoType(const wstring& _strNaviMeshPath)
{
	if (StrCmpW(_strNaviMeshPath.c_str(), TEXT("")))
	{
		_ulong		dwByte = 0;
		_bool		bIsRead = false;
		HANDLE		hFile = CreateFile(_strNaviMeshPath.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		if (0 == hFile)
			return E_FAIL;

		while (true)
		{
			_float3		vPoints[CCell::POINT_END] = {};

			bIsRead = ReadFile(hFile, vPoints, sizeof(_float3) * CCell::POINT_END, &dwByte, nullptr);

			if (0 == dwByte)
				break;

			CCell* pCell = CCell::Create(m_pDevice, m_pContext, vPoints, (_uint)m_vecCell.size());
			if (nullptr == pCell)
				return E_FAIL;

			m_vecCell.push_back(pCell);
		}

		CloseHandle(hFile);

		if (FAILED(SetUp_Neighbors()))
			return E_FAIL;
	}

#ifdef _DEBUG
	m_pShaderCom = CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFile/Shader_Cell.hlsl"), VTXPOSCELL::Elements, VTXPOSCELL::iNumElements);
	if (nullptr == m_pShaderCom)
		return E_FAIL;
#endif

	return S_OK;
}

HRESULT CNavigation::Initialize_Clone(void* _pArg)
{
	if (_pArg == nullptr)
		return S_OK;

	DESC_NAVIGATION* pNaviDesc = (DESC_NAVIGATION*)_pArg;

	/*  이 네비게이션을 이용하고자하는 객체가 어떤 셀에 있는지 저장한다. */
	m_iCurIndex = pNaviDesc->iCurIndex;

	return S_OK;
}

void CNavigation::Update()
{
	XMStoreFloat4x4(&m_matWorld, XMMatrixIdentity());

	for (auto& iter : m_vecCell)
	{
		if (iter != nullptr)
			iter->Update(XMLoadFloat4x4(&m_matWorld));
	}
}

_bool CNavigation::IsMove(_fvector _vPoint)
{
	_int		iNeighborIndex = 0;

	if (m_vecCell[m_iCurIndex]->IsOut(_vPoint, XMLoadFloat4x4(&m_matWorld), &iNeighborIndex))
	{
		/* 나간 방향에 이웃셀이 있으면 움직임. */
		if (-1 != iNeighborIndex)
		{
			while (true)
			{
				if (-1 == iNeighborIndex)
					return false;

				if (false == m_vecCell[iNeighborIndex]->IsOut(_vPoint, XMLoadFloat4x4(&m_matWorld), &iNeighborIndex))
				{
					m_iCurIndex = iNeighborIndex;
					break;
				}
			}
			return true;
		}
		else
			return false;

	}
	else
		return true;
}

HRESULT CNavigation::Init_Cell(const wstring& _strNaviMeshPath)
{
	for (auto& pCell : m_vecCell)
		Safe_Release(pCell);

	m_vecCell.clear();

	if (StrCmpW(_strNaviMeshPath.c_str(), TEXT("")))
	{
		_ulong		dwByte = 0;
		_bool		bIsRead = false;
		HANDLE		hFile = CreateFile(_strNaviMeshPath.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		if (0 == hFile)
			return E_FAIL;

		while (true)
		{
			_float3		vPoints[CCell::POINT_END] = {};

			bIsRead = ReadFile(hFile, vPoints, sizeof(_float3) * CCell::POINT_END, &dwByte, nullptr);

			if (0 == dwByte)
				break;

			CCell* pCell = CCell::Create(m_pDevice, m_pContext, vPoints, (_uint)m_vecCell.size());
			if (nullptr == pCell)
				return E_FAIL;

			m_vecCell.push_back(pCell);
		}

		CloseHandle(hFile);

		if (FAILED(SetUp_Neighbors()))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CNavigation::Add_Cell(_float3 _vMeshPos[3])
{
	/* 스냅 설정.(가까우면 붙이기) */
	for (size_t i = 0; i < CCell::POINT_END; ++i)
	{
		for (size_t j = 0; j < m_vecCell.size(); ++j)
		{
			for (_uint k = 0; k < CCell::POINT_END; ++k)
			{
				_float fDx = fabsf(m_vecCell[j]->Get_LocalPoints(k)->x - _vMeshPos[i].x);
				_float fDy = fabsf(m_vecCell[j]->Get_LocalPoints(k)->y - _vMeshPos[i].y);
				_float fDz = fabsf(m_vecCell[j]->Get_LocalPoints(k)->z - _vMeshPos[i].z);

				if (fDx <= 0.2f && fDy <= 0.2f && fDz <= 0.2f)
				{
					_vMeshPos[i] = *m_vecCell[j]->Get_LocalPoints(k);
					break;
				}
			}
		}
	}

	/* 시계방향 설정. */
	_float Value1 = (_vMeshPos[0].x * _vMeshPos[1].z) + (_vMeshPos[1].x * _vMeshPos[2].z) + (_vMeshPos[2].x * _vMeshPos[0].z);
	_float Value2 = (_vMeshPos[1].x * _vMeshPos[0].z) + (_vMeshPos[2].x * _vMeshPos[1].z) + (_vMeshPos[0].x * _vMeshPos[2].z);
	_float Result = Value1 - Value2;

	if (Result > 0)
	{
		_float3 temp = _vMeshPos[0];
		_float3 dest = _vMeshPos[2];

		_vMeshPos[0] = dest;
		_vMeshPos[2] = temp;
	}

	/* 셀에 추가. */
	CCell* pCell = CCell::Create(m_pDevice, m_pContext, _vMeshPos, (_uint)m_vecCell.size());
	if (nullptr == pCell)
		return E_FAIL;

	m_vecCell.push_back(pCell);

	if (FAILED(SetUp_Neighbors()))
		return E_FAIL;

	return S_OK;
}

HRESULT CNavigation::Cell_PopBack()
{
	CCell* pCell = m_vecCell.back();
	Safe_Release(pCell);

	m_vecCell.pop_back();

	return S_OK;
}

#ifdef _DEBUG

HRESULT CNavigation::Render()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_matWorld)))
		return E_FAIL;

	CPipeLine* pPipeLine = CPipeLine::GetInstance();
	Safe_AddRef(pPipeLine);

	_float4x4 matView = pPipeLine->Get_Transform_float4x4(CPipeLine::D3DTS_VIEW);
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &matView)))
		return E_FAIL;

	_float4x4 matProj = pPipeLine->Get_Transform_float4x4(CPipeLine::D3DTS_PROJ);
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &matProj)))
		return E_FAIL;

	Safe_Release(pPipeLine);

	_float4 vColor = {};

	if (-1 == m_iCurIndex)
	{
#pragma region Outline
		vColor = _float4(0.f, 1.f, 0.f, 1.f);
		if (FAILED(m_pShaderCom->Bind_RawValue("g_vLineColor", &vColor, sizeof(_float4))))
			return E_FAIL;

		_float		fHeight = 0.05f;
		if (FAILED(m_pShaderCom->Bind_RawValue("g_fHeight", &fHeight, sizeof(_float))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Begin(0)))
			return E_FAIL;

		for (auto& iter : m_vecCell)
		{
			if (nullptr != iter)
				iter->Render(CCell::CELL_OUTLINE);
		}
#pragma endregion 

#pragma region Fill
		vColor = _float4(0.f, 1.f, 0.f, 0.2f);
		if (FAILED(m_pShaderCom->Bind_RawValue("g_vLineColor", &vColor, sizeof(_float4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Bind_RawValue("g_fHeight", &fHeight, sizeof(_float))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Begin(0)))
			return E_FAIL;

		for (auto& iter : m_vecCell)
		{
			if (nullptr != iter)
				iter->Render(CCell::CELL_FILL);
		}
#pragma endregion
	}
	else
	{
		vColor = _float4(1.f, 0.f, 0.f, 1.f);
		if (FAILED(m_pShaderCom->Bind_RawValue("g_vLineColor", &vColor, sizeof(_float4))))
			return E_FAIL;

		_float		fHeight = 0.1f;
		if (FAILED(m_pShaderCom->Bind_RawValue("g_fHeight", &fHeight, sizeof(_float))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Begin(0)))
			return E_FAIL;

		m_vecCell[m_iCurIndex]->Render(CCell::CELL_OUTLINE);
	}

	return S_OK;
}

#endif


HRESULT CNavigation::SetUp_Neighbors()
{
	for (auto& sourCell : m_vecCell)
	{
		for (auto& destCell : m_vecCell)
		{
			if (sourCell == destCell)
				continue;

			if (destCell->Compare_Points(sourCell->Get_LocalPoints(CCell::POINT_A), sourCell->Get_LocalPoints(CCell::POINT_B)))
			{
				sourCell->Set_Neighbor(CCell::LINE_AB, destCell);
			}

			else if (destCell->Compare_Points(sourCell->Get_LocalPoints(CCell::POINT_B), sourCell->Get_LocalPoints(CCell::POINT_C)))
			{
				sourCell->Set_Neighbor(CCell::LINE_BC, destCell);
			}

			else if (destCell->Compare_Points(sourCell->Get_LocalPoints(CCell::POINT_C), sourCell->Get_LocalPoints(CCell::POINT_A)))
			{
				sourCell->Set_Neighbor(CCell::LINE_CA, destCell);
			}
		}
	}

	return S_OK;
}

HRESULT CNavigation::Set_CurCell(_fvector vPos)
{
	// Flt_Max : 부동 소수점 형식에서 표현 가능한 가장 큰 양수.
	_float fMinDistance = FLT_MAX;
	_int   iSaveCellIndex = -1;

	for(_int i = 0; i < m_vecCell.size(); ++i)
	{
		_float3 PosA = *m_vecCell[i]->Get_LocalPoints(CCell::POINT_A);
		_float3 PosB = *m_vecCell[i]->Get_LocalPoints(CCell::POINT_B);
		_float3 PosC = *m_vecCell[i]->Get_LocalPoints(CCell::POINT_C);

		_float3 vCellAvgPos = {};
		_float3 vPlayerPos = {};

		vCellAvgPos.x = (PosA.x + PosB.x + PosC.x) / 3.f;
		vCellAvgPos.z = (PosA.z + PosB.z + PosC.z) / 3.f;

		vPlayerPos.x = XMVectorGetX(vPos);
		vPlayerPos.z = XMVectorGetZ(vPos);

		_float fDistance = sqrt((vPlayerPos.x - vCellAvgPos.x) * (vPlayerPos.x - vCellAvgPos.x) +
			(vPlayerPos.z - vCellAvgPos.z) * (vPlayerPos.z - vCellAvgPos.z));
	
		if (fDistance < fMinDistance)
		{
			fMinDistance = fDistance;
			iSaveCellIndex = i;
		}
	}

	if (iSaveCellIndex != -1)
		m_iCurIndex = iSaveCellIndex;
	else
	{
		MSG_BOX("Fail Find : Current OnCell Index");
		return E_FAIL;
	}

	return S_OK;
}

_vector CNavigation::Set_OnCell(_fvector _vWorldPos)
{
	_float3 PosA = *m_vecCell[m_iCurIndex]->Get_LocalPoints(CCell::POINT_A);
	_float3 PosB = *m_vecCell[m_iCurIndex]->Get_LocalPoints(CCell::POINT_B);
	_float3 PosC = *m_vecCell[m_iCurIndex]->Get_LocalPoints(CCell::POINT_C);

	// _float fWidth = XMVectorGetX(_vWorldPos) - PosA.x;
	// _float fDepth = PosA.z - XMVectorGetZ(_vWorldPos);

	_vector vPlane;

	vPlane = XMPlaneFromPoints(XMLoadFloat3(&PosA),
		XMLoadFloat3(&PosB), XMLoadFloat3(&PosC));

	_float fY = ((-XMVectorGetX(vPlane) * XMVectorGetX(_vWorldPos)
		- (XMVectorGetZ(vPlane) * XMVectorGetZ(_vWorldPos))
		- XMVectorGetW(vPlane))) / XMVectorGetY(vPlane);

	_fvector vResultWorldPos = XMVectorSetY(_vWorldPos, fY);

	return vResultWorldPos;
}

_bool CNavigation::isMove(_fvector vPoint)
{
	_int		iNeighborIndex = 0;

	if (m_vecCell[m_iCurIndex]->IsOut(vPoint, XMLoadFloat4x4(&m_matWorld), &iNeighborIndex))
	{
		/* 나간 방향에 이웃셀이 있으면 움직임. */
		if (-1 != iNeighborIndex)
		{
			while (true)
			{
				if (-1 == iNeighborIndex)
					return false;

				// false라면 Cell 내부에 존재한다. 즉, 현재 올라타있는 Cell을 찾았다.
				if (false == m_vecCell[iNeighborIndex]->IsOut(vPoint, XMLoadFloat4x4(&m_matWorld), &iNeighborIndex))
				{
					m_iCurIndex = iNeighborIndex;
					break;
				}
			}
			return true;
		}
		else
			return false;

	}
	else
		return true;
}

CNavigation* CNavigation::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext, const wstring& _strNaviMeshPath)
{
	CNavigation* pInstance = new CNavigation(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize_ProtoType(_strNaviMeshPath)))
	{
		MSG_BOX("Fail Create : CNavigation");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CNavigation::Clone(void* _pArg)
{
	CNavigation* pInstance = new CNavigation(*this);

	if (FAILED(pInstance->Initialize_Clone(_pArg)))
	{
		MSG_BOX("Fail Clone : CNavigation");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNavigation::Free()
{
	__super::Free();

#ifdef _DEBUG
	Safe_Release(m_pShaderCom);
#endif

	for (auto& pCell : m_vecCell)
		Safe_Release(pCell);
}
