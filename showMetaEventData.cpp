#pragma once
#include <string.h>
#include <iostream>
#include <math.h>
#include "readLSB.cpp"
#include "eventTypes.cpp"

void showMetaEventData(MetaEvent *event)
{
    printf("========== ========== [MetaEvent] ========== ==========\n");
    printf("deltaTime : %d\n", event->deltaTime);
    printf("tHex      : %02X %02X\n", event->type[0], event->type[1]);
    printf("type      : %s\n", event->description);
    printf("length    : %d\n", event->length);
    switch (event->type[0])
    {
    case 0xFF:
        switch (event->type[1])
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
        // http://midi.te
        ragonaudio.com/tech/midifile/obsolete.htm
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
        }
        break;
    }
}
