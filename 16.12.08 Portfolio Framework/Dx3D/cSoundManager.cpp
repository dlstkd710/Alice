#include "stdafx.h"
#include "cSoundManager.h"


cSoundManager::cSoundManager()
{

}


cSoundManager::~cSoundManager()
{
}

HRESULT cSoundManager::init(void)
{
	return S_OK;
}

void cSoundManager::release(void)
{
	SoundsIter iter = _pTotalSounds.begin();
	for (iter; iter != _pTotalSounds.end(); ++iter)
	{
		_pTotalSounds.erase(iter);
	}
	SoundbuffersIter iterbuf = _pTotalBuffers.begin();
	for (iterbuf; iterbuf != _pTotalBuffers.end(); ++iterbuf)
	{
		_pTotalBuffers.erase(iterbuf);
	}
	ZeroMemory(&_pTotalSounds, sizeof(_pTotalSounds));
	ZeroMemory(&_pTotalBuffers, sizeof(_pTotalBuffers));
}

void cSoundManager::update(void)
{

}

void cSoundManager::addSound(string keyName, char *soundName)
{
	cSound* AdSound = NULL;
	AdSound = new cSound;
	LPDIRECTSOUNDBUFFER AdBuffer = NULL;
	AdSound->CreateDirectSound(g_hWnd);
	AdSound->LoadWave(soundName, &AdBuffer);
	_pTotalSounds.insert(make_pair(keyName, AdSound));
	_pTotalBuffers.insert(make_pair(keyName, AdBuffer));
}

void cSoundManager::play(string keyName,  bool loop, float volume)
{
	if (_pTotalSounds[keyName])
	{
		_pTotalSounds[keyName]->Play(_pTotalBuffers[keyName], TRUE);
		_pTotalSounds[keyName]->SetVolume(_pTotalBuffers[keyName], volume);
	}
}

void cSoundManager::stop(string keyName)
{
	if (_pTotalSounds[keyName])
	{
		_pTotalSounds[keyName]->stop(_pTotalBuffers[keyName]);
	}
}
