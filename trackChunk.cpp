#pragma once
#include <iostream>
#include "readLSB.cpp"
#include "eventTypes.cpp"

typedef struct metaEvent MetaEvent;

typedef struct track
{
    uint32_t length;
    uint64_t deltaTime;

} Track;

MetaEvent parseMetaEvent(uint8_t *arr, uint32_t *length)
{
    MetaEvent event;
    uint32_t timeCount;
    uint32_t lenCount;

    strcpy(event.description, handleMetaEventDescription(&event, arr));
    if (strlen(event.description) == 0)
    {
        printf("Error: No event registered.\n");
        exit(1);
    }
    // time catg type leng vari
    // VQL  1    1    VQL  leng
    
    event.deltaTime = readVQL(arr, &timeCount);
    event.type[0] = arr[timeCount];
    event.type[1] = arr[timeCount + 1];
    event.length = readVQL(arr + timeCount + 2, &lenCount);
    printf("[counts]%d %d\n", timeCount, lenCount);
    // deltatTime + 'FF' + 'type' + length
    handleMetaEventVariable(&event, arr + timeCount + 2 + lenCount);
    *length = event.length + timeCount + 2 + lenCount;

    // printf("[MetaEvent]");
    // for (size_t i = 0; i < *length; i++)
    // {
    //     printf("%02X ", arr[i]);
    // }
    // printf("\n");

    printf("========== ========== ========== ==========\n");
    printf("[MetaEvent]type   : %02X %02X\n", event.type[0], event.type[1]);
    printf("[MetaEvent]type   : %s\n", event.description);
    printf("[MetaEvent]length : %d\n", event.length);
    printf("[MetaEvent]text   : %s\n", event.text);

    return event;
}

int parseTrack(Track *track, uint8_t *arr)
{
    printf("%d \n", arr[0]);
    // Track :=
    //     <chunk type>
    //     <length>
    //     <MTrk event>+
    const uint8_t magicNumber[4] = {'M', 'T', 'r', 'k'};
    for (size_t i = 0; i < 4; i++)
    {
        if (arr[i] != magicNumber[i])
            return -1;
    }
    // uint32_t timeLength = 0;

    track->length = readLSB32(arr + 4);
    // +8부터

    // track->deltaTime = readVQL(arr + 8, &timeLength);

    printf("[track]length      : %d\n", track->length);
    // printf("[track]deltaTime   : %d\n", track->deltaTime);
    // printf("[VQL]timeLength    : %d\n", timeLength);

    uint32_t metaEventLength = 0;
    uint32_t tempLength = 0;
    
    while (metaEventLength <= track->length)
    {
        // initial 22
        printf("current position: %d\n",  14 + 8 + metaEventLength);
        parseMetaEvent(arr + 8 + metaEventLength, &tempLength);
        metaEventLength += tempLength;
    }
    return metaEventLength;
}