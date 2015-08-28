set ( SOURCES
	"src/sender.cpp"
	"src/interface.cpp"
)

set (HEADERS
	"include/socket_data_sender/sender.h"
)

include_directories("include")

add_library ( socket_data_sender MODULE ${SOURCES} ${HEADERS})
target_link_libraries(socket_data_sender PRIVATE lmscore socket_connection)
