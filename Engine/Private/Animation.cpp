#include "Animation.h"
#include "Channel.h"

#include "Bin_AIScene.h"

CAnimation::CAnimation()
{
}

CAnimation::CAnimation(const CAnimation& rhs)
	: m_fDuration(rhs.m_fDuration)
	, m_fTickPerSecond(rhs.m_fTickPerSecond)
	, m_fTrackPosition(rhs.m_fTrackPosition)
	, m_bIsLoop(rhs.m_bIsLoop)
	, m_bIsFinish(rhs.m_bIsFinish)
	, m_iNumChannel(rhs.m_iNumChannel)
	, m_vecChannel(rhs.m_vecChannel)
	, m_vecCurKeyFrame(rhs.m_vecCurKeyFrame)
{
	strcpy_s(m_szName, rhs.m_szName);

	for (auto& iter : m_vecChannel)
		Safe_AddRef(iter);
}

HRESULT CAnimation::Initailze(const class CModel* _pModel, const CBin_AIScene::DESC_ANIMATION* _pAiAnimation)
{
	strcpy_s(m_szName, _pAiAnimation->mName.data);

	/* aiAnimation의 druration이 double 타입. float으로 형변환 */
	m_fDuration = (_float)_pAiAnimation->mDuration; 

	/* 이 애니메이션의 초당 속도. (재생속도) */
	/* m_fTrackPosition += m_fTickPerSecond * fTimeDelta (누적) */
	m_fTickPerSecond = (_float)_pAiAnimation->mTicksPerSecond;

	/* 애니메이션이 사용하는 뼈의 개수 */
	m_iNumChannel = _pAiAnimation->mNumChannels;

	m_vecCurKeyFrame.resize(m_iNumChannel);

	for (size_t i = 0; i < m_iNumChannel; ++i)
	{
		/* 각 뼈들을 돌면서 정보를 생성해놓는다. */
		/* 정보 : 이 뼈가 이 애니메이션 안에서 몇개의 동작을 가지는지 */
		CChannel* pChannel = CChannel::Create(_pModel, &_pAiAnimation->mChannels[i]);
		if (pChannel == nullptr)
			return E_FAIL;

		m_vecChannel.push_back(pChannel);
	}

	return S_OK;
}

void CAnimation::Update_TransformationMatrix(vector<class CBone*>& _vecBone, _float _fTimeDelta)
{
	if (!m_bIsStop)
	{
		if (m_bIsFinish)
			return;

		/* 누적 */
		m_fTrackPosition += m_fTickPerSecond * _fTimeDelta;

		/* 현재 재생 위치가 총 재생 시간을 넘어가면 */
		if (m_fTrackPosition >= m_fDuration)
		{
			/* 루프 애니메이션이라면 다시 처음으로, 아니라면 애니메이션을 종료시킨다. */
			if (m_bIsLoop)
				m_fTrackPosition = 0.f;
			else
				m_bIsFinish = true;
		}

		/* 경과 시간에 따른 뼈 업데이트(선형 보간 등등). */
		for (size_t i = 0; i < m_vecChannel.size(); ++i)
		{
			m_vecChannel[i]->Update_TransformationMatrix(&m_vecCurKeyFrame[i], _vecBone, m_fTrackPosition);
		}
	}
}

_bool CAnimation::Change_TransformationMatrix(vector<class CBone*>& _vecBone, const vector<CChannel*>& _destVecChannel, _float _fTimeDelta)
{
	if (!m_bIsStop)
	{
		m_fChangeTrackPosition += _fTimeDelta;

		if (m_fChangeTrackPosition >= 0.2f)
		{
			return true;
		}

		for (size_t i = 0; i < m_vecChannel.size(); ++i)
		{
			if (m_vecChannel[i]->Get_BoneIndex() == _destVecChannel[i]->Get_BoneIndex())
			{
				m_vecChannel[i]->Change_TransformationMatrix(_vecBone, _destVecChannel[i]->Get_FisrtKeyFrame(), m_fChangeTrackPosition);
			}
		}
	}

	return false;
}

void CAnimation::ReSet()
{
	m_fTrackPosition = 0.f;
	m_bIsFinish = false;
	// m_bIsLoop = false;

	for (auto& iter : m_vecCurKeyFrame)
		iter = 0;
}

void CAnimation::Ready_ChangeAnimation(_uint _iChangeIndex)
{
	m_fChangeTrackPosition = _iChangeIndex;
}

CAnimation* CAnimation::Create(const class CModel* _pModel, const CBin_AIScene::DESC_ANIMATION* _pAiAnimation)
{
	CAnimation* pInstance = new CAnimation();

	if (FAILED(pInstance->Initailze(_pModel, _pAiAnimation)))
	{
		MSG_BOX("Fail Create : CAnimation");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CAnimation* CAnimation::Clone()
{
	return new CAnimation(*this);
}

void CAnimation::Free()
{
	__super::Free();

	for (auto& iter : m_vecChannel)
		Safe_Release(iter);
	m_vecChannel.clear();
}
