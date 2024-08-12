if (NOT EXISTS "/home/me/repos/HKD/cmake-build-debug/install_manifest.txt")
    message(FATAL_ERROR "Cannot find install manifest: \"/home/me/repos/HKD/cmake-build-debug/install_manifest.txt\"")
endif(NOT EXISTS "/home/me/repos/HKD/cmake-build-debug/install_manifest.txt")

file(READ "/home/me/repos/HKD/cmake-build-debug/install_manifest.txt" files)
string(REGEX REPLACE "\n" ";" files "${files}")
foreach (file ${files})
    message(STATUS "Uninstalling \"$ENV{DESTDIR}${file}\"")
    execute_process(
        COMMAND /opt/clion-2024.1.4/bin/cmake/linux/x64/bin/cmake -E remove "$ENV{DESTDIR}${file}"
        OUTPUT_VARIABLE rm_out
        RESULT_VARIABLE rm_retval
    )
    if(NOT ${rm_retval} EQUAL 0)
        message(FATAL_ERROR "Problem when removing \"$ENV{DESTDIR}${file}\"")
    endif (NOT ${rm_retval} EQUAL 0)
endforeach(file)

