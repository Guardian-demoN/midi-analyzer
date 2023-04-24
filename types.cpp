#include "types.hpp"

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
    return "";
}

KeySign toKeySign(uint8_t value)
{
    switch (value + 7)
    {
    case 0:
        return flat7;
        break;
    case 6:
        return flat1;
        break;
    case 7:
        return keyOfC;
        break;
    case 8:
        return sharp1;
        break;
    case 14:
        return sharp7;
        break;
    }
    return invalid;
}