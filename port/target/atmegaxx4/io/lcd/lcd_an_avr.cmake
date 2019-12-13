# Setup configuration options.
setup_option_def(LCD_AN_AVR_ROWS 4 INT "Number of rows in the alphanumeric LCD." CONFIG_FILE "lcd_an_avr_config")
setup_option_def(LCD_AN_AVR_COLS 20 INT "Number of columns in the alphanumeric LCD." CONFIG_FILE "lcd_an_avr_config")

if (${CONFIG_LCD_PCF8574})
    setup_option_def(LCD_AN_AVR_I2C_ADDRESS 0x3F INT "I2C address for LCD GPIO controller." CONFIG_FILE "lcd_an_avr_config")
    setup_option_def(LCD_AN_AVR_PIN_RW 1 INT "Read write (RW) pin for alphanumeric LCD over I2C." VALUE_LIST atmega_pinnum_values CONFIG_FILE "lcd_an_avr_config")
    setup_option_def(LCD_AN_AVR_PIN_RS 0 INT "Register select (RS) pin for alphanumeric LCD over I2C." VALUE_LIST atmega_pinnum_values CONFIG_FILE "lcd_an_avr_config")
    setup_option_def(LCD_AN_AVR_PIN_EN 2 INT "Enable (EN) pin for alphanumeric LCD over I2C." VALUE_LIST atmega_pinnum_values CONFIG_FILE "lcd_an_avr_config")
    setup_option_def(LCD_AN_AVR_PIN_D4 4 INT "Data 4 (D4) pin for alphanumeric LCD over I2C." VALUE_LIST atmega_pinnum_values CONFIG_FILE "lcd_an_avr_config")
    setup_option_def(LCD_AN_AVR_PIN_D5 5 INT "Data 5 (D5) pin for alphanumeric LCD over I2C." VALUE_LIST atmega_pinnum_values CONFIG_FILE "lcd_an_avr_config")
    setup_option_def(LCD_AN_AVR_PIN_D6 6 INT "Data 6 (D6)s pin for alphanumeric LCD over I2C." VALUE_LIST atmega_pinnum_values CONFIG_FILE "lcd_an_avr_config")
    setup_option_def(LCD_AN_AVR_PIN_D7 7 INT "Data 7 (D7) pin for alphanumeric LCD over I2C." VALUE_LIST atmega_pinnum_values CONFIG_FILE "lcd_an_avr_config")
    setup_option_def(LCD_AN_AVR_PIN_BL 3 INT "Back light pin for alphanumeric LCD over I2C." VALUE_LIST atmega_pinnum_values CONFIG_FILE "lcd_an_avr_config")

    setup_option_def(LCD_AN_AVR_PIN_SCL PINB INT "SCL PIN register for I2C of LCD GPIO controller." VALUE_FUN atmega_regmap VALUE_LIST atmega_pin_values CONFIG_FILE "lcd_an_avr_config")
    setup_option_def(LCD_AN_AVR_PIN_SDA PINB INT "SDA PIN register for I2C of LCD GPIO controller." VALUE_FUN atmega_regmap VALUE_LIST atmega_pin_values CONFIG_FILE "lcd_an_avr_config")
    setup_option_def(LCD_AN_AVR_DDR_SCL DDRB INT "SCL DDR register for I2C of LCD GPIO controller." VALUE_FUN atmega_regmap VALUE_LIST atmega_ddr_values CONFIG_FILE "lcd_an_avr_config")
    setup_option_def(LCD_AN_AVR_DDR_SDA DDRB INT "SDA DDR register for I2C of LCD GPIO controller." VALUE_FUN atmega_regmap VALUE_LIST atmega_ddr_values CONFIG_FILE "lcd_an_avr_config")
    setup_option_def(LCD_AN_AVR_PORT_SCL PORTB INT "SCL PORT register for I2C of LCD GPIO controller." VALUE_FUN atmega_regmap VALUE_LIST atmega_port_values CONFIG_FILE "lcd_an_avr_config")
    setup_option_def(LCD_AN_AVR_PORT_SDA PORTB INT "SDA PORT register for I2C of LCD GPIO controller." VALUE_FUN atmega_regmap VALUE_LIST atmega_port_values CONFIG_FILE "lcd_an_avr_config")
    setup_option_def(LCD_AN_AVR_PIN_NUM_SCL 0 INT "SCL pin number for I2C of LCD GPIO controller." VALUE_LIST atmega_pinnum_values CONFIG_FILE "lcd_an_avr_config")
    setup_option_def(LCD_AN_AVR_PIN_NUM_SDA 1 INT "SDA pin number for I2C of LCD GPIO controller." VALUE_LIST atmega_pinnum_values CONFIG_FILE "lcd_an_avr_config")
else ()
    setup_option_def(LCD_AN_AVR_RS 5 INT "Register select (RS) pin for alphanumeric LCD." VALUE_LIST atmega_pinnum_values CONFIG_FILE "lcd_an_avr_config")
    setup_option_def(LCD_AN_AVR_RS_PORT PORTD MACRO "Register select (RS) PORT register for alphanumeric LCD." VALUE_LIST atmega_port_values CONFIG_FILE "lcd_an_avr_config")
    setup_option_def(LCD_AN_AVR_RS_DDR DDRD MACRO "Register select (RS) DDR register for alphanumeric LCD." VALUE_LIST atmega_ddr_values CONFIG_FILE "lcd_an_avr_config")
    setup_option_def(LCD_AN_AVR_RS_PIN PIND MACRO "Register select (RS) PIN register for alphanumeric LCD." VALUE_LIST atmega_pin_values CONFIG_FILE "lcd_an_avr_config")

    setup_option_def(LCD_AN_AVR_RW 6 INT "Read write (RW) pin for alphanumeric LCD." VALUE_LIST atmega_pinnum_values CONFIG_FILE "lcd_an_avr_config")
    setup_option_def(LCD_AN_AVR_RW_PORT PORTD MACRO "Read write (RW) PORT register for alphanumeric LCD." VALUE_LIST atmega_port_values CONFIG_FILE "lcd_an_avr_config")
    setup_option_def(LCD_AN_AVR_RW_DDR DDRD MACRO "Read write (RW) DDR register for alphanumeric LCD." VALUE_LIST atmega_ddr_values CONFIG_FILE "lcd_an_avr_config")
    setup_option_def(LCD_AN_AVR_RW_PIN PIND MACRO "Read write (RW) PIN register for alphanumeric LCD." VALUE_LIST atmega_pin_values CONFIG_FILE "lcd_an_avr_config")

    setup_option_def(LCD_AN_AVR_EN 6 INT "Enable (EN) pin for alphanumeric LCD." VALUE_LIST atmega_pinnum_values CONFIG_FILE "lcd_an_avr_config")
    setup_option_def(LCD_AN_AVR_EN_PORT PORTC MACRO "Enable (EN) PORT register for alphanumeric LCD." VALUE_LIST atmega_port_values CONFIG_FILE "lcd_an_avr_config")
    setup_option_def(LCD_AN_AVR_EN_DDR DDRC MACRO "Enable (EN) DDR register for alphanumeric LCD." VALUE_LIST atmega_ddr_values CONFIG_FILE "lcd_an_avr_config")
    setup_option_def(LCD_AN_AVR_EN_PIN PINC MACRO "Enable (EN) PIN register for alphanumeric LCD." VALUE_LIST atmega_pin_values CONFIG_FILE "lcd_an_avr_config")

    setup_option_def(LCD_AN_AVR_D4 2 INT "Data 4 (D4) pin for alphanumeric LCD." VALUE_LIST atmega_pinnum_values CONFIG_FILE "lcd_an_avr_config")
    setup_option_def(LCD_AN_AVR_D4_PORT PORTC MACRO "Data 4 (D4) PORT register for alphanumeric LCD." VALUE_LIST atmega_port_values CONFIG_FILE "lcd_an_avr_config")
    setup_option_def(LCD_AN_AVR_D4_DDR DDRC MACRO "Data 4 (D4) DDR register for alphanumeric LCD." VALUE_LIST atmega_ddr_values CONFIG_FILE "lcd_an_avr_config")
    setup_option_def(LCD_AN_AVR_D4_PIN PINC MACRO "Data 4 (D4) PIN register for alphanumeric LCD." VALUE_LIST atmega_pin_values CONFIG_FILE "lcd_an_avr_config")

    setup_option_def(LCD_AN_AVR_D5 1 INT "Data 5 (D5) pin for alphanumeric LCD." VALUE_LIST atmega_pinnum_values CONFIG_FILE "lcd_an_avr_config")
    setup_option_def(LCD_AN_AVR_D5_PORT PORTC MACRO "Data 5 (D5) PORT register for alphanumeric LCD." VALUE_LIST atmega_port_values CONFIG_FILE "lcd_an_avr_config")
    setup_option_def(LCD_AN_AVR_D5_DDR DDRC MACRO "Data 5 (D5) DDR register for alphanumeric LCD." VALUE_LIST atmega_ddr_values CONFIG_FILE "lcd_an_avr_config")
    setup_option_def(LCD_AN_AVR_D5_PIN PINC MACRO "Data 5 (D5) PIN register for alphanumeric LCD." VALUE_LIST atmega_pin_values CONFIG_FILE "lcd_an_avr_config")

    setup_option_def(LCD_AN_AVR_D6 0 INT "Data 6 (D6) pin for alphanumeric LCD." VALUE_LIST atmega_pinnum_values CONFIG_FILE "lcd_an_avr_config")
    setup_option_def(LCD_AN_AVR_D6_PORT PORTC MACRO "Data 6 (D6) PORT register for alphanumeric LCD." VALUE_LIST atmega_port_values CONFIG_FILE "lcd_an_avr_config")
    setup_option_def(LCD_AN_AVR_D6_DDR DDRC MACRO "Data 6 (D6) DDR register for alphanumeric LCD." VALUE_LIST atmega_ddr_values CONFIG_FILE "lcd_an_avr_config")
    setup_option_def(LCD_AN_AVR_D6_PIN PINC MACRO "Data 6 (D6) PIN register for alphanumeric LCD." VALUE_LIST atmega_pin_values CONFIG_FILE "lcd_an_avr_config")

    setup_option_def(LCD_AN_AVR_D7 7 INT "Data 7 (D7) pin for alphanumeric LCD." VALUE_LIST atmega_pinnum_values CONFIG_FILE "lcd_an_avr_config")
    setup_option_def(LCD_AN_AVR_D7_PORT PORTD MACRO "Data 7 (D7) PORT register for alphanumeric LCD." VALUE_LIST atmega_port_values CONFIG_FILE "lcd_an_avr_config")
    setup_option_def(LCD_AN_AVR_D7_DDR DDRD MACRO "Data 7 (D7) DDR register for alphanumeric LCD." VALUE_LIST atmega_ddr_values CONFIG_FILE "lcd_an_avr_config")
    setup_option_def(LCD_AN_AVR_D7_PIN PIND MACRO "Data 7 (D7) PIN register for alphanumeric LCD." VALUE_LIST atmega_pin_values CONFIG_FILE "lcd_an_avr_config")
endif ()