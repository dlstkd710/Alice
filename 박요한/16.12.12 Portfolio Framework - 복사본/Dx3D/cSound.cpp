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

//DirectSound ��ü�� �����ϰ� ���·����� �����Ѵ�.
BOOL cSound::CreateDirectSound(HWND hWnd)
{
	//���̷�Ʈ ���� ��ü ����
	if (DirectSoundCreate8(NULL, &g_lpDS, NULL) != DS_OK)
		return FALSE;

	//���¼��� ���� - DSSCL_NORMAL�� ����
	if (g_lpDS->SetCooperativeLevel(hWnd, DSSCL_NORMAL) != DS_OK)
		return FALSE;

	return TRUE;
}

//���Ϸκ��� wav������ �о� �޸𸮿� �ε��Ѵ�
BOOL cSound::LoadWave(LPSTR lpFileName, LPDIRECTSOUNDBUFFER * lpDSBuffer)
{
	HMMIO hmmio;		//wave������ �ڵ�
	MMCKINFO	ckInRIFF, ckIn;	//�θ� ûũ, �ڽ�ûũ
	PCMWAVEFORMAT	pcmWaveFormat;
	WAVEFORMATEX*		pWaveFormat;

	//���̺� ������ ����, MMIO �ڵ��� ��´�
	hmmio = mmioOpen(lpFileName, NULL, MMIO_ALLOCBUF | MMIO_READ);
	if (hmmio == NULL) return FALSE;

	//������ ���� ûũ�̸��� ����ϰ�, ���� ��ġ�� RIFFûũ���� WAVEûũ��
	//ã�� ��������.
	ckInRIFF.fccType = mmioFOURCC('W', 'A', 'V', 'E');
	if ((mmioDescend(hmmio, &ckInRIFF, NULL, MMIO_FINDRIFF)) != 0)
	{
		mmioClose(hmmio, 0);
		return FALSE;
	}

	//������ ���� ûũ�̸��� ����ϰ�, ���� ��ġ�� waveûũ���� fmtûũ�� ã�� ��������
	//���� ûũ�� 4���� �����ڵ带 ���� ������ t ������ ���鹮�ڰ� �ִ�.
	ckIn.ckid = mmioFOURCC('f', 'm', 't', ' ');
	if (mmioDescend(hmmio, &ckIn, &ckInRIFF, MMIO_FINDCHUNK) != 0)
	{
		mmioClose(hmmio, 0);
		return FALSE;
	}
	//fmtûũ���� wav���� ����(format)�� �о� ���δ�
	if (mmioRead(hmmio, (HPSTR)&pcmWaveFormat, sizeof(pcmWaveFormat))
		!= sizeof(pcmWaveFormat))
	{
		mmioClose(hmmio, 0);
		return FALSE;
	}
	//WAVEFORMATEX�� �޸𸮿� �Ҵ�
	pWaveFormat = new WAVEFORMATEX;
	//PCMWAVEFORMAT�κ��� �����Ѵ�.
	memcpy(pWaveFormat, &pcmWaveFormat, sizeof(pcmWaveFormat));
	pWaveFormat->cbSize = 0;

	//fmtChunk���� �θ�ûũ�� WAVE Chunk�� �ö󰣴�
	if (mmioAscend(hmmio, &ckIn, 0))
	{
		mmioClose(hmmio, 0);
		return FALSE;
	}

	//������ ���� ûũ�̸��� ����ϰ�, ���� ��ġ�� WAVEûũ���� dataûũ��
	//ã�� ��������
	ckIn.ckid = mmioFOURCC('d', 'a', 't', 'a');
	if (mmioDescend(hmmio, &ckIn, &ckInRIFF, MMIO_FINDCHUNK) != 0)
	{
		mmioClose(hmmio, 0);
		return FALSE;
	}

	BYTE* pData = NULL;
	//data chunk ������ ��ŭ �޸� �Ҵ�
	pData = new BYTE[ckIn.cksize];
	//data chunk�� �ִ� ������ wave data�� �о� ���δ�.
	mmioRead(hmmio, (LPSTR)pData, ckIn.cksize);

	//������� ������ wav�����б⿡ �����Ѱ�
	mmioClose(hmmio, 0);

	//DSBUFFERDESC ����ü ������ ä���
	DSBUFFERDESC dsbd;
	ZeroMemory(&dsbd, sizeof(DSBUFFERDESC));
	dsbd.dwSize = sizeof(DSBUFFERDESC);
	dsbd.dwFlags = DSBCAPS_CTRLDEFAULT | DSBCAPS_STATIC | DSBCAPS_LOCSOFTWARE;
	dsbd.dwBufferBytes = ckIn.cksize;
	dsbd.lpwfxFormat = pWaveFormat;

	//���� ������ ����
	if (g_lpDS->CreateSoundBuffer(&dsbd, lpDSBuffer, NULL) != DS_OK)
		return FALSE;

	VOID* pBuff1 = NULL;	//���� ������ ù��° �����ּ�
	VOID* pBuff2 = NULL;	//���� ������ �ι�° �����ּ�
	DWORD dwLength;			//ù��° ����ũ��
	DWORD dwLength2;		//�ι�° ����ũ��
							//���� ���ۿ� ������ wav�����͸� �����ϱ� ���� ���� �Ǵ�
	if ((*lpDSBuffer)->Lock(0, dsbd.dwBufferBytes, &pBuff1, &dwLength, &pBuff2, &dwLength2, 0L) != DS_OK)
	{
		(*lpDSBuffer)->Release();
		(*lpDSBuffer) = NULL;
		return FALSE;
	}

	memcpy(pBuff1, pData, dwLength);
	memcpy(pBuff2, (pData + dwLength), dwLength2);
	//��� ���¸� Ǯ���ش�
	(*lpDSBuffer)->Unlock(pBuff1, dwLength, pBuff2, dwLength2);
	pBuff1 = pBuff2 = NULL;

	//�Ҵ�� �޸𸮸� ����
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
	//���۰� ��������� ����
	if (lpDSBuffer == NULL) return;

	//����� �����ϸ� ����
	if (!lpDSBuffer->Play(0, 0, (Loop) ? 1 : 0)) return;
	g_bPlay = TRUE;
}

void cSound::stop(LPDIRECTSOUNDBUFFER lpDSBuffer)
{
	if (lpDSBuffer == NULL) return;
	lpDSBuffer->Stop();
	g_bPlay = FALSE;
	lpDSBuffer->SetCurrentPosition(0L);	//ó�� ��ġ��
}
