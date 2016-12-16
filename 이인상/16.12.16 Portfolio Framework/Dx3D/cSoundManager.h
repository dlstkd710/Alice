#pragma once
#include "cSound.h"
#define g_pSoundManager cSoundManager::GetInstance()
using namespace std;

struct g_sound
{
	bool _bgm;
	cSound* _Sound;
};
class cSoundManager
{
private:
	SINGLETONE(cSoundManager);
private:
	typedef map<string, g_sound*> Sounds;
	typedef map<string, g_sound*>::iterator SoundsIter;
	typedef map<string, LPDIRECTSOUNDBUFFER> Soundbuffers;
	typedef map<string, LPDIRECTSOUNDBUFFER>::iterator SoundbuffersIter;
private:
	Sounds _pTotalSounds;
	Soundbuffers _pTotalBuffers;
	//SoundsIter	_pTotalSoundsIter;

public:
	HRESULT init(void);
	void release(void);
	void update(void);

	//���� �߰�(Ű��, �����̸�, bgm, ����)
	void addSound(string keyName, char* soundName, bool bgm);

	//���� �÷���(Ű��, ����) 0.0f 100.0f
	void play(string keyName, bool loop, float volume = 100.0f);
	//���� ����
	void stop(string keyName);


};

