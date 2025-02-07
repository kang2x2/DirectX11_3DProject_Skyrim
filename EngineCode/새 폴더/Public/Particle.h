#pragma once

#include "Component.h"

BEGIN(Engine)

class CTexture;

class ENGINE_DLL CParticle final : public CComponent
{
protected:
	CParticle(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CParticle(const CParticle& rhs);
	virtual ~CParticle() = default;

public:
	virtual HRESULT Initialize_ProtoType(const wstring& _strTextureFileName);
	virtual HRESULT Initialize_Clone(void* pArg) override;
	virtual void ShutDown();
	virtual HRESULT Tick(_float _fTimeDelta);
	virtual void Render();
	virtual ID3D11ShaderResourceView* GetTexture();
	virtual int  GetIndexCount();

private:
	bool LoadTexture(const wstring & _strTextureFilePath);
	void ReleaseTexture();

	bool InitializeParticleSystem();
	void ShutdownParticleSystem();

	bool InitializeBuffers();
	void ShutdownBuffers();

	void EmitParticles(float);
	void UpdateParticles(float);
	void KillParticles();

	bool UpdateBuffers();

	void RenderBuffers();

private:
	_float m_particleDeviationX = 0;
	_float m_particleDeviationY = 0;
	_float m_particleDeviationZ = 0;
	_float m_particleVelocity = 0;
	_float m_particleVelocityVariation = 0;
	_float m_particleSize = 0;
	_float m_particlesPerSecond = 0;
	_int m_maxParticles = 0;

	_int m_currentParticleCount = 0;
	_float m_accumulatedTime = 0;

	CTexture* m_pTexture = nullptr;
	PARTICLETYPE* m_particleList = nullptr;
	_int m_vertexCount = 0;
	_int m_indexCount = 0;
	VTXTEXCOL* m_vertices = nullptr;
	ID3D11Buffer* m_vertexBuffer = nullptr;
	ID3D11Buffer* m_indexBuffer = nullptr;

public:
	static  CParticle* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext, const wstring& _strTextureFileName);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};

END

