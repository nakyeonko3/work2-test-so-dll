#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    int TotalDataSize;
    int UserDataCount;
    int ParkingSpaceDataCount;
    int UserDataSize;
    int ParkingSpaceSize;
} Header;

typedef struct
{
    int UserDataOffset;
    int ParkingSpaceDataOffset;
} Offset;

typedef struct
{
    char Name[13];
    char CarType[20];
    char CarNumber[12];
} UserData;

typedef struct
{
    int ParkingSpace;
} ParkingSpace;

void ReadUserData(const char *filename, int index, UserData *userData, ParkingSpace *parkingSpace)
{
    FILE *file = fopen(filename, "rb");
    if (!file)
    {
        perror("File opening failed");
        exit(1);
    }

    Header header;
    fread(&header, sizeof(header), 1, file);

    if (index >= 0 && index < header.UserDataCount)
    {
        Offset offset = {
            .UserDataOffset = sizeof(Header) + (index * sizeof(UserData)),
            .ParkingSpaceDataOffset = sizeof(Header) + (header.UserDataCount * sizeof(UserData)) + (index * sizeof(ParkingSpace))};

        fseek(file, offset.UserDataOffset, SEEK_SET);
        fread(userData, sizeof(UserData), 1, file);

        userData->Name[sizeof(userData->Name) - 1] = '\0';

        fseek(file, offset.ParkingSpaceDataOffset, SEEK_SET);
        fread(parkingSpace, sizeof(ParkingSpace), 1, file);
    }

    fclose(file);
}

int main()
{
    const char *filename = "./UserData.bin";

    Header header;
    FILE *file = fopen(filename, "rb");
    if (!file)
    {
        perror("File opening failed");
        return 1;
    }
    fread(&header, sizeof(header), 1, file);

    int totalDataCount = header.UserDataCount;

    UserData *userDataArray = malloc(sizeof(UserData) * totalDataCount);
    ParkingSpace *parkingSpaceArray = malloc(sizeof(ParkingSpace) * totalDataCount);

    if (!userDataArray || !parkingSpaceArray)
    {
        perror("Memory allocation failed");
        fclose(file);
        free(userDataArray);
        free(parkingSpaceArray);
        return 1;
    }

    for (int i = 0; i < totalDataCount; i++)
    {
        ReadUserData(filename, i, &userDataArray[i], &parkingSpaceArray[i]);
        printf("UserData %d: %s, %s, %s\n", i, userDataArray[i].Name, userDataArray[i].CarType, userDataArray[i].CarNumber);
        printf("ParkingSpace %d: %d\n", i, parkingSpaceArray[i].ParkingSpace);
    }

    free(userDataArray);
    free(parkingSpaceArray);
    fclose(file);

    return 0;
}

// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>

// typedef struct {
//     int TotalDataSize;
//     int UserDataCount;
//     int ParkingSpaceDataCount;
//     int UserDataSize;
//     int ParkingSpaceSize;
// } Header;

// typedef struct {
//     int UserDataOffset;
//     int ParkingSpaceDataOffset;
// } Offset;

// typedef struct {
//     char Name[13];
//     char CarType[20];
//     char CarNumber[12];
// } UserData;

// typedef struct {
//     int ParkingSpace;
// } ParkingSpace;

// void InitializeDataFile(const char* filename) {
//     FILE* file = fopen(filename, "wb");
//     if (!file) {
//         perror("File opening failed");
//         exit(1);
//     }

//     Header header = {0, 0, 0, sizeof(UserData), sizeof(ParkingSpace)};
//     fwrite(&header, sizeof(header), 1, file);
//     fclose(file);
// }

// void AddData(const char* filename, UserData* userData, ParkingSpace* parkingSpace) {
//     FILE* file = fopen(filename, "rb+");
//     if (!file) {
//         InitializeDataFile(filename);
//         file = fopen(filename, "rb+");
//         if (!file) {
//             perror("File opening failed");
//             exit(1);
//         }
//     }

//     Header header;
//     fread(&header, sizeof(header), 1, file);

//     Offset offset = {
//         .UserDataOffset = sizeof(Header) + (header.UserDataCount * sizeof(UserData)),
//         .ParkingSpaceDataOffset = sizeof(Header) + (header.UserDataCount * sizeof(UserData)) + (header.ParkingSpaceDataCount * sizeof(ParkingSpace))
//     };

//     // 파일 끝으로 이동해서 데이터를 추가합니다.
//     fseek(file, 0, SEEK_END);
//     fwrite(userData, sizeof(UserData), 1, file);
//     header.UserDataCount++;

//     fseek(file, 0, SEEK_END);
//     fwrite(parkingSpace, sizeof(ParkingSpace), 1, file);
//     header.ParkingSpaceDataCount++;

//     header.TotalDataSize += sizeof(UserData) + sizeof(ParkingSpace);

//     fseek(file, 0, SEEK_SET);
//     fwrite(&header, sizeof(header), 1, file);

//     fclose(file);
// }

// void ReadUserData(const char* filename, int index, UserData* userData) {
//     FILE* file = fopen(filename, "rb");
//     if (!file) {
//         perror("File opening failed");
//         exit(1);
//     }

//     Header header;
//     fread(&header, sizeof(header), 1, file);

//     if (index >= 0 && index < header.UserDataCount) {
//         Offset offset = {
//             .UserDataOffset = sizeof(Header) + (index * sizeof(UserData)),
//             .ParkingSpaceDataOffset = sizeof(Header) + (header.UserDataCount * sizeof(UserData)) + (index * sizeof(ParkingSpace))
//         };

//         fseek(file, offset.UserDataOffset, SEEK_SET); // UserData 시작 위치로 이동
//         fread(userData, sizeof(UserData), 1, file);
//     } else {
//         fclose(file);
//         perror("Invalid index");
//         exit(1);
//     }

//     fclose(file); // 파일을 읽은 후 닫음
// }

// int main() {
//     const char* filename = "/home/tjchoi/Project/UserData.bin";

//     UserData readUserData; // 읽은 데이터를 저장할 구조체

//     ReadUserData(filename, 0, &readUserData);  // 첫 번째 사용자 데이터

//     if (strlen(readUserData.Name) == 0) {
//         printf("Failed to read user data.\n");
//     } else {
//         printf("Read Data: %s, %s, %s\n", readUserData.Name, readUserData.CarType, readUserData.CarNumber);
//     }

//     return 0;
// }