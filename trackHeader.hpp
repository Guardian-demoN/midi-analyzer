#pragma once

#include <iostream>

#include "types.hpp"
#include "readLSB.hpp"


int parseHeader(Header *header, uint8_t *arr)
{
    // Header :=
    //     <chunk type>
    //     <length>
    //     <format>
    //     <ntrks>
    //     <division>
    const uint8_t magicNumber[4] = {'M', 'T', 'h', 'd'};

    for (uint8_t i = 0; i < 4; i++)
    {
        if (arr[i] != magicNumber[i])
            return -1;
    }
    header->length = readLSB32(arr + 4);
    header->format = readLSB16(arr + 8);
    header->ntrks = readLSB16(arr + 10);

    if ((arr[12] & 0b10000000) == 0)
    {
        // ticks per quarter-note
        header->divisionSMPTEMode = 0;
        header->ticksPerQNote = readLSB16(arr + 12);
    }
    else
    {
        header->divisionSMPTEMode = 1;
        header->SMPTE = arr[12] &= 0b01111111;
        header->ticksPerFrame = arr[13];
    }

    printf("[header]length: %d\n", header->length);
    printf("[header]format: %d\n", header->format);
    printf("[header]ntrks : %d\n", header->ntrks);
    printf("[header]division : %d\n", header->divisionSMPTEMode);
    printf("[header]QNote    : %d\n", header->ticksPerQNote);
    
    return 14;
}