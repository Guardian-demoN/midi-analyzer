#pragma once
#include <stdint.h>

uint16_t readLSB16(uint8_t* arr){
    return (arr[0] << 8) + arr[1];
}

uint32_t readLSB24(uint8_t* arr){
    return (arr[0] << 16) + (arr[1] << 8) + arr[2];
}

uint32_t readLSB32(uint8_t* arr){
    uint32_t result = 0;
    for (size_t i = 0; i < 4; i++)
    {
        result <<= 8;
        result += arr[i];
    }
    return result;
}

uint64_t readVQL(uint8_t *arr, uint32_t *count)
{
    uint8_t index = 0;
    uint64_t value = 0;
    while (1)
    {
        value <<= 7;
        value += arr[index] & 0b01111111;
        uint8_t continued = arr[index] & 0b10000000;
        if(continued == 0){
            *count = index + 1;
            return value ;
        }
        index++;
    }
}