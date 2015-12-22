#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "TtsEngine.h"

#define VAL_TO_STR(val) TO_STR(val)
#define TO_STR(name) #name

#define OUTPUT_BUFFER_SIZE (128 * 1024)

using namespace android;

static bool synthesis_complete = false;

static FILE *outfp = stdout;

    /*
        UK English - eng / GBR
        US English - eng / USA
        French - fra / FRA
        German - deu / DEU
        Spanish - spa / SPA
        Italian - ita / ITA
    */
const char * getLanguage(const char * selector)
{
  if (strncmp(selector, "ITA", 3) == 0)
  {
      return "ita";
  } 
   else if (strncmp(selector, "FRA", 3) == 0)
  {
      return "fra";
  } 
   else if (strncmp(selector, "SPA", 3) == 0)
  {
      return "spa";
  } 
   else if (strncmp(selector, "DEU", 3) == 0)
  {
      return "deu";
  } 
   else 
  {
    return "eng";
  }
}
// @param [inout] void *&       - The userdata pointer set in the original
//                                 synth call
// @param [in]    uint32_t      - Track sampling rate in Hz
// @param [in] tts_audio_format - The audio format
// @param [in]    int           - The number of channels
// @param [inout] int8_t *&     - A buffer of audio data only valid during the
//                                execution of the callback
// @param [inout] size_t  &     - The size of the buffer
// @param [in] tts_synth_status - indicate whether the synthesis is done, or
//                                 if more data is to be synthesized.
// @return TTS_CALLBACK_HALT to indicate the synthesis must stop,
//         TTS_CALLBACK_CONTINUE to indicate the synthesis must continue if
//            there is more data to produce.
tts_callback_status synth_done(void *& userdata, uint32_t sample_rate,
        tts_audio_format audio_format, int channels, int8_t *& data, size_t& size, tts_synth_status status)
{
    //fprintf(stderr, "TTS callback, rate: %d, data size: %d, status: %i\n", sample_rate, size, status);

    if (status == TTS_SYNTH_DONE)
    {
        synthesis_complete = true;
    }

    if ((size == OUTPUT_BUFFER_SIZE) || (status == TTS_SYNTH_DONE))
    {
        fwrite(data, size, 1, outfp);
    }

    return TTS_CALLBACK_CONTINUE;
}

static void usage(const char * name)
{
    fprintf(stderr, "\nUsage:\n\n%s " \
                    "\t[-o filename] \"Text to speak\"\n\n" \
                    "\t-o\tFile to write audio to (default stdout)\n", name);
    exit(0);
}

int main(int argc, char *argv[])
{
    tts_result result;
    TtsEngine* ttsEngine = getTtsEngine();
    uint8_t* synthBuffer;
    char* synthInput = NULL;
    int currentOption;
    char* outputFilename = NULL;
    const char* ttsLang = NULL;
    const char* appName = NULL;

    appName = argv[0];
    fprintf(stderr, "%s\n", appName);

    if (argc == 1)
    {
        usage(appName);
    }

    while ( (currentOption = getopt(argc, argv, "o:h")) != -1)
    {
        switch (currentOption)
        {
        case 'o':
            outputFilename = optarg;
            fprintf(stderr, "Output audio to file '%s'\n", outputFilename);
            break;
        case 'h':
            usage(appName);
            break;
        default:
            printf ("Getopt returned character code 0%o ??\n", currentOption);
        }
    }

    if (optind < argc)
    {
        synthInput = argv[optind];
    }

    if (!synthInput)
    {
        fprintf(stderr, "Error: no input string\n");
        usage(appName);
    }

    fprintf(stderr, "Input string: \"%s\"\n", synthInput);

    synthBuffer = new uint8_t[OUTPUT_BUFFER_SIZE];

    result = ttsEngine->init(synth_done, "../lang/");

    if (result != TTS_SUCCESS)
    {
        fprintf(stderr, "Failed to init TTS\n");
    }

    // ---------------------------------------------------------------------------------
#ifdef TTSLANG
    ttsLang = VAL_TO_STR( TTSLANG ) ;
#else  
    ttsLang = "USA";
#endif
    /*
        UK English - eng / GBR
        US English - eng / USA
        French - fra / FRA
        German - deu / DEU
        Spanish - spa / SPA
        Italian - ita / ITA
    */
    fprintf(stderr, "locale %s/%s\n", getLanguage(ttsLang), ttsLang);
    result = ttsEngine->setLanguage(getLanguage(ttsLang), ttsLang, "");

    // ---------------------------------------------------------------------------------

    if (result != TTS_SUCCESS)
    {
        fprintf(stderr, "Failed to load language\n");
    }

    if (outputFilename)
    {
        outfp = fopen(outputFilename, "wb");
    }

    fprintf(stderr, "Synthesising text...\n");

    result = ttsEngine->synthesizeText(argv[1], synthBuffer, OUTPUT_BUFFER_SIZE, NULL);

    if (result != TTS_SUCCESS)
    {
        fprintf(stderr, "Failed to synth text\n");
    }

    while(!synthesis_complete)
    {
        usleep(100);
    }

    fprintf(stderr, "Completed.\n");

    if (outputFilename)
    {
        fclose(outfp);
    }

    result = ttsEngine->shutdown();

    if (result != TTS_SUCCESS)
    {
        fprintf(stderr, "Failed to shutdown TTS\n");
    }

    delete [] synthBuffer;

    return 0;
}

