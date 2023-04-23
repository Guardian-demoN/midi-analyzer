#pragma once

#include <stdio.h>

#include "types.cpp"
#include "readLSB.hpp"
#include "metaEvent.hpp"
#include "midiEvent.hpp"
#include "metaEvent_show.hpp"

MetaEvent handleMetaEvent(uint8_t *arr, uint32_t offset, uint32_t *length);

MetaEvent handleEvent(uint8_t *arr, uint32_t offset, uint32_t *length)
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

MetaEvent handleMetaEvent(uint8_t *arr, uint32_t offset, uint32_t *length)
{
    MetaEvent event;
    uint8_t *data = arr + offset;

    uint32_t timeCount; // deltaTime VQL
    uint32_t lenCount; // length VQL

    strcpy(event.description, "");

    // timeCount가 몇 개인지 모르기 때문에 여기에 추가 필요
    event.deltaTime = readVQL(data, &timeCount);

    // time catg type leng vari
    // VQL  1    1    VQL  leng
    // FF는 timeCount라서 패스(MetaEvent라서 고정)
    event.type = data[timeCount + 1];
    event.length = readVQL(data + timeCount + 2, &lenCount);

    // deltatTime + 'FF' + 'type' + length
    handleMetaEventVariable(&event, data, timeCount + lenCount + 2);
    *length = event.length + timeCount + 2 + lenCount;

#if DEBUG_SHOW_META_EVENT == true
    showMetaEventData(&event, arr, offset, *length);
#endif

    return event;
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

#if DEBUG_SHOW_LENGTH == true
    printf("[track]length      : %d\n", track->length);
#endif

    uint32_t metaEventLength = 0;
    uint32_t tempLength = 0;

    while (metaEventLength < track->length)
    {
        // initial 22
        // handleEvent(data, 8 + metaEventLength, &tempLength);
        handleEvent(arr, offset + 8 + metaEventLength, &tempLength);
        metaEventLength += tempLength;
#if DEBUG_SHOW_LENGTH == true
        printf("[tempLength]%d\n", metaEventLength);
#endif
    }
// 'MTrk' + length(4) + track = 8 + track
#if DEBUG_SHOW_LENGTH == true
    printf("[length calc]track.length: %d\n", track->length);
    printf("[length calc]pffset      : %d\n", offset);
#endif
    return track->length + 8 + offset;
}