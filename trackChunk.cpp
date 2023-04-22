#pragma once
#include <iostream>
#include "types.cpp"
#include "readLSB.cpp"
#include "eventTypes.cpp"
#include "showMetaEventData.cpp"


MetaEvent handleEvent(uint8_t *arr, uint32_t offset, uint32_t *length)
{
    MetaEvent event;
    uint32_t timeCount;
    uint32_t lenCount;
    uint8_t *data = arr + offset;

    strcpy(event.description, "");

    // timeCount가 몇 개인지 모르기 때문에 여기에 추가 필요
    event.deltaTime = readVQL(data, &timeCount);
    handleMetaEventDescription(&event, data, timeCount);
    if (strlen(event.description) == 0)
    {
        printf("Error: No event registered.\n");
        exit(1);
    }
    // time catg type leng vari
    // VQL  1    1    VQL  leng

    event.type[0] = data[timeCount];
    event.type[1] = data[timeCount + 1];
    event.length = readVQL(data + timeCount + 2, &lenCount);

    // deltatTime + 'FF' + 'type' + length
    handleMetaEventVariable(&event, data + timeCount + 2 + lenCount);
    *length = event.length + timeCount + 2 + lenCount;

#if DEBUG_SHOW_META_EVENT == true
    printf("[MetaEvent]");
    for (size_t i = 0; i < *length; i++)
    {
        printf("%02X ", data[i]);
    }
    printf("\n");

    showMetaEventData(&event);
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
    for (size_t i = 0; i < 4; i++)
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
    // printf("[track]deltaTime   : %d\n", track->deltaTime);
    // printf("[VQL]timeLength    : %d\n", timeLength);

    uint32_t metaEventLength = 0;
    uint32_t tempLength = 0;

    while (metaEventLength < track->length)
    {
// initial 22
#if DEBUG_SHOW_LENGTH == true
        printf("current position: %07X0 %02X %d\n", (14 + 8 + metaEventLength) / 16, (14 + 8 + metaEventLength) % 16, 14 + 8 + metaEventLength);
#endif
        handleEvent(data, 8 + metaEventLength, &tempLength);
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