#include "audio.h"
#include <AL/al.h>
#include <AL/alc.h>
#include <cmath>
#include <vector>
#include <iostream>

static ALCdevice*  alDevice  = nullptr;
static ALCcontext* alContext = nullptr;
static ALuint      alBuffer  = 0;
static ALuint      alSource  = 0;

// Generate a deep space ambient drone:
// Layered sine waves at low frequencies with slow LFO modulation
static std::vector<short> generateSpaceAmbient(int sampleRate, int seconds) {
    int totalSamples = sampleRate * seconds;
    std::vector<short> samples(totalSamples);

    for (int i = 0; i < totalSamples; i++) {
        double t = (double)i / sampleRate;

        // Base drone layers (low frequency harmonics)
        double wave = 0.0;
        wave += 0.30 * sin(2.0 * M_PI * 55.0  * t);   // A1 - deep bass
        wave += 0.20 * sin(2.0 * M_PI * 82.5  * t);   // E2 - fifth
        wave += 0.15 * sin(2.0 * M_PI * 110.0 * t);   // A2 - octave
        wave += 0.10 * sin(2.0 * M_PI * 165.0 * t);   // E3
        wave += 0.08 * sin(2.0 * M_PI * 220.0 * t);   // A3

        // Slow LFO modulation (0.05 Hz) — gives breathing/pulsing feel
        double lfo = 0.75 + 0.25 * sin(2.0 * M_PI * 0.05 * t);
        wave *= lfo;

        // Second slow LFO on a slightly detuned layer for shimmer
        double shimmer = 0.08 * sin(2.0 * M_PI * 111.0 * t)
                       * (0.5 + 0.5 * sin(2.0 * M_PI * 0.03 * t));
        wave += shimmer;

        // Very subtle high-frequency sparkle
        double sparkle = 0.03 * sin(2.0 * M_PI * 880.0 * t)
                       * (0.5 + 0.5 * sin(2.0 * M_PI * 0.07 * t));
        wave += sparkle;

        // Fade in first 3 seconds, fade out last 3 seconds (for seamless loop)
        if (t < 3.0)
            wave *= t / 3.0;
        else if (t > seconds - 3.0)
            wave *= (seconds - t) / 3.0;

        // Clamp and convert to 16-bit PCM
        wave *= 0.85;
        if (wave >  1.0) wave =  1.0;
        if (wave < -1.0) wave = -1.0;
        samples[i] = (short)(wave * 32767);
    }

    return samples;
}

void initAudio() {
    alDevice = alcOpenDevice(nullptr);
    if (!alDevice) {
        std::cerr << "Audio: failed to open device\n";
        return;
    }

    alContext = alcCreateContext(alDevice, nullptr);
    if (!alcMakeContextCurrent(alContext)) {
        std::cerr << "Audio: failed to create context\n";
        return;
    }

    const int SAMPLE_RATE = 44100;
    const int DURATION    = 60;   // 60-second loop

    std::cout << "Audio: generating space ambient sound..." << std::endl;
    auto samples = generateSpaceAmbient(SAMPLE_RATE, DURATION);

    alGenBuffers(1, &alBuffer);
    alBufferData(alBuffer, AL_FORMAT_MONO16,
                 samples.data(),
                 samples.size() * sizeof(short),
                 SAMPLE_RATE);

    alGenSources(1, &alSource);
    alSourcei(alSource, AL_BUFFER,   alBuffer);
    alSourcei(alSource, AL_LOOPING,  AL_TRUE);
    alSourcef(alSource, AL_GAIN,     0.6f);
    alSourcePlay(alSource);

    std::cout << "Audio: space ambient playing\n";
}

void cleanupAudio() {
    if (alSource) { alSourceStop(alSource); alDeleteSources(1, &alSource); }
    if (alBuffer) { alDeleteBuffers(1, &alBuffer); }
    if (alContext) { alcMakeContextCurrent(nullptr); alcDestroyContext(alContext); }
    if (alDevice)  { alcCloseDevice(alDevice); }
}
