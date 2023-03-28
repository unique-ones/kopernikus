# BEWARE USER!
#
#
#                ___-----------___
#                __--~~                 ~~--__
#            _-~~                             ~~-_
#         _-~                                     ~-_
#        /                                           \
#       |                                             |
#      |                                               |
#      |                                               |
#     |                                                 |
#     |                                                 |
#     |                                                 |
#      |                                               |
#      |  |    _-------_               _-------_    |  |
#      |  |  /~         ~\           /~         ~\  |  |
#       ||  |             |         |             |  ||
#       || |               |       |               | ||
#       || |              |         |              | ||
#       |   \_           /           \           _/   |
#      |      ~~--_____-~    /~V~\    ~-_____--~~      |
#      |                    |     |                    |
#     |                    |       |                    |
#     |                    |  /^\  |                    |
#      |                    ~~   ~~                    |
#       \_         _                       _         _/
#         ~--____-~ ~\                   /~ ~-____--~
#              \     /\                 /\     /
#               \    | ( ,           , ) |    /
#                |   | (~(__(  |  )__)~) |   |
#                 |   \/ (  (~~|~~)  ) \/   |
#                  |   |  [ [  |  ] ]  /   |
#                   |                     |
#                    \                   /
#                     ~-_             _-~
#                        ~--___-___--~
#     
#     
#

# imgui configuration
add_library(ImGui
        "imgui/imgui.cpp"
        "imgui/imgui.h"
        "imgui/imconfig.h"
        "imgui/imgui_demo.cpp"
        "imgui/imgui_draw.cpp"
        "imgui/imgui_impl_glfw.cpp"
        "imgui/imgui_impl_glfw.h"
        "imgui/imgui_impl_opengl3.cpp"
        "imgui/imgui_impl_opengl3.h"
        "imgui/imgui_impl_opengl3_loader.h"
        "imgui/imgui_internal.h"
        "imgui/imgui_tables.cpp"
        "imgui/imgui_widgets.cpp"
        "imgui/imstb_rectpack.h"
        "imgui/imstb_textedit.h"
        "imgui/imstb_truetype.h")

# implot configuration
add_library(ImPlot
        "implot/implot.cpp"
        "implot/implot.h"
        "implot/implot_demo.cpp"
        "implot/implot_internal.h"
        "implot/implot_items.cpp")

# glad configuration
add_library(glad
        "glad/glad.c"
        "glad/glad.h"
        "glad/khrplatform.h")

add_library(imnodes
		"imnodes/imnodes.h"
		"imnodes/imnodes.cpp"
		"imnodes/imnodes_internal.h")

# include dirs
target_include_directories(ImGui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(ImPlot PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/imgui)
target_include_directories(imnodes PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/imgui)
target_include_directories(glad PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# ImGui needs to link with glfw
target_link_libraries(ImGui PRIVATE glfw)

# ImNodes needs to link with ImGui
target_link_libraries(imnodes PRIVATE ImGui)