cmake_minimum_required(VERSION 3.16)

project(Project VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(appProject
    main.cpp
    Data.cpp
    Data.h
    TextData.cpp
    TextData.h
    AudioData.cpp
    AudioData.h
    VideoData.cpp
    VideoData.h
    VideoDataPacket.cpp
    VideoDataPacket.h
    AudioDataPacket.cpp
    AudioDataPacket.h
    TextDataPacket.cpp
    TextDataPacket.h
    DataPacket.cpp
    DataPacket.h
    AppConcepts.h
    TcpProtocol.cpp
    TcpProtocol.h
    TcpHost.cpp
    TcpHost.h
    TcpConnection.cpp
    TcpConnection.h
    TransportLayer.cpp
    TransportLayer.h
    TransportProtocol.cpp
    TransportProtocol.h
    TransportStrategy.cpp
    TransportStrategy.h
    BasicStrategy.cpp
    BasicStrategy.h
    SecureTransportProtocol.cpp
    SecureTransportProtocol.h
    EncryptionStrategy.cpp
    EncryptionStrategy.h
    WhoIsReceiver.cpp
    WhoIsReceiver.h
    WhoIsTransmitter.cpp
    WhoIsTransmitter.h
    DatagramReceiver.h
    DatagramReceiver.cpp
    DatagramProtocolInterface.h
    Signal.h
)

qt_add_qml_module(appProject
    URI Project
    VERSION 1.0
    QML_FILES
        Main.qml
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appProject PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appProject
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appProject
    PRIVATE Qt6::Quick
)

include(GNUInstallDirs)
install(TARGETS appProject
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
