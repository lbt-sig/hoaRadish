function(cat IN_FILE OUT_FILE)
    file(READ ${IN_FILE} CONTENTS)
    file(APPEND ${OUT_FILE} "${CONTENTS}")    
endfunction(cat IN_FILE OUT_FILE)

set(CONTRIB_TMP_FILE "${CMAKE_BINARY_DIR}/deps_tmp_CMakeLists.txt.in")
configure_file("${HOA_SUPPORT_DIR}/deps_CMakeLists.txt.in" ${CONTRIB_TMP_FILE})

cat("${HOA_SUPPORT_DIR}/opencv_CMakeLists.txt.in" ${CONTRIB_TMP_FILE})

configure_file(${CONTRIB_TMP_FILE} "${HOA_CONTRIB_DIR}/deps-download/CMakeLists.txt")
execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" . 
                WORKING_DIRECTORY "${HOA_CONTRIB_DIR}/deps-download")
execute_process(COMMAND "${CMAKE_COMMAND}" --build .
                WORKING_DIRECTORY "${HOA_CONTRIB_DIR}/deps-download") 
