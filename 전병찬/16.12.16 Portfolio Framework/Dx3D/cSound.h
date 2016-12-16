#pragma once
#ifndef DSBCAPS_CTRLDEFAULT
#define DSBCAPS_CTRLDEFAULT (DSBCAPS_CTRLFREQUENCY | DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME)
#endif

#define DSVOLUME_TO_DB(volume) ((DWORD)(-30*(100-volume)))

class cSound
{
private:
	LPDIRECTSOUND8	g_lpDS;
	BOOL			g_bPlay;
public:
	cSound();
	~cSound();
	BOOL CreateDirectSound(HWND hWnd);
	BOOL LoadWave(LPSTR lpFileName, LPDIRECTSOUNDBUFFER* lpDSBuffer);
	BOOL SetVolume(LPDIRECTSOUNDBUFFER lpDSBuffer, LONG lVolume);
	BOOL SetPan(LPDIRECTSOUNDBUFFER lpDSBuffer, LONG lPan);

	void DeleteDirectSound();
	void Play(LPDIRECTSOUNDBUFFER lpDSBuffer, BOOL Loop);
	void stop(LPDIRECTSOUNDBUFFER lpDSBuffer);

};

