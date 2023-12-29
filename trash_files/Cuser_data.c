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

void InitializeDataFile(const char *filename)
{
    FILE *file = fopen(filename, "wb");
    if (!file)
    {
        perror("File opening failed");
        exit(1);
    }

    Header header = {0, 0, 0, sizeof(UserData), sizeof(ParkingSpace)};
    fwrite(&header, sizeof(header), 1, file);
    fclose(file);
}

void AddData(const char *filename, UserData *userData, ParkingSpace *parkingSpace)
{
    FILE *file = fopen(filename, "rb+");
    if (!file)
    {
        InitializeDataFile(filename);
        file = fopen(filename, "rb+");
        if (!file)
        {
            perror("File opening failed");
            exit(1);
        }
    }

    Header header;
    fread(&header, sizeof(header), 1, file);

    Offset userDataOffset = {
        .UserDataOffset = sizeof(Header) + (header.UserDataCount * sizeof(UserData)),
        .ParkingSpaceDataOffset = sizeof(Header) + (header.UserDataCount * sizeof(UserData)) + (header.ParkingSpaceDataCount * sizeof(ParkingSpace))};

    fseek(file, userDataOffset.UserDataOffset, SEEK_SET);

    // 사용자 데이터의 이름 필드를 Null 문자로 종료합니다.
    strcpy(userData->Name, userData->Name);

    fwrite(userData, sizeof(UserData), 1, file);

    fseek(file, userDataOffset.ParkingSpaceDataOffset, SEEK_SET);
    fwrite(parkingSpace, sizeof(ParkingSpace), 1, file);

    header.UserDataCount++;
    header.ParkingSpaceDataCount++;
    header.TotalDataSize += sizeof(UserData) + sizeof(ParkingSpace);

    fseek(file, 0, SEEK_SET);
    fwrite(&header, sizeof(header), 1, file);

    fclose(file);
}

int main(int argc, char *argv[])
{
    const char *filename = "./UserData.bin";

    if (argc != 4)
    {
        fprintf(stderr, "Usage: %s Name CarType CarNumber\n", argv[0]);
        return 1;
    }

    UserData userData;
    strncpy(userData.Name, argv[1], sizeof(userData.Name) - 1);
    userData.Name[sizeof(userData.Name) - 1] = '\0';
    strncpy(userData.CarType, argv[2], sizeof(userData.CarType) - 1);
    userData.CarType[sizeof(userData.CarType) - 1] = '\0';
    strncpy(userData.CarNumber, argv[3], sizeof(userData.CarNumber) - 1);
    userData.CarNumber[sizeof(userData.CarNumber) - 1] = '\0';

    ParkingSpace parkingSpace = {0};

    AddData(filename, &userData, &parkingSpace);

    printf("Data added: %s, %s, %s\n", userData.Name, userData.CarType, userData.CarNumber);

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

//     Offset userDataOffset = {
//         .UserDataOffset = sizeof(Header) + (header.UserDataCount * sizeof(UserData)),
//         .ParkingSpaceDataOffset = sizeof(Header) + (header.UserDataCount * sizeof(UserData)) + (header.ParkingSpaceDataCount * sizeof(ParkingSpace))
//     };

//     // 파일 끝으로 이동해서 사용자 데이터를 추가합니다.
//     fseek(file, 0, SEEK_END);
//     fwrite(userData, sizeof(UserData), 1, file);

//     // 파일 끝으로 이동해서 주차 공간 데이터를 추가합니다.
//     fseek(file, 0, SEEK_END);
//     fwrite(parkingSpace, sizeof(ParkingSpace), 1, file);

//     header.UserDataCount++;
//     header.ParkingSpaceDataCount++;
//     header.TotalDataSize += sizeof(UserData) + sizeof(ParkingSpace);

//     fseek(file, 0, SEEK_SET);
//     fwrite(&header, sizeof(header), 1, file);

//     fclose(file);
// }

// int main(int argc, char *argv[]) {
//     const char* filename = "/home/tjchoi/Project/UserData.bin";

//     if (argc != 4) {
//         fprintf(stderr, "Usage: %s Name CarType CarNumber\n", argv[0]);
//         return 1;
//     }

//     UserData userData = {0};  // 모든 필드를 0으로 초기화
//     strncpy(userData.Name, argv[1], sizeof(userData.Name) - 1);
//     strncpy(userData.CarType, argv[2], sizeof(userData.CarType) - 1);
//     strncpy(userData.CarNumber, argv[3], sizeof(userData.CarNumber) - 1);

//     ParkingSpace parkingSpace = {0};

//     AddData(filename, &userData, &parkingSpace);

//     printf("Data added: %s, %s, %s\n", userData.Name, userData.CarType, userData.CarNumber);

//     return 0;
// }