#ifndef EXTIO_BLADERF_H
#define EXTIO_BLADERF_H

#define EXTIO_API __declspec(dllexport) __stdcall

#include "stdint.h"

/* Function prototypes taken from:
 *
 *  http://www.winrad.org/bin/Winrad_Extio.pdf
 *
 */

/* Required functions */
extern "C" bool     EXTIO_API InitHW(char *name, char *model, int& type);
extern "C" bool     EXTIO_API OpenHW(void);
extern "C" int      EXTIO_API StartHW(long freq);
extern "C" int64_t  EXTIO_API StartHW64(int64_t freq);
extern "C" void     EXTIO_API StopHW(void);
extern "C" void     EXTIO_API CloseHW(void);
extern "C" int      EXTIO_API SetHWLO(long LOfreq);
extern "C" int64_t  EXTIO_API SetHWLO64(int64_t LOfreq);
extern "C" int      EXTIO_API GetStatus(void);
extern "C" void     EXTIO_API SetCallback(void (*callback)(int, int, float, short *));

/* Optional Functions */
extern "C" long     EXTIO_API GetHWLO(void);
extern "C" int64_t  EXTIO_API GetHWLO64(void);
extern "C" long     EXTIO_API GetHWSR(void);
extern "C" long     EXTIO_API GetTune(void);
extern "C" void     EXTIO_API GetFilters(int& loCut, int* hiCut, int& pitch);
extern "C" char     EXTIO_API GetMode(void);
extern "C" void     EXTIO_API ModeChanged(char mode);
extern "C" void     EXTIO_API ShowGUI(void);
extern "C" void     EXTIO_API HideGUI(void);
extern "C" void     EXTIO_API IFLimitsChanged(long low, long high);
extern "C" void     EXTIO_API TuneChanged(long freq);
extern "C" void     EXTIO_API RawDataReady(long samprate, int *Ldata, int *Rdata, int numsamples);

extern "C" void     EXTIO_API VersionInfo(const char * progname, int ver_major, int ver_minor);

extern "C" int      EXTIO_API GetAttenuators(int idx, float * attenuation);
extern "C" int      EXTIO_API GetActualAttIdx(void);
extern "C" int      EXTIO_API SetAttenuator(int idx);

extern "C" int      EXTIO_API ExtIoGetSrates(int idx, double * samplerate);
extern "C" int      EXTIO_API ExtIoGetActualSrateIdx(void);
extern "C" int      EXTIO_API ExtIoSetSrate(int idx);
extern "C" long     EXTIO_API ExtIoGetBandwidth(int srate_idx);

extern "C" int      EXTIO_API ExtIoGetSetting( int idx, char * description, char * value );
extern "C" void     EXTIO_API ExtIoSetSetting( int idx, const char * value );

#endif