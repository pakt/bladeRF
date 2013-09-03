#include <stdio.h>
#include <string.h>
#include <libbladeRF.h>

#include "common.h"
#include "cmd.h"

int cmd_correct(struct cli_state *state, int argc, char **argv)
{

    /* Valid commands:
     *  correct [tx|rx] [dc|gain|phase] <values>
     */
    int rv = CMD_RET_OK;
    if(!cli_device_is_opened(state)) {
        return CMD_RET_NODEV;
    }

    if (argc == 1) {
        /* Print out correction parameters */
        int dc_i, dc_q;
        bladerf_get_dc_correction(state->dev, BLADERF_MODULE_RX, &dc_i, &dc_q);
        bladerf_get_dc_correction(state->dev, BLADERF_MODULE_TX, &dc_i, &dc_q);
    } else if (argc > 3) {
        /* Get the module we're working with */
        bladerf_module module;
        if (!strcasecmp(argv[1], "rx")) {

        } else if (!strcasecmp(argv[1], "tx")) {

        } else {
            return CMD_RET_INVPARAM;
        }

        /* Check for the type of correction */
        if (argc == 4) {
            /* Gain and phase corrections */
            if (!strcasecmp(argv[2], "gain")) {

            } else if (!strcasecmp(argv[2], "phase")) {

            } else {
                return CMD_RET_INVPARAM;
            }
        } else if( argc == 5) {
            /* DC corrections */
            if (!strcasecmp(argv[2], "dc")) {
                /* Get I and Q values */
            } else {
                return CMD_RET_INVPARAM;
            }
        } else {
            return CMD_RET_NARGS;
        }
    } else {
        return CMD_RET_NARGS;
    }

    return rv ;
}
