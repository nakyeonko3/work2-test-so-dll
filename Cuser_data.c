#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 데이터 헤더를 위한 구조체 정의
typedef struct
{
    int TotalDataSize;
    int UserDataCount;
    int SelectParkingSpaceDataCount;
    int UserDataSize;
    int SelectParkingSpaceSize;
} Header;

// 데이터 오프셋을 위한 구조체 정의
typedef struct
{
    int UserDataOffset;
    int SelectParkingSpaceDataOffset;
} Offset;

// 사용자 데이터 구조체 정의
typedef struct
{
    char Name[13];
    char CarType[20];
    char CarNumber[12];
} UserData;

// 선택한 주차 공간 데이터 구조체 정의
typedef struct
{
    int ParkingSpace;
} ParkingSpace;

// 데이터 파일 초기화 및 헤더 작성
void InitializeDataFile(const char *filename)
{
    FILE *file = fopen(filename, "wb");
    if (!file)
    {
        perror("File opening failed");
        return;
    }

    Header header = {0, 0, 0, sizeof(UserData), sizeof(ParkingSpace)};
    fwrite(&header, sizeof(header), 1, file); // 파일 시작 부분에 헤더 저장

    fclose(file);
}

// 사용자 데이터 및 주차 공간 데이터를 파일에 추가
void AddData(const char *filename, UserData *userData, ParkingSpace *parkingSpace)
{
    FILE *file = fopen(filename, "r+b"); // 읽기/쓰기 모드로 파일 열기 ('b'는 바이너리 모드)
    if (!file)
    {
        perror("File opening failed");
        return;
    }

    Header header;
    Offset offset;

    // 헤더 정보 읽기
    fread(&header, sizeof(header), 1, file);

    // 현재 오프셋 계산
    offset.UserDataOffset = sizeof(Header) + (header.UserDataCount * sizeof(UserData));
    offset.SelectParkingSpaceDataOffset = offset.UserDataOffset + (header.UserDataCount * sizeof(UserData)) + (header.SelectParkingSpaceDataCount * sizeof(ParkingSpace));

    // 오프셋으로 이동 후 사용자 데이터 쓰기
    fseek(file, offset.UserDataOffset, SEEK_SET);
    fwrite(userData, sizeof(UserData), 1, file);

    // 오프셋으로 이동 후 주차 공간 데이터 쓰기
    fseek(file, offset.SelectParkingSpaceDataOffset, SEEK_SET);
    fwrite(parkingSpace, sizeof(ParkingSpace), 1, file);

    // 헤더 업데이트
    header.TotalDataSize += sizeof(UserData) + sizeof(ParkingSpace);
    header.UserDataCount++;
    header.SelectParkingSpaceDataCount++;

    // 파일 시작으로 이동 후 헤더 정보 업데이트
    fseek(file, 0, SEEK_SET);
    fwrite(&header, sizeof(header), 1, file);

    fclose(file);
}
const char *search_data(const char *filename, const char *CarNumber)
{
    FILE *file = fopen(filename, "rb");
    if (!file)
    {
        perror("File opening failed");
        return "File opening failed";
    }

    Header header;
    fread(&header, sizeof(header), 1, file);

    // 사용자 데이터 읽기 (CarNumber에 해당하는 데이터 검색)
    UserData userData;
    const char *not_found_message = "no search_data";

    for (int i = 0; i < header.UserDataCount; ++i)
    {
        fread(&userData, sizeof(UserData), 1, file);
        if (strcmp(userData.CarNumber, CarNumber) == 0)
        {
            fclose(file);
            return "데이터를 찾았습니다."; // 원하는 데이터를 찾았을 때 원하는 결과 문자열로 반환
        }
    }

    fclose(file);
    return not_found_message; // 원하는 데이터를 찾지 못했을 때 기본 메시지 반환
}

const char *test_char()
{
    const char *name = "{'name':'고낙연', 'name':'고낙연', 'name':'고낙연'}";
    return name;
}

const char *get_data_all(const char *filename)
{
    FILE *file = fopen(filename, "rb");
    if (!file)
    {
        perror("File opening failed");
        return "File opening failed";
    }

    Header header;
    fread(&header, sizeof(header), 1, file);

    // 사용자 데이터 읽기 (CarNumber에 해당하는 데이터 검색)
    UserData userData;
    for (int i = 0; i < header.UserDataCount; ++i)
    {
        fread(&userData, sizeof(UserData), 1, file);
        printf("CarNumber:%s\n", userData.CarNumber);
        printf("CarType:%s\n", userData.CarType);
        printf("Name:%s\n", userData.Name);
        printf("----------------------\n");
    }
    fclose(file);
    return "ds"; //
}

// 사용자 데이터 등록 함수
void register_data(const char *filename, const char *Name, const char *CarType, const char *CarNumber)
{
    FILE *file = fopen(filename, "r+b"); // 읽기/쓰기 모드로 파일 열기 ('b'는 바이너리 모드)
    if (!file)
    {
        perror("File opening failed");
        return;
    }

    Header header;
    Offset offset;

    // 헤더 정보 읽기
    fread(&header, sizeof(header), 1, file);

    // 현재 오프셋 계산
    offset.UserDataOffset = sizeof(Header) + (header.UserDataCount * sizeof(UserData));
    offset.SelectParkingSpaceDataOffset = offset.UserDataOffset + (header.UserDataCount * sizeof(UserData)) + (header.SelectParkingSpaceDataCount * sizeof(ParkingSpace));

    // 오프셋으로 이동 후 사용자 데이터 쓰기
    fseek(file, offset.UserDataOffset, SEEK_SET);

    UserData userData;
    strncpy(userData.Name, Name, sizeof(userData.Name));
    strncpy(userData.CarType, CarType, sizeof(userData.CarType));
    strncpy(userData.CarNumber, CarNumber, sizeof(userData.CarNumber));

    fwrite(&userData, sizeof(UserData), 1, file);

    // 헤더 업데이트
    header.TotalDataSize += sizeof(UserData) + sizeof(ParkingSpace);
    header.UserDataCount++;

    // 파일 시작으로 이동 후 헤더 정보 업데이트
    fseek(file, 0, SEEK_SET);
    fwrite(&header, sizeof(header), 1, file);
    printf("Data added: %s, %s, %s\n", userData.Name, userData.CarType, userData.CarNumber);

    fclose(file);
}

int main(int argc, char *argv[])
{

    const char *filename = "UserData.bin";
    if (argc != 4)
    {
        fprintf(stderr, "Usage: %s Name CarType CarNumber\n", argv[0]);
        return 1;
    }
    // InitializeDataFile(filename); // 파일 초기화

    register_data(filename, argv[1], argv[2], argv[3]);
    // register_data(filename, "고낙연", "superX", "312314");
    // printf("%s", search_data(filename, "312314"));

    return 0;
}
