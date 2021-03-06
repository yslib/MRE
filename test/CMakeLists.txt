
#project(vmTest)

# Qt Gui Binding Test

if(VM_BUILD_QT5_EXAMPLE OR VM_BUILD_GLFW_EXAMPLE)

add_subdirectory("vmapps")

## example based on qt5 gui
if(VM_BUILD_QT5_EXAMPLE)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
find_package(Qt5 REQUIRED COMPONENTS Widgets Core Gui OpenGL)

add_executable(vmqtwindow_test)
target_sources(vmqtwindow_test PRIVATE "vmqtwindow_test.cpp")
target_link_libraries(vmqtwindow_test vmqtwindow Qt5::Core vm vmapps)
target_include_directories(vmqtwindow_test PRIVATE ${PROJECT_SOURCE_DIR}/guibinding/qt5widget ${PROJECT_SOURCE_DIR}/test/vmapps)

install(TARGETS vmqtwindow_test LIBRARY DESTINATION "lib" RUNTIME DESTINATION "bin" ARCHIVE DESTINATION "lib")

endif()		## if(VM_BUILD_QT5_EXAMPLE)

## example base on glfw Gui

if(VM_BUILD_GLFW_EXAMPLE)
## normal
add_executable(vmglfwwindow_test)
target_sources(vmglfwwindow_test PRIVATE "vmglfwwindow_test.cpp")
target_link_libraries(vmglfwwindow_test vmglfwwindow vm vmapps)
target_include_directories(vmglfwwindow_test PRIVATE ${PROJECT_SOURCE_DIR}/guibinding/glfwwindow ${PROJECT_SOURCE_DIR}/src/visualman ${PROJECT_SOURCE_DIR}/test/vmapps)

## VM Large Volume Renderer Test
add_executable(vm_largevolumerenderer_test)
target_sources(vm_largevolumerenderer_test PRIVATE "vmlargevolumeraycast_test.cpp")
target_link_libraries(vm_largevolumerenderer_test vmglfwwindow vm vmapps)
target_include_directories(vm_largevolumerenderer_test PRIVATE ${PROJECT_SOURCE_DIR}/guibinding/glfwwindow ${PROJECT_SOURCE_DIR}/src/visualman ${PROJECT_SOURCE_DIR}/test/vmapps)

if (MSVC)
  SET_TARGET_PROPERTIES(vm_largevolumerenderer_test PROPERTIES LINK_FLAGS "/PROFILE")
endif()
install(TARGETS vmglfwwindow_test vm_largevolumerenderer_test LIBRARY DESTINATION "lib" RUNTIME DESTINATION "bin" ARCHIVE DESTINATION "lib")

endif() ## if(VM_BUILD_GLFW_EXAMPLE)

endif()##if(VM_BUILD_QT5_EXAMPLE OR VM_BUILD_GLFW_EXAMPLE)

add_executable(vm_feature_test)
target_sources(vm_feature_test PRIVATE "vm_feature_test.cpp")
target_link_libraries(vm_feature_test vmcore)
target_include_directories(vm_feature_test PRIVATE "../VMCore/interface" "../VMCore/include/VMUtils/include")
install(TARGETS vm_feature_test LIBRARY DESTINATION "lib" RUNTIME DESTINATION "bin" ARCHIVE DESTINATION "lib")


# Large Volume Renderer Test


