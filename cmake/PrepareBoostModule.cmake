include(FetchContent)

function(PrepareBoostModule MODULE_NAME VERSION)

    # Обработка значения опционального параметра
    if(${ARGC} GREATER 2)
        set(IS_DEP ${ARGV2})
    else()
        set(IS_DEP FALSE)
    endif()

    # Если загружаемый модуль является зависимостью, то добавляем пометку об этом
    if(IS_DEP)
        set(IS_DEP_INFO " (зависимость)")
    else()
        set(IS_DEP_INFO "")
    endif()

    set(TARGET_NAME "Boost::${MODULE_NAME}")

    # Проверяем существует ли таргет, чтобы не загружать повторно
    if(TARGET ${TARGET_NAME})
        return()
    endif()

    # Загружаем исходный код модуля
    message(STATUS "Загрузка 'boostorg/${MODULE_NAME}.git' версии ${VERSION}${IS_DEP_INFO}")
    FetchContent_Declare("boost_${MODULE_NAME}"
        GIT_REPOSITORY "https://github.com/boostorg/${MODULE_NAME}.git"
        GIT_TAG "boost-${VERSION}"
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
        EXCLUDE_FROM_ALL
    )
    FetchContent_MakeAvailable("boost_${MODULE_NAME}")

    # Проверяем, что найден CMakeLists.txt модуля
    set(MODULE_CMAKE_CONFIG "${boost_${MODULE_NAME}_SOURCE_DIR}/CMakeLists.txt")
    if(NOT EXISTS ${MODULE_CMAKE_CONFIG})
        message(FATAL_ERROR "Не удалось обнаружить CMakeLists.txt boost модуля.")
        return() # Не уверен что он здесь нужен, FATAL_ERROR вроде сам прерывает
    endif()

    # Читам CMakeLists.txt модуля
    file(READ ${MODULE_CMAKE_CONFIG} MODULE_CMAKE_CONFIG_CONTENT)

    # Ищем блоки target_link_libraries
    string(REGEX MATCHALL
        "target_link_libraries\\([^)]*\\)"
        LINK_LIBS_BLOCKS
        "${MODULE_CMAKE_CONFIG_CONTENT}"
    )

    # Обрабатываем каждый блок
    foreach(BLOCK IN LISTS LINK_LIBS_BLOCKS)
        # Извлекаем названия таргетов вида `Boost::*`
        string(REGEX MATCHALL
            "Boost::[A-Za-z0-9_]+"
            MODULE_DEPS_TARGETS
            "${BLOCK}"
        )

        # Обрабатываем каждый полученный таргет
        foreach(MODULE_TARGET IN LISTS MODULE_DEPS_TARGETS)
            # Убираем приставку `Boost::`
            string(REGEX REPLACE "Boost::" "" MODULE_NAME "${MODULE_TARGET}")
            # Добавляем в общий список
            list(APPEND MODULE_DEPS "${MODULE_NAME}")
        endforeach()
    endforeach()

    # Убираем возможные дубликаты (на всякий случай)
    list(REMOVE_DUPLICATES MODULE_DEPS)

    # Подготавливаем модули-зависимости
    foreach(DEP IN LISTS MODULE_DEPS)
        PrepareBoostModule(${DEP} ${VERSION} TRUE)
    endforeach()

endfunction()