#include "stdafx.h"
#include "cSound.h"


cSound::cSound()
{
	LPDIRECTSOUND8	g_lpDS = NULL;
	BOOL			g_bPlay = FALSE;
}


cSound::~cSound()
{
}

//DirectSound 객체를 생성하고 협력레벨을 설정한다.
BOOL cSound::CreateDirectSound(HWND hWnd)
{
	//다이렉트 사운드 개체 생성
	if (DirectSoundCreate8(NULL, &g_lpDS, NULL) != DS_OK)
		return FALSE;

	//협력수준 설정 - DSSCL_NORMAL로 설정
	if (g_lpDS->SetCooperativeLevel(hWnd, DSSCL_NORMAL) != DS_OK)
		return FALSE;

	return TRUE;
}

//파일로부터 wav파일을 읽어 메모리에 로드한다
BOOL cSound::LoadWave(LPSTR lpFileName, LPDIRECTSOUNDBUFFER * lpDSBuffer)
{
	HMMIO hmmio;		//wave파일의 핸들
	MMCKINFO	ckInRIFF, ckIn;	//부모 청크, 자식청크
	PCMWAVEFORMAT	pcmWaveFormat;
	WAVEFORMATEX*		pWaveFormat;

	//웨이브 파일을 열어, MMIO 핸들을 얻는다
	hmmio = mmioOpen(lpFileName, NULL, MMIO_ALLOCBUF | MMIO_READ);
	if (hmmio == NULL) return FALSE;

	//내려갈 하위 청크이름을 등록하고, 현대 위치인 RIFF청크에서 WAVE청크를
	//찾아 내려간다.
	ckInRIFF.fccType = mmioFOURCC('W', 'A', 'V', 'E');
	if ((mmioDescend(hmmio, &ckInRIFF, NULL, MMIO_FINDRIFF)) != 0)
	{
		mmioClose(hmmio, 0);
		return FALSE;
	}

	//내려갈 하위 청크이름을 등록하고, 현재 위치인 wave청크에서 fmt청크를 찾아 내려간다
	//무든 청크는 4개의 문자코드를 갖기 때문에 t 다음에 공백문자가 있다.
	ckIn.ckid = mmioFOURCC('f', 'm', 't', ' ');
	if (mmioDescend(hmmio, &ckIn, &ckInRIFF, MMIO_FINDCHUNK) != 0)
	{
		mmioClose(hmmio, 0);
		return FALSE;
	}
	//fmt청크에서 wav파일 포맷(format)을 읽어 들인다
	if (mmioRead(hmmio, (HPSTR)&pcmWaveFormat, sizeof(pcmWaveFormat))
		!= sizeof(pcmWaveFormat))
	{
		mmioClose(hmmio, 0);
		return FALSE;
	}
	//WAVEFORMATEX를 메모리에 할당
	pWaveFormat = new WAVEFORMATEX;
	//PCMWAVEFORMAT로부터 복사한다.
	memcpy(pWaveFormat, &pcmWaveFormat, sizeof(pcmWaveFormat));
	pWaveFormat->cbSize = 0;

	//fmtChunk에서 부모청크인 WAVE Chunk로 올라간다
	if (mmioAscend(hmmio, &ckIn, 0))
	{
		mmioClose(hmmio, 0);
		return FALSE;
	}

	//내려갈 하위 청크이름을 등록하고, 현재 위치인 WAVE청크에서 data청크를
	//찾아 내려간다
	ckIn.ckid = mmioFOURCC('d', 'a', 't', 'a');
	if (mmioDescend(hmmio, &ckIn, &ckInRIFF, MMIO_FINDCHUNK) != 0)
	{
		mmioClose(hmmio, 0);
		return FALSE;
	}

	BYTE* pData = NULL;
	//data chunk 사이즈 만큼 메모리 할당
	pData = new BYTE[ckIn.cksize];
	//data chunk에 있는 순수한 wave data를 읽어 들인다.
	mmioRead(hmmio, (LPSTR)pData, ckIn.cksize);

	//여기까지 왔으면 wav파일읽기에 성고한것
	mmioClose(hmmio, 0);

	//DSBUFFERDESC 구조체 정보를 채운다
	DSBUFFERDESC dsbd;
	ZeroMemory(&dsbd, sizeof(DSBUFFERDESC));
	dsbd.dwSize = sizeof(DSBUFFERDESC);
	dsbd.dwFlags = DSBCAPS_CTRLDEFAULT | DSBCAPS_STATIC | DSBCAPS_LOCSOFTWARE;
	dsbd.dwBufferBytes = ckIn.cksize;
	dsbd.lpwfxFormat = pWaveFormat;

	//사운드 버퍼의 생성
	if (g_lpDS->CreateSoundBuffer(&dsbd, lpDSBuffer, NULL) != DS_OK)
		return FALSE;

	VOID* pBuff1 = NULL;	//사운드 버퍼의 첫번째 영역주소
	VOID* pBuff2 = NULL;	//사운드 버퍼의 두번째 영역주소
	DWORD dwLength;			//첫번째 영역크기
	DWORD dwLength2;		//두번째 영역크기
							//사운드 버퍼에 순수한 wav데이터를 복사하기 위해 락을 건다
	if ((*lpDSBuffer)->Lock(0, dsbd.dwBufferBytes, &pBuff1, &dwLength, &pBuff2, &dwLength2, 0L) != DS_OK)
	{
		(*lpDSBuffer)->Release();
		(*lpDSBuffer) = NULL;
		return FALSE;
	}

	memcpy(pBuff1, pData, dwLength);
	memcpy(pBuff2, (pData + dwLength), dwLength2);
	//잠금 상태를 풀어준다
	(*lpDSBuffer)->Unlock(pBuff1, dwLength, pBuff2, dwLength2);
	pBuff1 = pBuff2 = NULL;

	//할당된 메모리를 해제
	delete[] pData;
	delete pWaveFormat;

	return TRUE;
}

BOOL cSound::SetVolume(LPDIRECTSOUNDBUFFER lpDSBuffer, LONG lVolume)
{
	if (lpDSBuffer->SetVolume(DSVOLUME_TO_DB(lVolume)) != DS_OK)
		return FALSE;
	return TRUE;
}

BOOL cSound::SetPan(LPDIRECTSOUNDBUFFER lpDSBuffer, LONG lPan)
{
	if (lpDSBuffer->SetPan(lPan) != DS_OK)
		return FALSE;
	return TRUE;
}

void cSound::DeleteDirectSound()
{
	g_lpDS->Release();
	g_lpDS = NULL;
}

void cSound::Play(LPDIRECTSOUNDBUFFER lpDSBuffer, BOOL Loop)
{
	//버퍼가 비어있으면 종료
	if (lpDSBuffer == NULL) return;

	//재생중 실패하면 종료
	if (!lpDSBuffer->Play(0, 0, (Loop) ? 1 : 0)) return;
	g_bPlay = TRUE;
}

void cSound::stop(LPDIRECTSOUNDBUFFER lpDSBuffer)
{
	if (lpDSBuffer == NULL) return;
	lpDSBuffer->Stop();
	g_bPlay = FALSE;
	lpDSBuffer->SetCurrentPosition(0L);	//처음 위치로
}
