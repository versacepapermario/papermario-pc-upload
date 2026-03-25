#pragma once

#define LOAD_ASSET(path) (path == NULL ? NULL : (GameEngine_OTRSigCheck((const char*) path) ? ResourceGetDataByName((const char*) path) : path))
#define LOAD_ASSET_RAW(path) ResourceGetDataByName((const char*) path)

#ifdef __cplusplus
#include <vector>
#include <string>
#include <unordered_map>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <ship/controller/controldeck/ControlDeck.h>
#include <ship/Context.h>
#include <fast/interpreter.h>

#ifndef IDYES
#define IDYES 6
#endif
#ifndef IDNO
#define IDNO 7
#endif

// Paper Mario runs at 30fps NTSC (game logic at 30fps, display at 60fps via retrace toggle)
// Audio produces 32000 samples/sec. At 30fps game loop, each call must produce ~1067 samples.
// Use AUDIO_SAMPLES (184) aligned values to avoid buffer overflows in alAudioFrame.
#define PM64_GAME_FPS 30
#define SAMPLES_HIGH 1104  // 6 * 184 — 1104 * 30fps = 33120/sec (adaptive drains excess)
#define SAMPLES_LOW 920    // 5 * 184 — 920 * 30fps = 27600/sec (adaptive fills deficit)
#define AUDIO_FRAMES_PER_UPDATE 2
#define NUM_AUDIO_CHANNELS 2
#define SAMPLES_PER_FRAME (SAMPLES_HIGH * NUM_AUDIO_CHANNELS * 2)

struct AudioState {
    std::thread thread;
    std::mutex mutex;
    std::condition_variable cv_to_thread;
    std::condition_variable cv_from_thread;
    bool running = false;
    bool processing = false;
};

class GameEngine {
  public:
    static GameEngine* Instance;
    static AudioState audio;

    ImFont *fontStandard;
    ImFont *fontStandardLarger;
    ImFont *fontStandardLargest;
    ImFont *fontMono;
    ImFont *fontMonoLarger;
    ImFont *fontMonoLargest;

    std::shared_ptr<Ship::Context> context;

    GameEngine();
    static void Create(int argc, char* argv[]);
    static bool GenAssetFile(bool exitOnFail = true);
    void RunExtract(int argc, char* argv[]);
    void AudioInit();
    void FinishInit();
    void StartFrame() const;
    static void RunCommands(Gfx* Commands, const std::vector<std::unordered_map<Mtx*, MtxF>>& mtx_replacements);
    static uint32_t GetInterpolationFPS();
    static void HandleAudioThread();
    static void StartAudioFrame();
    static void EndAudioFrame();
    static void AudioExit();
    static void ProcessGfxCommands(Gfx* commands);
    static ImFont *CreateFontWithSize(float size, std::string fontPath);
    static void ScaleImGui();
    static bool IsAltAssetsEnabled();
    static uint32_t GetGameVersion();
    static void Destroy();
};

Fast::Interpreter* GameEngine_GetInterpreter();

extern "C" {
#endif

// HUD and Rendering related
float GameEngine_GetAspectRatio();
uint32_t OTRGetCurrentWidth(void);
uint32_t OTRGetCurrentHeight(void);
float OTRGetHUDAspectRatio();
int32_t OTRConvertHUDXToScreenX(int32_t v);
float OTRGetDimensionFromLeftEdge(float v);
float OTRGetDimensionFromRightEdge(float v);
int16_t OTRGetRectDimensionFromLeftEdge(float v);
int16_t OTRGetRectDimensionFromRightEdge(float v);
float OTRGetDimensionFromLeftEdgeForcedAspect(float v, float aspectRatio);
float OTRGetDimensionFromRightEdgeForcedAspect(float v, float aspectRatio);
int16_t OTRGetRectDimensionFromLeftEdgeForcedAspect(float v, float aspectRatio);
int16_t OTRGetRectDimensionFromRightEdgeForcedAspect(float v, float aspectRatio);
uint32_t OTRGetGameRenderWidth();
uint32_t OTRGetGameRenderHeight();

// Engine related
void GameEngine_ProcessGfxCommands(Gfx* commands);
uint32_t GameEngine_GetInterpolatedFPS();
uint32_t GameEngine_GetSampleRate();
uint32_t GameEngine_GetSamplesPerFrame();
uint32_t GameEngine_GetGameVersion();
bool GameEngine_OTRSigCheck(const char* imgData);

#ifdef __cplusplus
}
#endif
