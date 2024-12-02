#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "Logger.h"

int main(int argc, char** argv)
{
    Logger::initialize("TestOutput.log");
    Logger::setLogLevel(LogLevel::INFO);

    ::testing::InitGoogleTest(&argc, argv);
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
