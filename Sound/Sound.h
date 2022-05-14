#pragma once
#include "../BaseDirectX/BaseDirectX.h"

struct ChunkHeader
{
	char id[4];
	int32_t size;
};

struct RiffHeader
{
	ChunkHeader chunk;
	char type[4];
};

struct FormatChunk
{
	ChunkHeader chunk;
	WAVEFORMATEX fmt;
};

struct SoundData
{
	~SoundData();
	WAVEFORMATEX wfex;
	BYTE *pBuffer;
	unsigned int bufferSize;
	IXAudio2SourceVoice *pSourceVoice;
	XAUDIO2_BUFFER buf{};
};

class Sound
{
public:
	//音
	static ComPtr<IXAudio2> xAudio2;
	static IXAudio2MasteringVoice *masterVoice;
	//初期化
	static void CreateVoice();
};

//サウンド読み込み
void SoundLoad(const char *filename, SoundData &sound);
//サウンドの削除
void SoundUnload(SoundData *sounddata);
//サウンド再生
void SoundPlayerWave(SoundData &soundData);
void SoundPlayLoop(SoundData &soundData);
//サウンドを止める
void SoundStopWave(SoundData &soundData);
//一度だけ鳴らす
void SoundPlayOnce(SoundData &soundData);
//-10000~10000の間.0に向かって収束していく.0がミュート
void SoundVolume(SoundData &soundData, float volume);

class SoundControl
{
private:
	SoundControl();
	~SoundControl();
public:
	SoundControl(const SoundControl &obj) = delete;
	SoundControl &operator= (const SoundControl &obj) = delete;
	static SoundControl* GetSound();
};