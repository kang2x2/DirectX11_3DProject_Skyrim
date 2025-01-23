#pragma once
#include "Base.h"

BEGIN(Engine)

class CSound_Manager final : public CBase
{
	DECLARE_SINGLETON(CSound_Manager)

private:
	CSound_Manager();
	virtual ~CSound_Manager() = default;

public:
	HRESULT Ready_Sound();

public:
	HRESULT PlaySoundFile(const wstring& strSoundKey, CHANNELID eCh, _float fVolume);
	HRESULT CheckPlaySoundFile(const wstring& strSoundKey, CHANNELID eCh, _float fVolume);
	HRESULT PlayBGM(const wstring& strSoundKey, _float fVolume);
	HRESULT StopSound(CHANNELID eCh);
	HRESULT StopSoundAll();
	HRESULT SetChannelVolume(CHANNELID eCh, _float fVolume);

private:
	HRESULT LoadSoundFile();

private:
	FMOD_SYSTEM*	m_pSystem = { nullptr };

	FMOD_CHANNEL*	m_pChannelArr[_uint(CHANNELID::CHANNEL_END)];

	map<const wstring, FMOD_SOUND*> m_Sounds;


public:
	virtual void Free();

};

END