#pragma once
#include <string.h>
#include <iostream>
#include "readLSB.cpp"

typedef struct metaEvent
{
    uint64_t deltaTime;
    uint8_t type[2];
    uint8_t length;
    char *text;
    char description[30];
} MetaEvent;

const char* handleMetaEventDescription(MetaEvent* event, uint8_t* arr){
    switch(arr[1]){
        case 0xFF:
            switch(arr[2]){
                case 0x00: strcpy(event->description, "Sequence Number"); break;
                case 0x01: strcpy(event->description, "Text Event"); break;
                case 0x02: strcpy(event->description, "Copyright Notice"); break;
                case 0x03: strcpy(event->description, "Sequence/Track Name"); break;
                case 0x04: strcpy(event->description, "Instrument Name"); break;
                case 0x05: strcpy(event->description, "Lyric"); break;
                case 0x06: strcpy(event->description, "Marker"); break;
                case 0x07: strcpy(event->description, "Cue Point"); break;
                case 0x20: strcpy(event->description, "MIDI Channel Prefix"); break;
                case 0x2F: strcpy(event->description, "End of Track"); break;
                case 0x51: strcpy(event->description, "Set Tempo"); break;
                case 0x54: strcpy(event->description, "SMPTE Offset"); break;
                case 0x58: strcpy(event->description, "Time Signature"); break;
                case 0x59: strcpy(event->description, "Key Signature"); break;
                case 0x7F: strcpy(event->description, "Sequencer-Specific Meta-Event"); break;

                default: return ""; break;
            }
            break;

        default:
            return "";
            break;
    }
}

void handleMetaEventVariable(MetaEvent* event, uint8_t* arrFromVariable){
    switch(event->type[0]){
        case 0xFF:
            switch(event->type[1]){
                case 0x01:
                case 0x02:
                case 0x03:
                case 0x04:
                case 0x05:
                case 0x06:
                case 0x07:
                    event->text = new char[event->length + 1];
                    strncpy(event->text, (char *)arrFromVariable, event->length);
                    event->text[event->length] = 0;
                break;
            }
        break;
    }
}