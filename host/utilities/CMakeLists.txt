cmake_minimum_required(VERSION 2.8)
add_subdirectory(bladeRF-cli)

if(ENABLE_BACKEND_LIBUSB)
    find_package(LibUSB)

    # Should be checked the top level...included here just to be safe
    if(NOT LIBUSB_FOUND)
        message(FATAL_ERROR "Unable to find libusb")
    endif(NOT LIBUSB_FOUND)
else()
    # The remainder of this file contains libusb-specific items
    return()
endif(ENABLE_BACKEND_LIBUSB)

if (NOT LIBUSB_HAVE_HOTPLUG)
    if(WIN32)
        find_package(LibUSB)
        try_run(RET_RUN RET_COM
            ${CMAKE_CURRENT_BINARY_DIR}
            "${CMAKE_CURRENT_SOURCE_DIR}/ver.c"
            CMAKE_FLAGS "-DINCLUDE_DIRECTORIES:STRING=${LIBUSB_INCLUDE_DIRS}"
            RUN_OUTPUT_VARIABLE OUTPUT)

        if(NOT ${RET_RUN} MATCHES "0" OR NOT ${RET_COM} MATCHES "TRUE")
            message(FATAL_ERROR "Could not compile and run libusbx version test")
        endif()

        if(${OUTPUT} MATCHES "1")
            message(STATUS "Determined libusbx contains hotplug support via ver.c test.")
            set(LIBUSB_HAVE_HOTPLUG 1)
        else(${OUTPUT} MATCHES "1")
            message(WARNING "Your libusb version does not contain hotplug support. This functionality will be disabled. To update, see the instructions at:\n"
                    "https://github.com/Nuand/bladeRF/wiki/Getting-Started%3A-Windows#installing-libusbx")
        endif()
    else(WIN32)
        message(WARNING "Your libusb version does not contain hotplug support. This functionality will be disabled. To update, see the instructions at:\n"
               "https://github.com/Nuand/bladeRF/wiki/Debian-setup-guide#build-libusb-tools-and-libraries-from-nuand-git-master")
    endif()
endif(NOT LIBUSB_HAVE_HOTPLUG)

if(LIBUSB_HAVE_HOTPLUG)
    add_subdirectory(bladeRF-flash)
endif()
