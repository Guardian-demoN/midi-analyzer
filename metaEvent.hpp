#pragma once

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "types.hpp"
#include "readLSB.hpp"
#include "metaEvent.hpp"
#include "metaEvent_show.hpp"

void handleMetaEventVariable(MetaEvent *event, uint8_t *arr, uint32_t variableOffset);

MetaEvent handleMetaEvent(uint8_t *arr, uint32_t offset, uint32_t *length)
{
    MetaEvent event;
    uint8_t *data = arr + offset;

    uint32_t timeCount; // deltaTime VQL
    uint32_t lenCount;  // length VQL

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

    if (false)
    {
        showMetaEventDataDetail(&event, arr, offset, *length);
    }
    else
    {
        showMetaEventData(&event, arr, offset, *length);
    }

    return event;
}

void handleMetaEventVariable(MetaEvent *event, uint8_t *arr, uint32_t variableOffset)
{
    uint8_t *data = arr + variableOffset;
    switch (event->type)
    {
    case 0x00:
        strcpy(event->description, "Sequence Number");
        event->sequenceNo = readLSB16(data);
        break;
    case 0x01:
        strcpy(event->description, "Text Event");
        event->text = (char *)malloc(sizeof(char) * (event->length + 1));
        strncpy(event->text, (char *)data, event->length);
        event->text[event->length] = 0;
        break;
    case 0x02:
        strcpy(event->description, "Copyright Notice");
        event->text = (char *)malloc(sizeof(char) * (event->length + 1));
        strncpy(event->text, (char *)data, event->length);
        event->text[event->length] = 0;
        break;
    case 0x03:
        strcpy(event->description, "Sequence/Track Name");
        event->text = (char *)malloc(sizeof(char) * (event->length + 1));
        strncpy(event->text, (char *)data, event->length);
        event->text[event->length] = 0;
        break;
    case 0x04:
        strcpy(event->description, "Instrument Name");
        event->text = (char *)malloc(sizeof(char) * (event->length + 1));
        strncpy(event->text, (char *)data, event->length);
        event->text[event->length] = 0;
        break;

    case 0x05:
        strcpy(event->description, "Lyric");
        event->text = (char *)malloc(sizeof(char) * (event->length + 1));
        strncpy(event->text, (char *)data, event->length);
        event->text[event->length] = 0;
        break;
    case 0x06:
        strcpy(event->description, "Marker");
        event->text = (char *)malloc(sizeof(char) * (event->length + 1));
        strncpy(event->text, (char *)data, event->length);
        event->text[event->length] = 0;
        break;
    case 0x07:
        strcpy(event->description, "Cue Point");
        event->text = (char *)malloc(sizeof(char) * (event->length + 1));
        strncpy(event->text, (char *)data, event->length);
        event->text[event->length] = 0;
        break;

    case 0x21:
        strcpy(event->description, "[Obsolete]MIDI Port");
        event->port = data[0];
        break;
    case 0x2F:
        strcpy(event->description, "End of Track");
        break;

    // Set Tempo
    case 0x51:
        strcpy(event->description, "Set Tempo");
        event->ms24PerQNote = readLSB24(data);
        break;
    case 0x54:

        strcpy(event->description, "SMPTE Offset");
        break;

    case 0x58:
        strcpy(event->description, "Time Signature");
        event->timeSignNominator = data[0];
        event->timeSignDenominator = pow(2, data[1]);
        event->clocksPerQNote = data[2];
        event->notated32NotePerQNote = data[3];
        break;
    // Key Signature
    case 0x59:
        strcpy(event->description, "Key Signature");
        event->keySign = toKeySign(data[0]);
        event->isMinor = data[1];
        break;
    case 0x7F:

        strcpy(event->description, "Sequencer-Specific Meta-Event");
    default:
        printf("Error: No meta event registered.\n");
        exit(1);
        break;
    }
}
