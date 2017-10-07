# This function will convert the given AVR register to its address.
function (value_map value value_return)
    if (${value} STREQUAL "PINA")
        set(${value_return} 0x00 CACHE INTERNAL "" FORCE)
    elseif (${value} STREQUAL "DDRA")
        set(${value_return} 0x01 CACHE INTERNAL "" FORCE)
    elseif (${value} STREQUAL "PORTA")
        set(${value_return} 0x02 CACHE INTERNAL "" FORCE)
    elseif (${value} STREQUAL "PINB")
        set(${value_return} 0x03 CACHE INTERNAL "" FORCE)
    elseif (${value} STREQUAL "DDRB")
        set(${value_return} 0x04 CACHE INTERNAL "" FORCE)
    elseif (${value} STREQUAL "PORTB")
        set(${value_return} 0x05 CACHE INTERNAL "" FORCE)
    elseif (${value} STREQUAL "PINC")
        set(${value_return} 0x06 CACHE INTERNAL "" FORCE)
    elseif (${value} STREQUAL "DDRC")
        set(${value_return} 0x07 CACHE INTERNAL "" FORCE)
    elseif (${value} STREQUAL "PORTC")
        set(${value_return} 0x08 CACHE INTERNAL "" FORCE)
    elseif (${value} STREQUAL "PIND")
        set(${value_return} 0x09 CACHE INTERNAL "" FORCE)
    elseif (${value} STREQUAL "DDRD")
        set(${value_return} 0x0A CACHE INTERNAL "" FORCE)
    elseif (${value} STREQUAL "PORTD")
        set(${value_return} 0x0B CACHE INTERNAL "" FORCE)
    else ()
        message(FATAL_ERROR "Unknown AVR register ${value}.")
    endif ()
endfunction ()