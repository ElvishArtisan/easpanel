qt_add_executable(easp
	alertbutton.cpp
	alerttextedit.cpp
	easp.cpp
	modebutton.cpp
	alert.cpp
	cmdswitch.cpp
	config.cpp
	profile.cpp
)

target_link_libraries(easp PRIVATE
	Qt6::Gui
	Qt6::Network
	Qt6::Widgets
	rivwebcapi
)

set_target_properties(easp PROPERTIES
	WIN32_EXECUTABLE ON
	MACOSX_BUNDLE ON
)
