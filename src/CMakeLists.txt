SET(SOURCES screens/ui_Screen1.c
    screens/ui_Screen2.c
    ui.c
    components/ui_comp_hook.c
    ui_helpers.c
    images/ui_img_shower_png.c
    images/ui_img_765177840.c
    fonts/ui_font_Font1.c
    fonts/ui_font_Font16.c
    fonts/ui_font_Font24.c
    fonts/ui_font_Font36.c)

add_library(ui ${SOURCES})
