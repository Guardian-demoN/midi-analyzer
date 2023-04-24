#pragma once

#include <stdio.h>

#include "types.cpp"
#include "readLSB.hpp"
#include "metaEvent.hpp"
#include "midiEvent.hpp"
#include "metaEvent_show.hpp"

MetaEvent handleMetaEvent(uint8_t *arr, uint32_t offset, uint32_t *length);

void handleEvent(uint8_t *arr, uint32_t offset, uint32_t *length)
{
    uint8_t *data = arr + offset;

    uint32_t timeCount;
    readVQL(data, &timeCount);
    if (data[timeCount] == 0xFF)
    {
        handleMetaEvent(arr, offset, length);
    }
    else
    {
        handleMidiEvent(arr, offset, length);
    }
}



int parseTrack(Track *track, uint8_t *arr, uint32_t offset)
{
    uint8_t *data = (arr + offset);

    // Track :=
    //     <chunk type>
    //     <length>
    //     <MTrk event>+
    const uint8_t magicNumber[4] = {'M', 'T', 'r', 'k'};
    for (uint8_t i = 0; i < 4; i++)
    {
        if (data[i] != magicNumber[i])
            return -1;
    }
    // uint32_t timeLength = 0;

    track->length = readLSB32(data + 4);
    // +8부터

    // track->deltaTime = readVQL(data + 8, &timeLength);

    uint32_t metaEventLength = 0;
    uint32_t tempLength = 0;

    while (metaEventLength < track->length)
    {
        // initial 22
        // handleEvent(data, 8 + metaEventLength, &tempLength);
        handleEvent(arr, offset + 8 + metaEventLength, &tempLength);
        metaEventLength += tempLength;
    }
// 'MTrk' + length(4) + track = 8 + track
    return track->length + 8 + offset;
}