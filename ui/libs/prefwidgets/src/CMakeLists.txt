add_library(workrave-libs-ui-prefwidgets STATIC)

target_sources(workrave-libs-ui-prefwidgets PRIVATE
    Box.cc
    Choice.cc
    Frame.cc
    Time.cc
    Toggle.cc
    Value.cc
    Widget.cc
    SizeGroup.cc
    )

target_link_libraries(workrave-libs-ui-prefwidgets
  PUBLIC
  workrave-libs-config
  workrave-libs-utils)

target_include_directories(
    workrave-libs-ui-prefwidgets
    INTERFACE
    ${CMAKE_SOURCE_DIR}/ui/libs/prefwidgets/include
    PRIVATE
    ${CMAKE_SOURCE_DIR}/ui/libs/prefwidgets/include/ui/prefwidgets
)
