add_contract(sendinline sendinline ${CMAKE_CURRENT_SOURCE_DIR}/src/sendinline.cpp)
target_compile_definitions(sendinline PUBLIC IS_TESTNET=${IS_TESTNET})

target_include_directories(sendinline PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
                                             "$<TARGET_PROPERTY:eosio.system,INTERFACE_INCLUDE_DIRECTORIES>")

set_target_properties(sendinline PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")