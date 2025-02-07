#include "Particle.h"
#include "GameInstance.h"

CParticle::CParticle(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CComponent(_pDevice, _pContext)
{
}

CParticle::CParticle(const CParticle& rhs)
	: CComponent(rhs)
{
}

HRESULT CParticle::Initialize_ProtoType(const wstring& _strTextureFileName)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	pGameInstance->Clone_Component(2, TEXT("Test_Rain"));

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CParticle::Initialize_Clone(void* pArg)
{
	return S_OK;
}

void CParticle::ShutDown()
{
}

HRESULT CParticle::Tick(_float _fTimeDelta)
{
	return S_OK;
}

void CParticle::Render()
{
}

ID3D11ShaderResourceView* CParticle::GetTexture()
{
	return nullptr;
}

int CParticle::GetIndexCount()
{
	return 0;
}

bool CParticle::LoadTexture(const wstring& _strTextureFilePath)
{
	return false;
}

void CParticle::ReleaseTexture()
{
}

bool CParticle::InitializeParticleSystem()
{
	return false;
}

void CParticle::ShutdownParticleSystem()
{
}

bool CParticle::InitializeBuffers()
{
	return false;
}

void CParticle::ShutdownBuffers()
{
}

void CParticle::EmitParticles(float)
{
}

void CParticle::UpdateParticles(float)
{
}

void CParticle::KillParticles()
{
}

bool CParticle::UpdateBuffers()
{
	return false;
}

void CParticle::RenderBuffers()
{
}


CParticle* CParticle::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext, const wstring& _strTextureFileName)
{
	CParticle* _pInstance = new CParticle(_pDevice, _pContext);

	if (FAILED(_pInstance->Initialize_ProtoType(_strTextureFileName)))
	{
		MSG_BOX("Fail Create : CParticle ProtoType");
		Safe_Release(_pInstance);
	}

	return _pInstance;
}

CComponent* CParticle::Clone(void* pArg)
{
	CParticle* _pInstance = new CParticle(*this);

	if (FAILED(_pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("Fail Clone : CParticle Clone");
		Safe_Release(_pInstance);
	}

	return _pInstance;
}

void CParticle::Free()
{
	__super::Free();
}
