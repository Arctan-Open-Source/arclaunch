
XSD_SCHEMA( LAUNCH_SRC xsd/launch.xsd --std c++11 --generate-polymorphic )

add_library(arclaunch
  ${LAUNCH_SRC}
  Node.cxx
  )

target_link_libraries(arclaunch 
  ${XSD_LIBRARIES}
  )

add_executable(launch
  main.cxx
  )

target_link_libraries(launch arclaunch)

