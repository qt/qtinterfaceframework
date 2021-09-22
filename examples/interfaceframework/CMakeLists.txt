# Generated from interfaceframework.pro.

cmake_minimum_required(VERSION 3.16)
project(example LANGUAGES CXX)

if(QT_FEATURE_ifcodegen)
    add_subdirectory(qface-climate)
    add_subdirectory(qface-addressbook)
endif()
if(QT_FEATURE_ifcodegen AND QT_FEATURE_remoteobjects)
    add_subdirectory(qface-remote)
    add_subdirectory(qface-tutorial)
endif()
