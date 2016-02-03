cmake_minimum_required(VERSION 2.6)
project(sensor-hal CXX)

INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(plugin_pkgs REQUIRED dlog libxml-2.0)

FOREACH(flag ${plugin_pkgs_LDFLAGS})
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${flag}")
ENDFOREACH(flag)

FOREACH(flag ${plugin_pkgs_CFLAGS})
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${flag}")
ENDFOREACH(flag)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

include_directories(${CMAKE_SOURCE_DIR}/src/lib)
include_directories(${CMAKE_SOURCE_DIR}/src/interface)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

IF("${ACCEL}" STREQUAL "ON")
include_directories(${CMAKE_SOURCE_DIR}/src/plugins/accel)
list (APPEND SRCS "plugins/accel/accel_sensor_hal.cpp")
add_definitions(-DENABLE_ACCEL)
ENDIF()
IF("${GYRO}" STREQUAL "ON")
include_directories(${CMAKE_SOURCE_DIR}/src/plugins/gyro)
list (APPEND SRCS "plugins/gyro/gyro_sensor_hal.cpp")
add_definitions(-DENABLE_GYRO)
ENDIF()
IF("${PROXI}" STREQUAL "ON")
include_directories(${CMAKE_SOURCE_DIR}/src/plugins/proxi)
list (APPEND SRCS "plugins/proxi/proxi_sensor_hal.cpp")
add_definitions(-DENABLE_PROXI)
ENDIF()
IF("${LIGHT}" STREQUAL "ON")
include_directories(${CMAKE_SOURCE_DIR}/src/plugins/light)
list (APPEND SRCS "plugins/light/light_sensor_hal.cpp")
add_definitions(-DENABLE_LIGHT)
ENDIF()
IF("${GEO}" STREQUAL "ON")
include_directories(${CMAKE_SOURCE_DIR}/src/plugins/geo)
list (APPEND SRCS "plugins/geo/geo_sensor_hal.cpp")
add_definitions(-DENABLE_GEO)
ENDIF()
IF("${PRESSURE}" STREQUAL "ON")
include_directories(${CMAKE_SOURCE_DIR}/src/plugins/pressure)
list (APPEND SRCS "plugins/pressure/pressure_sensor_hal.cpp")
add_definitions(-DENABLE_PRESSURE)
ENDIF()
IF("${TEMPERATURE}" STREQUAL "ON")
include_directories(${CMAKE_SOURCE_DIR}/src/plugins/temperature)
list (APPEND SRCS "plugins/temperature/temperature_sensor_hal.cpp")
add_definitions(-DENABLE_TEMPERATURE)
ENDIF()
IF("${ULTRAVIOLET}" STREQUAL "ON")
include_directories(${CMAKE_SOURCE_DIR}/src/plugins/ultraviolet)
list (APPEND SRCS "plugins/ultraviolet/ultraviolet_sensor_hal.cpp")
add_definitions(-DENABLE_ULTRAVIOLET)
ENDIF()
IF("${BIO_LED_RED}" STREQUAL "ON")
include_directories(${CMAKE_SOURCE_DIR}/src/plugins/bio_led_red)
list (APPEND SRCS "plugins/bio_led_red/bio_led_red_sensor_hal.cpp")
add_definitions(-DENABLE_BIO_LED_RED)
ENDIF()
IF("${RV}" STREQUAL "ON")
include_directories(${CMAKE_SOURCE_DIR}/src/plugins/rotation_vector/rv_raw)
list (APPEND SRCS "plugins/rotation_vector/rv_raw/rv_raw_sensor_hal.cpp")
add_definitions(-DENABLE_RV_RAW)
ENDIF()

configure_file(hal_module_create.cpp.in hal_module_create.cpp)

add_library(${PROJECT_NAME} SHARED
	${SRCS}
	lib/cbase_lock.cpp
	lib/cconfig.cpp
	lib/cmutex.cpp
	lib/csensor_config.cpp
	lib/sensor_logs.cpp
	interface/sensor_hal_base.cpp
	interface/sensor_hal.h
	hal_module_create.cpp
)

target_link_libraries(${PROJECT_NAME} ${plugin_pkgs_LDFLAGS} "-lrt -ldl -pthread")

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_LIBDIR}/sensor-hal)
