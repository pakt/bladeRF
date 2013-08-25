proc compile_bladerf { root } {
    vlib pll
    vcom -work nuand  [file join $root ../../ip/altera/pll/pll.vhd]

    vlib serial_pll
    vcom -work nuand [file join $root ../../ip/altera/serial_pll/serial_pll.vhd]

    vlib nuand
#    vcom -work nuand -2008 [file join $root ./vhdl/fx3.vhd]
    vcom -work nuand -2008 [file join $root ./vhdl/bladerf_debug_p.vhd]
    vcom -work nuand -2008 [file join $root ./vhdl/ramp.vhd]
    vcom -work nuand -2008 [file join $root ../../ip/altera/tx_fifo/tx_fifo.vhd]
    vcom -work nuand -2008 [file join $root ../../ip/altera/rx_fifo/rx_fifo.vhd]
    vcom -work nuand -2008 [file join $root ./vhdl/bladerf.vhd]
    vcom -work nuand -2008 [file join $root ./vhdl/bladerf-hosted.vhd]

    vcom -work nuand -2008 [file join $root ./vhdl/tb/nios_system_model.vhd]
    vcom -work nuand -2008 [file join $root ./vhdl/tb/bladerf_tb.vhd]
}
