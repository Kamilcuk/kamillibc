cmake_minimum_required(VERSION 3.11)
project(gnulib LANGUAGES C)

if(BUILD_TESTING AND NOT GNULIB_MODULES)
	set(GNULIB_MODULES
		eealloc
		xnanosleep
		crc
	)
endif()

if(NOT GNULIB_MODULES)
	return()
endif()

set(gnulibdir "${CMAKE_CURRENT_BINARY_DIR}/gnulibrepo-prefix/src/gnulibrepo")
set(builddir "${gnulibdir}/_build")
set(libgnua "${builddir}/gllib/libgnu.a")
set(gnulib_headers "${CMAKE_CURRENT_BINARY_DIR}/gnulib-headers")
file(MAKE_DIRECTORY ${gnulib_headers})

include(ExternalProject)
ExternalProject_Add(gnulibrepo
	GIT_REPOSITORY "https://git.savannah.gnu.org/git/gnulib.git"
    UPDATE_COMMAND ""
    BUILD_IN_SOURCE 1
    CONFIGURE_COMMAND
		rm -rf _build &&
		yes | ./gnulib-tool --create-testdir --dir=_build ${GNULIB_MODULES} &&
		cd _build && yes | ./configure 
			"CC=${CMAKE_C_COMPILER} ${CMAKE_C_FLAGS}"
			"CXX=${CMAKE_CXX_COMPILER} ${CMAKE_CXX_FLAGS}"
			"--prefix=${gnulibdir}/install"
	BUILD_COMMAND make -C _build -j8
	BUILD_BYPRODUCTS "${libgnua}"
	INSTALL_COMMAND
		cp ${builddir}/config.h ${gnulib_headers} &&
		cd ${builddir}/gllib && find . -maxdepth 1 -type f -name "*.h" -print0 | xargs -0 -i{} cp {} ${gnulib_headers}
)

add_library(gnulib STATIC IMPORTED GLOBAL)
add_dependencies(gnulib gnulibrepo)
target_include_directories(gnulib INTERFACE
	${gnulib_headers}
)
set_property(TARGET gnulib
	PROPERTY IMPORTED_LOCATION "${libgnua}"
)
# message(STATUS "${libgnua}")

include(CTest)
if(BUILD_TESTING)
	file(GLOB tests tests/*.c)
	foreach(file IN LISTS tests)
		get_filename_component(name ${file} NAME_WE)
		add_executable(${name} ${file})
		target_link_libraries(${name} PUBLIC gnulib)
		add_test(NAME ${name} COMMAND ${name})
	endforeach()
endif()

