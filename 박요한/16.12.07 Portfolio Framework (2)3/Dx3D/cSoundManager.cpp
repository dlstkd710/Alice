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
	for (; iter != _pTotalSounds.end();)
	{
		if (iter->second->_Sound != NULL)
		{
			iter->second->_Sound->DeleteDirectSound();
			SAFE_DELETE(iter->second->_Sound);
			iter = _pTotalSounds.erase(iter);
		}
		else
		{
			++iter;
		}
	}

	//¸Ê ÀüÃ¼ »èÁ¦
	_pTotalSounds.clear();

	SoundbuffersIter iterbuf = _pTotalBuffers.begin();
	for (; iterbuf != _pTotalBuffers.end();)
	{
		if (iterbuf->second != NULL)
		{
			//iterbuf->second->AddRef();
			//SAFE_DELETE(iterbuf->second);
			iterbuf = _pTotalBuffers.erase(iterbuf);
		}
		else
		{
			++iterbuf;
		}
	}

	//¸Ê ÀüÃ¼ »èÁ¦
	_pTotalBuffers.clear();
}

void cSoundManager::update(void)
{

}

void cSoundManager::addSound(string keyName, char *soundName, bool bgm)
{
	cSound* AdSound = NULL;
	AdSound = new cSound;
	LPDIRECTSOUNDBUFFER AdBuffer = NULL;
	AdSound->CreateDirectSound(g_hWnd);
	AdSound->LoadWave(soundName, &AdBuffer);
	g_sound *NewGsound = NULL;
	NewGsound = new g_sound;
	NewGsound->_Sound = AdSound;
	NewGsound->_bgm = bgm;

	_pTotalSounds.insert(make_pair(keyName, NewGsound));
	_pTotalBuffers.insert(make_pair(keyName, AdBuffer));
}

void cSoundManager::play(string keyName, bool loop, float volume)
{
	if (_pTotalSounds[keyName])
	{
		_pTotalSounds[keyName]->_Sound->Play(_pTotalBuffers[keyName], TRUE);
		_pTotalSounds[keyName]->_Sound->SetVolume(_pTotalBuffers[keyName], volume);
	}
}

void cSoundManager::stop(string keyName)
{
	if (_pTotalSounds[keyName])
	{
		_pTotalSounds[keyName]->_Sound->stop(_pTotalBuffers[keyName]);
	}
}
