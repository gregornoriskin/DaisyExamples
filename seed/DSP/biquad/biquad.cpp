#include "daisysp.h"
#include "daisy_seed.h"

using namespace daisysp;
using namespace daisy;

static DaisySeed  seed;
static Biquad     flt;
static Oscillator osc, lfo;

static void AudioCallback(AudioHandle::InterleavingInputBuffer  in,
                          AudioHandle::InterleavingOutputBuffer out,
                          size_t                                size)
{
    float saw, freq, output;
    for(size_t i = 0; i < size; i += 2)
    {
        freq = 10000 + (lfo.Process() * 10000);
        saw  = osc.Process();

        flt.SetCutoff(freq);
        output = flt.Process(saw);

        // left out
        out[i] = output;

        // right out
        out[i + 1] = output;
    }
}

int main(void)
{
    // initialize seed hardware and daisysp modules
    float sample_rate;
    seed.Configure();
    seed.Init();
    seed.SetAudioBlockSize(4);
    sample_rate = seed.AudioSampleRate();

    // initialize Biquad and set parameters
    flt.Init(sample_rate);
    flt.SetRes(0.7);

    // set parameters for sine oscillator object
    lfo.Init(sample_rate);
    lfo.SetWaveform(Oscillator::WAVE_TRI);
    lfo.SetAmp(1);
    lfo.SetFreq(.4);

    // set parameters for sine oscillator object
    osc.Init(sample_rate);
    osc.SetWaveform(Oscillator::WAVE_POLYBLEP_SAW);
    osc.SetFreq(100);
    osc.SetAmp(0.25);


    // start callback
    seed.StartAudio(AudioCallback);


    while(1) {}
}
