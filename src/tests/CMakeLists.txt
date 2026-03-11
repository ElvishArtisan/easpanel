qt_add_executable(test_alert_manifest
	test_alert_manifest.cpp
	alert.cpp
	cmdswitch.cpp
)

target_link_libraries(test_alert_manifest PRIVATE
	Qt6::Network
)
