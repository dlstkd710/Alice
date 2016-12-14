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

	//사운드 추가(키값, 파일이름, bgm, 루프)
	void addSound(string keyName, char* soundName, bool bgm);

	//사운드 플레이(키값, 볼륨) 0.0f 100.0f
	void play(string keyName, bool loop, float volume = 100.0f);
	//사운드 정지
	void stop(string keyName);


};

