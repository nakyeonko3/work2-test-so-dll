import ctypes

# C 라이브러리 로드
user_data_lib = ctypes.CDLL("./user_data.so")

USER_DATA_BIN_FILE_PATH = "./UserData.bin"

class UserDataManagement:
    def __init__(self, user_data_file="./UserData.bin"):
        self.user_data_file = user_data_file
        self.user_data = self.load_data()  # 데이터를 파일에서 읽어와 user_data 딕셔너리에 초기화

    def load_data(self):
        user_data = {}
        # with open(self.user_data_file, 'r') as file:
        #     lines = file.readlines()
        #     for line in lines:
        #         parts = line.strip().split(',')
        #         if len(parts) == 4:
        #             CarNumber, Name, CarType, ParkingSpace = parts
        #             user_data[CarNumber] = {
        #                 "Name": Name,
        #                 "CarType": CarType,
        #                 "CarNumber": CarNumber,
        #                 "ParkingSpace": int(ParkingSpace)  # 주차 공간 정보를 정수로 변환
        #             }
        return user_data

    # C 라이브러리에서 사용자 데이터 검색 함수 호출
    def search_data(self, CarNumber):
        user_data_lib.search_data.restype = ctypes.c_char_p
        result = user_data_lib.search_data(ctypes.c_char_p(self.user_data_file.encode("utf-8")), ctypes.c_char_p(CarNumber.encode("utf-8")))
        return result.decode("utf-8")

    # C 라이브러리에서 사용자 데이터 등록 함수 호출
    def register_data(self, UserData):
        Name = ctypes.c_char_p(UserData["Name"].encode("utf-8"))
        CarType = ctypes.c_char_p(UserData["CarType"].encode("utf-8"))
        CarNumber = ctypes.c_char_p(UserData["CarNumber"].encode("utf-8"))
        filepath = ctypes.c_char_p(self.user_data_file.encode("utf-8"))

        user_data_lib.register_data(filepath, Name, CarType, CarNumber)
        return "데이터가 등록되었습니다."

    def match_and_register_data(self, CarNumber, ParkingSpace):
        if CarNumber in self.user_data:
            self.user_data[CarNumber]["ParkingSpace"] = ParkingSpace
            return "주차 공간이 등록되었습니다."
        else:
            return "데이터를 다시 입력해주세요."
