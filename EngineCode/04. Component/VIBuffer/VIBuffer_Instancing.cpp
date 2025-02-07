#include "VIBuffer_Instancing.h"

CVIBuffer_Instancing::CVIBuffer_Instancing(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CVIBuffer(_pDevice, _pContext)
{
}

CVIBuffer_Instancing::CVIBuffer_Instancing(const CVIBuffer_Instancing& rhs)
	: CVIBuffer(rhs)
	, m_iStrideInstance(rhs.m_iStrideInstance)
	, m_iNumInstance(rhs.m_iNumInstance)
	, m_iNumIndicesPerInstance(rhs.m_iNumIndicesPerInstance)
	, m_pVertices(rhs.m_pVertices)
	, m_pSpeedAry(rhs.m_pSpeedAry)
	, m_pLifeTimeAry(rhs.m_pLifeTimeAry)
	, m_pTimeAccAry(rhs.m_pTimeAccAry)
	, m_bIsDrop(rhs.m_bIsDrop)
{
}

HRESULT CVIBuffer_Instancing::Initialize_ProtoType(const INSTANCE_DESC& _InstanceDesc)
{
	m_iStrideInstance = sizeof(VTXINSTANCE);

	/* 정점버퍼와 인덱스 버퍼를 만든다. */
	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);

	// m_BufferDesc.ByteWidth = 정점하나의 크기(Byte) * 정점의 갯수;
	m_BufferDesc.ByteWidth = m_iStrideInstance * m_iNumInstance;
	m_BufferDesc.Usage = D3D11_USAGE_DYNAMIC; /* 동적버퍼로 할당한다. (Lock, unLock 호출 가능)*/
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iStrideInstance;

	m_pVertices = new VTXINSTANCE[m_iNumInstance];
	ZeroMemory(m_pVertices, sizeof(VTXINSTANCE) * m_iNumInstance);

#pragma region Random Initialize 
	random_device RandomDevice;

	mt19937_64								RandomNumber;
	uniform_real_distribution<float>		RandomX;
	uniform_real_distribution<float>		RandomY;
	uniform_real_distribution<float>		RandomZ;

	uniform_real_distribution<float>		ScaleRandom;
	uniform_real_distribution<float>		SpeedRandom;
	uniform_real_distribution<float>		LifeTimeRandom;

	SpeedRandom = uniform_real_distribution<float>(_InstanceDesc.fSpeedMin, _InstanceDesc.fSpeedMax);
	LifeTimeRandom = uniform_real_distribution<float>(_InstanceDesc.fLifeTimeMin, _InstanceDesc.fLifeTimeMax);
	
	m_pSpeedAry = new _float[_InstanceDesc.iNumInstance];
	m_pLifeTimeAry = new _float[_InstanceDesc.iNumInstance];
	m_pTimeAccAry = new _float[_InstanceDesc.iNumInstance];
	m_bIsDrop = new _bool[_InstanceDesc.iNumInstance];

	for (size_t i = 0; i < _InstanceDesc.iNumInstance; ++i)
	{
		m_pSpeedAry[i] = SpeedRandom(RandomDevice);
		m_pLifeTimeAry[i] = LifeTimeRandom(RandomDevice);
		m_pTimeAccAry[i] = 0.f;
		m_bIsDrop[i] = false;
	}

	/* 난수 생성 초기화.  */
	RandomNumber = mt19937_64(RandomDevice());
	RandomX = uniform_real_distribution<float>(_InstanceDesc.vRange.x * -0.5f, _InstanceDesc.vRange.x * 0.5f);
	RandomY = uniform_real_distribution<float>(_InstanceDesc.vRange.y * -0.5f, _InstanceDesc.vRange.y * 0.5f);
	RandomZ = uniform_real_distribution<float>(_InstanceDesc.vRange.z * -0.5f, _InstanceDesc.vRange.z * 0.5f);

	ScaleRandom = uniform_real_distribution<float>(_InstanceDesc.fScaleMin, _InstanceDesc.fScaleMax);
#pragma endregion

	for (size_t i = 0; i < m_iNumInstance; i++)
	{
		_float		fScale = ScaleRandom(RandomNumber);

		m_pVertices[i].vRight = _float4(fScale, 0.f, 0.f, 0.f);
		m_pVertices[i].vUp = _float4(0.f, fScale, 0.f, 0.f);
		m_pVertices[i].vLook = _float4(0.f, 0.f, fScale, 0.f);
		m_pVertices[i].vTranslation = _float4(
			  _InstanceDesc.vCenter.x + RandomX(RandomNumber)
			, _InstanceDesc.vCenter.y + RandomY(RandomNumber)
			, _InstanceDesc.vCenter.z + RandomZ(RandomNumber)
			, 1.f);
		// 랜덤한 각도로 방향 업데이트
		_float directionZ = std::cos(RandomZ(RandomNumber));
		_float directionY = std::sin(RandomY(RandomNumber));

	}

	ZeroMemory(&m_InitialData, sizeof m_InitialData);
	m_InitialData.pSysMem = m_pVertices;

	return S_OK;
}

HRESULT CVIBuffer_Instancing::Initialize_Clone(void* pArg)
{
	// 이렇게 하면 클론 할 때마다 각자 자기 정보를 들고 있을 수 있다.
	if (FAILED(__super::Create_Buffer(&m_pVBInstance)))
		return E_FAIL;

	return S_OK;
}

HRESULT CVIBuffer_Instancing::Update(_float _fTimeDelta)
{
	return S_OK;
}

HRESULT CVIBuffer_Instancing::Render()
{
	ID3D11Buffer* pVertexBuffers[] = {
		m_pVB,
		m_pVBInstance
	};

	_uint			iStrides[] = {
		m_iStride,
		m_iStrideInstance,

	};

	_uint			iOffsets[] = {
		0,
		0,
	};

	/* 버텍스 버퍼들을 할당한다. */
	/* 그리기용 정점버퍼 + 상태변환용 정점버퍼 */
	m_pContext->IASetVertexBuffers(0, m_iNumVBs, pVertexBuffers, iStrides, iOffsets);

	/* 인덱스 버퍼를 할당한다. */
	/* 그리고자 하는 인스턴스의 갯수만큼 확대되어있는 인덱스 버퍼. */
	m_pContext->IASetIndexBuffer(m_pIB, m_eIndexFormat, 0);

	/* 해당 정점들을 어떤 방식으로 그릴꺼야. */
	m_pContext->IASetPrimitiveTopology(m_eTopology);

	/* 인덱스가 가르키는 정점을 활용하여 그린다. */
	m_pContext->DrawIndexedInstanced(m_iNumIndicesPerInstance, m_iNumInstance, 0, 0, 0);

	return S_OK;
}

void CVIBuffer_Instancing::Free()
{
	__super::Free();

	if (!m_isCloned)
	{
		Safe_Delete_Array(m_pVertices);
		Safe_Delete_Array(m_pSpeedAry);
		Safe_Delete_Array(m_pLifeTimeAry);
		Safe_Delete_Array(m_pTimeAccAry);
		Safe_Delete_Array(m_bIsDrop);
	}

	Safe_Release(m_pVBInstance);
}
