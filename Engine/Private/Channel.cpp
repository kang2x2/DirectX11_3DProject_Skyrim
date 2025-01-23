#include "Channel.h"

#include "Model.h"
#include "Bone.h"

CChannel::CChannel()
{
}

HRESULT CChannel::Initialize(const CModel* _pModel, const CBin_AIScene::DESC_ANIMATIONCHANNEL* _pAIChannel)
{
	strcpy_s(m_szName, _pAIChannel->mNodeName.data);

	m_iBoneIndex = _pModel->Get_BoneIndex(m_szName);

	m_iNumKeyFrames = max(_pAIChannel->mNumScalingKeys, _pAIChannel->mNumRotationKeys);
	m_iNumKeyFrames = max(m_iNumKeyFrames, _pAIChannel->mNumPositionKeys);

	_float3 vScale = { 0.f, 0.f, 0.f };
	_float4 vRotation = { 0.f, 0.f, 0.f, 0.f };
	_float4 vTranslation = { 0.f, 0.f, 0.f, 0.f };

	for (size_t i = 0; i < m_iNumKeyFrames; ++i)
	{
		KEYFRAME tKeyFrame;
		ZeroMemory(&tKeyFrame, sizeof(tKeyFrame));

		/* 프레임마다 가지고 있는 정보가 다르다. 
		   예를 들어 3번 프레임부터는 Scale 정보가 없을 수도 있다.
		   예외 처리를 하면서 필요한 정보만 넣어주어야 한다. */
		
		if (_pAIChannel->mNumScalingKeys > i)
		{
			memcpy(&vScale, &_pAIChannel->mScalingKeys[i].mValue, sizeof(_float3));
			tKeyFrame.fTime = (_float)_pAIChannel->mScalingKeys[i].mTime;
		}

		if (_pAIChannel->mNumRotationKeys > i)
		{
			/*  tKeyFrame의 Rot변수는 _float4 타입을 가지는 반면
			 	_pAIChannel의 Rot Value는 쿼터니언 타입을 가진다.
				memcpy로 하면 각 자리가 안 맞기 때문에 하나 하나 직접 넣어주어야 한다. */
			vRotation.x = _pAIChannel->mRotationKeys[i].mValue.x;
			vRotation.y = _pAIChannel->mRotationKeys[i].mValue.y;
			vRotation.z = _pAIChannel->mRotationKeys[i].mValue.z;
			vRotation.w = _pAIChannel->mRotationKeys[i].mValue.w;
			tKeyFrame.fTime = (_float)_pAIChannel->mRotationKeys[i].mTime;
		}

		if (_pAIChannel->mNumPositionKeys > i)
		{
			memcpy(&vTranslation, &_pAIChannel->mPositionKeys[i].mValue, sizeof(_float3));
			/* _pAIChannel의 위치 키의 값은 vector3형식의 타입이기에 w는 1로 직접 초기화 해야 한다. */
			vTranslation.w = 1.f;
			tKeyFrame.fTime = (_float)_pAIChannel->mPositionKeys[i].mTime;
		}

		tKeyFrame.vScale = vScale;
		tKeyFrame.vRotation = vRotation;
		tKeyFrame.vTranslation = vTranslation;

		m_vecKeyFrame.push_back(tKeyFrame);
	}

	return S_OK;
}

void CChannel::Update_TransformationMatrix(_uint* _pCurKeyFrame, vector<class CBone*>& _vecBone, _float _fTrackPosition)
{
	/* 애니메이션 한번 끝나고 재시작. */
	if (_fTrackPosition == 0.f)
		*_pCurKeyFrame = 0;

	_float3 vScale;
	_float4 vRotation;
	_float4 vTranslation;

	KEYFRAME tLastKeyFrame = m_vecKeyFrame.back();

	/* 현재 트랙이 마지막 키 프레임을 넘어가면 마지막 뼈의 상태를 유지. */
	if (_fTrackPosition >= tLastKeyFrame.fTime)
	{
		*_pCurKeyFrame = (_uint)m_vecKeyFrame.size() - 1;
		vScale = tLastKeyFrame.vScale;
		vRotation = tLastKeyFrame.vRotation;
		vTranslation = _float4(tLastKeyFrame.vTranslation.x, tLastKeyFrame.vTranslation.y, tLastKeyFrame.vTranslation.z, 1.f );
	}
	else /* 그게 아니라면 현재 프레임과 다음 프레임간의 선형 보간을 실시한다. 
		    한 애니메이션 내에서 프레임간의 보간을 실행.*/
	{
		/* 트랙 위치가 다음 프레임의 시작 시간에 도달하면 다음 프레임으로 변경 */
		while (_fTrackPosition >= m_vecKeyFrame[*_pCurKeyFrame + 1].fTime)
			++*_pCurKeyFrame;

		/* 비율? 평균? 구하기 */
		/* 현재 트랙 위치 - 현재 프레임 시작 시간 / 다음 프레임 시작 시간 - 현재 프레임 시작 시간*/
		_float fRatio = (_fTrackPosition - m_vecKeyFrame[*_pCurKeyFrame].fTime) /
		(m_vecKeyFrame[*_pCurKeyFrame + 1].fTime - m_vecKeyFrame[*_pCurKeyFrame].fTime);

		/* 스케일 */
		_vector vSourScale = XMLoadFloat3(&m_vecKeyFrame[*_pCurKeyFrame].vScale);
		_vector vDestScale = XMLoadFloat3(&m_vecKeyFrame[*_pCurKeyFrame + 1].vScale);
		XMStoreFloat3(&vScale, XMVectorLerp(vSourScale, vDestScale, fRatio));

		/* 회전 */
		_vector vSourRot = XMLoadFloat4(&m_vecKeyFrame[*_pCurKeyFrame].vRotation);
		_vector vDestRot = XMLoadFloat4(&m_vecKeyFrame[*_pCurKeyFrame + 1].vRotation);
		XMStoreFloat4(&vRotation, XMQuaternionSlerp(vSourRot, vDestRot, fRatio));

		/* 위치 */
		_vector vSourTranslate = XMLoadFloat4(&m_vecKeyFrame[*_pCurKeyFrame].vTranslation);
		_vector vDestTranslate = XMLoadFloat4(&m_vecKeyFrame[*_pCurKeyFrame + 1].vTranslation);
		XMStoreFloat4(&vTranslation, XMVectorLerp(vSourTranslate, vDestTranslate, fRatio));
	}

	/* 애니메이션 간 보간을 위해 실시간으로 저장. */
	vCurScale = vScale;
	vCurRotation = vRotation;
	vCurTranslation = vTranslation;

	/* 아핀 행렬 생성 : 크 * 자 * 이 행렬이다. */
	/* 중간의 vectorSet은 객체가 어떤 지점을 기준으로 변환을 할 것인지 설정한다 .*/
	_matrix matTransformation = XMMatrixAffineTransformation(XMLoadFloat3(&vScale),
		XMVectorSet(0.f, 0.f, 0.f, 1.f), XMLoadFloat4(&vRotation), XMLoadFloat4(&vTranslation));

	/* 최종적으로 변환된 행렬을 이 애니메이션을 구동하는 모델의 메시가 가지는 뼈에게 전달한다. */
	_vecBone[m_iBoneIndex]->Set_TransformationMatrix(matTransformation);

}

_bool CChannel::Change_TransformationMatrix(vector<class CBone*>& _vecBone, const KEYFRAME& _destKeyFrame, _float _fChangeTrackPosition)
{
	_float fRatio = _fChangeTrackPosition / 0.2f;
		
	_float3 vScale;
	_float4 vRotation;
	_float4 vTranslation;

	/* 스케일 */
	_vector vSourScale = XMLoadFloat3(&vCurScale);
	_vector vDestScale = XMLoadFloat3(&_destKeyFrame.vScale);
	XMStoreFloat3(&vScale, XMVectorLerp(vSourScale, vDestScale, fRatio));

	/* 회전 */
	_vector vSourRot = XMLoadFloat4(&vCurRotation);
	_vector vDestRot = XMLoadFloat4(&_destKeyFrame.vRotation);
	XMStoreFloat4(&vRotation, XMQuaternionSlerp(vSourRot, vDestRot, fRatio));

	/* 위치 */
	_vector vSourTranslate = XMLoadFloat4(&vCurTranslation);
	_vector vDestTranslate = XMLoadFloat4(&_destKeyFrame.vTranslation);
	XMStoreFloat4(&vTranslation, XMVectorLerp(vSourTranslate, vDestTranslate, fRatio));

	/* 아핀 행렬 생성 : 크 * 자 * 이 행렬이다. */
	/* 중간의 vectorSet은 객체가 어떤 지점을 기준으로 변환을 할 것인지 설정한다 .*/
	_matrix matTransformation = XMMatrixAffineTransformation(XMLoadFloat3(&vScale),
		XMVectorSet(0.f, 0.f, 0.f, 1.f), XMLoadFloat4(&vRotation), XMLoadFloat4(&vTranslation));

	/* 최종적으로 변환된 행렬을 이 애니메이션을 구동하는 모델의 메시가 가지는 뼈에게 전달한다. */
	_vecBone[m_iBoneIndex]->Set_TransformationMatrix(matTransformation);

	return false;
}

CChannel* CChannel::Create(const class CModel* _pModel, const CBin_AIScene::DESC_ANIMATIONCHANNEL* _pAIChannel)
{
	CChannel* pInstance = new CChannel();

	if (FAILED(pInstance->Initialize(_pModel, _pAIChannel)))
	{
		MSG_BOX("Fail Create : CChannel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CChannel::Free()
{
	__super::Free();
}
