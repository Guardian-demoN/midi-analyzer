#pragma once
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "types.hpp"
#include "readLSB.hpp"

void showMidiEventData(MidiEvent *event, uint8_t *arr, uint32_t offset, uint32_t length)
{
    uint8_t *data = arr + offset;

    printf("<Midi> ch #%d: %s ", event->channelNo, event->description);

    switch (event->type)
    {
    case 0x00:
        break;

    case 0x80:
        printf("note %d ", event->noteNo);
        printf("with %d", event->velocity);
        break;
    case 0x90:
        printf("note %d ", event->noteNo);
        printf("with %d", event->velocity);
        break;
    case 0xB0:
        printf("ctrl %d ", event->controllerNo);
        printf("val %d", event->value);
        break;

    case 0xC0:
        printf("program %d", event->programNo);
        break;
    case 0x59:
        break;
    }
    printf("\n");
}

void showMidiEventDataDetail(MidiEvent *event, uint8_t *arr, uint32_t offset, uint32_t length)
{
    uint8_t *data = arr + offset;

    printf("========== ========== <MidiEvent> ========== ==========\n");
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
    case 0xB0:
        printf("Controller: %d\n", event->controllerNo);
        printf("Value     : %d\n", event->value);
        break;
    case 0xC0:
        printf("program   : %d\n", event->programNo);
        break;
    case 0x59:
        break;
    }
}