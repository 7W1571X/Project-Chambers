#include "../stdafx.h"
#include "audio.h"

Audio::Audio()
{
    hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    if (FAILED(hr))
        std::cout << hr;

    pXAudio2 = nullptr;
    if (FAILED(hr = XAudio2Create(&pXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR)))
        std::cout << hr;

    pMasterVoice = nullptr;
    if (FAILED(hr = pXAudio2->CreateMasteringVoice(&pMasterVoice)))
        std::cout << hr;
}

int Audio::Play(std::string path) {

    path = BasePath + "\\" + path;
    WAVEFORMATEXTENSIBLE wfx = { 0 };
    XAUDIO2_BUFFER buffer = { 0 };

#ifdef _XBOX
    char* strFileName = path;
#else
    auto* strFileName = new TCHAR[path.size() + 1];
    strFileName[path.size()] = 0;
    std::copy(path.begin(), path.end(), strFileName);
#endif

    // Open the file
    const HANDLE hFile = CreateFile(
        strFileName,
        GENERIC_READ,
        FILE_SHARE_READ,
        nullptr,
        OPEN_EXISTING,
        0,
        nullptr);

    if (INVALID_HANDLE_VALUE == hFile)
        return HRESULT_FROM_WIN32(GetLastError());

    if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, nullptr, FILE_BEGIN))
        return HRESULT_FROM_WIN32(GetLastError());

    DWORD dwChunkSize;
    DWORD dwChunkPosition;
    //check the file type, should be fourccWAVE or 'XWMA'
    FindChunk(hFile, fourccRIFF, dwChunkSize, dwChunkPosition);
    DWORD filetype;
    ReadChunkData(hFile, &filetype, sizeof(DWORD), dwChunkPosition);
    if (filetype != fourccWAVE)
        return S_FALSE;

    FindChunk(hFile, fourccFMT, dwChunkSize, dwChunkPosition);
    ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
    FindChunk(hFile, fourccDATA, dwChunkSize, dwChunkPosition);
    auto* pDataBuffer = new BYTE[dwChunkSize];
    ReadChunkData(hFile, pDataBuffer, dwChunkSize, dwChunkPosition);

    buffer.AudioBytes = dwChunkSize;  // size of the audio buffer in bytes
    buffer.pAudioData = pDataBuffer;  // buffer containing audio data
    buffer.Flags = XAUDIO2_END_OF_STREAM; // tell the source voice not to expect any data after this buffer
    if (loop) buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
    IXAudio2SourceVoice* pSourceVoice;

    if (FAILED(hr = pXAudio2->CreateSourceVoice(&pSourceVoice, (WAVEFORMATEX*)&wfx)))
        std::cout << hr;

    //if (FAILED(hr = pSourceVoice->SetSourceSampleRate(20000))) cout << hr;
    if (FAILED(hr = pSourceVoice->SubmitSourceBuffer(&buffer)))
        std::cout << hr;

    //if (FAILED(hr = pSourceVoice->SetFrequencyRatio(1.2))) cout << hr;
    pSourceVoice->SetVolume(volume);
    if (FAILED(hr = pSourceVoice->Start(0)))
        std::cout << hr;

    return 0;
}

void Audio::setLoop(bool shouldLoop) {
    loop = shouldLoop;
}

void Audio::setVolume(float vol) {
    volume = vol;
}

HRESULT Audio::FindChunk(const HANDLE hFile, DWORD fourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition)
{
    HRESULT hr = S_OK;
    if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, nullptr, FILE_BEGIN))
        return HRESULT_FROM_WIN32(GetLastError());

    DWORD dwChunkType;
    DWORD dwChunkDataSize;
    DWORD dwRIFFDataSize = 0;
    DWORD dwFileType;
    DWORD dwOffset = 0;

    while (hr == S_OK) {
        DWORD dwRead;
        if (0 == ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, nullptr))
            hr = HRESULT_FROM_WIN32(GetLastError());

        if (0 == ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, nullptr))
            hr = HRESULT_FROM_WIN32(GetLastError());

        switch (dwChunkType) {
        case fourccRIFF:
            dwRIFFDataSize = dwChunkDataSize;
            dwChunkDataSize = 4;

            if (0 == ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, nullptr))
                hr = HRESULT_FROM_WIN32(GetLastError());
            break;

        default:
            if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, dwChunkDataSize, nullptr, FILE_CURRENT))
                return HRESULT_FROM_WIN32(GetLastError());
        }

        dwOffset += sizeof(DWORD) * 2;

        if (dwChunkType == fourcc) {
            dwChunkSize = dwChunkDataSize;
            dwChunkDataPosition = dwOffset;
            return S_OK;
        }

        dwOffset += dwChunkDataSize;
        constexpr DWORD bytesRead = 0;
        if (bytesRead >= dwRIFFDataSize)
            return S_FALSE;

    }

    return S_OK;

}

HRESULT Audio::ReadChunkData(const HANDLE hFile, void* buffer, DWORD buffersize, DWORD bufferoffset)
{
    HRESULT hr = S_OK;
    if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, bufferoffset, nullptr, FILE_BEGIN))
        return HRESULT_FROM_WIN32(GetLastError());

    DWORD dwRead;
    if (0 == ReadFile(hFile, buffer, buffersize, &dwRead, nullptr))
        hr = HRESULT_FROM_WIN32(GetLastError());

    return hr;
}
