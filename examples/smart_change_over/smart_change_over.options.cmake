# Include helpers.
include(${RTOS_ROOT}/cmake/modules/helper.cmake)

# Initialize RTOS configurations.
setup_option(CONFIG_FS ON)
setup_option(CONFIG_NET ON)
setup_option(CONFIG_I2C ON)
setup_option(CONFIG_PCF8574 ON)
setup_option(CONFIG_SPI ON)
setup_option(CONFIG_MMC OFF)
setup_option(CONFIG_ADC ON)
setup_option(CONFIG_LCD_AN ON)
setup_option(CONFIG_LCD_PCF8574 ON)
setup_option(CONFIG_ETHERNET ON)
setup_option(CONFIG_WEIRD_VIEW ON)

# Setup IDLE task options.
setup_option(IDLE_WORK_MAX 1)
setup_option(IDLE_TASK_STACK_SIZE 196)

# Update the number of ticks per second to 10.
setup_option(SOFT_TICKS_PER_SEC 10)

# Setup task options.
setup_option(CONFIG_TASK_STATS ON)
setup_option(CONFIG_TASK_USAGE ON)

# Setup enc28j60 configurations.
setup_option(ENC28J60_MAX_BUFFER_SIZE 64)
setup_option(ENC28J60_NUM_BUFFERS 7)
setup_option(ENC28J60_NUM_BUFFER_LISTS 4)
setup_option(ENC28J60_NUM_THR_BUFFER 0)
setup_option(ENC28J60_NUM_THR_LIST 0)
setup_option(ENC28J60_NUM_ARP 1)
setup_option(ENC28J60_NUM_IPV4_FRAGS 0)

# Setup networking stack configurations.
setup_option(NET_COND_STACK_SIZE 512)
setup_option(IPV4_ENABLE_FRAG OFF)
setup_option(NET_NUM_ROUTES 2)
setup_option(NET_TCP OFF)

# Setup AVR configurations.
setup_option(ADC_ATMEGA644P_PRESCALE ADC_ATMEGA644P_DIV_64)

# Setup target configuration.
setup_option(PLATFORM atmega644)
setup_option(F_CPU 20000000UL)
