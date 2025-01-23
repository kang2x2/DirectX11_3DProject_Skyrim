#pragma once

#include "Base.h"
#include "Bin_AIScene.h"

#include "Channel.h"

BEGIN(Engine)

class CAnimation final : public CBase
{
private:
	CAnimation();
	CAnimation(const CAnimation& rhs);
	virtual ~CAnimation() = default;

public:
	HRESULT Initailze(const class CModel* _pModel, const CBin_AIScene::DESC_ANIMATION* _pAiAnimation);
	void Update_TransformationMatrix(vector<class CBone*>& _vecBone, _float _fTimeDelta);
	_bool Change_TransformationMatrix(vector<class CBone*>& _vecBone, const vector<CChannel*>& _destVecChannel, _float _fTimeDelta);
	void ReSet();
	void Ready_ChangeAnimation(_uint _iChangeIndex);

public:
	void  Set_Loop(_bool _bIsLoop) { m_bIsLoop = _bIsLoop; }
	
	_char* Get_AnimationName() { return m_szName; }
	_bool  Get_CurAnimationIsLoop() { return m_bIsLoop; }

	void  Reset_TrackPosition(_float _fInitIndex) { m_fTrackPosition = _fInitIndex; }
	_bool Get_Finish() { return m_bIsFinish; }
	const vector<CChannel*>& Get_Channel() { return m_vecChannel; }
	const vector<_uint>& Get_CurKeyFrame() { return m_vecCurKeyFrame; }
	void  Set_KeyFrame(_uint _iFrame) { m_fTrackPosition = _iFrame; }

	void   Set_AnimationStop(_bool _bIsStop) { m_bIsStop = _bIsStop; }

 private:
	_float			m_fDuration = 0.f; // 총 길이라고 생각하자.
	_float			m_fTickPerSecond = 0.f; // 재생 속도?
	_float			m_fTrackPosition = 0.f; // 애니메이션 현재 재생 위치.
	_float			m_fChangeTrackPosition = 0.f; // 애니메이션 간 변환 시 재생 위치.
	_bool			m_bIsLoop = false; // 루프 변수(무한히 재생 할 애니메이션 판별)
	_bool			m_bIsFinish = false; // 애니메이션이 끝났는지 확인.
	_bool			m_bIsStop = false; // 애니메이션 스탑.

	/* 애니메이션이 구동해야 하는 뼈의 정보들 */
	_char					m_szName[MAX_PATH] = "";
	_uint					m_iNumChannel = 0;
	vector<CChannel*>		m_vecChannel;
	vector<_uint>			m_vecCurKeyFrame; // 각 채널이 가지는 키프레임. 따로 보관한다.


public:
	static CAnimation* Create(const class CModel* _pModel, const CBin_AIScene::DESC_ANIMATION* _pAiAnimation);
	CAnimation* Clone();
	virtual void Free() override;
};

END