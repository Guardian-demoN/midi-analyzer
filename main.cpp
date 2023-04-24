#include <stdio.h>
#include <string.h>

#define DEBUG_SHOW_HEADER 1

#include "readLSB.hpp"
#include "metaEvent.hpp"
#include "trackHeader.hpp"
#include "trackChunk.hpp"

// extern uint16_t readLSB16(uint8_t* arr);
// extern uint32_t readLSB32(uint8_t* arr);
// extern uint64_t readVQL(uint8_t *arr, uint32_t *count);

// header : 14
// 

#define BUFFER_LENGTH 20000
uint8_t buffer[BUFFER_LENGTH];

#define PATH_LENGTH 256
char filePath[PATH_LENGTH];

uint8_t getDirectory(char *str, uint8_t len, char *buffer)
{
    char c;
    for (uint8_t  i = len - 1; i >= 0; i--)
    {
        c = str[i];
        if (c == '\\' || c == '/')
        {
            strncpy(buffer, str, i);
            return 1;
        }
    }
    return 0;
}

int main(int argc, char **argv)
{
    printf("%s\n", argv[0]);

    // get file path
    getDirectory(argv[0], strlen(argv[0]), filePath);
    strcat(filePath, "\\resource\\After_You've_Gone_chorus_Nashville_Number_System_score.mid");

    // read file
    FILE *readFile = fopen(filePath, "rb");
    if (readFile == NULL)
    {
        printf("no file detected.");
        return 1;
    }

    // get file length
    fseek(readFile, 0L, SEEK_END);
    uint32_t fileLength = ftell(readFile);
    printf("[file length]%d\n", fileLength);
    rewind(readFile);

    // copy file into buffer
    fread(buffer, 1, sizeof(buffer), readFile);

    // parse header
    Header header;
    uint32_t index = parseHeader(&header, buffer);
    uint32_t headerLength = index;
    if (index == -1)
    {
        printf("header invalid");
        return 1;
    }
    // 14 230 8 252
    // parse track
    uint8_t trackIndex = 1;
    while (index < fileLength)
    {
        printf(">> [Track #%d]\n", trackIndex);
        Track track;
        index = parseTrack(&track, buffer, index);
        if (index == -1)
        {
            printf("track invalid");
            return 1;
        }
    }
    return 0;
}