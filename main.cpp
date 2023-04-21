#include <string.h>
#include <iostream>
#include "readLSB.cpp"
#include "eventTypes.cpp"
#include "trackHeader.cpp"
#include "trackChunk.cpp"

using namespace std;

// extern uint16_t readLSB16(uint8_t* arr);
// extern uint32_t readLSB32(uint8_t* arr);
// extern uint64_t readVQL(uint8_t *arr, uint32_t *count);

#define BUFFER_LENGTH 2000
uint8_t buffer[BUFFER_LENGTH];

#define PATH_LENGTH 256
char filePath[PATH_LENGTH];

bool getDirectory(char *str, size_t len, char *buffer)
{
    char c;
    printf("%d ", len);
    for (size_t i = len - 1; i >= 0; i--)
    {
        c = str[i];
        if (c == '\\' || c == '/')
        {
            strncpy(buffer, str, i);
            return true;
        }
    }
    return false;
}

int main(int argc, char **argv)
{
    printf("%s\n", argv[0]);
    // printf("%d\n", sizeof(argv[0]));
    getDirectory(argv[0], strlen(argv[0]), filePath);
    printf("%s\n", filePath);
    strcat(filePath, "\\resource\\MIDI_sample.mid");
    printf("%s\n", filePath);
    FILE *readFile = fopen(filePath, "rb");
    if (readFile == NULL)
    {
        printf("no file detected.");
        return 1;
    }
    fread(buffer, 1, sizeof(buffer), readFile);
    Header header;
    int index = parseHeader(&header, buffer);
    if (index == -1)
    {
        printf("header invalid");
        return 1;
    }
    Track track;
    index = parseTrack(&track, buffer + index);
    if (index == -1)
    {
        printf("track invalid");
        return 1;
    }
    return 0;
}