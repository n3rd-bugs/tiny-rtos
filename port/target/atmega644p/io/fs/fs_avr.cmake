# Setup configuration options.
setup_option_def(MMC_ATMEGA644P_SPI_SS_BB 3 INT "SPI slave select pin number for MMC card (bit-bang)." VALUE_LIST atmega_pinnum_values)
setup_option_def(MMC_ATMEGA644P_SPI_PIN_SS_BB PINA INT "SPI slave select PIN register for MMC card (bit-bang)." VALUE_FUN atmega_regmap VALUE_LIST atmega_pin_values)
setup_option_def(MMC_ATMEGA644P_SPI_DDR_SS_BB DDRA INT "SPI slave select DDR register for MMC card (bit-bang)." VALUE_FUN atmega_regmap VALUE_LIST atmega_ddr_values)
setup_option_def(MMC_ATMEGA644P_SPI_PORT_SS_BB PORTA INT "SPI slave select PORT register for MMC card (bit-bang)." VALUE_FUN atmega_regmap VALUE_LIST atmega_port_values)
setup_option_def(MMC_ATMEGA644P_SPI_MOSI_BB 5 INT "SPI MOSI pin number for MMC card (bit-bang)." VALUE_LIST atmega_pinnum_values)
setup_option_def(MMC_ATMEGA644P_SPI_PIN_MOSI_BB PINA INT "SPI MOSI PIN register for MMC card (bit-bang)." VALUE_FUN atmega_regmap VALUE_LIST atmega_pin_values)
setup_option_def(MMC_ATMEGA644P_SPI_DDR_MOSI_BB DDRA INT "SPI MOSI DDR register for MMC card (bit-bang)." VALUE_FUN atmega_regmap VALUE_LIST atmega_ddr_values)
setup_option_def(MMC_ATMEGA644P_SPI_PORT_MOSI_BB PORTA INT "SPI MOSI PORT register for MMC card (bit-bang)." VALUE_FUN atmega_regmap VALUE_LIST atmega_port_values)
setup_option_def(MMC_ATMEGA644P_SPI_MISO_BB 0 INT "SPI MISO pin number for MMC card (bit-bang)." VALUE_LIST atmega_pinnum_values)
setup_option_def(MMC_ATMEGA644P_SPI_PIN_MISO_BB PINA INT "SPI MISO PIN register for MMC card (bit-bang)." VALUE_FUN atmega_regmap VALUE_LIST atmega_pin_values)
setup_option_def(MMC_ATMEGA644P_SPI_DDR_MISO_BB DDRA INT "SPI MISO DDR register for MMC card (bit-bang)." VALUE_FUN atmega_regmap VALUE_LIST atmega_ddr_values)
setup_option_def(MMC_ATMEGA644P_SPI_PORT_MISO_BB PORTA INT "SPI MISO PORT register for MMC card (bit-bang)." VALUE_FUN atmega_regmap VALUE_LIST atmega_port_values)
setup_option_def(MMC_ATMEGA644P_SPI_SCLK_BB 2 INT "SPI clock pin number for MMC card (bit-bang)." VALUE_LIST atmega_pinnum_values)
setup_option_def(MMC_ATMEGA644P_SPI_PIN_SCLK_BB PINA INT "SPI clock PIN register for MMC card (bit-bang)." VALUE_FUN atmega_regmap VALUE_LIST atmega_pin_values)
setup_option_def(MMC_ATMEGA644P_SPI_DDR_SCLK_BB DDRA INT "SPI clock DDR register for MMC card (bit-bang)." VALUE_FUN atmega_regmap VALUE_LIST atmega_ddr_values)
setup_option_def(MMC_ATMEGA644P_SPI_PORT_SCLK_BB PORTA INT "SPI clock PORT register for MMC card (bit-bang)." VALUE_FUN atmega_regmap VALUE_LIST atmega_port_values)