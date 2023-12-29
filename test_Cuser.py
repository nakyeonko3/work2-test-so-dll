# C 라이브러리 로드
import ctypes
import pickle
USER_DATA_SO_FILE_PATH = "./user_data.so"
USER_DATA_BIN_FILE_PATH = "./UserData.bin"

user_data_lib = ctypes.CDLL(USER_DATA_SO_FILE_PATH)

def search_data(carNumber):
    user_data_lib.search_data.restype = ctypes.c_char_p
    result = user_data_lib.search_data(ctypes.c_char_p(USER_DATA_BIN_FILE_PATH.encode("utf-8")), ctypes.c_char_p(carNumber.encode("utf-8")))
    return result.decode("utf-8")


def register_data(UserData):
    Name = ctypes.c_char_p(UserData["Name"].encode("utf-8"))
    CarType = ctypes.c_char_p(UserData["CarType"].encode("utf-8"))
    CarNumber = ctypes.c_char_p(UserData["CarNumber"].encode("utf-8"))
    filepath = ctypes.c_char_p(USER_DATA_BIN_FILE_PATH.encode("utf-8"))

    user_data_lib.register_data(filepath, Name, CarType, CarNumber)
    return "데이터가 등록되었습니다."

def get_all_data():
    user_data_lib.test_char.restype = ctypes.c_char_p
    result = user_data_lib.test_char()
    print(result.decode("utf-8"))

def load_data():
    user_data = {}
    with open(USER_DATA_BIN_FILE_PATH, 'rb') as file:
        # lines = file.readlines()
        # print(file.read())
        print(file.read().decode('ascii'))
        # for line in lines:
        #     parts = line.strip().split(',')
        #     if len(parts) == 4:
        #         CarNumber, Name, CarType, ParkingSpace = parts
        #         user_data[CarNumber] = {
        #             "Name": Name,
        #             "CarType": CarType,
        #             "CarNumber": CarNumber,
        #             "ParkingSpace": int(ParkingSpace)  # 주차 공간 정보를 정수로 변환
        #         }
    return user_data


# with open(USER_DATA_BIN_FILE_PATH, "wb") as f:
#     pickle.dump("dsd",f)

# with open(USER_DATA_BIN_FILE_PATH, "rb") as f:
#     result = pickle.load(f)
#     print(result)

get_all_data()
# register_data({"Name":"konakayon","CarType":"abc","CarNumber":"12345"})
print(search_data("312314"))


# load_data()