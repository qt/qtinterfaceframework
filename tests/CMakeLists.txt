if(QT_BUILD_STANDALONE_TESTS)
    # Add qt_find_package calls for extra dependencies that need to be found when building
    # the standalone tests here.
endif()

# Intermittently skip auto test buuild for OHOS, the
# underlying issue tracked as part of bug QTBUG-148650
if(NOT OHOS)
    qt_build_tests()
endif()
