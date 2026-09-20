/*******************************************************************************
 * Size: 16 px
 * Bpp: 1
 * Opts: --bpp 1 --size 16 --font C:/Users/LukaszJ/SquareLine/assets/digit.ttf -o C:/Users/LukaszJ/SquareLine/assets\ui_font_Font16.c --format lvgl -r 0x20-0x7f --no-compress --no-prefilter
 ******************************************************************************/

#include "../ui.h"

#ifndef UI_FONT_FONT16
#define UI_FONT_FONT16 1
#endif

#if UI_FONT_FONT16

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0020 " " */
    0x0,

    /* U+0022 "\"" */
    0x8c, 0x40,

    /* U+0024 "$" */
    0x3e, 0x93, 0x66, 0xcd, 0x12, 0x1f, 0xb, 0x36,
    0x48, 0x91, 0x2f, 0x80,

    /* U+002A "*" */
    0x20, 0x90, 0xcc, 0xc6, 0x41, 0x60, 0xa3, 0xdc,
    0x38, 0x3c, 0x32, 0x11, 0x98, 0x48, 0x20,

    /* U+002B "+" */
    0x10, 0x84, 0x62, 0x7c, 0x84, 0x63, 0x0,

    /* U+002C "," */
    0x58,

    /* U+002D "-" */
    0xf8,

    /* U+002E "." */
    0x80,

    /* U+0030 "0" */
    0x1f, 0x10, 0xd8, 0x6c, 0x26, 0x12, 0x8, 0x0,
    0x82, 0xc3, 0x61, 0x30, 0x90, 0x47, 0xc0,

    /* U+0031 "1" */
    0x6d, 0x24, 0x16, 0x92, 0x0,

    /* U+0032 "2" */
    0x1f, 0x0, 0xc0, 0x60, 0x20, 0x10, 0x8, 0xf8,
    0x80, 0xc0, 0x60, 0x30, 0x10, 0x7, 0xc0,

    /* U+0033 "3" */
    0x3e, 0x3, 0x3, 0x2, 0x2, 0x2, 0x7c, 0x6,
    0x6, 0x4, 0x4, 0x4, 0xf8,

    /* U+0034 "4" */
    0x43, 0x43, 0xc2, 0xc2, 0x82, 0x7c, 0x6, 0x6,
    0x4, 0x4, 0x4,

    /* U+0035 "5" */
    0x3e, 0x83, 0x6, 0xc, 0x10, 0x1f, 0x3, 0x6,
    0x8, 0x10, 0x2f, 0x80,

    /* U+0036 "6" */
    0x1f, 0x20, 0x60, 0x60, 0x60, 0x40, 0x3e, 0x43,
    0xc3, 0xc2, 0xc2, 0x82, 0x7c,

    /* U+0037 "7" */
    0xf8, 0x30, 0xc2, 0x8, 0x20, 0x6, 0x18, 0x41,
    0x4,

    /* U+0038 "8" */
    0x1f, 0x10, 0xd8, 0x6c, 0x26, 0x12, 0x8, 0xf8,
    0x86, 0xc3, 0x61, 0x30, 0x90, 0x47, 0xc0,

    /* U+0039 "9" */
    0x3e, 0x43, 0xc3, 0xc2, 0xc2, 0x82, 0x7c, 0x6,
    0x6, 0x4, 0x4, 0x4, 0xf8,

    /* U+003A ":" */
    0x42,

    /* U+003B ";" */
    0x21, 0x40,

    /* U+0041 "A" */
    0x1f, 0x10, 0xd8, 0x6c, 0x26, 0x12, 0x8, 0xf8,
    0x86, 0xc3, 0x61, 0x30, 0x90, 0x40,

    /* U+0042 "B" */
    0x3e, 0x9, 0x9, 0xb, 0x1b, 0x1a, 0xe, 0x12,
    0x12, 0x16, 0x36, 0x34, 0xf8,

    /* U+0043 "C" */
    0x1f, 0x20, 0x60, 0x60, 0x60, 0x40, 0x0, 0x40,
    0xc0, 0xc0, 0xc0, 0x80, 0x7c,

    /* U+0044 "D" */
    0x3e, 0x9, 0x9, 0xb, 0x1b, 0x12, 0x0, 0x12,
    0x12, 0x16, 0x36, 0x34, 0xf8,

    /* U+0045 "E" */
    0x1f, 0x20, 0x60, 0x60, 0x60, 0x40, 0x3e, 0x40,
    0xc0, 0xc0, 0xc0, 0x80, 0x7c,

    /* U+0046 "F" */
    0x1f, 0x20, 0x60, 0x60, 0x60, 0x40, 0x3e, 0x40,
    0xc0, 0xc0, 0xc0, 0x80,

    /* U+0047 "G" */
    0x1f, 0x20, 0x60, 0x60, 0x60, 0x40, 0xe, 0x43,
    0xc3, 0xc2, 0xc2, 0x82, 0x7c,

    /* U+0048 "H" */
    0x21, 0x90, 0xd8, 0x4c, 0x24, 0x11, 0xf1, 0xd,
    0x86, 0xc2, 0x61, 0x20, 0x80,

    /* U+0049 "I" */
    0x2d, 0xa4, 0x16, 0x92, 0x0,

    /* U+004A "J" */
    0x0, 0x80, 0xc0, 0x40, 0x20, 0x10, 0x1, 0x5,
    0x86, 0xc2, 0x61, 0x20, 0x8f, 0x80,

    /* U+004B "K" */
    0x0, 0x90, 0xc8, 0x4c, 0x46, 0x62, 0x20, 0xc0,
    0x98, 0xcc, 0x62, 0x31, 0x90, 0x40, 0x20,

    /* U+004C "L" */
    0x20, 0x86, 0x18, 0x40, 0x4, 0x30, 0xc3, 0x8,
    0x1f,

    /* U+004D "M" */
    0x1f, 0x12, 0xdb, 0x6d, 0x26, 0x92, 0x48, 0x0,
    0x86, 0xc3, 0x61, 0x20, 0x90, 0x40,

    /* U+004E "N" */
    0x1f, 0x10, 0xd8, 0x6c, 0x26, 0x12, 0x8, 0x0,
    0x86, 0xc3, 0x61, 0x20, 0x90, 0x40,

    /* U+004F "O" */
    0x1f, 0x10, 0xd8, 0x6c, 0x26, 0x12, 0x8, 0x0,
    0x82, 0xc3, 0x61, 0x30, 0x90, 0x47, 0xc0,

    /* U+0050 "P" */
    0x1f, 0x10, 0xd8, 0x6c, 0x26, 0x12, 0x8, 0xf8,
    0x80, 0xc0, 0x60, 0x30, 0x10, 0x0,

    /* U+0051 "Q" */
    0x1f, 0x10, 0xd8, 0x6c, 0x26, 0x12, 0x8, 0x0,
    0x82, 0xcb, 0x63, 0x32, 0x90, 0x47, 0xc0,

    /* U+0052 "R" */
    0x1f, 0x10, 0xd8, 0x6c, 0x26, 0x12, 0x8, 0xf8,
    0x90, 0xd8, 0x68, 0x34, 0x12, 0x0,

    /* U+0053 "S" */
    0x3e, 0x83, 0x6, 0xc, 0x10, 0x1f, 0x3, 0x6,
    0x8, 0x10, 0x2f, 0x80,

    /* U+0054 "T" */
    0xf9, 0x18, 0x84, 0x20, 0x18, 0xc4, 0x21, 0x0,

    /* U+0055 "U" */
    0x20, 0x90, 0xd8, 0x4c, 0x24, 0x10, 0x1, 0x5,
    0x86, 0xc2, 0x61, 0x20, 0x8f, 0x80,

    /* U+0056 "V" */
    0x20, 0x90, 0xd8, 0x4c, 0x24, 0x10, 0x19, 0x18,
    0x98, 0xd8, 0x78, 0x28, 0x10, 0x0,

    /* U+0057 "W" */
    0x20, 0x90, 0xd8, 0x4c, 0x24, 0x10, 0x1, 0x25,
    0xb6, 0xda, 0x69, 0x24, 0x8f, 0x80,

    /* U+0058 "X" */
    0x0, 0x90, 0xc8, 0x46, 0x41, 0x60, 0xa0, 0x0,
    0x30, 0x3c, 0x12, 0x19, 0x98, 0xc8, 0x20,

    /* U+0059 "Y" */
    0x43, 0x43, 0xc2, 0xc2, 0x82, 0x7c, 0x6, 0x6,
    0x4, 0x4, 0x4, 0xf8,

    /* U+005A "Z" */
    0x1f, 0x1, 0x3, 0x2, 0x6, 0xc, 0x0, 0x30,
    0x20, 0x60, 0xc0, 0x80, 0xfc,

    /* U+0061 "a" */
    0x3e, 0x3, 0x3, 0x7e, 0xc2, 0x82, 0x7c,

    /* U+0062 "b" */
    0x20, 0x20, 0x60, 0x60, 0x40, 0x3e, 0x43, 0xc3,
    0xc2, 0xc2, 0x82, 0x7c,

    /* U+0063 "c" */
    0x3e, 0x83, 0x6, 0xc, 0x10, 0x1f, 0x0,

    /* U+0064 "d" */
    0x1, 0x80, 0xc0, 0x40, 0x20, 0x11, 0xf1, 0xd,
    0x86, 0xc2, 0x61, 0x20, 0x8f, 0x80,

    /* U+0065 "e" */
    0x3e, 0x43, 0xc3, 0x7e, 0xc0, 0x80, 0x7c,

    /* U+0066 "f" */
    0x3e, 0x83, 0x3, 0xf8, 0x10, 0x0,

    /* U+0067 "g" */
    0x3e, 0x43, 0xc3, 0x7e, 0x2, 0x2, 0x7c,

    /* U+0068 "h" */
    0x20, 0x20, 0x60, 0x60, 0x40, 0x3e, 0x43, 0xc3,
    0xc2, 0xc2, 0x82,

    /* U+0069 "i" */
    0x72, 0x80,

    /* U+006A "j" */
    0x1, 0x3, 0x2, 0x82, 0x82, 0x7c,

    /* U+006B "k" */
    0x0, 0x20, 0x20, 0x20, 0x20, 0x60, 0x0, 0x42,
    0x44, 0x70, 0xc8, 0x84, 0x0,

    /* U+006C "l" */
    0x22, 0x66, 0x40, 0x4c, 0xcc, 0x86,

    /* U+006D "m" */
    0x3e, 0x4b, 0xdb, 0xd2, 0xd2, 0x92,

    /* U+006E "n" */
    0x3e, 0x43, 0xc3, 0xc2, 0xc2, 0x82,

    /* U+006F "o" */
    0x3e, 0x43, 0xc3, 0xc2, 0xc2, 0x82, 0x7c,

    /* U+0070 "p" */
    0x3e, 0x43, 0xc3, 0x7e, 0x80, 0x80,

    /* U+0071 "q" */
    0x3e, 0x43, 0xc3, 0x7e, 0x2, 0x2,

    /* U+0072 "r" */
    0x34, 0xcc, 0xc8,

    /* U+0073 "s" */
    0x3e, 0x83, 0x3, 0xf0, 0x20, 0x5f, 0x0,

    /* U+0074 "t" */
    0x20, 0x86, 0x18, 0x40, 0xc4, 0x30, 0xc3, 0x8,
    0x1f,

    /* U+0075 "u" */
    0x41, 0xc3, 0xc2, 0xc2, 0x82, 0x7c,

    /* U+0076 "v" */
    0x42, 0x85, 0x8, 0x69, 0x9c, 0x0,

    /* U+0077 "w" */
    0x49, 0x5b, 0x2, 0x92, 0x92, 0x7c,

    /* U+0078 "x" */
    0x22, 0x24, 0x18, 0x18, 0x2c, 0x44,

    /* U+0079 "y" */
    0x43, 0xc3, 0x7e, 0x2, 0x2, 0x7c,

    /* U+007A "z" */
    0x3e, 0x4, 0x38, 0xc6, 0x18, 0x1f, 0x0
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 75, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1, .adv_w = 138, .box_w = 5, .box_h = 2, .ofs_x = 2, .ofs_y = 13},
    {.bitmap_index = 3, .adv_w = 138, .box_w = 7, .box_h = 13, .ofs_x = 3, .ofs_y = 1},
    {.bitmap_index = 15, .adv_w = 138, .box_w = 9, .box_h = 13, .ofs_x = 2, .ofs_y = 1},
    {.bitmap_index = 30, .adv_w = 138, .box_w = 5, .box_h = 11, .ofs_x = 2, .ofs_y = 2},
    {.bitmap_index = 37, .adv_w = 63, .box_w = 2, .box_h = 3, .ofs_x = 2, .ofs_y = 1},
    {.bitmap_index = 38, .adv_w = 113, .box_w = 5, .box_h = 1, .ofs_x = 2, .ofs_y = 7},
    {.bitmap_index = 39, .adv_w = 63, .box_w = 1, .box_h = 1, .ofs_x = 2, .ofs_y = 1},
    {.bitmap_index = 40, .adv_w = 138, .box_w = 9, .box_h = 13, .ofs_x = 2, .ofs_y = 1},
    {.bitmap_index = 55, .adv_w = 138, .box_w = 3, .box_h = 12, .ofs_x = 8, .ofs_y = 1},
    {.bitmap_index = 60, .adv_w = 138, .box_w = 9, .box_h = 13, .ofs_x = 2, .ofs_y = 1},
    {.bitmap_index = 75, .adv_w = 138, .box_w = 8, .box_h = 13, .ofs_x = 3, .ofs_y = 1},
    {.bitmap_index = 88, .adv_w = 138, .box_w = 8, .box_h = 11, .ofs_x = 3, .ofs_y = 2},
    {.bitmap_index = 99, .adv_w = 138, .box_w = 7, .box_h = 13, .ofs_x = 3, .ofs_y = 1},
    {.bitmap_index = 111, .adv_w = 138, .box_w = 8, .box_h = 13, .ofs_x = 2, .ofs_y = 1},
    {.bitmap_index = 124, .adv_w = 138, .box_w = 6, .box_h = 12, .ofs_x = 5, .ofs_y = 2},
    {.bitmap_index = 133, .adv_w = 138, .box_w = 9, .box_h = 13, .ofs_x = 2, .ofs_y = 1},
    {.bitmap_index = 148, .adv_w = 138, .box_w = 8, .box_h = 13, .ofs_x = 3, .ofs_y = 1},
    {.bitmap_index = 161, .adv_w = 63, .box_w = 2, .box_h = 4, .ofs_x = 2, .ofs_y = 5},
    {.bitmap_index = 162, .adv_w = 75, .box_w = 3, .box_h = 4, .ofs_x = 2, .ofs_y = 1},
    {.bitmap_index = 164, .adv_w = 138, .box_w = 9, .box_h = 12, .ofs_x = 2, .ofs_y = 2},
    {.bitmap_index = 178, .adv_w = 138, .box_w = 8, .box_h = 13, .ofs_x = 2, .ofs_y = 1},
    {.bitmap_index = 191, .adv_w = 138, .box_w = 8, .box_h = 13, .ofs_x = 2, .ofs_y = 1},
    {.bitmap_index = 204, .adv_w = 138, .box_w = 8, .box_h = 13, .ofs_x = 2, .ofs_y = 1},
    {.bitmap_index = 217, .adv_w = 138, .box_w = 8, .box_h = 13, .ofs_x = 2, .ofs_y = 1},
    {.bitmap_index = 230, .adv_w = 138, .box_w = 8, .box_h = 12, .ofs_x = 2, .ofs_y = 2},
    {.bitmap_index = 242, .adv_w = 138, .box_w = 8, .box_h = 13, .ofs_x = 2, .ofs_y = 1},
    {.bitmap_index = 255, .adv_w = 138, .box_w = 9, .box_h = 11, .ofs_x = 2, .ofs_y = 2},
    {.bitmap_index = 268, .adv_w = 63, .box_w = 3, .box_h = 11, .ofs_x = 2, .ofs_y = 2},
    {.bitmap_index = 273, .adv_w = 138, .box_w = 9, .box_h = 12, .ofs_x = 2, .ofs_y = 1},
    {.bitmap_index = 287, .adv_w = 138, .box_w = 9, .box_h = 13, .ofs_x = 2, .ofs_y = 1},
    {.bitmap_index = 302, .adv_w = 138, .box_w = 6, .box_h = 12, .ofs_x = 2, .ofs_y = 1},
    {.bitmap_index = 311, .adv_w = 138, .box_w = 9, .box_h = 12, .ofs_x = 2, .ofs_y = 2},
    {.bitmap_index = 325, .adv_w = 138, .box_w = 9, .box_h = 12, .ofs_x = 2, .ofs_y = 2},
    {.bitmap_index = 339, .adv_w = 138, .box_w = 9, .box_h = 13, .ofs_x = 2, .ofs_y = 1},
    {.bitmap_index = 354, .adv_w = 138, .box_w = 9, .box_h = 12, .ofs_x = 2, .ofs_y = 2},
    {.bitmap_index = 368, .adv_w = 138, .box_w = 9, .box_h = 13, .ofs_x = 2, .ofs_y = 1},
    {.bitmap_index = 383, .adv_w = 138, .box_w = 9, .box_h = 12, .ofs_x = 2, .ofs_y = 2},
    {.bitmap_index = 397, .adv_w = 138, .box_w = 7, .box_h = 13, .ofs_x = 3, .ofs_y = 1},
    {.bitmap_index = 409, .adv_w = 138, .box_w = 5, .box_h = 12, .ofs_x = 5, .ofs_y = 2},
    {.bitmap_index = 417, .adv_w = 138, .box_w = 9, .box_h = 12, .ofs_x = 2, .ofs_y = 1},
    {.bitmap_index = 431, .adv_w = 138, .box_w = 9, .box_h = 12, .ofs_x = 2, .ofs_y = 1},
    {.bitmap_index = 445, .adv_w = 138, .box_w = 9, .box_h = 12, .ofs_x = 2, .ofs_y = 1},
    {.bitmap_index = 459, .adv_w = 138, .box_w = 9, .box_h = 13, .ofs_x = 2, .ofs_y = 1},
    {.bitmap_index = 474, .adv_w = 138, .box_w = 8, .box_h = 12, .ofs_x = 3, .ofs_y = 1},
    {.bitmap_index = 486, .adv_w = 138, .box_w = 8, .box_h = 13, .ofs_x = 2, .ofs_y = 1},
    {.bitmap_index = 499, .adv_w = 138, .box_w = 8, .box_h = 7, .ofs_x = 2, .ofs_y = 1},
    {.bitmap_index = 506, .adv_w = 138, .box_w = 8, .box_h = 12, .ofs_x = 2, .ofs_y = 1},
    {.bitmap_index = 518, .adv_w = 138, .box_w = 7, .box_h = 7, .ofs_x = 2, .ofs_y = 1},
    {.bitmap_index = 525, .adv_w = 138, .box_w = 9, .box_h = 12, .ofs_x = 2, .ofs_y = 1},
    {.bitmap_index = 539, .adv_w = 138, .box_w = 8, .box_h = 7, .ofs_x = 2, .ofs_y = 1},
    {.bitmap_index = 546, .adv_w = 138, .box_w = 7, .box_h = 6, .ofs_x = 2, .ofs_y = 2},
    {.bitmap_index = 552, .adv_w = 138, .box_w = 8, .box_h = 7, .ofs_x = 2, .ofs_y = 1},
    {.bitmap_index = 559, .adv_w = 138, .box_w = 8, .box_h = 11, .ofs_x = 2, .ofs_y = 2},
    {.bitmap_index = 570, .adv_w = 63, .box_w = 2, .box_h = 5, .ofs_x = 2, .ofs_y = 2},
    {.bitmap_index = 572, .adv_w = 138, .box_w = 8, .box_h = 6, .ofs_x = 2, .ofs_y = 1},
    {.bitmap_index = 578, .adv_w = 138, .box_w = 8, .box_h = 13, .ofs_x = 2, .ofs_y = 1},
    {.bitmap_index = 591, .adv_w = 88, .box_w = 4, .box_h = 12, .ofs_x = 2, .ofs_y = 1},
    {.bitmap_index = 597, .adv_w = 138, .box_w = 8, .box_h = 6, .ofs_x = 2, .ofs_y = 2},
    {.bitmap_index = 603, .adv_w = 138, .box_w = 8, .box_h = 6, .ofs_x = 2, .ofs_y = 2},
    {.bitmap_index = 609, .adv_w = 138, .box_w = 8, .box_h = 7, .ofs_x = 2, .ofs_y = 1},
    {.bitmap_index = 616, .adv_w = 138, .box_w = 8, .box_h = 6, .ofs_x = 2, .ofs_y = 2},
    {.bitmap_index = 622, .adv_w = 138, .box_w = 8, .box_h = 6, .ofs_x = 2, .ofs_y = 2},
    {.bitmap_index = 628, .adv_w = 100, .box_w = 4, .box_h = 6, .ofs_x = 2, .ofs_y = 2},
    {.bitmap_index = 631, .adv_w = 138, .box_w = 7, .box_h = 7, .ofs_x = 2, .ofs_y = 1},
    {.bitmap_index = 638, .adv_w = 125, .box_w = 6, .box_h = 12, .ofs_x = 2, .ofs_y = 1},
    {.bitmap_index = 647, .adv_w = 138, .box_w = 8, .box_h = 6, .ofs_x = 2, .ofs_y = 1},
    {.bitmap_index = 653, .adv_w = 138, .box_w = 7, .box_h = 6, .ofs_x = 2, .ofs_y = 1},
    {.bitmap_index = 659, .adv_w = 138, .box_w = 8, .box_h = 6, .ofs_x = 2, .ofs_y = 1},
    {.bitmap_index = 665, .adv_w = 138, .box_w = 8, .box_h = 6, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 671, .adv_w = 138, .box_w = 8, .box_h = 6, .ofs_x = 2, .ofs_y = 1},
    {.bitmap_index = 677, .adv_w = 138, .box_w = 7, .box_h = 7, .ofs_x = 2, .ofs_y = 1}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/

static const uint8_t glyph_id_ofs_list_0[] = {
    0, 0, 1, 0, 2, 0, 0, 0,
    0, 0, 3, 4, 5, 6, 7, 0,
    8, 9, 10, 11, 12, 13, 14, 15,
    16, 17, 18, 19
};

/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 32, .range_length = 28, .glyph_id_start = 1,
        .unicode_list = NULL, .glyph_id_ofs_list = glyph_id_ofs_list_0, .list_length = 28, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_FULL
    },
    {
        .range_start = 65, .range_length = 26, .glyph_id_start = 21,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    },
    {
        .range_start = 97, .range_length = 26, .glyph_id_start = 47,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    }
};



/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

#if LVGL_VERSION_MAJOR == 8
/*Store all the custom data of the font*/
static  lv_font_fmt_txt_glyph_cache_t cache;
#endif

#if LVGL_VERSION_MAJOR >= 8
static const lv_font_fmt_txt_dsc_t font_dsc = {
#else
static lv_font_fmt_txt_dsc_t font_dsc = {
#endif
    .glyph_bitmap = glyph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps,
    .kern_dsc = NULL,
    .kern_scale = 0,
    .cmap_num = 3,
    .bpp = 1,
    .kern_classes = 0,
    .bitmap_format = 0,
#if LVGL_VERSION_MAJOR == 8
    .cache = &cache
#endif
};



/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LVGL_VERSION_MAJOR >= 8
const lv_font_t ui_font_Font16 = {
#else
lv_font_t ui_font_Font16 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 15,          /*The maximum line height required by the font*/
    .base_line = 0,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = 0,
    .underline_thickness = 0,
#endif
    .dsc = &font_dsc,          /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
#if LV_VERSION_CHECK(8, 2, 0) || LVGL_VERSION_MAJOR >= 9
    .fallback = NULL,
#endif
    .user_data = NULL,
};



#endif /*#if UI_FONT_FONT16*/

