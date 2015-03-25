set ( SOURCES 
	"src/receiver.cpp"
	"src/interface.cpp"
)

set (HEADERS 
	"include/socket_data_receiver/receiver.h"
)

include_directories("include")

add_library ( socket_data_receiver MODULE ${SOURCES} ${HEADERS})
target_link_libraries(socket_data_receiver PRIVATE lms)
