# Setup configuration options.
if (${CONFIG_BOOTLOAD})
    setup_option_def(BOOTLOADER_LOADED ON DEFINE "If bootloader is already loaded." CONFIG_FILE "bootload_avr_config")
    setup_option_def(BOOTLOAD_MMC ON DEFINE "If we want to bootload from MMC card." CONFIG_FILE "bootload_avr_config")
    setup_option_def(BOOTLOAD_STK ON DEFINE "If we want to bootload from STK." CONFIG_FILE "bootload_avr_config")
    if (${BOOTLOAD_MMC})
        setup_option_def(BOOTLOAD_MMC_HEX_NONLINEAR OFF DEFINE "If HEX on MMC can be nonlinear." CONFIG_FILE "bootload_avr_config")
        setup_option_def(BOOTLOAD_MMC_BOOTLOAD_MARK_SECTOR_LOCATION 0 UINT32 "Sector from which bootload marker will be read from." CONFIG_FILE "bootload_avr_config")
        setup_option_def(BOOTLOAD_MMC_CS_PIN_NUM 3 INT "MMC SPI slave-select (CS) pin number to be used by MMC bootloader." VALUE_LIST atmega_pinnum_values CONFIG_FILE "bootload_avr_config")
        setup_option_def(BOOTLOAD_MMC_CS_DDR DDRA MACRO "MMC SPI slave-select (CS) DDR to be used by MMC bootloader." VALUE_LIST atmega_ddr_values CONFIG_FILE "bootload_avr_config")
        setup_option_def(BOOTLOAD_MMC_CS_PORT PORTA MACRO "MMC SPI slave-select (CS) PORT to be used by MMC bootloader." VALUE_LIST atmega_port_values CONFIG_FILE "bootload_avr_config")
        setup_option_def(BOOTLOAD_MMC_CLK_PIN_NUM 2 INT "MMC SPI clock (CLK) pin number to be used by MMC bootloader." VALUE_LIST atmega_pinnum_values CONFIG_FILE "bootload_avr_config")
        setup_option_def(BOOTLOAD_MMC_CLK_DDR DDRA MACRO "MMC SPI clock (CLK) DDR to be used by MMC bootloader." VALUE_LIST atmega_ddr_values CONFIG_FILE "bootload_avr_config")
        setup_option_def(BOOTLOAD_MMC_CLK_PORT PORTA MACRO "MMC SPI clock (CLK) PORT to be used by MMC bootloader." VALUE_LIST atmega_port_values CONFIG_FILE "bootload_avr_config")
        setup_option_def(BOOTLOAD_MMC_DI_PIN_NUM 5 INT "MMC SPI data-in (DI) pin number to be used by MMC bootloader." VALUE_LIST atmega_pinnum_values CONFIG_FILE "bootload_avr_config")
        setup_option_def(BOOTLOAD_MMC_DI_DDR DDRA MACRO "MMC SPI data-in (DI) DDR to be used by MMC bootloader." VALUE_LIST atmega_ddr_values CONFIG_FILE "bootload_avr_config")
        setup_option_def(BOOTLOAD_MMC_DI_PORT PORTA MACRO "MMC SPI data-in (DI) PORT to be used by MMC bootloader." VALUE_LIST atmega_port_values CONFIG_FILE "bootload_avr_config")
        setup_option_def(BOOTLOAD_MMC_DO_PIN_NUM 0 INT "MMC SPI data-out (DO) pin number to be used by MMC bootloader." VALUE_LIST atmega_pinnum_values CONFIG_FILE "bootload_avr_config")
        setup_option_def(BOOTLOAD_MMC_DO_DDR DDRA MACRO "MMC SPI data-out (DO) DDR to be used by MMC bootloader." VALUE_LIST atmega_ddr_values CONFIG_FILE "bootload_avr_config")
        setup_option_def(BOOTLOAD_MMC_DO_PIN PINA MACRO "MMC SPI data-out (DO) PIN to be used by MMC bootloader." VALUE_LIST atmega_pin_values CONFIG_FILE "bootload_avr_config")
    endif ()

    # Setup AVR dude options.
    setup_option_def(${TGT_PLATFORM}_DUDE_MCU "m1284p" STRING "AVR dude MCU." CONFIG_FILE "unused")
    setup_option_def(${TGT_PLATFORM}_DUDE_DRIVER "arduino" STRING "AVR dude driver." CONFIG_FILE "unused")
    setup_option_def(${TGT_PLATFORM}_DUDE_SER "COM1" STRING "AVR dude serial port." CONFIG_FILE "unused")
    setup_option_def(${TGT_PLATFORM}_DUDE_BOUD "115200" STRING "AVR dude boudrate." CONFIG_FILE "unused")
endif ()
