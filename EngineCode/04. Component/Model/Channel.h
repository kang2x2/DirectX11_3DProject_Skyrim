#pragma once

#include "Base.h"
#include "Bin_AIScene.h"

BEGIN(Engine)

class CChannel final : public CBase
{
private:
	CChannel();
	virtual ~CChannel() = default;

public:
	HRESULT Initialize(const class CModel* _pModel, const CBin_AIScene::DESC_ANIMATIONCHANNEL* _pAIChannel);
	void Update_TransformationMatrix(_uint* _pCurKeyFrame, vector<class CBone*>& _vecBone, _float _fTrackPosition);
	_bool Change_TransformationMatrix(vector<class CBone*>& _vecBone, const KEYFRAME& _destKeyFrame, _float _fChangeTrackPosition);

public:
	const _uint Get_BoneIndex() { return m_iBoneIndex; }
	const KEYFRAME& Get_FisrtKeyFrame() {
		return m_vecKeyFrame[0];
	}

private:
	_char				m_szName[MAX_PATH] = ""; /* 뼈의 이름 */

	 /* 시간에 따른 상태값 보관 */
	_uint				m_iNumKeyFrames = 0;
	_uint				m_iBoneIndex = 0;

	vector<KEYFRAME>	m_vecKeyFrame; /* keyframe : 특정 시간에 표현해야 할 뼈의 상태 */

	/* 애니메이션 간의 보간을 위해 실시간으로 저장하는 현재 상태 */
	XMFLOAT3			vCurScale = {};
	XMFLOAT4			vCurRotation = {};
	XMFLOAT4			vCurTranslation = {};

public:
	static CChannel* Create(const class CModel* _pModel, const CBin_AIScene::DESC_ANIMATIONCHANNEL* _pAIChannel);
	virtual void Free() override;
};

END

