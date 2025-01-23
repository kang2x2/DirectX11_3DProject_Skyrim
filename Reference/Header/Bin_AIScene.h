#pragma once

#include "Base.h"

BEGIN(Engine)

class CBin_AIScene final : public CBase
{
public:
#pragma region Node

	// 노드
	typedef struct tagNodeDesc
	{
		int							mParentIndex = 0;
		unsigned int				mNumChildren = 0;
		aiString					mName;
		aiMatrix4x4					mTransformation;
	}DESC_NODE;

#pragma endregion

#pragma region Mesh

	// 메시 본 웨이트
	typedef struct tagMeshBoneWeightDesc
	{
		unsigned int mVertexId = 0;
		_float		 mWeight;
	}DESC_MESHBONEWEIGHT;

	// 메시 본
	typedef struct tagMeshBoneDesc
	{
		aiMatrix4x4			 mOffsetMatrix;
		aiString			 mName;
		unsigned int		 mNumWeights = 0;

		vector<DESC_MESHBONEWEIGHT> mWeights;
	}DESC_MESHBONE;

	// 메시 면
	typedef struct tagMeshFaceDesc
	{
		unsigned int mIndices[3];
	}DESC_MESHFACE;

	// 메시
	typedef struct tagMeshDesc
	{
		aiString		mName;
		unsigned int	mMaterialIndex = 0;
		unsigned int	mNumVertices = 0;
		unsigned int	mNumFaces = 0;
		unsigned int	mNumBones = 0;

		vector<DESC_MESHFACE>   mFaces;
		vector<aiVector3D>		mVertices;
		vector<aiVector3D>		mNormals;
		vector<_float2>		mTextureCoords;
		vector<aiVector3D>		mTangents;
		vector<DESC_MESHBONE>   mBones;
	}DESC_MESH;

#pragma endregion

#pragma region Material
	typedef struct tagMaterialDesc
	{
		_int   mBIsReturn[AI_TEXTURE_TYPE_MAX];
		string mStrTextureFilePath[AI_TEXTURE_TYPE_MAX];
	}DESC_MATERIAL;

#pragma region Animation

	// 채널 키(time, value)
	typedef struct tagChannelScaleKeyDesc
	{
		double		mTime = 0.f;
		aiVector3D  mValue;
	}DESC_CHANNELSCALEKEY;

	typedef struct tagChannelRotKeyDesc
	{
		double		 mTime = 0.f;
		aiQuaternion mValue;
	}DESC_CHANNELROTKEY;

	typedef struct tagChannelPosKeyDesc
	{
		double		mTime = 0.f;
		aiVector3D  mValue;
	}DESC_CHANNELPOSKEY;

	// 채널
	typedef struct tagAnimationChannelDesc
	{
		aiString		mNodeName;
		unsigned int	mNumScalingKeys = 0;
		unsigned int	mNumRotationKeys = 0;
		unsigned int	mNumPositionKeys = 0;

		vector<DESC_CHANNELSCALEKEY>	mScalingKeys;
		vector<DESC_CHANNELROTKEY>		mRotationKeys;
		vector<DESC_CHANNELPOSKEY>		mPositionKeys;

	}DESC_ANIMATIONCHANNEL;

	// 애니메이션
	typedef struct tagAnimationDesc
	{
		aiString				mName;
		double					mDuration = 0.f;
		double					mTicksPerSecond = 0.f;
		unsigned int			mNumChannels = 0;

		vector<DESC_ANIMATIONCHANNEL>  mChannels;

	}DESC_ANIMATION;

#pragma endregion

private:
	CBin_AIScene();
	virtual ~CBin_AIScene() = default;

public:
	vector<DESC_NODE>		mVecNode;

	unsigned int			mNumMeshes = 0;
	vector<DESC_MESH>		mMeshs;

	unsigned int			mNumMaterials = 0;
	vector<DESC_MATERIAL>	mMaterials;

	unsigned int			m_iNumAnimation = 0;
	vector<DESC_ANIMATION>	mAnimations;

public:
	static CBin_AIScene* Create();
	virtual void Free() override;
};

END

