cmake_minimum_required(VERSION 3.13)
project(slither_io)

set(CMAKE_CXX_STANDARD 14)

add_executable(slither_io server.cpp model/Cell.cpp model/Cell.h model/Client.cpp model/Client.h model/Direction.cpp model/ServerQueue.cpp model/ServerQueue.h model/Direction.cpp model/Direction.h model/ServerUser.cpp model/ServerUser.h model/Map.cpp model/Map.h model/ServerGameController.cpp model/ServerGameController.h)