#pragma once

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "types.hpp"
#include "readLSB.hpp"

void showMetaEventData(MetaEvent *event, uint8_t *arr, uint32_t offset, uint32_t length)
{
    uint8_t *data = (arr + offset);

    printf("========== ========== [MetaEvent] ========== ==========\n");
    printf("currentPos: %07X0 %02X %d\n", offset / 16, offset % 16, offset);
    printf("raw       : ");
    for (uint32_t i = 0; i < length; i++)
    {
        printf("%02X ", data[i]);
    }
    printf("\n");
    printf("deltaTime : %d\n", event->deltaTime);
    printf("tHex      : %02X\n", event->type);
    printf("descript  : %s\n", event->description);
    printf("length    : %d\n", event->length);

    switch (event->type)
    {
    case 0x00:
        printf("seq. no    : %d\n", event->sequenceNo);
        break;

    case 0x01:
    case 0x02:
    case 0x03:
    case 0x04:
    case 0x05:
    case 0x06:
    case 0x07:
        printf("text      : %s\n", event->text);
        break;
    // [Obsolete] MIDI Port
    // http://midi.teragonaudio.com/tech/midifile/obsolete.htm
    case 0x21:
        printf("[obsolete]");
        printf("Midi Port : %d\n", event->port);
        break;
    // Set Tempo
    case 0x51:
        printf("24ms/QNote: %d\n", event->ms24PerQNote);
        break;
    // Time Signature
    case 0x58:
        printf("timeSign  : %d/%d\n", event->timeSignNominator, event->timeSignDenominator);
        printf("clks/QNote: %d\n", event->clocksPerQNote);
        printf("32nt/Qnote: %d\n", event->notated32NotePerQNote);

        break;

    // Key Signature
    case 0x59:
        printf("keySign   : %s\n", parseEnum(event->keySign));
        printf("maj./min. : %s\n", event->isMinor ? "Minor" : "Major");
        break;
    }
}
