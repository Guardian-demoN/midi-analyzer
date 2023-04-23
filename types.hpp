#pragma once

#include <stdio.h>
#include <stdint.h>
#include <string.h>

typedef enum keySign
{
    invalid,
    flat7,
    flat1,
    keyOfC,
    sharp1,
    sharp7
} KeySign;

typedef enum namedVelocity{
    off,
    pianississimo,
    pianissimo,
    piano,
    mezzoPiano,
    mezzoForte,
    forte,
    fortissimo,
    fortississimo // very very loud
}Velocity;

const char *parseEnum(KeySign);
KeySign toKeySign(uint8_t value);

typedef struct header
{
    uint32_t length;
    uint16_t format;
    uint16_t ntrks;
    uint8_t divisionSMPTEMode;
    uint16_t ticksPerQNote;
    uint8_t SMPTE;
    uint8_t ticksPerFrame;
} Header;

typedef struct track
{
    uint32_t length;
    uint32_t deltaTime;
} Track;

typedef struct channel
{
    uint8_t program;

} Channel;



class MidiEvent
{
public:
    uint32_t deltaTime;
    uint8_t type;
    uint8_t length;

    uint8_t channelNo;
    uint8_t programNo;

    uint8_t noteNo;
    uint8_t velocity;

    uint32_t controllerNo;
    uint32_t value; // value, amount, etc


    char description[50];
};

class MetaEvent
{
public:
    uint32_t deltaTime;

    // sequenceNo
    uint32_t sequenceNo;

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
    uint8_t type;
    uint8_t length;
    char *text;
    char description[30];
    // Common

    // keySign_2
    uint8_t isMinor;

    // obsolete_port
    uint8_t port;
};