file(GLOB LOCAL_SOURCE
	"source/*.*"
	"source/Entities/*.*"
	"source/GameplayManagers/*.*"
	"source/GUI/*.*"
	"source/Managers/*.*"
	"source/Network/*.*"
	"source/States/*.*"
	"source/World/*.*"
)

set(CLIENT_SOURCES
	${CLIENT_SOURCES}
	${LOCAL_SOURCE}
	PARENT_SCOPE
	)