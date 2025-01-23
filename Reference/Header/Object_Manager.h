#pragma once

#include "Base.h"

BEGIN(Engine)

class CObject_Manager final : public CBase
{
	DECLARE_SINGLETON(CObject_Manager)
private:
	CObject_Manager();
	virtual ~CObject_Manager() = default;

public:
	HRESULT Reserve_Manager(_uint _iLevelCount);
	HRESULT Add_ProtoObject(const wstring& _strProtoTypeTag, class CGameObject* _pGameObject);
	HRESULT Add_CloneObject(_uint _iLevelIndex, const wstring& _strLayerTag, const wstring& _strProtoTypeTag, void* pArg);
	HRESULT Add_CloneObject(_uint _iLevelIndex, const wstring& _strLayerTag, const wstring& _strProtoTypeTag, const wstring& _strModelComTag, void* pArg);
	class CGameObject* Add_InstanceCloneObject(_uint _iLevelIndex, const wstring& _strLayerTag, const wstring& _strPrototypeTag, void* _pArg);
	class CGameObject* Add_ClonePartObject(const wstring& _strPrototypeTag, void* _pArg);
	HRESULT Delete_CloneObject(_uint _iLevelIndex, const wstring& _strLayerTag, const wstring& _strName);
	void PriorityTick(_float _fTimeDelta);
	void Tick(_float _fTimeDelta);
	void LateTick(_float _fTimeDelta);
	void ClearTick();
	void Clear(_uint _iLevelIndex);

	// 급한대로 라이트용 추가.
	HRESULT Add_LightObject(_uint _iLevelIndex, const wstring& _strLayerTag, const wstring& _strProtoTypeTag, const wstring& _strModelComTag, void* _pArg, LIGHT_DESC* _pLightDesc);


public:
	class CGameObject* Find_CloneObject(_uint _iLevelIndex, const wstring& _strLayerTag, const wstring& _strName);
	map<const wstring, class CLayer*>* Get_CloneObjectMapAry(_uint _iLevel){ return &m_mapLayer[_iLevel]; }
	class CGameObject* Find_ProtoObject(const wstring& _strPrototypeTag);
	map<const wstring, class CGameObject*> Get_ProtoObjectMapAry() { return m_mapProtoTypeObj; }
	class CLayer* Find_Layer(_uint _iLevelIndex, const wstring& _strLayerTag);

private:
	// 원본
	map<const wstring, class CGameObject*> m_mapProtoTypeObj;
	// 사본(레벨별로 그룹지어 레이어로 관리)
	map<const wstring, class CLayer*>*	   m_mapLayer = nullptr;
	_uint								   m_iLevelNum = 0;

public:
	virtual void Free() override;
};

END

