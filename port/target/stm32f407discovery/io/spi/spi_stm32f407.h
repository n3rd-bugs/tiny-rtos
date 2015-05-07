/*
 * spi_stm32f407.h
 *
 * Copyright (c) 2015 Usama Masood <mirzaon@gmail.com>
 *
 * Standard MIT License apply on this source code, with the inclusion of below
 * clause.
 *
 * This source is for educational purpose only, and should never be used for
 * any other purpose. If this source is used for other than educational purpose
 * (in any form) the author will not be liable for any legal charges.
 */
#ifndef _SPI_STM32F407_H_
#define _SPI_STM32F407_H_
#include <os.h>

#ifdef CONFIG_SPI

/* STM32F407 SPI CR1 register definitions. */
#define STM32F407_SPI_CR1_BIDI_SHIFT    (15)
#define STM32F407_SPI_CR1_BIDIOE_SHIFT  (14)
#define STM32F407_SPI_CR1_CRCEN_SHIFT   (13)
#define STM32F407_SPI_CR1_CRCNEXT_SHIFT (12)
#define STM32F407_SPI_CR1_DFF_SHIFT     (11)
#define STM32F407_SPI_CR1_RXONLY_SHIFT  (10)
#define STM32F407_SPI_CR1_SMM_SHIFT     (9)
#define STM32F407_SPI_CR1_SSI_SHIFT     (8)
#define STM32F407_SPI_CR1_LSB_SHIFT     (7)
#define STM32F407_SPI_CR1_SPE_SHIFT     (6)
#define STM32F407_SPI_CR1_BR_SHIFT      (3)
#define STM32F407_SPI_CR1_MSTR_SHIFT    (2)
#define STM32F407_SPI_CR1_CPOL_SHIFT    (1)
#define STM32F407_SPI_CR1_CPHA_SHIFT    (0)

/* STM32F407 SPI CR2 register definitions. */
#define STM32F407_SPI_CR2_TXEIE_SHIFT   (7)
#define STM32F407_SPI_CR1_RXNEIE_SHIFT  (6)
#define STM32F407_SPI_CR1_ERRIE_SHIFT   (5)
#define STM32F407_SPI_CR1_FRF_SHIFT     (4)
#define STM32F407_SPI_CR1_SSOE_SHIFT    (2)
#define STM32F407_SPI_CR1_TXDMAE_SHIFT  (1)
#define STM32F407_SPI_CR1_RXDMAE_SHIFT  (0)

/* STM32F407 SPI I2SCFG register definitions. */
#define STM32F407_SPI_I2SCFG_MOD_SHIFT  (11)

/* SPI device structure. */
typedef struct _stm32f407_spi
{
    /* Physical device ID. */
    uint32_t    device_num;

    /* STM32F407 SPI device register. */
    SPI_TypeDef *reg;

} STM32F407_SPI;

/* Function prototypes. */
void spi_stm32f407_init();

#endif /* CONFIG_SPI */
#endif /* _SPI_STM32F407_H_ */
