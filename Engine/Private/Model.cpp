#include <filesystem>
#include <fstream>
#include <commdlg.h>

#include "Model.h"

#include "GameInstance.h"

#include "Mesh.h"
#include "Texture.h"
#include "Bone.h"
#include "Animation.h"

CModel::CModel(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CComponent(_pDevice, _pContext)
{
}

CModel::CModel(const CModel& rhs)
	: CComponent(rhs)
	, m_iNumMeshes(rhs.m_iNumMeshes)
	, m_vecMesh(rhs.m_vecMesh)
	, m_iNumMaterails(rhs.m_iNumMaterails)
	, m_vecMaterial(rhs.m_vecMaterial)
	, m_iNumAnimation(rhs.m_iNumAnimation)
	, m_matPivot(rhs.m_matPivot)
{
	// 애니메이션과 뼈는 깊은 복제 하여 가진다.
	for (auto& iter : rhs.m_vecAnimation)
		m_vecAnimation.push_back(iter->Clone());
	
	for (auto& iter : rhs.m_vecBone)
		m_vecBone.push_back(iter->Clone());

	for (auto& iter : m_vecMaterial)
	{
		for (size_t i = 0; i < AI_TEXTURE_TYPE_MAX; ++i)
		{
			Safe_AddRef(iter.pTextures[i]);
		}
	}

	for (auto& iter : m_vecMesh)
	{
		Safe_AddRef(iter);
	}
}

HRESULT CModel::Initialize_ProtoType(const char* _strModelFilePath, _fmatrix _matPivot, MODEL_TYPE _eType)
{
	// fbx파일을 읽어 데이터를 m_pAIScene으로 리턴.
	/*  aiProcess_PreTransformVertices 
		메시의 이름과 같은 이름의 뼈가 있는지 검사한다.
		있다면 로드 할 때 그 뼈의 행렬을 가져와 메시의 모든 정점에 곱해준다.
		정적 모델을 불러올 때에는 상관이 없으나 애니메이션이 존재하는 동적 모델을 로드할 때
		이미 연산이 끝난 상태로 로드하고 애니메이션 정보를 빼고 로드한다.*/

	// aiProcess_ConvertToLeftHanded : 왼손 좌표계로 알아서 변환하여 가져와줌(FBX SDK는 지원 안함)
	// aiProcessPreset_TargetRealtime_Fast : 옵션을 변경해 줌으로써 퀄리티의 차이가 있다.
	
	// 기본적인 플래그.
	//_uint iFlag = aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Fast;
	//// 매개로 들어온 타입이 NonAnim일 때 aiProcess_PreTransformVertices도 바인딩 해준다.
	//if (_eType == TYPE_NONANIM)
	//	iFlag = iFlag | aiProcess_PreTransformVertices;
	//
	//m_pAIScene = m_Importer.ReadFile(_strModleFilePath, iFlag);
	//if (m_pAIScene == nullptr)
	//	return E_FAIL;

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	m_pAIScene = pGameInstance->Binary_InFile(_strModelFilePath, _eType);

	Safe_Release(pGameInstance);

	// matrix 타입을 보관용 float4로 변환
	XMStoreFloat4x4(&m_matPivot, _matPivot);

	// DirectX로 그려낼 수 있게 데이터를 정리하는 부분
	
	/* 뼈 로드(최상위 부모 노드를 매개로 보낸다.) */
	//if (FAILED(Ready_Bone(&m_pAIScene->mVecNode[0], -1)))
	//if (FAILED(Ready_Bone(m_pAIScene->mVecNode[0], -1)))
	if (FAILED(Ready_Bone()))
		return E_FAIL;

	/* 모델의 각 파츠 정점 정보를 로드. */
	if (FAILED(Ready_Mesh(_eType)))
		return E_FAIL;

	/* 모델의 픽셀별로 정의된 재질 정보를 저장해준 텍스쳐를 읽음. */
	if (FAILED(Ready_Material(_strModelFilePath)))
		return E_FAIL;

	if (FAILED(Ready_Animation()))
		return E_FAIL;


	return S_OK;
}

HRESULT CModel::Initialize_Clone(void* pArg)
{
	return S_OK;
}

HRESULT CModel::Bind_BondMatrices(CShader* _pShader, _uint _iMeshIndex, const char* _strConstantName)
{
	return m_vecMesh[_iMeshIndex]->Bind_BondMatrices(_pShader, m_vecBone, _strConstantName, XMLoadFloat4x4(&m_matPivot));
}

HRESULT CModel::Bind_MaterialTexture(CShader* _pShader, const char* _pConstantName, _uint _iMeshIndex, aiTextureType _eType)
{
	if (_iMeshIndex >= m_iNumMeshes)
		return E_FAIL;

	_uint iMaterialIndex = m_vecMesh[_iMeshIndex]->Get_MaterialIndex();
	if (iMaterialIndex >= m_iNumMaterails)
		return E_FAIL;

	if (m_vecMaterial[iMaterialIndex].pTextures[_eType] == nullptr)
		return S_OK;

	return m_vecMaterial[iMaterialIndex].pTextures[_eType]->Bind_ShaderResource(_pShader, _pConstantName, 0);
}

HRESULT CModel::Render(_uint _iMeshIndex)
{
	// 메시별로 사용하는 재질 정보가 다를 수도 있기에 기존 방법은 폐기
	//for (auto& iter : m_vecMesh)
	//{
	//	if (iter != nullptr)
	//	{
	//		iter->Render();
	//	}
	//}

	m_vecMesh[_iMeshIndex]->Render();

	return S_OK;
}

HRESULT CModel::SwapDesc_Armor(CModel* _pModel)
{
	//for (auto& iter : m_vecMaterial)
	//{
	//	for (size_t i = 0; i < AI_TEXTURE_TYPE_MAX; ++i)
	//	{
	//		Safe_Release(iter.pTextures[i]);
	//	}
	//}
	//m_vecMaterial.clear();
	//
	//for (auto& iter : m_vecMesh)
	//{
	//	Safe_Release(iter);
	//}
	//m_vecMesh.clear();
	//
	//for (auto& iter : m_vecBone)
	//{
	//	Safe_Release(iter);
	//}
	//m_vecBone.clear();


	// safe_addref
	// safe_release는 무조건 지우는게 아니라 카운트가 0이면 지운다.

	// m_vecBone = _pModel->m_vecBone;
	m_iNumMeshes = _pModel->m_iNumMeshes;
	m_vecMesh = _pModel->m_vecMesh;
	m_iNumMaterails = _pModel->m_iNumMaterails;
	m_vecMaterial = _pModel->m_vecMaterial;
	m_strModelPath = _pModel->m_strModelPath;
	//m_vecBone = _pModel->m_vecBone;
	//m_iNumAnimation = _pModel->m_iNumAnimation;
	//m_vecAnimation = _pModel->m_vecAnimation;

	return S_OK;
}

HRESULT CModel::Ready_Bone()
{
	for (size_t i = 0; i < m_pAIScene->mVecNode.size(); ++i)
	{
		CBone* pBone = CBone::Create(&m_pAIScene->mVecNode[i], m_pAIScene->mVecNode[i].mParentIndex);
		m_vecBone.push_back(pBone);
	}

	// 뼈 생성 (최상위 부모 노드가 가장 처음에 들어오기 때문에 매개로 -1을 받음)
	//CBone* pBone = CBone::Create(_pRootNode, _iParentBoneIndex);
	//if (pBone == nullptr)
	//	return E_FAIL;
	//

	// 	m_vecBone.push_back(pBone);

	// 뼈의 부모 인덱스 설정.
	//_int iParentIndex = m_vecBone.size() - 1;

	// 자식 노드 수 만큼 재귀 함수 수행하여 뼈 생성.
	// 더 이상 생성 할 자식이 없을 때 까지 계속해서 재귀를 수행 할 것이다.
	//for (size_t i = 0; i < _pRootNode->mNumChildren; ++i)
	//{
	//	// 재귀를 돌며 i번째 자식 노드를 생성한다.
	//	// 부모의 인덱스는 반복문 이전에 세팅해 둔 인덱스가 될 것이다.
	//	Ready_Bone(&_pRootNode->mChildren[i], iParentIndex);
	//}

	return S_OK;
}
HRESULT CModel::Ready_Mesh(MODEL_TYPE _eType)
{
	m_iNumMeshes = m_pAIScene->mNumMeshes;

	m_vecMesh.reserve(m_iNumMeshes);

	for (size_t i = 0; i < m_iNumMeshes; ++i)
	{
		CMesh* pMesh = CMesh::Create(m_pDevice, m_pContext, this, &m_pAIScene->mMeshs[i], XMLoadFloat4x4(&m_matPivot), _eType, &m_vecBone);
		if (pMesh == nullptr)
			return E_FAIL;

		m_vecMesh.push_back(pMesh);
	}

	return S_OK;
}
HRESULT CModel::Ready_Material(const char* _pModelFilePath)
{
	// 재질 정보가 몇개인지
	m_iNumMaterails = m_pAIScene->mNumMaterials;

	for (size_t i = 0; i < m_iNumMaterails; ++i)
	{
		// aiMaterial* pMaterials = m_pAIScene->mMaterials[i];
		// aiMaterial* pMaterials = new aiMaterial;
		// 초기화
		MESH_MATERIAL MeshMaterial;
		ZeroMemory(&MeshMaterial, sizeof(MeshMaterial));

		for (_int j = 0; j < AI_TEXTURE_TYPE_MAX; ++j)
		{
			//aiString strTexturePath;
			//
			//// 해당 재질 정보가 없으면 다음으로
			//if (FAILED(pMaterials->GetTexture(aiTextureType(j), 0, &strTexturePath)))
			//	continue;
			//
			//// 드라이브, 경로 추출
			//char			szDrive[MAX_PATH] = "";
			//char			szDirectory[MAX_PATH] = "";
			//_splitpath_s(_pModelFilePath, szDrive, MAX_PATH, szDirectory, MAX_PATH, nullptr, 0, nullptr, 0);
			//
			//// 파일명.확장자 추출
			//char			szFileName[MAX_PATH] = "";
			//char			szExt[MAX_PATH] = "";
			//_splitpath_s(strTexturePath.data, nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szExt, MAX_PATH);
			//
			//// 드라이브 복사 후 뒤에 경로, 파일명, 확장자를 이어 붙인다.
			//char			szTmp[MAX_PATH] = "";
			//strcpy_s(szTmp, szDrive);
			//strcat_s(szTmp, szDirectory);
			//strcat_s(szTmp, szFileName);
			//strcat_s(szTmp, szExt);

			if (m_pAIScene->mMaterials[i].mBIsReturn[j] != 1)
				continue;

			// 문자열 타입 변환 필요.
			_tchar			szFullPath[MAX_PATH] = TEXT("");
			MultiByteToWideChar(CP_ACP, 0, m_pAIScene->mMaterials[i].mStrTextureFilePath[j].c_str(), 
				strlen(m_pAIScene->mMaterials[i].mStrTextureFilePath[j].c_str()), szFullPath, MAX_PATH);

			// 위에서 조합한 경로로 텍스쳐를 불러온다.
			MeshMaterial.pTextures[j] = CTexture::Create(m_pDevice, m_pContext, szFullPath);
			if (nullptr == MeshMaterial.pTextures[j])
				return E_FAIL;
		}

		// 텍스쳐들을 다 저장한 구조체 push.
		m_vecMaterial.push_back(MeshMaterial);
	}

	return S_OK;
}
HRESULT CModel::Ready_Animation()
{
	m_iNumAnimation = m_pAIScene->m_iNumAnimation;

	for (size_t i = 0; i < m_iNumAnimation; ++i)
	{
		CAnimation* pAnimation = CAnimation::Create(this, &m_pAIScene->mAnimations[i]);
		if (pAnimation == nullptr)
			return E_FAIL;

		m_vecAnimation.push_back(pAnimation);
	}

	return S_OK;
}

_int CModel::Get_BoneIndex(const char* _strBoneName) const
{
	for (size_t iBoneIndex = 0; iBoneIndex < m_vecBone.size(); ++iBoneIndex)
	{
		if (false == strcmp(m_vecBone[iBoneIndex]->Get_BoneName(), _strBoneName))
			return (_int)iBoneIndex;
	}

	return -1;
}
CBone* CModel::Get_BonePtr(const char* _strBoneName) const
{
	//auto	iter = find_if(m_vecBone.begin(), m_vecBone.end(), [&](CBone* pBone)
	//	{
	//		if (false == strcmp(pBone->Get_BoneName(), _strBoneName))
	//			return true;
	//		return false;
	//	});
	//
	//return *iter;

	for (size_t iBoneIndex = 0; iBoneIndex < m_vecBone.size(); ++iBoneIndex)
	{
		if (false == strcmp(m_vecBone[iBoneIndex]->Get_BoneName(), _strBoneName))
			return m_vecBone[iBoneIndex];
	}

	return nullptr;
}

void CModel::Set_AnimationStop(_bool _bIsStop)
{
	m_vecAnimation[m_iCurAnimationIndex]->Set_AnimationStop(_bIsStop);
}

HRESULT CModel::SetUp_Animation(_bool _bIsLoop, string _strAnimationName, _uint _iChangeIndex, _bool _bIsReset, _bool _bIsQuickChange)
{
	m_bIsFindAnimation = false;
	_int iAnimationIndex = -1;

	/* 예외 처리 */
	/* 이미 진행 중인 애니메이션인지 */
	// if (!strcmp(m_vecAnimation[m_iCurAnimationIndex]->Get_AnimationName(), _strAnimationName.c_str()))
	// {
	// 	return S_OK;
	// }

	/* 존재하는 애니메이션인지 */
	for (_uint i = 0; i < m_iNumAnimation; ++i)
	{
		if (!strcmp(m_vecAnimation[i]->Get_AnimationName(), _strAnimationName.c_str()))
		{
			iAnimationIndex = i;
			m_bIsFindAnimation = true;
			break;
		}
	}

	if (!m_bIsFindAnimation)
		return S_OK;


	if (m_bIsChanging)
	{
		m_iNextAnimationIndex = iAnimationIndex;
		m_bIsChanging = false;
	}

	// 리셋 하기 전 보간
	if (m_iCurAnimationIndex != iAnimationIndex)
	{
		if (!_bIsQuickChange)
		{
			m_iNextAnimationIndex = iAnimationIndex;
			m_vecAnimation[m_iNextAnimationIndex]->Set_Loop(_bIsLoop);
			m_iChangeIndex = _iChangeIndex;
			m_vecAnimation[m_iCurAnimationIndex]->Ready_ChangeAnimation(m_iChangeIndex);
			m_bIsChanging = true;
		}
		else
		{
			m_iCurAnimationIndex = iAnimationIndex;
			m_vecAnimation[m_iCurAnimationIndex]->ReSet();
			m_vecAnimation[m_iCurAnimationIndex]->Set_Loop(_bIsLoop);
		}

	}
	else
	{
		if (_bIsReset)
		{
			m_vecAnimation[m_iCurAnimationIndex]->ReSet();
			m_vecAnimation[m_iCurAnimationIndex]->Set_Loop(_bIsLoop);
		}
	}

	return S_OK;
}
HRESULT CModel::Play_Animation(_float _fTimeDelta, _float _fPlaySpeed)
{
	/* Non Anim 모델이면 바로 return */
	if (0 == m_iNumAnimation)
		return S_OK;

	/* 뼈들의 TransformationMatrix를 애니메이션 상태에 맞도록 바꿔준다.
	   (Animation -> Channel -> 시간에 맞는 KeyFrame)*/

	   /* 이 애니메이션에 사용되는 부모에 상대적인 자기 자신의 뼈를 갱신하고 */
	if (!m_bIsChanging)
	{
		m_vecAnimation[m_iCurAnimationIndex]->Update_TransformationMatrix(m_vecBone, _fTimeDelta * _fPlaySpeed);
	}
	else if (m_bIsChanging)
	{
		if (m_vecAnimation[m_iCurAnimationIndex]->Change_TransformationMatrix(m_vecBone, m_vecAnimation[m_iNextAnimationIndex]->Get_Channel(), _fTimeDelta * _fPlaySpeed))
		{
			m_bIsChanging = false;
			m_vecAnimation[m_iCurAnimationIndex]->ReSet();
			m_iCurAnimationIndex = m_iNextAnimationIndex;
			m_vecAnimation[m_iNextAnimationIndex]->Reset_TrackPosition(m_iChangeIndex);
		}
	}

	/* 모든 뼈들의 CombinedTransformationMatrix를 갱신한다. */
	for (size_t i = 0; i < m_vecBone.size(); ++i)
	{
		m_vecBone[i]->Update_CombinedTransformationMatrix(m_vecBone);
	}

	return S_OK;
}
string CModel::Get_CurAnimationName()
{
	return m_vecAnimation[m_iCurAnimationIndex]->Get_AnimationName();
}
string CModel::Get_NextAnimationName()
{
	return m_vecAnimation[m_iNextAnimationIndex]->Get_AnimationName();
}
_bool CModel::Get_IsAnimationFin()
{
	return m_vecAnimation[m_iCurAnimationIndex]->Get_Finish();
}
_bool CModel::Get_CurAnimationIsLoop()
{
	return m_vecAnimation[m_iCurAnimationIndex]->Get_CurAnimationIsLoop();
}
_uint CModel::Get_CurFrameIndex()
{
	_uint iIdx = 0;

	vector<_uint> vecCurKeyFrame = m_vecAnimation[m_iCurAnimationIndex]->Get_CurKeyFrame();

	for (_uint i = 0; i < vecCurKeyFrame.size(); ++i)
	{
		if (iIdx < vecCurKeyFrame[i])
			iIdx = vecCurKeyFrame[i];
	}

	return iIdx;
}

void CModel::Set_KeyFrame(_uint _iFrame)
{
	m_vecAnimation[m_iCurAnimationIndex]->Set_KeyFrame(_iFrame);
}

void CModel::Update_VI(const _fmatrix& _matPivot)
{
	for (size_t i = 0; i < m_vecMesh.size(); ++i)
	{
		m_vecMesh[i]->Update_VI(_matPivot);
	}
}

CModel* CModel::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext, const char* _strModleFilePath, _fmatrix _matPivot, MODEL_TYPE _eType)
{
	CModel* pInstance = new CModel(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize_ProtoType(_strModleFilePath, _matPivot, _eType)))
	{
		MSG_BOX("Fail Create : CModel ProtoType");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CModel::Clone(void* _pArg)
{
	CModel* pInstance = new CModel(*this);

	if (FAILED(pInstance->Initialize_Clone(_pArg)))
	{
		MSG_BOX("Fail Cloen : CModel Clone");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CModel::Free()
{
	__super::Free();

	for (auto& iter : m_vecAnimation)
	{
		Safe_Release(iter);
	}
	m_vecAnimation.clear();

	for (auto& iter : m_vecMaterial)
	{
		for (size_t i = 0; i < AI_TEXTURE_TYPE_MAX; ++i)
		{
			Safe_Release(iter.pTextures[i]);
		}
	}
	m_vecMaterial.clear();

	for (auto& iter : m_vecMesh)
	{
		Safe_Release(iter);
	}
	m_vecMesh.clear();

	for (auto& iter : m_vecBone)
	{
		Safe_Release(iter);
	}
	m_vecBone.clear();

	Safe_Release(m_pAIScene);

	// m_Importer.FreeScene();
}
