#pragma once

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CGameObject abstract : public CBase
{
protected:
	typedef struct tagObjFileDesc
	{
		wstring		m_strLayerTag;
		wstring		m_strProtoObjTag;
		wstring		m_strProtoModelComTag;
	}FILE_OBJDESC;

	typedef struct tagLightFileDesc
	{
		LIGHT_DESC lightDesc;
	}LIGHT_OBJDESC;

protected:
	CGameObject(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext); // 원본
	CGameObject(const CGameObject& rhs); // 사본
	virtual ~CGameObject() = default;

public:
	virtual HRESULT Initialize_ProtoType(); // 원본
	virtual HRESULT Initialize_Clone(void* pArg); // 사본
	virtual HRESULT Initialize_Clone(_uint _iLevel, const wstring & _strModelComTag, void* pArg); // 사본
	virtual void    PriorityTick(_float _fTimeDelta);
	virtual void	Tick(_float _fTimeDelta);
	virtual void	LateTick(_float _fTimeDelta);
	virtual HRESULT Render();

public:
	const wstring& Get_Name() { return m_strName; } // 클론으로 생성된 오브젝트들은 모두 고유한 이름을 가진다.
	const wstring& Get_LayerTag() { return m_strLayerTag; } // 클론으로 생성된 오브젝트들은 모두 레이어 이름을 가진다.
	_bool Get_IsHasMesh() { return m_bHasMesh; } // 메시를 가지는 오브젝트인지 확인한다.
	_bool Get_IsHasPart() { return m_bHasPart; } // 파츠가 나뉘어져 있는 오브젝트인지 확인한다.
	_bool Get_IsCreature() { return m_bCreature; } // 생명체인지.
	class CComponent* Get_Component(const wstring& _strComponentName); // 가지고 있는 컴포넌트 정보를 반환.

	_bool Get_IsDead() { return m_bDead; } // 삭제될 오브젝트인지 확인한다.
	_bool Get_IsReadyDead() { return m_bReadyDead; } // 삭제 준비된 오브젝트인지 확인한다.
	void  Set_IsDead(_bool _bDead) { m_bDead = _bDead; } // 삭제 여부를 설정한다.
	void  Set_IsReadyDead(_bool _bReadyDead) { m_bReadyDead = _bReadyDead; } // 삭제 준비 여부를 설정한다.

	// 오브젝트 자신의 파일 관련 구조체 Get, Set
	void			Set_ObjFileDesc(const wstring& _strLayerTag, const wstring& _strProtoObjTag, const wstring& _strProtoModelComTag = TEXT(""));
	FILE_OBJDESC    Get_ObjFileDesc() { return m_tObjFileDesc; }

	void			 Set_LightFileDesc(LIGHT_DESC _lightDesc)
	{
		m_tLightFileDesc.lightDesc = _lightDesc;
	}
	LIGHT_OBJDESC    Get_LightFileDesc() { return m_tLightFileDesc; }


	_float			  Get_AnimationSpeed() { return m_fAnimationSpeed; }
	void			  Set_AnimationSpeed(_float _fSpeed) { m_fAnimationSpeed = _fSpeed; }


	_bool  Get_IsMaintain() { return m_bIsMaintain; }

protected:
	wstring					m_strName = TEXT(""); // 고유한 이름을 가지고 있어야 탐색이 용이 할 것 같다.
	wstring					m_strModelComTag = TEXT(""); // 자신이 가지고 있는 모델 컴포넌트의 원본 태그
	wstring					m_strLayerTag = TEXT(""); // 자신이 가지고 있는 레이어 태그.
	_bool					m_bHasMesh = false; // 메시를 가지고 있는 객체인지 판별이 필요 할 것 같다.
	_bool					m_bHasPart = false; // 파츠가 나뉘어져 있는지? 그렇다면 검사할 때 파츠도 검사해야 한다.
	_bool					m_bCreature = false; // 생명체인지.

	// 삭제를 위해 고유 인덱스를 주는 것 보단 bool 값으로 현재 선택된 상태를 전달하자.
	// 같은 이름의 오브젝트가 많을 것이기에.
	_bool					m_bReadyDead = false;
	_bool					m_bDead = false;
	_bool					m_bIsMaintain = false; // 씬 전환되도 지워지지 않을 것들.
	

	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pContext = nullptr;

	_float			 m_fX, m_fY, m_fSizeX, m_fSizeY;
	_float4x4		 m_ViewMatrix, m_ProjMatrix;

	// 파일 입출력을 위해 사용할 정보
	FILE_OBJDESC      m_tObjFileDesc;
	LIGHT_OBJDESC     m_tLightFileDesc;

	// 애니메이션 재생 속도.
	_float							m_fAnimationSpeed = 1.f;

protected:
	/* 컴포넌트의 검색을 용이하게 하기 위해 map으로 보관. 
	   다른 객체가 현 객체의 컴포넌트(Transform 등)을 참조하는 경우가 빈번해서 */
	map<const wstring, class CComponent*> m_mapComponent; 

protected:
	/* 컴포넌트도 레벨별로 관리하고 있어서 해당 레벨의 원본 컴포넌트를 검색하기 위해 레벨 인덱스 필요  */
	HRESULT Add_CloneComponent(_uint _iLevelIndex, const wstring& _strProtoTypeTag, const wstring& _strComponentTag, _Inout_ CComponent** _ppOut, void* pArg = nullptr);
	
	/*  파츠가 필요한 컴포넌트를 가지는 경우 부모에게 그대로 대입해주기 위함.
		현재는 툴에서 픽킹을 위해 사용.*/
	HRESULT Add_CloneComponent(const wstring& _strComponentTag, CComponent* _pCopyTargetCom, _Inout_ CComponent** _ppOut);
	class CComponent* Find_ProtoTypeComponent(const wstring& _strProtoTypeTag);

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual CGameObject* Clone(_uint _iLevel, const wstring& _strModelComTag, void* _pArg);
	virtual CGameObject* LightClone(_uint _iLevel, const wstring& _strModelComTag, void* _pArg, LIGHT_DESC* _LightDesc);
	virtual void Free() override;
};

END

