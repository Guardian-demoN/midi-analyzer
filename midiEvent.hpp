#pragma once
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "types.hpp"
#include "readLSB.hpp"

void handleMidiEventVariable(MidiEvent *event, uint8_t *arr, uint32_t variableOffset);
void showMidiEventData(MidiEvent *event, uint8_t *arr, uint32_t offset, uint32_t length);
MidiEvent handleMidiEvent(uint8_t *arr, uint32_t offset, uint32_t *length)
{
    MidiEvent event;
    uint8_t *data = arr + offset;

    uint32_t timeCount;

    strcpy(event.description, "");

    // timeCount가 몇 개인지 모르기 때문에 여기에 추가 필요
    event.deltaTime = readVQL(data, &timeCount);


    // time catg vari
    // VQL  1    leng
    event.type = data[timeCount] & 0xF0;
    event.channelNo = (data[timeCount] & 0x0F) + 1;

    // length는 따로 값이 주어지지 않기 때문에 showMidiEventData 내부에서 처리
    handleMidiEventVariable(&event, data, timeCount + 1);
    showMidiEventData(&event, arr, offset, *length);

    *length = timeCount + 1 + event.length;
}

void handleMidiEventVariable(MidiEvent *event, uint8_t *arr, uint32_t variableOffset)
{
    // https://www.mixagesoftware.com/en/midikit/help/
    uint8_t *data = arr + variableOffset;
    switch (event->type)
    {
    case 0x80:
        strcpy(event->description, "Note Off");
        event->length = 2;
        event->noteNo = data[0];
        event->velocity = data[1];
        break;
    case 0x90:
        strcpy(event->description, "Note On");
        event->length = 2;
        event->noteNo = data[0];
        event->velocity = data[1];
        break;
    case 0xA0:
        strcpy(event->description, "Note Polyphonic key pressure/Aftertouch");
        event->length = 2;
        event->noteNo = data[0];
        event->value = data[1];
        break;
    case 0xB0:
        strcpy(event->description, "Control Change");
        event->length = 2;
        event->controllerNo = data[0];
        event->value = data[1];
        break;
    case 0xC0:
        strcpy(event->description, "Program Change");
        event->length = 1;
        event->programNo = data[0];
        break;
    case 0xD0:
        strcpy(event->description, "Channel pressure/Aftertouch");
        event->length = 1;
        event->value = data[0];
        break;
    case 0xE0:
        strcpy(event->description, "Pitch bend change");
        event->length = 2;
        break;
    case 0xF0:
        switch (event->channelNo & 0b00001000)
        {
        case 0x00000000:
            if (event->channelNo == 0)
            {
                strcpy(event->description, "System Message: System Exclusive");
                exit(1);
            }
            else
            {
                strcpy(event->description, "System Message: System Common");
            }
            break;

        case 0x00001000:
            strcpy(event->description, "System Message: System Real Time");
        }
    default:
        printf("Error: No midi event registered.\n");
        exit(1);
        break;
    }
}

void showMidiEventData(MidiEvent *event, uint8_t *arr, uint32_t offset, uint32_t length)
{
    uint8_t *data = arr + offset;

    printf("========== ========== <MidiEvent> ========== ==========\n");
    printf("currentPos: %07X0 %02X %d\n", offset / 16, offset % 16, offset);
    printf("raw       : ");
    for (uint32_t i = 0; i < event->length; i++)
    {
        printf("%02X ", data[i]);
    }
    printf("\n");
    printf("deltaTime : %d\n", event->deltaTime);
    printf("tHex      : %02X\n", event->type);
    printf("descript  : %s\n", event->description);
    printf("length    : %d\n", event->length);
    printf("channel   : %d\n", event->channelNo);

    switch (event->type)
    {
    case 0x00:
        break;

    case 0x80:
        printf("NoteNumber: %d\n", event->noteNo);
        printf("Velocity  : %d\n", event->velocity);
        break;
    case 0x90:
        printf("NoteNumber: %d\n", event->noteNo);
        printf("Velocity  : %d\n", event->velocity);
        break;

    case 0xC0:
        printf("program   : %d\n", event->programNo);
        break;
    case 0x59:
        break;
    }
}