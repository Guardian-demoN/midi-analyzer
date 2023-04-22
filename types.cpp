#pragma once
#include <iostream>

enum KeySign
{
    flat7,
    flat1,
    keyOfC,
    sharp1,
    sharp7
};

KeySign toKeySign(uint8_t value)
{
    switch (value)
    {
    case -7:
        return flat7;
        break;
    case -1:
        return flat1;
        break;
    case 0:
        return keyOfC;
        break;
    case 1:
        return sharp1;
        break;
    case 7:
        return sharp7;
        break;
    }
}

const char *parseEnum(KeySign keySign)
{
    switch (keySign)
    {
    case flat7:
        return "7 flats";
        break;

    case flat1:
        return "1 flats";
        break;

    case keyOfC:
        return "key of C";
        break;

    case sharp1:
        return "1 sharp";
        break;

    case sharp7:
        return "7 sharps";
        break;
    }
}



typedef struct header
{
    uint32_t length;
    uint16_t format;
    uint16_t ntrks;
    bool divisionSMPTEMode;
    uint16_t ticksPerQNote;
    uint8_t SMPTE;
    uint8_t ticksPerFrame;
} Header;

typedef struct track
{
    uint32_t length;
    uint64_t deltaTime;
} Track;

typedef struct channel{
    uint8_t program;

}Channel;

typedef struct event{
    uint64_t deltaTime;

    uint8_t channelNo;
    uint8_t programNo;

}Event;

typedef struct metaEvent
{
    uint64_t deltaTime;

    // sequenceNo
    uint16_t sequenceNo;

    // setTempo
    uint32_t ms24PerQNote;

    // timeSign
    uint8_t timeSignNominator;
    uint8_t timeSignDenominator;

    uint8_t clocksPerQNote;
    uint8_t notated32NotePerQNote;


    // keySign
    KeySign keySign;

    // Common
    uint8_t type[2];
    uint8_t length;
    char *text;
    char description[30];
    // Common

    // keySign_2
    bool isMinor;

    // obsolete_port
    uint8_t port;

} MetaEvent;