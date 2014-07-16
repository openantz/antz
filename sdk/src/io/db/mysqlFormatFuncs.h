char** npFinalFormatChunks(char **buffer, int *numberOfChunks);
int npFormatChunk(char *buffer, char* formattedValues, char* values);
char** npNewChunkAllValues(char* buffer, int *numOfChunks, void* dataRef);
int fixedChunkValues(char* buffer, char* chunked, int chunkSize);