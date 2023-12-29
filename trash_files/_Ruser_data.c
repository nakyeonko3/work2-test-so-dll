#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int TotalDataSize;
    int UserDataCount;
    int ParkingSpaceDataCount;
    int UserDataSize;
    int ParkingSpaceSize;
} Header;

typedef struct {
    char Name[13];
    char CarType[20];
    char CarNumber[12];
} UserData;

typedef struct {
    int ParkingSpace;
} ParkingSpace;

void ReadData(const char* filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("File opening failed");
        return;
    }

    Header header;
    fread(&header, sizeof(header), 1, file);

    printf("Total Data Size: %d\n", header.TotalDataSize);
    printf("User Data Count: %d\n", header.UserDataCount);
    printf("Parking Space Data Count: %d\n", header.ParkingSpaceDataCount);

    // 사용자 데이터 읽기
    UserData* userArray = malloc(header.UserDataCount * sizeof(UserData));
    if (!userArray) {
        fclose(file);
        perror("Memory allocation failed");
        return;
    }

    fread(userArray, sizeof(UserData), header.UserDataCount, file);
    for (int i = 0; i < header.UserDataCount; ++i) {
        printf("User %d: Name: %s, CarType: %s, CarNumber: %s\n", i + 1, userArray[i].Name, userArray[i].CarType, userArray[i].CarNumber);
    }
    free(userArray);

    // 주차 공간 데이터 읽기
    ParkingSpace* spaceArray = malloc(header.ParkingSpaceDataCount * sizeof(ParkingSpace));
    if (!spaceArray) {
        fclose(file);
        perror("Memory allocation failed");
        return;
    }

    fread(spaceArray, sizeof(ParkingSpace), header.ParkingSpaceDataCount, file);
    for (int i = 0; i < header.ParkingSpaceDataCount; ++i) {
        printf("Parking Space %d: %d\n", i + 1, spaceArray[i].ParkingSpace);
    }
    free(spaceArray);

    fclose(file);
}

// int main() {
//     const char* filename = "UserData.bin";
//     ReadData(filename);  // 파일 이름을 전달하여 ReadData 함수 호출
//     return 0;
// }
