#pragma once
//#define g_pSoundManager cSoundManager::GetInstance()
#ifndef DSBCAPS_CTRLDEFAULT
#define DSBCAPS_CTRLDEFAULT (DSBCAPS_CTRLFREQUENCY | DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME)
#endif

#define DSVOLUME_TO_DB(volume) ((DWORD)(-30*(100-volume)))

class cSoundManager
{
private:
	LPDIRECTSOUND8	g_lpDS;
	BOOL			g_bPlay;
	//SINGLETONE(cSoundManager);
private:
	
public:
	cSoundManager();
	~cSoundManager();
	BOOL CreateDirectSound(HWND hWnd);
	BOOL LoadWave(LPSTR lpFileName, LPDIRECTSOUNDBUFFER* lpDSBuffer);
	BOOL SetVolume(LPDIRECTSOUNDBUFFER lpDSBuffer, LONG lVolume);
	BOOL SetPan(LPDIRECTSOUNDBUFFER lpDSBuffer, LONG lPan);

	void DeleteDirectSound();
	void Play(LPDIRECTSOUNDBUFFER lpDSBuffer, BOOL Loop);
	void stop(LPDIRECTSOUNDBUFFER lpDSBuffer);

};

