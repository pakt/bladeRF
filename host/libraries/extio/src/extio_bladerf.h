/* 
 * bladeRF ExtIO interface
 *
 * See the following document for a specification of the
 * the ExtIO interface:
 *      http://www.winrad.org/bin/Winrad_Extio.pdf
 *
 */
#ifndef EXTIO_BLADERF_H
#define EXTIO_BLADERF_H

#ifdef __cplusplus
extern "C" {
#endif

#define EXTIO_API __declspec(dllexport) __stdcall
#include <stdint.h>

/* Required functions */
bool     EXTIO_API InitHW(char *name, char *model, int& type);
bool     EXTIO_API OpenHW(void);
int      EXTIO_API StartHW(long freq);
int64_t  EXTIO_API StartHW64(int64_t freq);
void     EXTIO_API StopHW(void);
void     EXTIO_API CloseHW(void);
int      EXTIO_API SetHWLO(long LOfreq);
int64_t  EXTIO_API SetHWLO64(int64_t LOfreq);
int      EXTIO_API GetStatus(void);
void     EXTIO_API SetCallback(void (*callback)(int, int, float, short *));

/* Optional Functions */
long     EXTIO_API GetHWLO(void);
int64_t  EXTIO_API GetHWLO64(void);
long     EXTIO_API GetHWSR(void);
long     EXTIO_API GetTune(void);
void     EXTIO_API GetFilters(int& loCut, int* hiCut, int& pitch);
char     EXTIO_API GetMode(void);
void     EXTIO_API ModeChanged(char mode);
void     EXTIO_API ShowGUI(void);
void     EXTIO_API HideGUI(void);
void     EXTIO_API IFLimitsChanged(long low, long high);
void     EXTIO_API TuneChanged(long freq);
void     EXTIO_API RawDataReady(long samprate, int *Ldata, int *Rdata, int numsamples);

void     EXTIO_API VersionInfo(const char * progname, int ver_major, int ver_minor);

int      EXTIO_API GetAttenuators(int idx, float * attenuation);
int      EXTIO_API GetActualAttIdx(void);
int      EXTIO_API SetAttenuator(int idx);

int      EXTIO_API ExtIoGetSrates(int idx, double * samplerate);
int      EXTIO_API ExtIoGetActualSrateIdx(void);
int      EXTIO_API ExtIoSetSrate(int idx);
long     EXTIO_API ExtIoGetBandwidth(int srate_idx);

int      EXTIO_API ExtIoGetSetting( int idx, char * description, char * value );
void     EXTIO_API ExtIoSetSetting( int idx, const char * value );

#ifdef __cplusplus
}
#endif

#endif
