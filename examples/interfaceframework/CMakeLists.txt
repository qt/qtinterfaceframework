# Generated from interfaceframework.pro.

cmake_minimum_required(VERSION 3.14)
project(example LANGUAGES CXX)

if(QT_FEATURE_ifcodegen)
    add_subdirectory(qface-if-climate)
    add_subdirectory(qface-if-addressbook)
endif()
if(QT_FEATURE_ifcodegen AND QT_FEATURE_remoteobjects)
    add_subdirectory(qface-if-remote)
    add_subdirectory(qface-tutorial)
endif()
