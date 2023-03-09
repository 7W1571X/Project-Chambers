#ifndef PROJECT_CHAMBERS_AUDIO_H_
#define PROJECT_CHAMBERS_AUDIO_H_
#include "../stdafx.h"

#ifdef _XBOX 
#define fourccRIFF 'RIFF'
#define fourccDATA 'data'
#define fourccFMT 'fmt '
#define fourccWAVE 'WAVE'
#define fourccXWMA 'XWMA'
#define fourccDPDS 'dpds'
#endif

#ifndef _XBOX
#define fourccRIFF 'FFIR'
#define fourccDATA 'atad'
#define fourccFMT ' tmf'
#define fourccWAVE 'EVAW'
#define fourccXWMA 'AMWX'
#define fourccDPDS 'sdpd'
#endif

class Audio
{
private:
    float volume{ 1.0f };
    bool loop{ false };
    HRESULT hr;
    IXAudio2* pXAudio2;
    IXAudio2MasteringVoice* pMasterVoice;
    static HRESULT FindChunk(const HANDLE hFile, DWORD fourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition);
    static HRESULT ReadChunkData(const HANDLE hFile, void* buffer, DWORD buffersize, DWORD bufferoffset);

public:
    Audio();
    int Play(std::string path); // plays the audio file with specified volume and can be looped
    void setLoop(bool shouldLoop);
    void setVolume(float vol);

    std::string BasePath{ "res//sounds" }; // Directory where all audio files (relevant to project) are stored i.e if all audio files are stored in "D:\game" than set BasePath to "D:\game", this will be automatically added in path of every audio file
};

#endif