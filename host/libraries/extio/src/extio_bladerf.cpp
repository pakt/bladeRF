#include <string.h>
#include "extio_bladerf.h"
#include "libbladeRF.h"

/* Some constants */
#define HWNAME              "nuand"
#define HWMODEL             "bladeRF"
#define HWTYPE              3
#define SETTINGS_IDENTIFIER "nuand-bladeRF-1.0"
#define LO_MIN              300000000LL
#define LO_MAX              3800000000LL
#define LO_PRECISION        1

#define SAMPLES_PER_BUFFER  4096

/* Some global state */
struct bladerf *dev = NULL;

/* Callback that is registered for data */
typedef void (*extio_callback)(int count, int status, float iqoffs, short *iqdata);
extio_callback *cb;

/* Configured from GUI */
char *fpga_filename = "hostedx115.rbf";
char *device_identifier = NULL;

/* Required functions */
bool InitHW(char *name, char *model, int& type) {
    int status;

    /* Return back name, model and type */
    strcpy(name, HWNAME);
    strcpy(model, HWMODEL);
    type = HWTYPE;

    /* Make sure we have an open device handle */
    if (dev == NULL) {
        return false;
    }

    /* Check to see if the FPGA has been loaded */
    if (bladerf_is_fpga_configured(dev) == 0) {
        /* Load the FPGA */
		status = bladerf_load_fpga(dev, fpga_filename);
        if (status < 0) {
            return false;
        }
    }

    /* Everything is cool */
    return true;
}

bool OpenHW(void) {
    int status;

    /* Open up the device as configured */
    status = bladerf_open(&dev, device_identifier);
    if (status < 0) {
        return false;
    }
    return true;
}

int StartHW(long freq) {
    int status;

    if (dev == NULL) {
        return 0;
    }

    /* TODO: Start reading thread */

    /* Return number of samples per buffer */
    return SAMPLES_PER_BUFFER ;
}

int64_t StartHW64(int64_t freq) {
    int status;

    if (dev == NULL) {
        return 0;
    }
    /* TODO: Start reading thread */

    /* Return number of samples per buffer */
    return 0;
}

void StopHW(void) {
    /* TODO: Stop reading thread */
    return;
}

void CloseHW(void) {
    int status;

    /* Check to see if we have a device open */
    if (dev) {
        /* ... and close it */
        bladerf_close(dev);
        dev = NULL;
    }

    if (status) {
        /* TODO: Say something? */
    }
    return;
}

int SetHWLO(long LOfreq) {
    int status;

    /* Check limits */
    if (LOfreq < LO_MIN) {
        return -LO_MIN;
    } else if (LOfreq > LO_MAX) {
        return LO_MAX;
    }

    /* Check device */
    if (dev == NULL) {
        return -LO_MIN;
    }

    /* Set frequency */
    status = bladerf_set_frequency(dev, BLADERF_MODULE_RX, LOfreq);

    if (status) {
        /* Something bad happened, but not sure what to do? */
    }

    /* Pretend like everything is fine */
    return 0;
}

int64_t SetHWLO64(int64_t LOfreq) {
    int status;

    /* Check limits */
    if( LOfreq < LO_MIN) {
        return -LO_MIN;
    } else if (LOfreq > LO_MAX) {
        return LO_MAX;
    }

    /* Check device */
    if (dev == NULL) {
        return -LO_MIN;
    }

    /* Set the frequency */
    status = bladerf_set_frequency(dev, BLADERF_MODULE_TX, LOfreq);

    if (status) {
        /* Something bad happened, but not sure what to do? */
    }

    /* Pretend like everything is fine */
    return 0;
}

int GetStatus(void) {
    /* Non-functional dummy */
    return 0;
}

void SetCallback(void (* callback)(int, int, float, short *)) {
    /* Globally register callback */
    cb = (extio_callback *)callback;
    return;
}

/* Optional Functions */

/* TODO: Fill these in */
/*
long GetHWLO(void) {
    int status;

    if (dev == NULL) {

    }
    return 0;
}

int64_t GetHWLO64(void) {
    return 0;
}

long GetHWSR(void) {
    return 0;
}

long GetTune(void) {
    return 0;
}

void GetFilters(int& loCut, int* hiCut, int& pitch) {
    return 0;
}

char GetMode(void) {
    return '\0';
}

void ModeChanged(char mode) {
    return;
}

void ShowGUI(void) {
    return;
}

void HideGUI(void) {
    return;
}

void IFLimitsChanged(long low, long high) {
    return;
}

void TuneChanged(long freq) {
    return;
}

void RawDataReady(long samprate, int *Ldata, int *Rdata, int numsamples) {
    return;
}

void  VersionInfo(const char * progname, int ver_major, int ver_minor) {
    return;
}

int GetAttenuators(int idx, float * attenuation) {
    return 0;
}

int GetActualAttIdx(void) {
    return 0;
}

int SetAttenuator(int idx) {
    return 0;
}

int ExtIoGetSrates(int idx, double * samplerate) {
    return 0;
}

int ExtIoGetActualSrateIdx(void) {
    return 0;
}

int ExtIoSetSrate(int idx) {
    return 0;
}

long ExtIoGetBandwidth(int srate_idx) {
    return 0;
}

int ExtIoGetSetting( int idx, char * description, char * value ) {
    return 0;
}

void ExtIoSetSetting( int idx, const char * value ) {
    return 0;
}
*/
