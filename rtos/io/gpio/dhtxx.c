/*
 * dhtxx.c
 *
 * Copyright (c) 2020 Usama Masood <mirzaon@gmail.com> All rights reserved.
 *
 * This file is part of a non-commercial software. For more details please
 * refer to the license agreement that comes with this software.
 *
 * If you have not received a license file please contact:
 *  Usama Masood <mirzaon@gmail.com>
 *
 */
#include <dhtxx.h>

#ifdef CONFIG_DHT

/*
 * dhtxx_init
 * This function will initialize DHTXX devices.
 */
void dhtxx_init()
{
#ifdef DHT_TGT_INIT
    DHT_TGT_INIT();
#endif
} /* dhtxx_init */

/*
 * dhtxx_register
 * @dht: DHTXX device needed to be register.
 * @return: Always return success.
 * This function will register a DHTXX device and perform necessary initializations.
 */
int32_t dhtxx_register(DHT_XX *dht)
{
    /* Initialize data pin. */
    dht->pin_init(dht);

    /* Always return success. */
    return (SUCCESS);

} /* dhtxx_register */

/*
 * dhtxx_read
 * @dht: DHTXX device needed to be read.
 * @rh: Value of relative humidity will be returned here.
 * @temperature: Raw value of temperature will be returned here.
 * @disable_interrupts: If set to true interrupts will be disabled while reading
 *  from sensor.
 * @return: Return success if readings were successfully read.
 * This function will read and return sensor values from the given DHT sensor.
 */
int32_t dhtxx_read(DHT_XX *dht, uint16_t *rh, uint16_t *temperature, uint8_t disable_interrupts)
{
    int32_t status = SUCCESS;
    uint64_t reading = 0;
    uint16_t delta;
    uint8_t i;
    INT_LVL interrupt_level;

    /* Start by pulling down the data line. */
    dht->set_pin_mode(dht, TRUE);
    dht->set_pin_state(dht, FALSE);

    /* Sleep for some time. */
    sleep_us(DHTXX_INIT_DELAY);

    /* Set the output pin. */
    dht->set_pin_state(dht, TRUE);

    /* Set pin mode as input. */
    dht->set_pin_mode(dht, FALSE);

    /* Wait for at-least 60us for the sensor to pull down the line. */
    DHTXX_TIMED((dht->get_pin_state(dht) == TRUE), DHTXX_INIT_SIGNAL_DELAY, delta);

    if (status == SUCCESS)
    {
        /* Wait for at-least 100us for the sensor to pull up the line. */
        DHTXX_TIMED((dht->get_pin_state(dht) == FALSE), DHTXX_SIGNAL_DELAY, delta);
    }

    if (status == SUCCESS)
    {
        /* Wait for at-least 100us for the sensor to pull down the line. */
        DHTXX_TIMED((dht->get_pin_state(dht) == TRUE), DHTXX_SIGNAL_DELAY, delta);
    }

    /* If interrupts are needed to be disabled. */
    if (disable_interrupts)
    {
        /* Disable interrupts. */
        interrupt_level = GET_INTERRUPT_LEVEL();
        DISABLE_INTERRUPTS();
    }

    if (status == SUCCESS)
    {
        /* Read the data. */
        for (i = 0; ((status == SUCCESS) && (i < 40)); i ++)
        {
            /* Wait for at-least 60us for the sensor to pull up the line. */
            DHTXX_TIMED((dht->get_pin_state(dht) == FALSE), DHTXX_DATA_LOW_DELAY, delta);

            if (status == SUCCESS)
            {
                /* Wait for at-least 100us for the sensor to pull down the line. */
                DHTXX_TIMED((dht->get_pin_state(dht) == TRUE), DHTXX_DATA_HIGH_DELAY, delta);
            }

            if (status == SUCCESS)
            {
                /* If this was a 1. */
                if (delta > DHTXX_DATA_ZERO_DELAY)
                {
                    /* Put it in the value being read. */
                    reading |= ((uint64_t)1 << (39 - i));
                }
            }
        }
    }

    /* At the end configure the pin as output and pull it high. */
    dht->set_pin_state(dht, TRUE);
    dht->set_pin_mode(dht, TRUE);
    dht->set_pin_state(dht, TRUE);

    if (status == SUCCESS)
    {
        /* If we do not have anticipated checksum. */
        if ((uint8_t)(((reading >> 32) & 0xFF) + ((reading >> 24) & 0xFF) + ((reading >> 16) & 0xFF) + ((reading >> 8) & 0xFF)) != (reading & 0xFF))
        {
            /* Return error. */
            status = DHTXX_CSUM_ERROR;
        }
        else
        {
            /* Return the readings read. */
            *rh = (uint16_t)((reading >> 24) & 0xFFFFF);
            *temperature = (uint16_t)((reading >> 8) & 0xFFFFF);
        }
    }

    /* If interrupts were disabled. */
    if (disable_interrupts)
    {
        /* Restore interrupts. */
        SET_INTERRUPT_LEVEL(interrupt_level);
    }

    /* Return status to the caller. */
    return (status);

} /* dhtxx_read */

#endif /* CONFIG_DHT */