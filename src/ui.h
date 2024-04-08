//
// MIT License
//
// Copyright (c) 2023 Elias Engelbert Plank
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef KOPERNIKUS_UI_H
#define KOPERNIKUS_UI_H

#include <libcore/display.h>
#include <libcore/string.h>

// ===================================================================================
// Generic UI functions
// ===================================================================================

/// Initialize the ui context
/// @param display
void ui_initialize(Display *display);

/// Destroy the ui context
void ui_destroy(void);

/// Begin a new ui draw pass
void ui_begin(void);

/// End the current ui draw pass
void ui_end(void);

// ===================================================================================
// UI window
// ===================================================================================

/// Begin a new UI window
/// @param label Label of the window
/// @param open Controls whether the window is open or not
/// @return Boolean that indicates whether the window is open
///
/// @note One must not call window_end when this function returns false. The corresponding
///       end call is performed automatically.
b8 ui_window_begin(const char *label, b8 *open);

/// Ends the current window
void ui_window_end(void);

// ===================================================================================
// UI menu
// ===================================================================================

/// Begin the main menu
/// @return Open state of the main menu
b8 ui_main_menu_begin(void);

/// End the main menu
void ui_main_menu_end(void);

/// Begin a menu
/// @param label Label of the menu
/// @return Open state of the menu
b8 ui_menu_begin(const char *label);

/// End the menu
void ui_menu_end(void);

/// Draw a menu item
/// @param title The title of the menu item
/// @param shortcut The keyboard shortcut for the menu item
/// @return Open state of the menu item
b8 ui_menu_item(const char *title, const char *shortcut);

// ===================================================================================
// UI text
// ===================================================================================

/// Draw text
/// @param fmt Format string
/// @param ... Variadic arguments
void ui_text(const char *fmt, ...);

/// Draw wrapped text
/// @param fmt Format string
/// @param ... Variadic arguments
void ui_text_wrapped(const char *fmt, ...);

/// Draw a note text
/// @param fmt Format string
/// @param ... Variadic arguments
void ui_note(const char *fmt, ...);

// ===================================================================================
// UI misc
// ===================================================================================

/// Draws a tooltip
/// @param fmt Format string
/// @param ... Variadic arguments
void ui_tooltip(const char *fmt, ...);

/// Draws a tooltip whenever the last item is hovered
/// @param fmt Format string
/// @param ... Variadic arguments
void ui_tooltip_hovered(const char *fmt, ...);

/// Draw a separator
void ui_separator(void);

/// Draw a selectable item
/// @param label The label
/// @param icon Icon at end of the line
/// @return Selection state
b8 ui_selectable(const char *label, const char *icon);

/// Begins setting the width of the next items
/// @param width The width
void ui_item_width_begin(f32 width);

/// Ends setting the width of the next items
void ui_item_width_end(void);

// ===================================================================================
// UI property
// ===================================================================================

/// Draw a readonly text property
/// @param property The property
/// @param text The text
void ui_property_text_readonly(const char *property, const char *text);

/// Draw a number property
/// @param property The property
/// @param x The number
/// @param fmt The format
b8 ui_property_number(const char *property, s64 *x, const char *fmt);

/// Draw a readonly number property
/// @param property The property
/// @param x The number
/// @param fmt The format
void ui_property_number_readonly(const char *property, s64 x, const char *fmt);

/// Draw a real property
/// @param property The property
/// @param x The real
/// @param fmt The format
/// @return A boolean value that indicates whether the property changed
b8 ui_property_real(const char *property, f64 *x, const char *fmt);

/// Draw a readonly real property
/// @param property The property
/// @param x The real
/// @param fmt The format
void ui_property_real_readonly(const char *property, f64 x, const char *fmt);

/// Draw a searchbar
/// @param buffer The search buffer
/// @param label The label
/// @param placeholder The placeholder
/// @return A boolean value that indicates whether the input changed
b8 ui_searchbar(StringBuffer *buffer, const char *label, const char *placeholder);

/// Draw a combobox
/// @param The title of the combobox
/// @param selected The index of the selected item
/// @param items The items
/// @param length The number of items
/// @return A boolean value that indicates whether the input changed
b8 ui_combobox(const char *title, s32 *selected, const char **items, usize length);

// ===================================================================================
// UI tree
// ===================================================================================

/// Draw a tree node with an optional icon
/// @param label The label of the tree node
/// @param icon Icon at end of tree node (optional)
/// @param selected The tree node is selected
/// @return Boolean that indicates whether the tree node is open
b8 ui_tree_node_begin(const char *label, const char *icon, b8 selected);

/// End the current tree node
void ui_tree_node_end(void);

/// Draw a tree node item with an optional icon
/// @param label The label of the item
/// @param icon Icon at the end of tree node item (optional)
/// @param selected Draw the item as selected
/// @return Boolean if the item is clicked
b8 ui_tree_item(const char *label, const char *icon, b8 selected);

/// Draw a tree node item that is draggable with an optional icon
/// @param label The label of the item
/// @param icon Icon at the end of tree node item (optional)
/// @param selected Draw the item as selected
/// @param data The drag and drop payload
/// @param size The size of the drag and drop payload
/// @return Boolean if the item is clicked
b8 ui_tree_item_drag_drop_source(const char *label, const char *icon, b8 selected, void *data, usize size);

// ===================================================================================
// UI popups
// ===================================================================================

/// Open the popup with the specified ID
/// @param id The popup ID
void ui_popup_open(const char *id);

/// Begin a popup
/// @param id The popup ID
/// @return Boolean if the popup is open
b8 ui_popup_begin(const char *id);

// ===================================================================================
// UI nodes
// ===================================================================================

/// Begin a node item
void ui_node_editor_begin(void);

/// End a node item
void ui_node_editor_end(void);

/// Check whether a node editor ection is present (right click)
/// @return Boolean if a node editor action is present
b8 ui_node_editor_action(void);

/// Begin a node item
/// @param id The id of the node
void ui_node_begin(s32 id);

/// End a node item
void ui_node_end(void);

// ===================================================================================
// UI state
// ===================================================================================

/// Check if the last ui item is selected
/// @return Selection state
b8 ui_selected(void);

/// Check if the left mouse button is clicked
/// @return Click state
b8 ui_click_left(void);

/// Check if the right mouse button is clicked
/// @return Click state
b8 ui_click_right(void);

/// Checks if the last ui item is hovered
/// @return Hovered state
b8 ui_hovered(void);

/// Keep the current line of the UI cursor
void ui_keep_line(void);

// ===================================================================================
// UI icons
// ===================================================================================

#define ICON_MIN_FA 0xe005
#define ICON_MAX_16_FA 0xf8ff
#define ICON_MAX_FA 0xf8ff
#define ICON_FA_0 "0"                                                // U+0030
#define ICON_FA_1 "1"                                                // U+0031
#define ICON_FA_2 "2"                                                // U+0032
#define ICON_FA_3 "3"                                                // U+0033
#define ICON_FA_4 "4"                                                // U+0034
#define ICON_FA_5 "5"                                                // U+0035
#define ICON_FA_6 "6"                                                // U+0036
#define ICON_FA_7 "7"                                                // U+0037
#define ICON_FA_8 "8"                                                // U+0038
#define ICON_FA_9 "9"                                                // U+0039
#define ICON_FA_A "A"                                                // U+0041
#define ICON_FA_ADDRESS_BOOK "\xef\x8a\xb9"                          // U+f2b9
#define ICON_FA_ADDRESS_CARD "\xef\x8a\xbb"                          // U+f2bb
#define ICON_FA_ALIGN_CENTER "\xef\x80\xb7"                          // U+f037
#define ICON_FA_ALIGN_JUSTIFY "\xef\x80\xb9"                         // U+f039
#define ICON_FA_ALIGN_LEFT "\xef\x80\xb6"                            // U+f036
#define ICON_FA_ALIGN_RIGHT "\xef\x80\xb8"                           // U+f038
#define ICON_FA_ANCHOR "\xef\x84\xbd"                                // U+f13d
#define ICON_FA_ANCHOR_CIRCLE_CHECK "\xee\x92\xaa"                   // U+e4aa
#define ICON_FA_ANCHOR_CIRCLE_EXCLAMATION "\xee\x92\xab"             // U+e4ab
#define ICON_FA_ANCHOR_CIRCLE_XMARK "\xee\x92\xac"                   // U+e4ac
#define ICON_FA_ANCHOR_LOCK "\xee\x92\xad"                           // U+e4ad
#define ICON_FA_ANGLE_DOWN "\xef\x84\x87"                            // U+f107
#define ICON_FA_ANGLE_LEFT "\xef\x84\x84"                            // U+f104
#define ICON_FA_ANGLE_RIGHT "\xef\x84\x85"                           // U+f105
#define ICON_FA_ANGLE_UP "\xef\x84\x86"                              // U+f106
#define ICON_FA_ANGLES_DOWN "\xef\x84\x83"                           // U+f103
#define ICON_FA_ANGLES_LEFT "\xef\x84\x80"                           // U+f100
#define ICON_FA_ANGLES_RIGHT "\xef\x84\x81"                          // U+f101
#define ICON_FA_ANGLES_UP "\xef\x84\x82"                             // U+f102
#define ICON_FA_ANKH "\xef\x99\x84"                                  // U+f644
#define ICON_FA_APPLE_WHOLE "\xef\x97\x91"                           // U+f5d1
#define ICON_FA_ARCHWAY "\xef\x95\x97"                               // U+f557
#define ICON_FA_ARROW_DOWN "\xef\x81\xa3"                            // U+f063
#define ICON_FA_ARROW_DOWN_1_9 "\xef\x85\xa2"                        // U+f162
#define ICON_FA_ARROW_DOWN_9_1 "\xef\xa2\x86"                        // U+f886
#define ICON_FA_ARROW_DOWN_A_Z "\xef\x85\x9d"                        // U+f15d
#define ICON_FA_ARROW_DOWN_LONG "\xef\x85\xb5"                       // U+f175
#define ICON_FA_ARROW_DOWN_SHORT_WIDE "\xef\xa2\x84"                 // U+f884
#define ICON_FA_ARROW_DOWN_UP_ACROSS_LINE "\xee\x92\xaf"             // U+e4af
#define ICON_FA_ARROW_DOWN_UP_LOCK "\xee\x92\xb0"                    // U+e4b0
#define ICON_FA_ARROW_DOWN_WIDE_SHORT "\xef\x85\xa0"                 // U+f160
#define ICON_FA_ARROW_DOWN_Z_A "\xef\xa2\x81"                        // U+f881
#define ICON_FA_ARROW_LEFT "\xef\x81\xa0"                            // U+f060
#define ICON_FA_ARROW_LEFT_LONG "\xef\x85\xb7"                       // U+f177
#define ICON_FA_ARROW_POINTER "\xef\x89\x85"                         // U+f245
#define ICON_FA_ARROW_RIGHT "\xef\x81\xa1"                           // U+f061
#define ICON_FA_ARROW_RIGHT_ARROW_LEFT "\xef\x83\xac"                // U+f0ec
#define ICON_FA_ARROW_RIGHT_FROM_BRACKET "\xef\x82\x8b"              // U+f08b
#define ICON_FA_ARROW_RIGHT_LONG "\xef\x85\xb8"                      // U+f178
#define ICON_FA_ARROW_RIGHT_TO_BRACKET "\xef\x82\x90"                // U+f090
#define ICON_FA_ARROW_RIGHT_TO_CITY "\xee\x92\xb3"                   // U+e4b3
#define ICON_FA_ARROW_ROTATE_LEFT "\xef\x83\xa2"                     // U+f0e2
#define ICON_FA_ARROW_ROTATE_RIGHT "\xef\x80\x9e"                    // U+f01e
#define ICON_FA_ARROW_TREND_DOWN "\xee\x82\x97"                      // U+e097
#define ICON_FA_ARROW_TREND_UP "\xee\x82\x98"                        // U+e098
#define ICON_FA_ARROW_TURN_DOWN "\xef\x85\x89"                       // U+f149
#define ICON_FA_ARROW_TURN_UP "\xef\x85\x88"                         // U+f148
#define ICON_FA_ARROW_UP "\xef\x81\xa2"                              // U+f062
#define ICON_FA_ARROW_UP_1_9 "\xef\x85\xa3"                          // U+f163
#define ICON_FA_ARROW_UP_9_1 "\xef\xa2\x87"                          // U+f887
#define ICON_FA_ARROW_UP_A_Z "\xef\x85\x9e"                          // U+f15e
#define ICON_FA_ARROW_UP_FROM_BRACKET "\xee\x82\x9a"                 // U+e09a
#define ICON_FA_ARROW_UP_FROM_GROUND_WATER "\xee\x92\xb5"            // U+e4b5
#define ICON_FA_ARROW_UP_FROM_WATER_PUMP "\xee\x92\xb6"              // U+e4b6
#define ICON_FA_ARROW_UP_LONG "\xef\x85\xb6"                         // U+f176
#define ICON_FA_ARROW_UP_RIGHT_DOTS "\xee\x92\xb7"                   // U+e4b7
#define ICON_FA_ARROW_UP_RIGHT_FROM_SQUARE "\xef\x82\x8e"            // U+f08e
#define ICON_FA_ARROW_UP_SHORT_WIDE "\xef\xa2\x85"                   // U+f885
#define ICON_FA_ARROW_UP_WIDE_SHORT "\xef\x85\xa1"                   // U+f161
#define ICON_FA_ARROW_UP_Z_A "\xef\xa2\x82"                          // U+f882
#define ICON_FA_ARROWS_DOWN_TO_LINE "\xee\x92\xb8"                   // U+e4b8
#define ICON_FA_ARROWS_DOWN_TO_PEOPLE "\xee\x92\xb9"                 // U+e4b9
#define ICON_FA_ARROWS_LEFT_RIGHT "\xef\x81\xbe"                     // U+f07e
#define ICON_FA_ARROWS_LEFT_RIGHT_TO_LINE "\xee\x92\xba"             // U+e4ba
#define ICON_FA_ARROWS_ROTATE "\xef\x80\xa1"                         // U+f021
#define ICON_FA_ARROWS_SPIN "\xee\x92\xbb"                           // U+e4bb
#define ICON_FA_ARROWS_SPLIT_UP_AND_LEFT "\xee\x92\xbc"              // U+e4bc
#define ICON_FA_ARROWS_TO_CIRCLE "\xee\x92\xbd"                      // U+e4bd
#define ICON_FA_ARROWS_TO_DOT "\xee\x92\xbe"                         // U+e4be
#define ICON_FA_ARROWS_TO_EYE "\xee\x92\xbf"                         // U+e4bf
#define ICON_FA_ARROWS_TURN_RIGHT "\xee\x93\x80"                     // U+e4c0
#define ICON_FA_ARROWS_TURN_TO_DOTS "\xee\x93\x81"                   // U+e4c1
#define ICON_FA_ARROWS_UP_DOWN "\xef\x81\xbd"                        // U+f07d
#define ICON_FA_ARROWS_UP_DOWN_LEFT_RIGHT "\xef\x81\x87"             // U+f047
#define ICON_FA_ARROWS_UP_TO_LINE "\xee\x93\x82"                     // U+e4c2
#define ICON_FA_ASTERISK "*"                                         // U+002a
#define ICON_FA_AT "@"                                               // U+0040
#define ICON_FA_ATOM "\xef\x97\x92"                                  // U+f5d2
#define ICON_FA_AUDIO_DESCRIPTION "\xef\x8a\x9e"                     // U+f29e
#define ICON_FA_AUSTRAL_SIGN "\xee\x82\xa9"                          // U+e0a9
#define ICON_FA_AWARD "\xef\x95\x99"                                 // U+f559
#define ICON_FA_B "B"                                                // U+0042
#define ICON_FA_BABY "\xef\x9d\xbc"                                  // U+f77c
#define ICON_FA_BABY_CARRIAGE "\xef\x9d\xbd"                         // U+f77d
#define ICON_FA_BACKWARD "\xef\x81\x8a"                              // U+f04a
#define ICON_FA_BACKWARD_FAST "\xef\x81\x89"                         // U+f049
#define ICON_FA_BACKWARD_STEP "\xef\x81\x88"                         // U+f048
#define ICON_FA_BACON "\xef\x9f\xa5"                                 // U+f7e5
#define ICON_FA_BACTERIA "\xee\x81\x99"                              // U+e059
#define ICON_FA_BACTERIUM "\xee\x81\x9a"                             // U+e05a
#define ICON_FA_BAG_SHOPPING "\xef\x8a\x90"                          // U+f290
#define ICON_FA_BAHAI "\xef\x99\xa6"                                 // U+f666
#define ICON_FA_BAHT_SIGN "\xee\x82\xac"                             // U+e0ac
#define ICON_FA_BAN "\xef\x81\x9e"                                   // U+f05e
#define ICON_FA_BAN_SMOKING "\xef\x95\x8d"                           // U+f54d
#define ICON_FA_BANDAGE "\xef\x91\xa2"                               // U+f462
#define ICON_FA_BANGLADESHI_TAKA_SIGN "\xee\x8b\xa6"                 // U+e2e6
#define ICON_FA_BARCODE "\xef\x80\xaa"                               // U+f02a
#define ICON_FA_BARS "\xef\x83\x89"                                  // U+f0c9
#define ICON_FA_BARS_PROGRESS "\xef\xa0\xa8"                         // U+f828
#define ICON_FA_BARS_STAGGERED "\xef\x95\x90"                        // U+f550
#define ICON_FA_BASEBALL "\xef\x90\xb3"                              // U+f433
#define ICON_FA_BASEBALL_BAT_BALL "\xef\x90\xb2"                     // U+f432
#define ICON_FA_BASKET_SHOPPING "\xef\x8a\x91"                       // U+f291
#define ICON_FA_BASKETBALL "\xef\x90\xb4"                            // U+f434
#define ICON_FA_BATH "\xef\x8b\x8d"                                  // U+f2cd
#define ICON_FA_BATTERY_EMPTY "\xef\x89\x84"                         // U+f244
#define ICON_FA_BATTERY_FULL "\xef\x89\x80"                          // U+f240
#define ICON_FA_BATTERY_HALF "\xef\x89\x82"                          // U+f242
#define ICON_FA_BATTERY_QUARTER "\xef\x89\x83"                       // U+f243
#define ICON_FA_BATTERY_THREE_QUARTERS "\xef\x89\x81"                // U+f241
#define ICON_FA_BED "\xef\x88\xb6"                                   // U+f236
#define ICON_FA_BED_PULSE "\xef\x92\x87"                             // U+f487
#define ICON_FA_BEER_MUG_EMPTY "\xef\x83\xbc"                        // U+f0fc
#define ICON_FA_BELL "\xef\x83\xb3"                                  // U+f0f3
#define ICON_FA_BELL_CONCIERGE "\xef\x95\xa2"                        // U+f562
#define ICON_FA_BELL_SLASH "\xef\x87\xb6"                            // U+f1f6
#define ICON_FA_BEZIER_CURVE "\xef\x95\x9b"                          // U+f55b
#define ICON_FA_BICYCLE "\xef\x88\x86"                               // U+f206
#define ICON_FA_BINOCULARS "\xef\x87\xa5"                            // U+f1e5
#define ICON_FA_BIOHAZARD "\xef\x9e\x80"                             // U+f780
#define ICON_FA_BITCOIN_SIGN "\xee\x82\xb4"                          // U+e0b4
#define ICON_FA_BLENDER "\xef\x94\x97"                               // U+f517
#define ICON_FA_BLENDER_PHONE "\xef\x9a\xb6"                         // U+f6b6
#define ICON_FA_BLOG "\xef\x9e\x81"                                  // U+f781
#define ICON_FA_BOLD "\xef\x80\xb2"                                  // U+f032
#define ICON_FA_BOLT "\xef\x83\xa7"                                  // U+f0e7
#define ICON_FA_BOLT_LIGHTNING "\xee\x82\xb7"                        // U+e0b7
#define ICON_FA_BOMB "\xef\x87\xa2"                                  // U+f1e2
#define ICON_FA_BONE "\xef\x97\x97"                                  // U+f5d7
#define ICON_FA_BONG "\xef\x95\x9c"                                  // U+f55c
#define ICON_FA_BOOK "\xef\x80\xad"                                  // U+f02d
#define ICON_FA_BOOK_ATLAS "\xef\x95\x98"                            // U+f558
#define ICON_FA_BOOK_BIBLE "\xef\x99\x87"                            // U+f647
#define ICON_FA_BOOK_BOOKMARK "\xee\x82\xbb"                         // U+e0bb
#define ICON_FA_BOOK_JOURNAL_WHILLS "\xef\x99\xaa"                   // U+f66a
#define ICON_FA_BOOK_MEDICAL "\xef\x9f\xa6"                          // U+f7e6
#define ICON_FA_BOOK_OPEN "\xef\x94\x98"                             // U+f518
#define ICON_FA_BOOK_OPEN_READER "\xef\x97\x9a"                      // U+f5da
#define ICON_FA_BOOK_QURAN "\xef\x9a\x87"                            // U+f687
#define ICON_FA_BOOK_SKULL "\xef\x9a\xb7"                            // U+f6b7
#define ICON_FA_BOOK_TANAKH "\xef\xa0\xa7"                           // U+f827
#define ICON_FA_BOOKMARK "\xef\x80\xae"                              // U+f02e
#define ICON_FA_BORDER_ALL "\xef\xa1\x8c"                            // U+f84c
#define ICON_FA_BORDER_NONE "\xef\xa1\x90"                           // U+f850
#define ICON_FA_BORDER_TOP_LEFT "\xef\xa1\x93"                       // U+f853
#define ICON_FA_BORE_HOLE "\xee\x93\x83"                             // U+e4c3
#define ICON_FA_BOTTLE_DROPLET "\xee\x93\x84"                        // U+e4c4
#define ICON_FA_BOTTLE_WATER "\xee\x93\x85"                          // U+e4c5
#define ICON_FA_BOWL_FOOD "\xee\x93\x86"                             // U+e4c6
#define ICON_FA_BOWL_RICE "\xee\x8b\xab"                             // U+e2eb
#define ICON_FA_BOWLING_BALL "\xef\x90\xb6"                          // U+f436
#define ICON_FA_BOX "\xef\x91\xa6"                                   // U+f466
#define ICON_FA_BOX_ARCHIVE "\xef\x86\x87"                           // U+f187
#define ICON_FA_BOX_OPEN "\xef\x92\x9e"                              // U+f49e
#define ICON_FA_BOX_TISSUE "\xee\x81\x9b"                            // U+e05b
#define ICON_FA_BOXES_PACKING "\xee\x93\x87"                         // U+e4c7
#define ICON_FA_BOXES_STACKED "\xef\x91\xa8"                         // U+f468
#define ICON_FA_BRAILLE "\xef\x8a\xa1"                               // U+f2a1
#define ICON_FA_BRAIN "\xef\x97\x9c"                                 // U+f5dc
#define ICON_FA_BRAZILIAN_REAL_SIGN "\xee\x91\xac"                   // U+e46c
#define ICON_FA_BREAD_SLICE "\xef\x9f\xac"                           // U+f7ec
#define ICON_FA_BRIDGE "\xee\x93\x88"                                // U+e4c8
#define ICON_FA_BRIDGE_CIRCLE_CHECK "\xee\x93\x89"                   // U+e4c9
#define ICON_FA_BRIDGE_CIRCLE_EXCLAMATION "\xee\x93\x8a"             // U+e4ca
#define ICON_FA_BRIDGE_CIRCLE_XMARK "\xee\x93\x8b"                   // U+e4cb
#define ICON_FA_BRIDGE_LOCK "\xee\x93\x8c"                           // U+e4cc
#define ICON_FA_BRIDGE_WATER "\xee\x93\x8e"                          // U+e4ce
#define ICON_FA_BRIEFCASE "\xef\x82\xb1"                             // U+f0b1
#define ICON_FA_BRIEFCASE_MEDICAL "\xef\x91\xa9"                     // U+f469
#define ICON_FA_BROOM "\xef\x94\x9a"                                 // U+f51a
#define ICON_FA_BROOM_BALL "\xef\x91\x98"                            // U+f458
#define ICON_FA_BRUSH "\xef\x95\x9d"                                 // U+f55d
#define ICON_FA_BUCKET "\xee\x93\x8f"                                // U+e4cf
#define ICON_FA_BUG "\xef\x86\x88"                                   // U+f188
#define ICON_FA_BUG_SLASH "\xee\x92\x90"                             // U+e490
#define ICON_FA_BUGS "\xee\x93\x90"                                  // U+e4d0
#define ICON_FA_BUILDING "\xef\x86\xad"                              // U+f1ad
#define ICON_FA_BUILDING_CIRCLE_ARROW_RIGHT "\xee\x93\x91"           // U+e4d1
#define ICON_FA_BUILDING_CIRCLE_CHECK "\xee\x93\x92"                 // U+e4d2
#define ICON_FA_BUILDING_CIRCLE_EXCLAMATION "\xee\x93\x93"           // U+e4d3
#define ICON_FA_BUILDING_CIRCLE_XMARK "\xee\x93\x94"                 // U+e4d4
#define ICON_FA_BUILDING_COLUMNS "\xef\x86\x9c"                      // U+f19c
#define ICON_FA_BUILDING_FLAG "\xee\x93\x95"                         // U+e4d5
#define ICON_FA_BUILDING_LOCK "\xee\x93\x96"                         // U+e4d6
#define ICON_FA_BUILDING_NGO "\xee\x93\x97"                          // U+e4d7
#define ICON_FA_BUILDING_SHIELD "\xee\x93\x98"                       // U+e4d8
#define ICON_FA_BUILDING_UN "\xee\x93\x99"                           // U+e4d9
#define ICON_FA_BUILDING_USER "\xee\x93\x9a"                         // U+e4da
#define ICON_FA_BUILDING_WHEAT "\xee\x93\x9b"                        // U+e4db
#define ICON_FA_BULLHORN "\xef\x82\xa1"                              // U+f0a1
#define ICON_FA_BULLSEYE "\xef\x85\x80"                              // U+f140
#define ICON_FA_BURGER "\xef\xa0\x85"                                // U+f805
#define ICON_FA_BURST "\xee\x93\x9c"                                 // U+e4dc
#define ICON_FA_BUS "\xef\x88\x87"                                   // U+f207
#define ICON_FA_BUS_SIMPLE "\xef\x95\x9e"                            // U+f55e
#define ICON_FA_BUSINESS_TIME "\xef\x99\x8a"                         // U+f64a
#define ICON_FA_C "C"                                                // U+0043
#define ICON_FA_CABLE_CAR "\xef\x9f\x9a"                             // U+f7da
#define ICON_FA_CAKE_CANDLES "\xef\x87\xbd"                          // U+f1fd
#define ICON_FA_CALCULATOR "\xef\x87\xac"                            // U+f1ec
#define ICON_FA_CALENDAR "\xef\x84\xb3"                              // U+f133
#define ICON_FA_CALENDAR_CHECK "\xef\x89\xb4"                        // U+f274
#define ICON_FA_CALENDAR_DAY "\xef\x9e\x83"                          // U+f783
#define ICON_FA_CALENDAR_DAYS "\xef\x81\xb3"                         // U+f073
#define ICON_FA_CALENDAR_MINUS "\xef\x89\xb2"                        // U+f272
#define ICON_FA_CALENDAR_PLUS "\xef\x89\xb1"                         // U+f271
#define ICON_FA_CALENDAR_WEEK "\xef\x9e\x84"                         // U+f784
#define ICON_FA_CALENDAR_XMARK "\xef\x89\xb3"                        // U+f273
#define ICON_FA_CAMERA "\xef\x80\xb0"                                // U+f030
#define ICON_FA_CAMERA_RETRO "\xef\x82\x83"                          // U+f083
#define ICON_FA_CAMERA_ROTATE "\xee\x83\x98"                         // U+e0d8
#define ICON_FA_CAMPGROUND "\xef\x9a\xbb"                            // U+f6bb
#define ICON_FA_CANDY_CANE "\xef\x9e\x86"                            // U+f786
#define ICON_FA_CANNABIS "\xef\x95\x9f"                              // U+f55f
#define ICON_FA_CAPSULES "\xef\x91\xab"                              // U+f46b
#define ICON_FA_CAR "\xef\x86\xb9"                                   // U+f1b9
#define ICON_FA_CAR_BATTERY "\xef\x97\x9f"                           // U+f5df
#define ICON_FA_CAR_BURST "\xef\x97\xa1"                             // U+f5e1
#define ICON_FA_CAR_ON "\xee\x93\x9d"                                // U+e4dd
#define ICON_FA_CAR_REAR "\xef\x97\x9e"                              // U+f5de
#define ICON_FA_CAR_SIDE "\xef\x97\xa4"                              // U+f5e4
#define ICON_FA_CAR_TUNNEL "\xee\x93\x9e"                            // U+e4de
#define ICON_FA_CARAVAN "\xef\xa3\xbf"                               // U+f8ff
#define ICON_FA_CARET_DOWN "\xef\x83\x97"                            // U+f0d7
#define ICON_FA_CARET_LEFT "\xef\x83\x99"                            // U+f0d9
#define ICON_FA_CARET_RIGHT "\xef\x83\x9a"                           // U+f0da
#define ICON_FA_CARET_UP "\xef\x83\x98"                              // U+f0d8
#define ICON_FA_CARROT "\xef\x9e\x87"                                // U+f787
#define ICON_FA_CART_ARROW_DOWN "\xef\x88\x98"                       // U+f218
#define ICON_FA_CART_FLATBED "\xef\x91\xb4"                          // U+f474
#define ICON_FA_CART_FLATBED_SUITCASE "\xef\x96\x9d"                 // U+f59d
#define ICON_FA_CART_PLUS "\xef\x88\x97"                             // U+f217
#define ICON_FA_CART_SHOPPING "\xef\x81\xba"                         // U+f07a
#define ICON_FA_CASH_REGISTER "\xef\x9e\x88"                         // U+f788
#define ICON_FA_CAT "\xef\x9a\xbe"                                   // U+f6be
#define ICON_FA_CEDI_SIGN "\xee\x83\x9f"                             // U+e0df
#define ICON_FA_CENT_SIGN "\xee\x8f\xb5"                             // U+e3f5
#define ICON_FA_CERTIFICATE "\xef\x82\xa3"                           // U+f0a3
#define ICON_FA_CHAIR "\xef\x9b\x80"                                 // U+f6c0
#define ICON_FA_CHALKBOARD "\xef\x94\x9b"                            // U+f51b
#define ICON_FA_CHALKBOARD_USER "\xef\x94\x9c"                       // U+f51c
#define ICON_FA_CHAMPAGNE_GLASSES "\xef\x9e\x9f"                     // U+f79f
#define ICON_FA_CHARGING_STATION "\xef\x97\xa7"                      // U+f5e7
#define ICON_FA_CHART_AREA "\xef\x87\xbe"                            // U+f1fe
#define ICON_FA_CHART_BAR "\xef\x82\x80"                             // U+f080
#define ICON_FA_CHART_COLUMN "\xee\x83\xa3"                          // U+e0e3
#define ICON_FA_CHART_GANTT "\xee\x83\xa4"                           // U+e0e4
#define ICON_FA_CHART_LINE "\xef\x88\x81"                            // U+f201
#define ICON_FA_CHART_PIE "\xef\x88\x80"                             // U+f200
#define ICON_FA_CHART_SIMPLE "\xee\x91\xb3"                          // U+e473
#define ICON_FA_CHECK "\xef\x80\x8c"                                 // U+f00c
#define ICON_FA_CHECK_DOUBLE "\xef\x95\xa0"                          // U+f560
#define ICON_FA_CHECK_TO_SLOT "\xef\x9d\xb2"                         // U+f772
#define ICON_FA_CHEESE "\xef\x9f\xaf"                                // U+f7ef
#define ICON_FA_CHESS "\xef\x90\xb9"                                 // U+f439
#define ICON_FA_CHESS_BISHOP "\xef\x90\xba"                          // U+f43a
#define ICON_FA_CHESS_BOARD "\xef\x90\xbc"                           // U+f43c
#define ICON_FA_CHESS_KING "\xef\x90\xbf"                            // U+f43f
#define ICON_FA_CHESS_KNIGHT "\xef\x91\x81"                          // U+f441
#define ICON_FA_CHESS_PAWN "\xef\x91\x83"                            // U+f443
#define ICON_FA_CHESS_QUEEN "\xef\x91\x85"                           // U+f445
#define ICON_FA_CHESS_ROOK "\xef\x91\x87"                            // U+f447
#define ICON_FA_CHEVRON_DOWN "\xef\x81\xb8"                          // U+f078
#define ICON_FA_CHEVRON_LEFT "\xef\x81\x93"                          // U+f053
#define ICON_FA_CHEVRON_RIGHT "\xef\x81\x94"                         // U+f054
#define ICON_FA_CHEVRON_UP "\xef\x81\xb7"                            // U+f077
#define ICON_FA_CHILD "\xef\x86\xae"                                 // U+f1ae
#define ICON_FA_CHILD_COMBATANT "\xee\x93\xa0"                       // U+e4e0
#define ICON_FA_CHILD_DRESS "\xee\x96\x9c"                           // U+e59c
#define ICON_FA_CHILD_REACHING "\xee\x96\x9d"                        // U+e59d
#define ICON_FA_CHILDREN "\xee\x93\xa1"                              // U+e4e1
#define ICON_FA_CHURCH "\xef\x94\x9d"                                // U+f51d
#define ICON_FA_CIRCLE "\xef\x84\x91"                                // U+f111
#define ICON_FA_CIRCLE_ARROW_DOWN "\xef\x82\xab"                     // U+f0ab
#define ICON_FA_CIRCLE_ARROW_LEFT "\xef\x82\xa8"                     // U+f0a8
#define ICON_FA_CIRCLE_ARROW_RIGHT "\xef\x82\xa9"                    // U+f0a9
#define ICON_FA_CIRCLE_ARROW_UP "\xef\x82\xaa"                       // U+f0aa
#define ICON_FA_CIRCLE_CHECK "\xef\x81\x98"                          // U+f058
#define ICON_FA_CIRCLE_CHEVRON_DOWN "\xef\x84\xba"                   // U+f13a
#define ICON_FA_CIRCLE_CHEVRON_LEFT "\xef\x84\xb7"                   // U+f137
#define ICON_FA_CIRCLE_CHEVRON_RIGHT "\xef\x84\xb8"                  // U+f138
#define ICON_FA_CIRCLE_CHEVRON_UP "\xef\x84\xb9"                     // U+f139
#define ICON_FA_CIRCLE_DOLLAR_TO_SLOT "\xef\x92\xb9"                 // U+f4b9
#define ICON_FA_CIRCLE_DOT "\xef\x86\x92"                            // U+f192
#define ICON_FA_CIRCLE_DOWN "\xef\x8d\x98"                           // U+f358
#define ICON_FA_CIRCLE_EXCLAMATION "\xef\x81\xaa"                    // U+f06a
#define ICON_FA_CIRCLE_H "\xef\x91\xbe"                              // U+f47e
#define ICON_FA_CIRCLE_HALF_STROKE "\xef\x81\x82"                    // U+f042
#define ICON_FA_CIRCLE_INFO "\xef\x81\x9a"                           // U+f05a
#define ICON_FA_CIRCLE_LEFT "\xef\x8d\x99"                           // U+f359
#define ICON_FA_CIRCLE_MINUS "\xef\x81\x96"                          // U+f056
#define ICON_FA_CIRCLE_NODES "\xee\x93\xa2"                          // U+e4e2
#define ICON_FA_CIRCLE_NOTCH "\xef\x87\x8e"                          // U+f1ce
#define ICON_FA_CIRCLE_PAUSE "\xef\x8a\x8b"                          // U+f28b
#define ICON_FA_CIRCLE_PLAY "\xef\x85\x84"                           // U+f144
#define ICON_FA_CIRCLE_PLUS "\xef\x81\x95"                           // U+f055
#define ICON_FA_CIRCLE_QUESTION "\xef\x81\x99"                       // U+f059
#define ICON_FA_CIRCLE_RADIATION "\xef\x9e\xba"                      // U+f7ba
#define ICON_FA_CIRCLE_RIGHT "\xef\x8d\x9a"                          // U+f35a
#define ICON_FA_CIRCLE_STOP "\xef\x8a\x8d"                           // U+f28d
#define ICON_FA_CIRCLE_UP "\xef\x8d\x9b"                             // U+f35b
#define ICON_FA_CIRCLE_USER "\xef\x8a\xbd"                           // U+f2bd
#define ICON_FA_CIRCLE_XMARK "\xef\x81\x97"                          // U+f057
#define ICON_FA_CITY "\xef\x99\x8f"                                  // U+f64f
#define ICON_FA_CLAPPERBOARD "\xee\x84\xb1"                          // U+e131
#define ICON_FA_CLIPBOARD "\xef\x8c\xa8"                             // U+f328
#define ICON_FA_CLIPBOARD_CHECK "\xef\x91\xac"                       // U+f46c
#define ICON_FA_CLIPBOARD_LIST "\xef\x91\xad"                        // U+f46d
#define ICON_FA_CLIPBOARD_QUESTION "\xee\x93\xa3"                    // U+e4e3
#define ICON_FA_CLIPBOARD_USER "\xef\x9f\xb3"                        // U+f7f3
#define ICON_FA_CLOCK "\xef\x80\x97"                                 // U+f017
#define ICON_FA_CLOCK_ROTATE_LEFT "\xef\x87\x9a"                     // U+f1da
#define ICON_FA_CLONE "\xef\x89\x8d"                                 // U+f24d
#define ICON_FA_CLOSED_CAPTIONING "\xef\x88\x8a"                     // U+f20a
#define ICON_FA_CLOUD "\xef\x83\x82"                                 // U+f0c2
#define ICON_FA_CLOUD_ARROW_DOWN "\xef\x83\xad"                      // U+f0ed
#define ICON_FA_CLOUD_ARROW_UP "\xef\x83\xae"                        // U+f0ee
#define ICON_FA_CLOUD_BOLT "\xef\x9d\xac"                            // U+f76c
#define ICON_FA_CLOUD_MEATBALL "\xef\x9c\xbb"                        // U+f73b
#define ICON_FA_CLOUD_MOON "\xef\x9b\x83"                            // U+f6c3
#define ICON_FA_CLOUD_MOON_RAIN "\xef\x9c\xbc"                       // U+f73c
#define ICON_FA_CLOUD_RAIN "\xef\x9c\xbd"                            // U+f73d
#define ICON_FA_CLOUD_SHOWERS_HEAVY "\xef\x9d\x80"                   // U+f740
#define ICON_FA_CLOUD_SHOWERS_WATER "\xee\x93\xa4"                   // U+e4e4
#define ICON_FA_CLOUD_SUN "\xef\x9b\x84"                             // U+f6c4
#define ICON_FA_CLOUD_SUN_RAIN "\xef\x9d\x83"                        // U+f743
#define ICON_FA_CLOVER "\xee\x84\xb9"                                // U+e139
#define ICON_FA_CODE "\xef\x84\xa1"                                  // U+f121
#define ICON_FA_CODE_BRANCH "\xef\x84\xa6"                           // U+f126
#define ICON_FA_CODE_COMMIT "\xef\x8e\x86"                           // U+f386
#define ICON_FA_CODE_COMPARE "\xee\x84\xba"                          // U+e13a
#define ICON_FA_CODE_FORK "\xee\x84\xbb"                             // U+e13b
#define ICON_FA_CODE_MERGE "\xef\x8e\x87"                            // U+f387
#define ICON_FA_CODE_PULL_REQUEST "\xee\x84\xbc"                     // U+e13c
#define ICON_FA_COINS "\xef\x94\x9e"                                 // U+f51e
#define ICON_FA_COLON_SIGN "\xee\x85\x80"                            // U+e140
#define ICON_FA_COMMENT "\xef\x81\xb5"                               // U+f075
#define ICON_FA_COMMENT_DOLLAR "\xef\x99\x91"                        // U+f651
#define ICON_FA_COMMENT_DOTS "\xef\x92\xad"                          // U+f4ad
#define ICON_FA_COMMENT_MEDICAL "\xef\x9f\xb5"                       // U+f7f5
#define ICON_FA_COMMENT_SLASH "\xef\x92\xb3"                         // U+f4b3
#define ICON_FA_COMMENT_SMS "\xef\x9f\x8d"                           // U+f7cd
#define ICON_FA_COMMENTS "\xef\x82\x86"                              // U+f086
#define ICON_FA_COMMENTS_DOLLAR "\xef\x99\x93"                       // U+f653
#define ICON_FA_COMPACT_DISC "\xef\x94\x9f"                          // U+f51f
#define ICON_FA_COMPASS "\xef\x85\x8e"                               // U+f14e
#define ICON_FA_COMPASS_DRAFTING "\xef\x95\xa8"                      // U+f568
#define ICON_FA_COMPRESS "\xef\x81\xa6"                              // U+f066
#define ICON_FA_COMPUTER "\xee\x93\xa5"                              // U+e4e5
#define ICON_FA_COMPUTER_MOUSE "\xef\xa3\x8c"                        // U+f8cc
#define ICON_FA_COOKIE "\xef\x95\xa3"                                // U+f563
#define ICON_FA_COOKIE_BITE "\xef\x95\xa4"                           // U+f564
#define ICON_FA_COPY "\xef\x83\x85"                                  // U+f0c5
#define ICON_FA_COPYRIGHT "\xef\x87\xb9"                             // U+f1f9
#define ICON_FA_COUCH "\xef\x92\xb8"                                 // U+f4b8
#define ICON_FA_COW "\xef\x9b\x88"                                   // U+f6c8
#define ICON_FA_CREDIT_CARD "\xef\x82\x9d"                           // U+f09d
#define ICON_FA_CROP "\xef\x84\xa5"                                  // U+f125
#define ICON_FA_CROP_SIMPLE "\xef\x95\xa5"                           // U+f565
#define ICON_FA_CROSS "\xef\x99\x94"                                 // U+f654
#define ICON_FA_CROSSHAIRS "\xef\x81\x9b"                            // U+f05b
#define ICON_FA_CROW "\xef\x94\xa0"                                  // U+f520
#define ICON_FA_CROWN "\xef\x94\xa1"                                 // U+f521
#define ICON_FA_CRUTCH "\xef\x9f\xb7"                                // U+f7f7
#define ICON_FA_CRUZEIRO_SIGN "\xee\x85\x92"                         // U+e152
#define ICON_FA_CUBE "\xef\x86\xb2"                                  // U+f1b2
#define ICON_FA_CUBES "\xef\x86\xb3"                                 // U+f1b3
#define ICON_FA_CUBES_STACKED "\xee\x93\xa6"                         // U+e4e6
#define ICON_FA_D "D"                                                // U+0044
#define ICON_FA_DATABASE "\xef\x87\x80"                              // U+f1c0
#define ICON_FA_DELETE_LEFT "\xef\x95\x9a"                           // U+f55a
#define ICON_FA_DEMOCRAT "\xef\x9d\x87"                              // U+f747
#define ICON_FA_DESKTOP "\xef\x8e\x90"                               // U+f390
#define ICON_FA_DHARMACHAKRA "\xef\x99\x95"                          // U+f655
#define ICON_FA_DIAGRAM_NEXT "\xee\x91\xb6"                          // U+e476
#define ICON_FA_DIAGRAM_PREDECESSOR "\xee\x91\xb7"                   // U+e477
#define ICON_FA_DIAGRAM_PROJECT "\xef\x95\x82"                       // U+f542
#define ICON_FA_DIAGRAM_SUCCESSOR "\xee\x91\xba"                     // U+e47a
#define ICON_FA_DIAMOND "\xef\x88\x99"                               // U+f219
#define ICON_FA_DIAMOND_TURN_RIGHT "\xef\x97\xab"                    // U+f5eb
#define ICON_FA_DICE "\xef\x94\xa2"                                  // U+f522
#define ICON_FA_DICE_D20 "\xef\x9b\x8f"                              // U+f6cf
#define ICON_FA_DICE_D6 "\xef\x9b\x91"                               // U+f6d1
#define ICON_FA_DICE_FIVE "\xef\x94\xa3"                             // U+f523
#define ICON_FA_DICE_FOUR "\xef\x94\xa4"                             // U+f524
#define ICON_FA_DICE_ONE "\xef\x94\xa5"                              // U+f525
#define ICON_FA_DICE_SIX "\xef\x94\xa6"                              // U+f526
#define ICON_FA_DICE_THREE "\xef\x94\xa7"                            // U+f527
#define ICON_FA_DICE_TWO "\xef\x94\xa8"                              // U+f528
#define ICON_FA_DISEASE "\xef\x9f\xba"                               // U+f7fa
#define ICON_FA_DISPLAY "\xee\x85\xa3"                               // U+e163
#define ICON_FA_DIVIDE "\xef\x94\xa9"                                // U+f529
#define ICON_FA_DNA "\xef\x91\xb1"                                   // U+f471
#define ICON_FA_DOG "\xef\x9b\x93"                                   // U+f6d3
#define ICON_FA_DOLLAR_SIGN "$"                                      // U+0024
#define ICON_FA_DOLLY "\xef\x91\xb2"                                 // U+f472
#define ICON_FA_DONG_SIGN "\xee\x85\xa9"                             // U+e169
#define ICON_FA_DOOR_CLOSED "\xef\x94\xaa"                           // U+f52a
#define ICON_FA_DOOR_OPEN "\xef\x94\xab"                             // U+f52b
#define ICON_FA_DOVE "\xef\x92\xba"                                  // U+f4ba
#define ICON_FA_DOWN_LEFT_AND_UP_RIGHT_TO_CENTER "\xef\x90\xa2"      // U+f422
#define ICON_FA_DOWN_LONG "\xef\x8c\x89"                             // U+f309
#define ICON_FA_DOWNLOAD "\xef\x80\x99"                              // U+f019
#define ICON_FA_DRAGON "\xef\x9b\x95"                                // U+f6d5
#define ICON_FA_DRAW_POLYGON "\xef\x97\xae"                          // U+f5ee
#define ICON_FA_DROPLET "\xef\x81\x83"                               // U+f043
#define ICON_FA_DROPLET_SLASH "\xef\x97\x87"                         // U+f5c7
#define ICON_FA_DRUM "\xef\x95\xa9"                                  // U+f569
#define ICON_FA_DRUM_STEELPAN "\xef\x95\xaa"                         // U+f56a
#define ICON_FA_DRUMSTICK_BITE "\xef\x9b\x97"                        // U+f6d7
#define ICON_FA_DUMBBELL "\xef\x91\x8b"                              // U+f44b
#define ICON_FA_DUMPSTER "\xef\x9e\x93"                              // U+f793
#define ICON_FA_DUMPSTER_FIRE "\xef\x9e\x94"                         // U+f794
#define ICON_FA_DUNGEON "\xef\x9b\x99"                               // U+f6d9
#define ICON_FA_E "E"                                                // U+0045
#define ICON_FA_EAR_DEAF "\xef\x8a\xa4"                              // U+f2a4
#define ICON_FA_EAR_LISTEN "\xef\x8a\xa2"                            // U+f2a2
#define ICON_FA_EARTH_AFRICA "\xef\x95\xbc"                          // U+f57c
#define ICON_FA_EARTH_AMERICAS "\xef\x95\xbd"                        // U+f57d
#define ICON_FA_EARTH_ASIA "\xef\x95\xbe"                            // U+f57e
#define ICON_FA_EARTH_EUROPE "\xef\x9e\xa2"                          // U+f7a2
#define ICON_FA_EARTH_OCEANIA "\xee\x91\xbb"                         // U+e47b
#define ICON_FA_EGG "\xef\x9f\xbb"                                   // U+f7fb
#define ICON_FA_EJECT "\xef\x81\x92"                                 // U+f052
#define ICON_FA_ELEVATOR "\xee\x85\xad"                              // U+e16d
#define ICON_FA_ELLIPSIS "\xef\x85\x81"                              // U+f141
#define ICON_FA_ELLIPSIS_VERTICAL "\xef\x85\x82"                     // U+f142
#define ICON_FA_ENVELOPE "\xef\x83\xa0"                              // U+f0e0
#define ICON_FA_ENVELOPE_CIRCLE_CHECK "\xee\x93\xa8"                 // U+e4e8
#define ICON_FA_ENVELOPE_OPEN "\xef\x8a\xb6"                         // U+f2b6
#define ICON_FA_ENVELOPE_OPEN_TEXT "\xef\x99\x98"                    // U+f658
#define ICON_FA_ENVELOPES_BULK "\xef\x99\xb4"                        // U+f674
#define ICON_FA_EQUALS "="                                           // U+003d
#define ICON_FA_ERASER "\xef\x84\xad"                                // U+f12d
#define ICON_FA_ETHERNET "\xef\x9e\x96"                              // U+f796
#define ICON_FA_EURO_SIGN "\xef\x85\x93"                             // U+f153
#define ICON_FA_EXCLAMATION "!"                                      // U+0021
#define ICON_FA_EXPAND "\xef\x81\xa5"                                // U+f065
#define ICON_FA_EXPLOSION "\xee\x93\xa9"                             // U+e4e9
#define ICON_FA_EYE "\xef\x81\xae"                                   // U+f06e
#define ICON_FA_EYE_DROPPER "\xef\x87\xbb"                           // U+f1fb
#define ICON_FA_EYE_LOW_VISION "\xef\x8a\xa8"                        // U+f2a8
#define ICON_FA_EYE_SLASH "\xef\x81\xb0"                             // U+f070
#define ICON_FA_F "F"                                                // U+0046
#define ICON_FA_FACE_ANGRY "\xef\x95\x96"                            // U+f556
#define ICON_FA_FACE_DIZZY "\xef\x95\xa7"                            // U+f567
#define ICON_FA_FACE_FLUSHED "\xef\x95\xb9"                          // U+f579
#define ICON_FA_FACE_FROWN "\xef\x84\x99"                            // U+f119
#define ICON_FA_FACE_FROWN_OPEN "\xef\x95\xba"                       // U+f57a
#define ICON_FA_FACE_GRIMACE "\xef\x95\xbf"                          // U+f57f
#define ICON_FA_FACE_GRIN "\xef\x96\x80"                             // U+f580
#define ICON_FA_FACE_GRIN_BEAM "\xef\x96\x82"                        // U+f582
#define ICON_FA_FACE_GRIN_BEAM_SWEAT "\xef\x96\x83"                  // U+f583
#define ICON_FA_FACE_GRIN_HEARTS "\xef\x96\x84"                      // U+f584
#define ICON_FA_FACE_GRIN_SQUINT "\xef\x96\x85"                      // U+f585
#define ICON_FA_FACE_GRIN_SQUINT_TEARS "\xef\x96\x86"                // U+f586
#define ICON_FA_FACE_GRIN_STARS "\xef\x96\x87"                       // U+f587
#define ICON_FA_FACE_GRIN_TEARS "\xef\x96\x88"                       // U+f588
#define ICON_FA_FACE_GRIN_TONGUE "\xef\x96\x89"                      // U+f589
#define ICON_FA_FACE_GRIN_TONGUE_SQUINT "\xef\x96\x8a"               // U+f58a
#define ICON_FA_FACE_GRIN_TONGUE_WINK "\xef\x96\x8b"                 // U+f58b
#define ICON_FA_FACE_GRIN_WIDE "\xef\x96\x81"                        // U+f581
#define ICON_FA_FACE_GRIN_WINK "\xef\x96\x8c"                        // U+f58c
#define ICON_FA_FACE_KISS "\xef\x96\x96"                             // U+f596
#define ICON_FA_FACE_KISS_BEAM "\xef\x96\x97"                        // U+f597
#define ICON_FA_FACE_KISS_WINK_HEART "\xef\x96\x98"                  // U+f598
#define ICON_FA_FACE_LAUGH "\xef\x96\x99"                            // U+f599
#define ICON_FA_FACE_LAUGH_BEAM "\xef\x96\x9a"                       // U+f59a
#define ICON_FA_FACE_LAUGH_SQUINT "\xef\x96\x9b"                     // U+f59b
#define ICON_FA_FACE_LAUGH_WINK "\xef\x96\x9c"                       // U+f59c
#define ICON_FA_FACE_MEH "\xef\x84\x9a"                              // U+f11a
#define ICON_FA_FACE_MEH_BLANK "\xef\x96\xa4"                        // U+f5a4
#define ICON_FA_FACE_ROLLING_EYES "\xef\x96\xa5"                     // U+f5a5
#define ICON_FA_FACE_SAD_CRY "\xef\x96\xb3"                          // U+f5b3
#define ICON_FA_FACE_SAD_TEAR "\xef\x96\xb4"                         // U+f5b4
#define ICON_FA_FACE_SMILE "\xef\x84\x98"                            // U+f118
#define ICON_FA_FACE_SMILE_BEAM "\xef\x96\xb8"                       // U+f5b8
#define ICON_FA_FACE_SMILE_WINK "\xef\x93\x9a"                       // U+f4da
#define ICON_FA_FACE_SURPRISE "\xef\x97\x82"                         // U+f5c2
#define ICON_FA_FACE_TIRED "\xef\x97\x88"                            // U+f5c8
#define ICON_FA_FAN "\xef\xa1\xa3"                                   // U+f863
#define ICON_FA_FAUCET "\xee\x80\x85"                                // U+e005
#define ICON_FA_FAUCET_DRIP "\xee\x80\x86"                           // U+e006
#define ICON_FA_FAX "\xef\x86\xac"                                   // U+f1ac
#define ICON_FA_FEATHER "\xef\x94\xad"                               // U+f52d
#define ICON_FA_FEATHER_POINTED "\xef\x95\xab"                       // U+f56b
#define ICON_FA_FERRY "\xee\x93\xaa"                                 // U+e4ea
#define ICON_FA_FILE "\xef\x85\x9b"                                  // U+f15b
#define ICON_FA_FILE_ARROW_DOWN "\xef\x95\xad"                       // U+f56d
#define ICON_FA_FILE_ARROW_UP "\xef\x95\xb4"                         // U+f574
#define ICON_FA_FILE_AUDIO "\xef\x87\x87"                            // U+f1c7
#define ICON_FA_FILE_CIRCLE_CHECK "\xee\x96\xa0"                     // U+e5a0
#define ICON_FA_FILE_CIRCLE_EXCLAMATION "\xee\x93\xab"               // U+e4eb
#define ICON_FA_FILE_CIRCLE_MINUS "\xee\x93\xad"                     // U+e4ed
#define ICON_FA_FILE_CIRCLE_PLUS "\xee\x92\x94"                      // U+e494
#define ICON_FA_FILE_CIRCLE_QUESTION "\xee\x93\xaf"                  // U+e4ef
#define ICON_FA_FILE_CIRCLE_XMARK "\xee\x96\xa1"                     // U+e5a1
#define ICON_FA_FILE_CODE "\xef\x87\x89"                             // U+f1c9
#define ICON_FA_FILE_CONTRACT "\xef\x95\xac"                         // U+f56c
#define ICON_FA_FILE_CSV "\xef\x9b\x9d"                              // U+f6dd
#define ICON_FA_FILE_EXCEL "\xef\x87\x83"                            // U+f1c3
#define ICON_FA_FILE_EXPORT "\xef\x95\xae"                           // U+f56e
#define ICON_FA_FILE_IMAGE "\xef\x87\x85"                            // U+f1c5
#define ICON_FA_FILE_IMPORT "\xef\x95\xaf"                           // U+f56f
#define ICON_FA_FILE_INVOICE "\xef\x95\xb0"                          // U+f570
#define ICON_FA_FILE_INVOICE_DOLLAR "\xef\x95\xb1"                   // U+f571
#define ICON_FA_FILE_LINES "\xef\x85\x9c"                            // U+f15c
#define ICON_FA_FILE_MEDICAL "\xef\x91\xb7"                          // U+f477
#define ICON_FA_FILE_PDF "\xef\x87\x81"                              // U+f1c1
#define ICON_FA_FILE_PEN "\xef\x8c\x9c"                              // U+f31c
#define ICON_FA_FILE_POWERPOINT "\xef\x87\x84"                       // U+f1c4
#define ICON_FA_FILE_PRESCRIPTION "\xef\x95\xb2"                     // U+f572
#define ICON_FA_FILE_SHIELD "\xee\x93\xb0"                           // U+e4f0
#define ICON_FA_FILE_SIGNATURE "\xef\x95\xb3"                        // U+f573
#define ICON_FA_FILE_VIDEO "\xef\x87\x88"                            // U+f1c8
#define ICON_FA_FILE_WAVEFORM "\xef\x91\xb8"                         // U+f478
#define ICON_FA_FILE_WORD "\xef\x87\x82"                             // U+f1c2
#define ICON_FA_FILE_ZIPPER "\xef\x87\x86"                           // U+f1c6
#define ICON_FA_FILL "\xef\x95\xb5"                                  // U+f575
#define ICON_FA_FILL_DRIP "\xef\x95\xb6"                             // U+f576
#define ICON_FA_FILM "\xef\x80\x88"                                  // U+f008
#define ICON_FA_FILTER "\xef\x82\xb0"                                // U+f0b0
#define ICON_FA_FILTER_CIRCLE_DOLLAR "\xef\x99\xa2"                  // U+f662
#define ICON_FA_FILTER_CIRCLE_XMARK "\xee\x85\xbb"                   // U+e17b
#define ICON_FA_FINGERPRINT "\xef\x95\xb7"                           // U+f577
#define ICON_FA_FIRE "\xef\x81\xad"                                  // U+f06d
#define ICON_FA_FIRE_BURNER "\xee\x93\xb1"                           // U+e4f1
#define ICON_FA_FIRE_EXTINGUISHER "\xef\x84\xb4"                     // U+f134
#define ICON_FA_FIRE_FLAME_CURVED "\xef\x9f\xa4"                     // U+f7e4
#define ICON_FA_FIRE_FLAME_SIMPLE "\xef\x91\xaa"                     // U+f46a
#define ICON_FA_FISH "\xef\x95\xb8"                                  // U+f578
#define ICON_FA_FISH_FINS "\xee\x93\xb2"                             // U+e4f2
#define ICON_FA_FLAG "\xef\x80\xa4"                                  // U+f024
#define ICON_FA_FLAG_CHECKERED "\xef\x84\x9e"                        // U+f11e
#define ICON_FA_FLAG_USA "\xef\x9d\x8d"                              // U+f74d
#define ICON_FA_FLASK "\xef\x83\x83"                                 // U+f0c3
#define ICON_FA_FLASK_VIAL "\xee\x93\xb3"                            // U+e4f3
#define ICON_FA_FLOPPY_DISK "\xef\x83\x87"                           // U+f0c7
#define ICON_FA_FLORIN_SIGN "\xee\x86\x84"                           // U+e184
#define ICON_FA_FOLDER "\xef\x81\xbb"                                // U+f07b
#define ICON_FA_FOLDER_CLOSED "\xee\x86\x85"                         // U+e185
#define ICON_FA_FOLDER_MINUS "\xef\x99\x9d"                          // U+f65d
#define ICON_FA_FOLDER_OPEN "\xef\x81\xbc"                           // U+f07c
#define ICON_FA_FOLDER_PLUS "\xef\x99\x9e"                           // U+f65e
#define ICON_FA_FOLDER_TREE "\xef\xa0\x82"                           // U+f802
#define ICON_FA_FONT "\xef\x80\xb1"                                  // U+f031
#define ICON_FA_FONT_AWESOME "\xef\x8a\xb4"                          // U+f2b4
#define ICON_FA_FOOTBALL "\xef\x91\x8e"                              // U+f44e
#define ICON_FA_FORWARD "\xef\x81\x8e"                               // U+f04e
#define ICON_FA_FORWARD_FAST "\xef\x81\x90"                          // U+f050
#define ICON_FA_FORWARD_STEP "\xef\x81\x91"                          // U+f051
#define ICON_FA_FRANC_SIGN "\xee\x86\x8f"                            // U+e18f
#define ICON_FA_FROG "\xef\x94\xae"                                  // U+f52e
#define ICON_FA_FUTBOL "\xef\x87\xa3"                                // U+f1e3
#define ICON_FA_G "G"                                                // U+0047
#define ICON_FA_GAMEPAD "\xef\x84\x9b"                               // U+f11b
#define ICON_FA_GAS_PUMP "\xef\x94\xaf"                              // U+f52f
#define ICON_FA_GAUGE "\xef\x98\xa4"                                 // U+f624
#define ICON_FA_GAUGE_HIGH "\xef\x98\xa5"                            // U+f625
#define ICON_FA_GAUGE_SIMPLE "\xef\x98\xa9"                          // U+f629
#define ICON_FA_GAUGE_SIMPLE_HIGH "\xef\x98\xaa"                     // U+f62a
#define ICON_FA_GAVEL "\xef\x83\xa3"                                 // U+f0e3
#define ICON_FA_GEAR "\xef\x80\x93"                                  // U+f013
#define ICON_FA_GEARS "\xef\x82\x85"                                 // U+f085
#define ICON_FA_GEM "\xef\x8e\xa5"                                   // U+f3a5
#define ICON_FA_GENDERLESS "\xef\x88\xad"                            // U+f22d
#define ICON_FA_GHOST "\xef\x9b\xa2"                                 // U+f6e2
#define ICON_FA_GIFT "\xef\x81\xab"                                  // U+f06b
#define ICON_FA_GIFTS "\xef\x9e\x9c"                                 // U+f79c
#define ICON_FA_GLASS_WATER "\xee\x93\xb4"                           // U+e4f4
#define ICON_FA_GLASS_WATER_DROPLET "\xee\x93\xb5"                   // U+e4f5
#define ICON_FA_GLASSES "\xef\x94\xb0"                               // U+f530
#define ICON_FA_GLOBE "\xef\x82\xac"                                 // U+f0ac
#define ICON_FA_GOLF_BALL_TEE "\xef\x91\x90"                         // U+f450
#define ICON_FA_GOPURAM "\xef\x99\xa4"                               // U+f664
#define ICON_FA_GRADUATION_CAP "\xef\x86\x9d"                        // U+f19d
#define ICON_FA_GREATER_THAN ">"                                     // U+003e
#define ICON_FA_GREATER_THAN_EQUAL "\xef\x94\xb2"                    // U+f532
#define ICON_FA_GRIP "\xef\x96\x8d"                                  // U+f58d
#define ICON_FA_GRIP_LINES "\xef\x9e\xa4"                            // U+f7a4
#define ICON_FA_GRIP_LINES_VERTICAL "\xef\x9e\xa5"                   // U+f7a5
#define ICON_FA_GRIP_VERTICAL "\xef\x96\x8e"                         // U+f58e
#define ICON_FA_GROUP_ARROWS_ROTATE "\xee\x93\xb6"                   // U+e4f6
#define ICON_FA_GUARANI_SIGN "\xee\x86\x9a"                          // U+e19a
#define ICON_FA_GUITAR "\xef\x9e\xa6"                                // U+f7a6
#define ICON_FA_GUN "\xee\x86\x9b"                                   // U+e19b
#define ICON_FA_H "H"                                                // U+0048
#define ICON_FA_HAMMER "\xef\x9b\xa3"                                // U+f6e3
#define ICON_FA_HAMSA "\xef\x99\xa5"                                 // U+f665
#define ICON_FA_HAND "\xef\x89\x96"                                  // U+f256
#define ICON_FA_HAND_BACK_FIST "\xef\x89\x95"                        // U+f255
#define ICON_FA_HAND_DOTS "\xef\x91\xa1"                             // U+f461
#define ICON_FA_HAND_FIST "\xef\x9b\x9e"                             // U+f6de
#define ICON_FA_HAND_HOLDING "\xef\x92\xbd"                          // U+f4bd
#define ICON_FA_HAND_HOLDING_DOLLAR "\xef\x93\x80"                   // U+f4c0
#define ICON_FA_HAND_HOLDING_DROPLET "\xef\x93\x81"                  // U+f4c1
#define ICON_FA_HAND_HOLDING_HAND "\xee\x93\xb7"                     // U+e4f7
#define ICON_FA_HAND_HOLDING_HEART "\xef\x92\xbe"                    // U+f4be
#define ICON_FA_HAND_HOLDING_MEDICAL "\xee\x81\x9c"                  // U+e05c
#define ICON_FA_HAND_LIZARD "\xef\x89\x98"                           // U+f258
#define ICON_FA_HAND_MIDDLE_FINGER "\xef\xa0\x86"                    // U+f806
#define ICON_FA_HAND_PEACE "\xef\x89\x9b"                            // U+f25b
#define ICON_FA_HAND_POINT_DOWN "\xef\x82\xa7"                       // U+f0a7
#define ICON_FA_HAND_POINT_LEFT "\xef\x82\xa5"                       // U+f0a5
#define ICON_FA_HAND_POINT_RIGHT "\xef\x82\xa4"                      // U+f0a4
#define ICON_FA_HAND_POINT_UP "\xef\x82\xa6"                         // U+f0a6
#define ICON_FA_HAND_POINTER "\xef\x89\x9a"                          // U+f25a
#define ICON_FA_HAND_SCISSORS "\xef\x89\x97"                         // U+f257
#define ICON_FA_HAND_SPARKLES "\xee\x81\x9d"                         // U+e05d
#define ICON_FA_HAND_SPOCK "\xef\x89\x99"                            // U+f259
#define ICON_FA_HANDCUFFS "\xee\x93\xb8"                             // U+e4f8
#define ICON_FA_HANDS "\xef\x8a\xa7"                                 // U+f2a7
#define ICON_FA_HANDS_ASL_INTERPRETING "\xef\x8a\xa3"                // U+f2a3
#define ICON_FA_HANDS_BOUND "\xee\x93\xb9"                           // U+e4f9
#define ICON_FA_HANDS_BUBBLES "\xee\x81\x9e"                         // U+e05e
#define ICON_FA_HANDS_CLAPPING "\xee\x86\xa8"                        // U+e1a8
#define ICON_FA_HANDS_HOLDING "\xef\x93\x82"                         // U+f4c2
#define ICON_FA_HANDS_HOLDING_CHILD "\xee\x93\xba"                   // U+e4fa
#define ICON_FA_HANDS_HOLDING_CIRCLE "\xee\x93\xbb"                  // U+e4fb
#define ICON_FA_HANDS_PRAYING "\xef\x9a\x84"                         // U+f684
#define ICON_FA_HANDSHAKE "\xef\x8a\xb5"                             // U+f2b5
#define ICON_FA_HANDSHAKE_ANGLE "\xef\x93\x84"                       // U+f4c4
#define ICON_FA_HANDSHAKE_SIMPLE "\xef\x93\x86"                      // U+f4c6
#define ICON_FA_HANDSHAKE_SIMPLE_SLASH "\xee\x81\x9f"                // U+e05f
#define ICON_FA_HANDSHAKE_SLASH "\xee\x81\xa0"                       // U+e060
#define ICON_FA_HANUKIAH "\xef\x9b\xa6"                              // U+f6e6
#define ICON_FA_HARD_DRIVE "\xef\x82\xa0"                            // U+f0a0
#define ICON_FA_HASHTAG "#"                                          // U+0023
#define ICON_FA_HAT_COWBOY "\xef\xa3\x80"                            // U+f8c0
#define ICON_FA_HAT_COWBOY_SIDE "\xef\xa3\x81"                       // U+f8c1
#define ICON_FA_HAT_WIZARD "\xef\x9b\xa8"                            // U+f6e8
#define ICON_FA_HEAD_SIDE_COUGH "\xee\x81\xa1"                       // U+e061
#define ICON_FA_HEAD_SIDE_COUGH_SLASH "\xee\x81\xa2"                 // U+e062
#define ICON_FA_HEAD_SIDE_MASK "\xee\x81\xa3"                        // U+e063
#define ICON_FA_HEAD_SIDE_VIRUS "\xee\x81\xa4"                       // U+e064
#define ICON_FA_HEADING "\xef\x87\x9c"                               // U+f1dc
#define ICON_FA_HEADPHONES "\xef\x80\xa5"                            // U+f025
#define ICON_FA_HEADPHONES_SIMPLE "\xef\x96\x8f"                     // U+f58f
#define ICON_FA_HEADSET "\xef\x96\x90"                               // U+f590
#define ICON_FA_HEART "\xef\x80\x84"                                 // U+f004
#define ICON_FA_HEART_CIRCLE_BOLT "\xee\x93\xbc"                     // U+e4fc
#define ICON_FA_HEART_CIRCLE_CHECK "\xee\x93\xbd"                    // U+e4fd
#define ICON_FA_HEART_CIRCLE_EXCLAMATION "\xee\x93\xbe"              // U+e4fe
#define ICON_FA_HEART_CIRCLE_MINUS "\xee\x93\xbf"                    // U+e4ff
#define ICON_FA_HEART_CIRCLE_PLUS "\xee\x94\x80"                     // U+e500
#define ICON_FA_HEART_CIRCLE_XMARK "\xee\x94\x81"                    // U+e501
#define ICON_FA_HEART_CRACK "\xef\x9e\xa9"                           // U+f7a9
#define ICON_FA_HEART_PULSE "\xef\x88\x9e"                           // U+f21e
#define ICON_FA_HELICOPTER "\xef\x94\xb3"                            // U+f533
#define ICON_FA_HELICOPTER_SYMBOL "\xee\x94\x82"                     // U+e502
#define ICON_FA_HELMET_SAFETY "\xef\xa0\x87"                         // U+f807
#define ICON_FA_HELMET_UN "\xee\x94\x83"                             // U+e503
#define ICON_FA_HIGHLIGHTER "\xef\x96\x91"                           // U+f591
#define ICON_FA_HILL_AVALANCHE "\xee\x94\x87"                        // U+e507
#define ICON_FA_HILL_ROCKSLIDE "\xee\x94\x88"                        // U+e508
#define ICON_FA_HIPPO "\xef\x9b\xad"                                 // U+f6ed
#define ICON_FA_HOCKEY_PUCK "\xef\x91\x93"                           // U+f453
#define ICON_FA_HOLLY_BERRY "\xef\x9e\xaa"                           // U+f7aa
#define ICON_FA_HORSE "\xef\x9b\xb0"                                 // U+f6f0
#define ICON_FA_HORSE_HEAD "\xef\x9e\xab"                            // U+f7ab
#define ICON_FA_HOSPITAL "\xef\x83\xb8"                              // U+f0f8
#define ICON_FA_HOSPITAL_USER "\xef\xa0\x8d"                         // U+f80d
#define ICON_FA_HOT_TUB_PERSON "\xef\x96\x93"                        // U+f593
#define ICON_FA_HOTDOG "\xef\xa0\x8f"                                // U+f80f
#define ICON_FA_HOTEL "\xef\x96\x94"                                 // U+f594
#define ICON_FA_HOURGLASS "\xef\x89\x94"                             // U+f254
#define ICON_FA_HOURGLASS_END "\xef\x89\x93"                         // U+f253
#define ICON_FA_HOURGLASS_HALF "\xef\x89\x92"                        // U+f252
#define ICON_FA_HOURGLASS_START "\xef\x89\x91"                       // U+f251
#define ICON_FA_HOUSE "\xef\x80\x95"                                 // U+f015
#define ICON_FA_HOUSE_CHIMNEY "\xee\x8e\xaf"                         // U+e3af
#define ICON_FA_HOUSE_CHIMNEY_CRACK "\xef\x9b\xb1"                   // U+f6f1
#define ICON_FA_HOUSE_CHIMNEY_MEDICAL "\xef\x9f\xb2"                 // U+f7f2
#define ICON_FA_HOUSE_CHIMNEY_USER "\xee\x81\xa5"                    // U+e065
#define ICON_FA_HOUSE_CHIMNEY_WINDOW "\xee\x80\x8d"                  // U+e00d
#define ICON_FA_HOUSE_CIRCLE_CHECK "\xee\x94\x89"                    // U+e509
#define ICON_FA_HOUSE_CIRCLE_EXCLAMATION "\xee\x94\x8a"              // U+e50a
#define ICON_FA_HOUSE_CIRCLE_XMARK "\xee\x94\x8b"                    // U+e50b
#define ICON_FA_HOUSE_CRACK "\xee\x8e\xb1"                           // U+e3b1
#define ICON_FA_HOUSE_FIRE "\xee\x94\x8c"                            // U+e50c
#define ICON_FA_HOUSE_FLAG "\xee\x94\x8d"                            // U+e50d
#define ICON_FA_HOUSE_FLOOD_WATER "\xee\x94\x8e"                     // U+e50e
#define ICON_FA_HOUSE_FLOOD_WATER_CIRCLE_ARROW_RIGHT "\xee\x94\x8f"  // U+e50f
#define ICON_FA_HOUSE_LAPTOP "\xee\x81\xa6"                          // U+e066
#define ICON_FA_HOUSE_LOCK "\xee\x94\x90"                            // U+e510
#define ICON_FA_HOUSE_MEDICAL "\xee\x8e\xb2"                         // U+e3b2
#define ICON_FA_HOUSE_MEDICAL_CIRCLE_CHECK "\xee\x94\x91"            // U+e511
#define ICON_FA_HOUSE_MEDICAL_CIRCLE_EXCLAMATION "\xee\x94\x92"      // U+e512
#define ICON_FA_HOUSE_MEDICAL_CIRCLE_XMARK "\xee\x94\x93"            // U+e513
#define ICON_FA_HOUSE_MEDICAL_FLAG "\xee\x94\x94"                    // U+e514
#define ICON_FA_HOUSE_SIGNAL "\xee\x80\x92"                          // U+e012
#define ICON_FA_HOUSE_TSUNAMI "\xee\x94\x95"                         // U+e515
#define ICON_FA_HOUSE_USER "\xee\x86\xb0"                            // U+e1b0
#define ICON_FA_HRYVNIA_SIGN "\xef\x9b\xb2"                          // U+f6f2
#define ICON_FA_HURRICANE "\xef\x9d\x91"                             // U+f751
#define ICON_FA_I "I"                                                // U+0049
#define ICON_FA_I_CURSOR "\xef\x89\x86"                              // U+f246
#define ICON_FA_ICE_CREAM "\xef\xa0\x90"                             // U+f810
#define ICON_FA_ICICLES "\xef\x9e\xad"                               // U+f7ad
#define ICON_FA_ICONS "\xef\xa1\xad"                                 // U+f86d
#define ICON_FA_ID_BADGE "\xef\x8b\x81"                              // U+f2c1
#define ICON_FA_ID_CARD "\xef\x8b\x82"                               // U+f2c2
#define ICON_FA_ID_CARD_CLIP "\xef\x91\xbf"                          // U+f47f
#define ICON_FA_IGLOO "\xef\x9e\xae"                                 // U+f7ae
#define ICON_FA_IMAGE "\xef\x80\xbe"                                 // U+f03e
#define ICON_FA_IMAGE_PORTRAIT "\xef\x8f\xa0"                        // U+f3e0
#define ICON_FA_IMAGES "\xef\x8c\x82"                                // U+f302
#define ICON_FA_INBOX "\xef\x80\x9c"                                 // U+f01c
#define ICON_FA_INDENT "\xef\x80\xbc"                                // U+f03c
#define ICON_FA_INDIAN_RUPEE_SIGN "\xee\x86\xbc"                     // U+e1bc
#define ICON_FA_INDUSTRY "\xef\x89\xb5"                              // U+f275
#define ICON_FA_INFINITY "\xef\x94\xb4"                              // U+f534
#define ICON_FA_INFO "\xef\x84\xa9"                                  // U+f129
#define ICON_FA_ITALIC "\xef\x80\xb3"                                // U+f033
#define ICON_FA_J "J"                                                // U+004a
#define ICON_FA_JAR "\xee\x94\x96"                                   // U+e516
#define ICON_FA_JAR_WHEAT "\xee\x94\x97"                             // U+e517
#define ICON_FA_JEDI "\xef\x99\xa9"                                  // U+f669
#define ICON_FA_JET_FIGHTER "\xef\x83\xbb"                           // U+f0fb
#define ICON_FA_JET_FIGHTER_UP "\xee\x94\x98"                        // U+e518
#define ICON_FA_JOINT "\xef\x96\x95"                                 // U+f595
#define ICON_FA_JUG_DETERGENT "\xee\x94\x99"                         // U+e519
#define ICON_FA_K "K"                                                // U+004b
#define ICON_FA_KAABA "\xef\x99\xab"                                 // U+f66b
#define ICON_FA_KEY "\xef\x82\x84"                                   // U+f084
#define ICON_FA_KEYBOARD "\xef\x84\x9c"                              // U+f11c
#define ICON_FA_KHANDA "\xef\x99\xad"                                // U+f66d
#define ICON_FA_KIP_SIGN "\xee\x87\x84"                              // U+e1c4
#define ICON_FA_KIT_MEDICAL "\xef\x91\xb9"                           // U+f479
#define ICON_FA_KITCHEN_SET "\xee\x94\x9a"                           // U+e51a
#define ICON_FA_KIWI_BIRD "\xef\x94\xb5"                             // U+f535
#define ICON_FA_L "L"                                                // U+004c
#define ICON_FA_LAND_MINE_ON "\xee\x94\x9b"                          // U+e51b
#define ICON_FA_LANDMARK "\xef\x99\xaf"                              // U+f66f
#define ICON_FA_LANDMARK_DOME "\xef\x9d\x92"                         // U+f752
#define ICON_FA_LANDMARK_FLAG "\xee\x94\x9c"                         // U+e51c
#define ICON_FA_LANGUAGE "\xef\x86\xab"                              // U+f1ab
#define ICON_FA_LAPTOP "\xef\x84\x89"                                // U+f109
#define ICON_FA_LAPTOP_CODE "\xef\x97\xbc"                           // U+f5fc
#define ICON_FA_LAPTOP_FILE "\xee\x94\x9d"                           // U+e51d
#define ICON_FA_LAPTOP_MEDICAL "\xef\xa0\x92"                        // U+f812
#define ICON_FA_LARI_SIGN "\xee\x87\x88"                             // U+e1c8
#define ICON_FA_LAYER_GROUP "\xef\x97\xbd"                           // U+f5fd
#define ICON_FA_LEAF "\xef\x81\xac"                                  // U+f06c
#define ICON_FA_LEFT_LONG "\xef\x8c\x8a"                             // U+f30a
#define ICON_FA_LEFT_RIGHT "\xef\x8c\xb7"                            // U+f337
#define ICON_FA_LEMON "\xef\x82\x94"                                 // U+f094
#define ICON_FA_LESS_THAN "<"                                        // U+003c
#define ICON_FA_LESS_THAN_EQUAL "\xef\x94\xb7"                       // U+f537
#define ICON_FA_LIFE_RING "\xef\x87\x8d"                             // U+f1cd
#define ICON_FA_LIGHTBULB "\xef\x83\xab"                             // U+f0eb
#define ICON_FA_LINES_LEANING "\xee\x94\x9e"                         // U+e51e
#define ICON_FA_LINK "\xef\x83\x81"                                  // U+f0c1
#define ICON_FA_LINK_SLASH "\xef\x84\xa7"                            // U+f127
#define ICON_FA_LIRA_SIGN "\xef\x86\x95"                             // U+f195
#define ICON_FA_LIST "\xef\x80\xba"                                  // U+f03a
#define ICON_FA_LIST_CHECK "\xef\x82\xae"                            // U+f0ae
#define ICON_FA_LIST_OL "\xef\x83\x8b"                               // U+f0cb
#define ICON_FA_LIST_UL "\xef\x83\x8a"                               // U+f0ca
#define ICON_FA_LITECOIN_SIGN "\xee\x87\x93"                         // U+e1d3
#define ICON_FA_LOCATION_ARROW "\xef\x84\xa4"                        // U+f124
#define ICON_FA_LOCATION_CROSSHAIRS "\xef\x98\x81"                   // U+f601
#define ICON_FA_LOCATION_DOT "\xef\x8f\x85"                          // U+f3c5
#define ICON_FA_LOCATION_PIN "\xef\x81\x81"                          // U+f041
#define ICON_FA_LOCATION_PIN_LOCK "\xee\x94\x9f"                     // U+e51f
#define ICON_FA_LOCK "\xef\x80\xa3"                                  // U+f023
#define ICON_FA_LOCK_OPEN "\xef\x8f\x81"                             // U+f3c1
#define ICON_FA_LOCUST "\xee\x94\xa0"                                // U+e520
#define ICON_FA_LUNGS "\xef\x98\x84"                                 // U+f604
#define ICON_FA_LUNGS_VIRUS "\xee\x81\xa7"                           // U+e067
#define ICON_FA_M "M"                                                // U+004d
#define ICON_FA_MAGNET "\xef\x81\xb6"                                // U+f076
#define ICON_FA_MAGNIFYING_GLASS "\xef\x80\x82"                      // U+f002
#define ICON_FA_MAGNIFYING_GLASS_ARROW_RIGHT "\xee\x94\xa1"          // U+e521
#define ICON_FA_MAGNIFYING_GLASS_CHART "\xee\x94\xa2"                // U+e522
#define ICON_FA_MAGNIFYING_GLASS_DOLLAR "\xef\x9a\x88"               // U+f688
#define ICON_FA_MAGNIFYING_GLASS_LOCATION "\xef\x9a\x89"             // U+f689
#define ICON_FA_MAGNIFYING_GLASS_MINUS "\xef\x80\x90"                // U+f010
#define ICON_FA_MAGNIFYING_GLASS_PLUS "\xef\x80\x8e"                 // U+f00e
#define ICON_FA_MANAT_SIGN "\xee\x87\x95"                            // U+e1d5
#define ICON_FA_MAP "\xef\x89\xb9"                                   // U+f279
#define ICON_FA_MAP_LOCATION "\xef\x96\x9f"                          // U+f59f
#define ICON_FA_MAP_LOCATION_DOT "\xef\x96\xa0"                      // U+f5a0
#define ICON_FA_MAP_PIN "\xef\x89\xb6"                               // U+f276
#define ICON_FA_MARKER "\xef\x96\xa1"                                // U+f5a1
#define ICON_FA_MARS "\xef\x88\xa2"                                  // U+f222
#define ICON_FA_MARS_AND_VENUS "\xef\x88\xa4"                        // U+f224
#define ICON_FA_MARS_AND_VENUS_BURST "\xee\x94\xa3"                  // U+e523
#define ICON_FA_MARS_DOUBLE "\xef\x88\xa7"                           // U+f227
#define ICON_FA_MARS_STROKE "\xef\x88\xa9"                           // U+f229
#define ICON_FA_MARS_STROKE_RIGHT "\xef\x88\xab"                     // U+f22b
#define ICON_FA_MARS_STROKE_UP "\xef\x88\xaa"                        // U+f22a
#define ICON_FA_MARTINI_GLASS "\xef\x95\xbb"                         // U+f57b
#define ICON_FA_MARTINI_GLASS_CITRUS "\xef\x95\xa1"                  // U+f561
#define ICON_FA_MARTINI_GLASS_EMPTY "\xef\x80\x80"                   // U+f000
#define ICON_FA_MASK "\xef\x9b\xba"                                  // U+f6fa
#define ICON_FA_MASK_FACE "\xee\x87\x97"                             // U+e1d7
#define ICON_FA_MASK_VENTILATOR "\xee\x94\xa4"                       // U+e524
#define ICON_FA_MASKS_THEATER "\xef\x98\xb0"                         // U+f630
#define ICON_FA_MATTRESS_PILLOW "\xee\x94\xa5"                       // U+e525
#define ICON_FA_MAXIMIZE "\xef\x8c\x9e"                              // U+f31e
#define ICON_FA_MEDAL "\xef\x96\xa2"                                 // U+f5a2
#define ICON_FA_MEMORY "\xef\x94\xb8"                                // U+f538
#define ICON_FA_MENORAH "\xef\x99\xb6"                               // U+f676
#define ICON_FA_MERCURY "\xef\x88\xa3"                               // U+f223
#define ICON_FA_MESSAGE "\xef\x89\xba"                               // U+f27a
#define ICON_FA_METEOR "\xef\x9d\x93"                                // U+f753
#define ICON_FA_MICROCHIP "\xef\x8b\x9b"                             // U+f2db
#define ICON_FA_MICROPHONE "\xef\x84\xb0"                            // U+f130
#define ICON_FA_MICROPHONE_LINES "\xef\x8f\x89"                      // U+f3c9
#define ICON_FA_MICROPHONE_LINES_SLASH "\xef\x94\xb9"                // U+f539
#define ICON_FA_MICROPHONE_SLASH "\xef\x84\xb1"                      // U+f131
#define ICON_FA_MICROSCOPE "\xef\x98\x90"                            // U+f610
#define ICON_FA_MILL_SIGN "\xee\x87\xad"                             // U+e1ed
#define ICON_FA_MINIMIZE "\xef\x9e\x8c"                              // U+f78c
#define ICON_FA_MINUS "\xef\x81\xa8"                                 // U+f068
#define ICON_FA_MITTEN "\xef\x9e\xb5"                                // U+f7b5
#define ICON_FA_MOBILE "\xef\x8f\x8e"                                // U+f3ce
#define ICON_FA_MOBILE_BUTTON "\xef\x84\x8b"                         // U+f10b
#define ICON_FA_MOBILE_RETRO "\xee\x94\xa7"                          // U+e527
#define ICON_FA_MOBILE_SCREEN "\xef\x8f\x8f"                         // U+f3cf
#define ICON_FA_MOBILE_SCREEN_BUTTON "\xef\x8f\x8d"                  // U+f3cd
#define ICON_FA_MONEY_BILL "\xef\x83\x96"                            // U+f0d6
#define ICON_FA_MONEY_BILL_1 "\xef\x8f\x91"                          // U+f3d1
#define ICON_FA_MONEY_BILL_1_WAVE "\xef\x94\xbb"                     // U+f53b
#define ICON_FA_MONEY_BILL_TRANSFER "\xee\x94\xa8"                   // U+e528
#define ICON_FA_MONEY_BILL_TREND_UP "\xee\x94\xa9"                   // U+e529
#define ICON_FA_MONEY_BILL_WAVE "\xef\x94\xba"                       // U+f53a
#define ICON_FA_MONEY_BILL_WHEAT "\xee\x94\xaa"                      // U+e52a
#define ICON_FA_MONEY_BILLS "\xee\x87\xb3"                           // U+e1f3
#define ICON_FA_MONEY_CHECK "\xef\x94\xbc"                           // U+f53c
#define ICON_FA_MONEY_CHECK_DOLLAR "\xef\x94\xbd"                    // U+f53d
#define ICON_FA_MONUMENT "\xef\x96\xa6"                              // U+f5a6
#define ICON_FA_MOON "\xef\x86\x86"                                  // U+f186
#define ICON_FA_MORTAR_PESTLE "\xef\x96\xa7"                         // U+f5a7
#define ICON_FA_MOSQUE "\xef\x99\xb8"                                // U+f678
#define ICON_FA_MOSQUITO "\xee\x94\xab"                              // U+e52b
#define ICON_FA_MOSQUITO_NET "\xee\x94\xac"                          // U+e52c
#define ICON_FA_MOTORCYCLE "\xef\x88\x9c"                            // U+f21c
#define ICON_FA_MOUND "\xee\x94\xad"                                 // U+e52d
#define ICON_FA_MOUNTAIN "\xef\x9b\xbc"                              // U+f6fc
#define ICON_FA_MOUNTAIN_CITY "\xee\x94\xae"                         // U+e52e
#define ICON_FA_MOUNTAIN_SUN "\xee\x94\xaf"                          // U+e52f
#define ICON_FA_MUG_HOT "\xef\x9e\xb6"                               // U+f7b6
#define ICON_FA_MUG_SAUCER "\xef\x83\xb4"                            // U+f0f4
#define ICON_FA_MUSIC "\xef\x80\x81"                                 // U+f001
#define ICON_FA_N "N"                                                // U+004e
#define ICON_FA_NAIRA_SIGN "\xee\x87\xb6"                            // U+e1f6
#define ICON_FA_NETWORK_WIRED "\xef\x9b\xbf"                         // U+f6ff
#define ICON_FA_NEUTER "\xef\x88\xac"                                // U+f22c
#define ICON_FA_NEWSPAPER "\xef\x87\xaa"                             // U+f1ea
#define ICON_FA_NOT_EQUAL "\xef\x94\xbe"                             // U+f53e
#define ICON_FA_NOTDEF "\xee\x87\xbe"                                // U+e1fe
#define ICON_FA_NOTE_STICKY "\xef\x89\x89"                           // U+f249
#define ICON_FA_NOTES_MEDICAL "\xef\x92\x81"                         // U+f481
#define ICON_FA_O "O"                                                // U+004f
#define ICON_FA_OBJECT_GROUP "\xef\x89\x87"                          // U+f247
#define ICON_FA_OBJECT_UNGROUP "\xef\x89\x88"                        // U+f248
#define ICON_FA_OIL_CAN "\xef\x98\x93"                               // U+f613
#define ICON_FA_OIL_WELL "\xee\x94\xb2"                              // U+e532
#define ICON_FA_OM "\xef\x99\xb9"                                    // U+f679
#define ICON_FA_OTTER "\xef\x9c\x80"                                 // U+f700
#define ICON_FA_OUTDENT "\xef\x80\xbb"                               // U+f03b
#define ICON_FA_P "P"                                                // U+0050
#define ICON_FA_PAGER "\xef\xa0\x95"                                 // U+f815
#define ICON_FA_PAINT_ROLLER "\xef\x96\xaa"                          // U+f5aa
#define ICON_FA_PAINTBRUSH "\xef\x87\xbc"                            // U+f1fc
#define ICON_FA_PALETTE "\xef\x94\xbf"                               // U+f53f
#define ICON_FA_PALLET "\xef\x92\x82"                                // U+f482
#define ICON_FA_PANORAMA "\xee\x88\x89"                              // U+e209
#define ICON_FA_PAPER_PLANE "\xef\x87\x98"                           // U+f1d8
#define ICON_FA_PAPERCLIP "\xef\x83\x86"                             // U+f0c6
#define ICON_FA_PARACHUTE_BOX "\xef\x93\x8d"                         // U+f4cd
#define ICON_FA_PARAGRAPH "\xef\x87\x9d"                             // U+f1dd
#define ICON_FA_PASSPORT "\xef\x96\xab"                              // U+f5ab
#define ICON_FA_PASTE "\xef\x83\xaa"                                 // U+f0ea
#define ICON_FA_PAUSE "\xef\x81\x8c"                                 // U+f04c
#define ICON_FA_PAW "\xef\x86\xb0"                                   // U+f1b0
#define ICON_FA_PEACE "\xef\x99\xbc"                                 // U+f67c
#define ICON_FA_PEN "\xef\x8c\x84"                                   // U+f304
#define ICON_FA_PEN_CLIP "\xef\x8c\x85"                              // U+f305
#define ICON_FA_PEN_FANCY "\xef\x96\xac"                             // U+f5ac
#define ICON_FA_PEN_NIB "\xef\x96\xad"                               // U+f5ad
#define ICON_FA_PEN_RULER "\xef\x96\xae"                             // U+f5ae
#define ICON_FA_PEN_TO_SQUARE "\xef\x81\x84"                         // U+f044
#define ICON_FA_PENCIL "\xef\x8c\x83"                                // U+f303
#define ICON_FA_PEOPLE_ARROWS "\xee\x81\xa8"                         // U+e068
#define ICON_FA_PEOPLE_CARRY_BOX "\xef\x93\x8e"                      // U+f4ce
#define ICON_FA_PEOPLE_GROUP "\xee\x94\xb3"                          // U+e533
#define ICON_FA_PEOPLE_LINE "\xee\x94\xb4"                           // U+e534
#define ICON_FA_PEOPLE_PULLING "\xee\x94\xb5"                        // U+e535
#define ICON_FA_PEOPLE_ROBBERY "\xee\x94\xb6"                        // U+e536
#define ICON_FA_PEOPLE_ROOF "\xee\x94\xb7"                           // U+e537
#define ICON_FA_PEPPER_HOT "\xef\xa0\x96"                            // U+f816
#define ICON_FA_PERCENT "%"                                          // U+0025
#define ICON_FA_PERSON "\xef\x86\x83"                                // U+f183
#define ICON_FA_PERSON_ARROW_DOWN_TO_LINE "\xee\x94\xb8"             // U+e538
#define ICON_FA_PERSON_ARROW_UP_FROM_LINE "\xee\x94\xb9"             // U+e539
#define ICON_FA_PERSON_BIKING "\xef\xa1\x8a"                         // U+f84a
#define ICON_FA_PERSON_BOOTH "\xef\x9d\x96"                          // U+f756
#define ICON_FA_PERSON_BREASTFEEDING "\xee\x94\xba"                  // U+e53a
#define ICON_FA_PERSON_BURST "\xee\x94\xbb"                          // U+e53b
#define ICON_FA_PERSON_CANE "\xee\x94\xbc"                           // U+e53c
#define ICON_FA_PERSON_CHALKBOARD "\xee\x94\xbd"                     // U+e53d
#define ICON_FA_PERSON_CIRCLE_CHECK "\xee\x94\xbe"                   // U+e53e
#define ICON_FA_PERSON_CIRCLE_EXCLAMATION "\xee\x94\xbf"             // U+e53f
#define ICON_FA_PERSON_CIRCLE_MINUS "\xee\x95\x80"                   // U+e540
#define ICON_FA_PERSON_CIRCLE_PLUS "\xee\x95\x81"                    // U+e541
#define ICON_FA_PERSON_CIRCLE_QUESTION "\xee\x95\x82"                // U+e542
#define ICON_FA_PERSON_CIRCLE_XMARK "\xee\x95\x83"                   // U+e543
#define ICON_FA_PERSON_DIGGING "\xef\xa1\x9e"                        // U+f85e
#define ICON_FA_PERSON_DOTS_FROM_LINE "\xef\x91\xb0"                 // U+f470
#define ICON_FA_PERSON_DRESS "\xef\x86\x82"                          // U+f182
#define ICON_FA_PERSON_DRESS_BURST "\xee\x95\x84"                    // U+e544
#define ICON_FA_PERSON_DROWNING "\xee\x95\x85"                       // U+e545
#define ICON_FA_PERSON_FALLING "\xee\x95\x86"                        // U+e546
#define ICON_FA_PERSON_FALLING_BURST "\xee\x95\x87"                  // U+e547
#define ICON_FA_PERSON_HALF_DRESS "\xee\x95\x88"                     // U+e548
#define ICON_FA_PERSON_HARASSING "\xee\x95\x89"                      // U+e549
#define ICON_FA_PERSON_HIKING "\xef\x9b\xac"                         // U+f6ec
#define ICON_FA_PERSON_MILITARY_POINTING "\xee\x95\x8a"              // U+e54a
#define ICON_FA_PERSON_MILITARY_RIFLE "\xee\x95\x8b"                 // U+e54b
#define ICON_FA_PERSON_MILITARY_TO_PERSON "\xee\x95\x8c"             // U+e54c
#define ICON_FA_PERSON_PRAYING "\xef\x9a\x83"                        // U+f683
#define ICON_FA_PERSON_PREGNANT "\xee\x8c\x9e"                       // U+e31e
#define ICON_FA_PERSON_RAYS "\xee\x95\x8d"                           // U+e54d
#define ICON_FA_PERSON_RIFLE "\xee\x95\x8e"                          // U+e54e
#define ICON_FA_PERSON_RUNNING "\xef\x9c\x8c"                        // U+f70c
#define ICON_FA_PERSON_SHELTER "\xee\x95\x8f"                        // U+e54f
#define ICON_FA_PERSON_SKATING "\xef\x9f\x85"                        // U+f7c5
#define ICON_FA_PERSON_SKIING "\xef\x9f\x89"                         // U+f7c9
#define ICON_FA_PERSON_SKIING_NORDIC "\xef\x9f\x8a"                  // U+f7ca
#define ICON_FA_PERSON_SNOWBOARDING "\xef\x9f\x8e"                   // U+f7ce
#define ICON_FA_PERSON_SWIMMING "\xef\x97\x84"                       // U+f5c4
#define ICON_FA_PERSON_THROUGH_WINDOW "\xee\x96\xa9"                 // U+e5a9
#define ICON_FA_PERSON_WALKING "\xef\x95\x94"                        // U+f554
#define ICON_FA_PERSON_WALKING_ARROW_LOOP_LEFT "\xee\x95\x91"        // U+e551
#define ICON_FA_PERSON_WALKING_ARROW_RIGHT "\xee\x95\x92"            // U+e552
#define ICON_FA_PERSON_WALKING_DASHED_LINE_ARROW_RIGHT "\xee\x95\x93"// U+e553
#define ICON_FA_PERSON_WALKING_LUGGAGE "\xee\x95\x94"                // U+e554
#define ICON_FA_PERSON_WALKING_WITH_CANE "\xef\x8a\x9d"              // U+f29d
#define ICON_FA_PESETA_SIGN "\xee\x88\xa1"                           // U+e221
#define ICON_FA_PESO_SIGN "\xee\x88\xa2"                             // U+e222
#define ICON_FA_PHONE "\xef\x82\x95"                                 // U+f095
#define ICON_FA_PHONE_FLIP "\xef\xa1\xb9"                            // U+f879
#define ICON_FA_PHONE_SLASH "\xef\x8f\x9d"                           // U+f3dd
#define ICON_FA_PHONE_VOLUME "\xef\x8a\xa0"                          // U+f2a0
#define ICON_FA_PHOTO_FILM "\xef\xa1\xbc"                            // U+f87c
#define ICON_FA_PIGGY_BANK "\xef\x93\x93"                            // U+f4d3
#define ICON_FA_PILLS "\xef\x92\x84"                                 // U+f484
#define ICON_FA_PIZZA_SLICE "\xef\xa0\x98"                           // U+f818
#define ICON_FA_PLACE_OF_WORSHIP "\xef\x99\xbf"                      // U+f67f
#define ICON_FA_PLANE "\xef\x81\xb2"                                 // U+f072
#define ICON_FA_PLANE_ARRIVAL "\xef\x96\xaf"                         // U+f5af
#define ICON_FA_PLANE_CIRCLE_CHECK "\xee\x95\x95"                    // U+e555
#define ICON_FA_PLANE_CIRCLE_EXCLAMATION "\xee\x95\x96"              // U+e556
#define ICON_FA_PLANE_CIRCLE_XMARK "\xee\x95\x97"                    // U+e557
#define ICON_FA_PLANE_DEPARTURE "\xef\x96\xb0"                       // U+f5b0
#define ICON_FA_PLANE_LOCK "\xee\x95\x98"                            // U+e558
#define ICON_FA_PLANE_SLASH "\xee\x81\xa9"                           // U+e069
#define ICON_FA_PLANE_UP "\xee\x88\xad"                              // U+e22d
#define ICON_FA_PLANT_WILT "\xee\x96\xaa"                            // U+e5aa
#define ICON_FA_PLATE_WHEAT "\xee\x95\x9a"                           // U+e55a
#define ICON_FA_PLAY "\xef\x81\x8b"                                  // U+f04b
#define ICON_FA_PLUG "\xef\x87\xa6"                                  // U+f1e6
#define ICON_FA_PLUG_CIRCLE_BOLT "\xee\x95\x9b"                      // U+e55b
#define ICON_FA_PLUG_CIRCLE_CHECK "\xee\x95\x9c"                     // U+e55c
#define ICON_FA_PLUG_CIRCLE_EXCLAMATION "\xee\x95\x9d"               // U+e55d
#define ICON_FA_PLUG_CIRCLE_MINUS "\xee\x95\x9e"                     // U+e55e
#define ICON_FA_PLUG_CIRCLE_PLUS "\xee\x95\x9f"                      // U+e55f
#define ICON_FA_PLUG_CIRCLE_XMARK "\xee\x95\xa0"                     // U+e560
#define ICON_FA_PLUS "+"                                             // U+002b
#define ICON_FA_PLUS_MINUS "\xee\x90\xbc"                            // U+e43c
#define ICON_FA_PODCAST "\xef\x8b\x8e"                               // U+f2ce
#define ICON_FA_POO "\xef\x8b\xbe"                                   // U+f2fe
#define ICON_FA_POO_STORM "\xef\x9d\x9a"                             // U+f75a
#define ICON_FA_POOP "\xef\x98\x99"                                  // U+f619
#define ICON_FA_POWER_OFF "\xef\x80\x91"                             // U+f011
#define ICON_FA_PRESCRIPTION "\xef\x96\xb1"                          // U+f5b1
#define ICON_FA_PRESCRIPTION_BOTTLE "\xef\x92\x85"                   // U+f485
#define ICON_FA_PRESCRIPTION_BOTTLE_MEDICAL "\xef\x92\x86"           // U+f486
#define ICON_FA_PRINT "\xef\x80\xaf"                                 // U+f02f
#define ICON_FA_PUMP_MEDICAL "\xee\x81\xaa"                          // U+e06a
#define ICON_FA_PUMP_SOAP "\xee\x81\xab"                             // U+e06b
#define ICON_FA_PUZZLE_PIECE "\xef\x84\xae"                          // U+f12e
#define ICON_FA_Q "Q"                                                // U+0051
#define ICON_FA_QRCODE "\xef\x80\xa9"                                // U+f029
#define ICON_FA_QUESTION "?"                                         // U+003f
#define ICON_FA_QUOTE_LEFT "\xef\x84\x8d"                            // U+f10d
#define ICON_FA_QUOTE_RIGHT "\xef\x84\x8e"                           // U+f10e
#define ICON_FA_R "R"                                                // U+0052
#define ICON_FA_RADIATION "\xef\x9e\xb9"                             // U+f7b9
#define ICON_FA_RADIO "\xef\xa3\x97"                                 // U+f8d7
#define ICON_FA_RAINBOW "\xef\x9d\x9b"                               // U+f75b
#define ICON_FA_RANKING_STAR "\xee\x95\xa1"                          // U+e561
#define ICON_FA_RECEIPT "\xef\x95\x83"                               // U+f543
#define ICON_FA_RECORD_VINYL "\xef\xa3\x99"                          // U+f8d9
#define ICON_FA_RECTANGLE_AD "\xef\x99\x81"                          // U+f641
#define ICON_FA_RECTANGLE_LIST "\xef\x80\xa2"                        // U+f022
#define ICON_FA_RECTANGLE_XMARK "\xef\x90\x90"                       // U+f410
#define ICON_FA_RECYCLE "\xef\x86\xb8"                               // U+f1b8
#define ICON_FA_REGISTERED "\xef\x89\x9d"                            // U+f25d
#define ICON_FA_REPEAT "\xef\x8d\xa3"                                // U+f363
#define ICON_FA_REPLY "\xef\x8f\xa5"                                 // U+f3e5
#define ICON_FA_REPLY_ALL "\xef\x84\xa2"                             // U+f122
#define ICON_FA_REPUBLICAN "\xef\x9d\x9e"                            // U+f75e
#define ICON_FA_RESTROOM "\xef\x9e\xbd"                              // U+f7bd
#define ICON_FA_RETWEET "\xef\x81\xb9"                               // U+f079
#define ICON_FA_RIBBON "\xef\x93\x96"                                // U+f4d6
#define ICON_FA_RIGHT_FROM_BRACKET "\xef\x8b\xb5"                    // U+f2f5
#define ICON_FA_RIGHT_LEFT "\xef\x8d\xa2"                            // U+f362
#define ICON_FA_RIGHT_LONG "\xef\x8c\x8b"                            // U+f30b
#define ICON_FA_RIGHT_TO_BRACKET "\xef\x8b\xb6"                      // U+f2f6
#define ICON_FA_RING "\xef\x9c\x8b"                                  // U+f70b
#define ICON_FA_ROAD "\xef\x80\x98"                                  // U+f018
#define ICON_FA_ROAD_BARRIER "\xee\x95\xa2"                          // U+e562
#define ICON_FA_ROAD_BRIDGE "\xee\x95\xa3"                           // U+e563
#define ICON_FA_ROAD_CIRCLE_CHECK "\xee\x95\xa4"                     // U+e564
#define ICON_FA_ROAD_CIRCLE_EXCLAMATION "\xee\x95\xa5"               // U+e565
#define ICON_FA_ROAD_CIRCLE_XMARK "\xee\x95\xa6"                     // U+e566
#define ICON_FA_ROAD_LOCK "\xee\x95\xa7"                             // U+e567
#define ICON_FA_ROAD_SPIKES "\xee\x95\xa8"                           // U+e568
#define ICON_FA_ROBOT "\xef\x95\x84"                                 // U+f544
#define ICON_FA_ROCKET "\xef\x84\xb5"                                // U+f135
#define ICON_FA_ROTATE "\xef\x8b\xb1"                                // U+f2f1
#define ICON_FA_ROTATE_LEFT "\xef\x8b\xaa"                           // U+f2ea
#define ICON_FA_ROTATE_RIGHT "\xef\x8b\xb9"                          // U+f2f9
#define ICON_FA_ROUTE "\xef\x93\x97"                                 // U+f4d7
#define ICON_FA_RSS "\xef\x82\x9e"                                   // U+f09e
#define ICON_FA_RUBLE_SIGN "\xef\x85\x98"                            // U+f158
#define ICON_FA_RUG "\xee\x95\xa9"                                   // U+e569
#define ICON_FA_RULER "\xef\x95\x85"                                 // U+f545
#define ICON_FA_RULER_COMBINED "\xef\x95\x86"                        // U+f546
#define ICON_FA_RULER_HORIZONTAL "\xef\x95\x87"                      // U+f547
#define ICON_FA_RULER_VERTICAL "\xef\x95\x88"                        // U+f548
#define ICON_FA_RUPEE_SIGN "\xef\x85\x96"                            // U+f156
#define ICON_FA_RUPIAH_SIGN "\xee\x88\xbd"                           // U+e23d
#define ICON_FA_S "S"                                                // U+0053
#define ICON_FA_SACK_DOLLAR "\xef\xa0\x9d"                           // U+f81d
#define ICON_FA_SACK_XMARK "\xee\x95\xaa"                            // U+e56a
#define ICON_FA_SAILBOAT "\xee\x91\x85"                              // U+e445
#define ICON_FA_SATELLITE "\xef\x9e\xbf"                             // U+f7bf
#define ICON_FA_SATELLITE_DISH "\xef\x9f\x80"                        // U+f7c0
#define ICON_FA_SCALE_BALANCED "\xef\x89\x8e"                        // U+f24e
#define ICON_FA_SCALE_UNBALANCED "\xef\x94\x95"                      // U+f515
#define ICON_FA_SCALE_UNBALANCED_FLIP "\xef\x94\x96"                 // U+f516
#define ICON_FA_SCHOOL "\xef\x95\x89"                                // U+f549
#define ICON_FA_SCHOOL_CIRCLE_CHECK "\xee\x95\xab"                   // U+e56b
#define ICON_FA_SCHOOL_CIRCLE_EXCLAMATION "\xee\x95\xac"             // U+e56c
#define ICON_FA_SCHOOL_CIRCLE_XMARK "\xee\x95\xad"                   // U+e56d
#define ICON_FA_SCHOOL_FLAG "\xee\x95\xae"                           // U+e56e
#define ICON_FA_SCHOOL_LOCK "\xee\x95\xaf"                           // U+e56f
#define ICON_FA_SCISSORS "\xef\x83\x84"                              // U+f0c4
#define ICON_FA_SCREWDRIVER "\xef\x95\x8a"                           // U+f54a
#define ICON_FA_SCREWDRIVER_WRENCH "\xef\x9f\x99"                    // U+f7d9
#define ICON_FA_SCROLL "\xef\x9c\x8e"                                // U+f70e
#define ICON_FA_SCROLL_TORAH "\xef\x9a\xa0"                          // U+f6a0
#define ICON_FA_SD_CARD "\xef\x9f\x82"                               // U+f7c2
#define ICON_FA_SECTION "\xee\x91\x87"                               // U+e447
#define ICON_FA_SEEDLING "\xef\x93\x98"                              // U+f4d8
#define ICON_FA_SERVER "\xef\x88\xb3"                                // U+f233
#define ICON_FA_SHAPES "\xef\x98\x9f"                                // U+f61f
#define ICON_FA_SHARE "\xef\x81\xa4"                                 // U+f064
#define ICON_FA_SHARE_FROM_SQUARE "\xef\x85\x8d"                     // U+f14d
#define ICON_FA_SHARE_NODES "\xef\x87\xa0"                           // U+f1e0
#define ICON_FA_SHEET_PLASTIC "\xee\x95\xb1"                         // U+e571
#define ICON_FA_SHEKEL_SIGN "\xef\x88\x8b"                           // U+f20b
#define ICON_FA_SHIELD "\xef\x84\xb2"                                // U+f132
#define ICON_FA_SHIELD_CAT "\xee\x95\xb2"                            // U+e572
#define ICON_FA_SHIELD_DOG "\xee\x95\xb3"                            // U+e573
#define ICON_FA_SHIELD_HALVED "\xef\x8f\xad"                         // U+f3ed
#define ICON_FA_SHIELD_HEART "\xee\x95\xb4"                          // U+e574
#define ICON_FA_SHIELD_VIRUS "\xee\x81\xac"                          // U+e06c
#define ICON_FA_SHIP "\xef\x88\x9a"                                  // U+f21a
#define ICON_FA_SHIRT "\xef\x95\x93"                                 // U+f553
#define ICON_FA_SHOE_PRINTS "\xef\x95\x8b"                           // U+f54b
#define ICON_FA_SHOP "\xef\x95\x8f"                                  // U+f54f
#define ICON_FA_SHOP_LOCK "\xee\x92\xa5"                             // U+e4a5
#define ICON_FA_SHOP_SLASH "\xee\x81\xb0"                            // U+e070
#define ICON_FA_SHOWER "\xef\x8b\x8c"                                // U+f2cc
#define ICON_FA_SHRIMP "\xee\x91\x88"                                // U+e448
#define ICON_FA_SHUFFLE "\xef\x81\xb4"                               // U+f074
#define ICON_FA_SHUTTLE_SPACE "\xef\x86\x97"                         // U+f197
#define ICON_FA_SIGN_HANGING "\xef\x93\x99"                          // U+f4d9
#define ICON_FA_SIGNAL "\xef\x80\x92"                                // U+f012
#define ICON_FA_SIGNATURE "\xef\x96\xb7"                             // U+f5b7
#define ICON_FA_SIGNS_POST "\xef\x89\xb7"                            // U+f277
#define ICON_FA_SIM_CARD "\xef\x9f\x84"                              // U+f7c4
#define ICON_FA_SINK "\xee\x81\xad"                                  // U+e06d
#define ICON_FA_SITEMAP "\xef\x83\xa8"                               // U+f0e8
#define ICON_FA_SKULL "\xef\x95\x8c"                                 // U+f54c
#define ICON_FA_SKULL_CROSSBONES "\xef\x9c\x94"                      // U+f714
#define ICON_FA_SLASH "\xef\x9c\x95"                                 // U+f715
#define ICON_FA_SLEIGH "\xef\x9f\x8c"                                // U+f7cc
#define ICON_FA_SLIDERS "\xef\x87\x9e"                               // U+f1de
#define ICON_FA_SMOG "\xef\x9d\x9f"                                  // U+f75f
#define ICON_FA_SMOKING "\xef\x92\x8d"                               // U+f48d
#define ICON_FA_SNOWFLAKE "\xef\x8b\x9c"                             // U+f2dc
#define ICON_FA_SNOWMAN "\xef\x9f\x90"                               // U+f7d0
#define ICON_FA_SNOWPLOW "\xef\x9f\x92"                              // U+f7d2
#define ICON_FA_SOAP "\xee\x81\xae"                                  // U+e06e
#define ICON_FA_SOCKS "\xef\x9a\x96"                                 // U+f696
#define ICON_FA_SOLAR_PANEL "\xef\x96\xba"                           // U+f5ba
#define ICON_FA_SORT "\xef\x83\x9c"                                  // U+f0dc
#define ICON_FA_SORT_DOWN "\xef\x83\x9d"                             // U+f0dd
#define ICON_FA_SORT_UP "\xef\x83\x9e"                               // U+f0de
#define ICON_FA_SPA "\xef\x96\xbb"                                   // U+f5bb
#define ICON_FA_SPAGHETTI_MONSTER_FLYING "\xef\x99\xbb"              // U+f67b
#define ICON_FA_SPELL_CHECK "\xef\xa2\x91"                           // U+f891
#define ICON_FA_SPIDER "\xef\x9c\x97"                                // U+f717
#define ICON_FA_SPINNER "\xef\x84\x90"                               // U+f110
#define ICON_FA_SPLOTCH "\xef\x96\xbc"                               // U+f5bc
#define ICON_FA_SPOON "\xef\x8b\xa5"                                 // U+f2e5
#define ICON_FA_SPRAY_CAN "\xef\x96\xbd"                             // U+f5bd
#define ICON_FA_SPRAY_CAN_SPARKLES "\xef\x97\x90"                    // U+f5d0
#define ICON_FA_SQUARE "\xef\x83\x88"                                // U+f0c8
#define ICON_FA_SQUARE_ARROW_UP_RIGHT "\xef\x85\x8c"                 // U+f14c
#define ICON_FA_SQUARE_CARET_DOWN "\xef\x85\x90"                     // U+f150
#define ICON_FA_SQUARE_CARET_LEFT "\xef\x86\x91"                     // U+f191
#define ICON_FA_SQUARE_CARET_RIGHT "\xef\x85\x92"                    // U+f152
#define ICON_FA_SQUARE_CARET_UP "\xef\x85\x91"                       // U+f151
#define ICON_FA_SQUARE_CHECK "\xef\x85\x8a"                          // U+f14a
#define ICON_FA_SQUARE_ENVELOPE "\xef\x86\x99"                       // U+f199
#define ICON_FA_SQUARE_FULL "\xef\x91\x9c"                           // U+f45c
#define ICON_FA_SQUARE_H "\xef\x83\xbd"                              // U+f0fd
#define ICON_FA_SQUARE_MINUS "\xef\x85\x86"                          // U+f146
#define ICON_FA_SQUARE_NFI "\xee\x95\xb6"                            // U+e576
#define ICON_FA_SQUARE_PARKING "\xef\x95\x80"                        // U+f540
#define ICON_FA_SQUARE_PEN "\xef\x85\x8b"                            // U+f14b
#define ICON_FA_SQUARE_PERSON_CONFINED "\xee\x95\xb7"                // U+e577
#define ICON_FA_SQUARE_PHONE "\xef\x82\x98"                          // U+f098
#define ICON_FA_SQUARE_PHONE_FLIP "\xef\xa1\xbb"                     // U+f87b
#define ICON_FA_SQUARE_PLUS "\xef\x83\xbe"                           // U+f0fe
#define ICON_FA_SQUARE_POLL_HORIZONTAL "\xef\x9a\x82"                // U+f682
#define ICON_FA_SQUARE_POLL_VERTICAL "\xef\x9a\x81"                  // U+f681
#define ICON_FA_SQUARE_ROOT_VARIABLE "\xef\x9a\x98"                  // U+f698
#define ICON_FA_SQUARE_RSS "\xef\x85\x83"                            // U+f143
#define ICON_FA_SQUARE_SHARE_NODES "\xef\x87\xa1"                    // U+f1e1
#define ICON_FA_SQUARE_UP_RIGHT "\xef\x8d\xa0"                       // U+f360
#define ICON_FA_SQUARE_VIRUS "\xee\x95\xb8"                          // U+e578
#define ICON_FA_SQUARE_XMARK "\xef\x8b\x93"                          // U+f2d3
#define ICON_FA_STAFF_SNAKE "\xee\x95\xb9"                           // U+e579
#define ICON_FA_STAIRS "\xee\x8a\x89"                                // U+e289
#define ICON_FA_STAMP "\xef\x96\xbf"                                 // U+f5bf
#define ICON_FA_STAPLER "\xee\x96\xaf"                               // U+e5af
#define ICON_FA_STAR "\xef\x80\x85"                                  // U+f005
#define ICON_FA_STAR_AND_CRESCENT "\xef\x9a\x99"                     // U+f699
#define ICON_FA_STAR_HALF "\xef\x82\x89"                             // U+f089
#define ICON_FA_STAR_HALF_STROKE "\xef\x97\x80"                      // U+f5c0
#define ICON_FA_STAR_OF_DAVID "\xef\x9a\x9a"                         // U+f69a
#define ICON_FA_STAR_OF_LIFE "\xef\x98\xa1"                          // U+f621
#define ICON_FA_STERLING_SIGN "\xef\x85\x94"                         // U+f154
#define ICON_FA_STETHOSCOPE "\xef\x83\xb1"                           // U+f0f1
#define ICON_FA_STOP "\xef\x81\x8d"                                  // U+f04d
#define ICON_FA_STOPWATCH "\xef\x8b\xb2"                             // U+f2f2
#define ICON_FA_STOPWATCH_20 "\xee\x81\xaf"                          // U+e06f
#define ICON_FA_STORE "\xef\x95\x8e"                                 // U+f54e
#define ICON_FA_STORE_SLASH "\xee\x81\xb1"                           // U+e071
#define ICON_FA_STREET_VIEW "\xef\x88\x9d"                           // U+f21d
#define ICON_FA_STRIKETHROUGH "\xef\x83\x8c"                         // U+f0cc
#define ICON_FA_STROOPWAFEL "\xef\x95\x91"                           // U+f551
#define ICON_FA_SUBSCRIPT "\xef\x84\xac"                             // U+f12c
#define ICON_FA_SUITCASE "\xef\x83\xb2"                              // U+f0f2
#define ICON_FA_SUITCASE_MEDICAL "\xef\x83\xba"                      // U+f0fa
#define ICON_FA_SUITCASE_ROLLING "\xef\x97\x81"                      // U+f5c1
#define ICON_FA_SUN "\xef\x86\x85"                                   // U+f185
#define ICON_FA_SUN_PLANT_WILT "\xee\x95\xba"                        // U+e57a
#define ICON_FA_SUPERSCRIPT "\xef\x84\xab"                           // U+f12b
#define ICON_FA_SWATCHBOOK "\xef\x97\x83"                            // U+f5c3
#define ICON_FA_SYNAGOGUE "\xef\x9a\x9b"                             // U+f69b
#define ICON_FA_SYRINGE "\xef\x92\x8e"                               // U+f48e
#define ICON_FA_T "T"                                                // U+0054
#define ICON_FA_TABLE "\xef\x83\x8e"                                 // U+f0ce
#define ICON_FA_TABLE_CELLS "\xef\x80\x8a"                           // U+f00a
#define ICON_FA_TABLE_CELLS_LARGE "\xef\x80\x89"                     // U+f009
#define ICON_FA_TABLE_COLUMNS "\xef\x83\x9b"                         // U+f0db
#define ICON_FA_TABLE_LIST "\xef\x80\x8b"                            // U+f00b
#define ICON_FA_TABLE_TENNIS_PADDLE_BALL "\xef\x91\x9d"              // U+f45d
#define ICON_FA_TABLET "\xef\x8f\xbb"                                // U+f3fb
#define ICON_FA_TABLET_BUTTON "\xef\x84\x8a"                         // U+f10a
#define ICON_FA_TABLET_SCREEN_BUTTON "\xef\x8f\xba"                  // U+f3fa
#define ICON_FA_TABLETS "\xef\x92\x90"                               // U+f490
#define ICON_FA_TACHOGRAPH_DIGITAL "\xef\x95\xa6"                    // U+f566
#define ICON_FA_TAG "\xef\x80\xab"                                   // U+f02b
#define ICON_FA_TAGS "\xef\x80\xac"                                  // U+f02c
#define ICON_FA_TAPE "\xef\x93\x9b"                                  // U+f4db
#define ICON_FA_TARP "\xee\x95\xbb"                                  // U+e57b
#define ICON_FA_TARP_DROPLET "\xee\x95\xbc"                          // U+e57c
#define ICON_FA_TAXI "\xef\x86\xba"                                  // U+f1ba
#define ICON_FA_TEETH "\xef\x98\xae"                                 // U+f62e
#define ICON_FA_TEETH_OPEN "\xef\x98\xaf"                            // U+f62f
#define ICON_FA_TEMPERATURE_ARROW_DOWN "\xee\x80\xbf"                // U+e03f
#define ICON_FA_TEMPERATURE_ARROW_UP "\xee\x81\x80"                  // U+e040
#define ICON_FA_TEMPERATURE_EMPTY "\xef\x8b\x8b"                     // U+f2cb
#define ICON_FA_TEMPERATURE_FULL "\xef\x8b\x87"                      // U+f2c7
#define ICON_FA_TEMPERATURE_HALF "\xef\x8b\x89"                      // U+f2c9
#define ICON_FA_TEMPERATURE_HIGH "\xef\x9d\xa9"                      // U+f769
#define ICON_FA_TEMPERATURE_LOW "\xef\x9d\xab"                       // U+f76b
#define ICON_FA_TEMPERATURE_QUARTER "\xef\x8b\x8a"                   // U+f2ca
#define ICON_FA_TEMPERATURE_THREE_QUARTERS "\xef\x8b\x88"            // U+f2c8
#define ICON_FA_TENGE_SIGN "\xef\x9f\x97"                            // U+f7d7
#define ICON_FA_TENT "\xee\x95\xbd"                                  // U+e57d
#define ICON_FA_TENT_ARROW_DOWN_TO_LINE "\xee\x95\xbe"               // U+e57e
#define ICON_FA_TENT_ARROW_LEFT_RIGHT "\xee\x95\xbf"                 // U+e57f
#define ICON_FA_TENT_ARROW_TURN_LEFT "\xee\x96\x80"                  // U+e580
#define ICON_FA_TENT_ARROWS_DOWN "\xee\x96\x81"                      // U+e581
#define ICON_FA_TENTS "\xee\x96\x82"                                 // U+e582
#define ICON_FA_TERMINAL "\xef\x84\xa0"                              // U+f120
#define ICON_FA_TEXT_HEIGHT "\xef\x80\xb4"                           // U+f034
#define ICON_FA_TEXT_SLASH "\xef\xa1\xbd"                            // U+f87d
#define ICON_FA_TEXT_WIDTH "\xef\x80\xb5"                            // U+f035
#define ICON_FA_THERMOMETER "\xef\x92\x91"                           // U+f491
#define ICON_FA_THUMBS_DOWN "\xef\x85\xa5"                           // U+f165
#define ICON_FA_THUMBS_UP "\xef\x85\xa4"                             // U+f164
#define ICON_FA_THUMBTACK "\xef\x82\x8d"                             // U+f08d
#define ICON_FA_TICKET "\xef\x85\x85"                                // U+f145
#define ICON_FA_TICKET_SIMPLE "\xef\x8f\xbf"                         // U+f3ff
#define ICON_FA_TIMELINE "\xee\x8a\x9c"                              // U+e29c
#define ICON_FA_TOGGLE_OFF "\xef\x88\x84"                            // U+f204
#define ICON_FA_TOGGLE_ON "\xef\x88\x85"                             // U+f205
#define ICON_FA_TOILET "\xef\x9f\x98"                                // U+f7d8
#define ICON_FA_TOILET_PAPER "\xef\x9c\x9e"                          // U+f71e
#define ICON_FA_TOILET_PAPER_SLASH "\xee\x81\xb2"                    // U+e072
#define ICON_FA_TOILET_PORTABLE "\xee\x96\x83"                       // U+e583
#define ICON_FA_TOILETS_PORTABLE "\xee\x96\x84"                      // U+e584
#define ICON_FA_TOOLBOX "\xef\x95\x92"                               // U+f552
#define ICON_FA_TOOTH "\xef\x97\x89"                                 // U+f5c9
#define ICON_FA_TORII_GATE "\xef\x9a\xa1"                            // U+f6a1
#define ICON_FA_TORNADO "\xef\x9d\xaf"                               // U+f76f
#define ICON_FA_TOWER_BROADCAST "\xef\x94\x99"                       // U+f519
#define ICON_FA_TOWER_CELL "\xee\x96\x85"                            // U+e585
#define ICON_FA_TOWER_OBSERVATION "\xee\x96\x86"                     // U+e586
#define ICON_FA_TRACTOR "\xef\x9c\xa2"                               // U+f722
#define ICON_FA_TRADEMARK "\xef\x89\x9c"                             // U+f25c
#define ICON_FA_TRAFFIC_LIGHT "\xef\x98\xb7"                         // U+f637
#define ICON_FA_TRAILER "\xee\x81\x81"                               // U+e041
#define ICON_FA_TRAIN "\xef\x88\xb8"                                 // U+f238
#define ICON_FA_TRAIN_SUBWAY "\xef\x88\xb9"                          // U+f239
#define ICON_FA_TRAIN_TRAM "\xee\x96\xb4"                            // U+e5b4
#define ICON_FA_TRANSGENDER "\xef\x88\xa5"                           // U+f225
#define ICON_FA_TRASH "\xef\x87\xb8"                                 // U+f1f8
#define ICON_FA_TRASH_ARROW_UP "\xef\xa0\xa9"                        // U+f829
#define ICON_FA_TRASH_CAN "\xef\x8b\xad"                             // U+f2ed
#define ICON_FA_TRASH_CAN_ARROW_UP "\xef\xa0\xaa"                    // U+f82a
#define ICON_FA_TREE "\xef\x86\xbb"                                  // U+f1bb
#define ICON_FA_TREE_CITY "\xee\x96\x87"                             // U+e587
#define ICON_FA_TRIANGLE_EXCLAMATION "\xef\x81\xb1"                  // U+f071
#define ICON_FA_TROPHY "\xef\x82\x91"                                // U+f091
#define ICON_FA_TROWEL "\xee\x96\x89"                                // U+e589
#define ICON_FA_TROWEL_BRICKS "\xee\x96\x8a"                         // U+e58a
#define ICON_FA_TRUCK "\xef\x83\x91"                                 // U+f0d1
#define ICON_FA_TRUCK_ARROW_RIGHT "\xee\x96\x8b"                     // U+e58b
#define ICON_FA_TRUCK_DROPLET "\xee\x96\x8c"                         // U+e58c
#define ICON_FA_TRUCK_FAST "\xef\x92\x8b"                            // U+f48b
#define ICON_FA_TRUCK_FIELD "\xee\x96\x8d"                           // U+e58d
#define ICON_FA_TRUCK_FIELD_UN "\xee\x96\x8e"                        // U+e58e
#define ICON_FA_TRUCK_FRONT "\xee\x8a\xb7"                           // U+e2b7
#define ICON_FA_TRUCK_MEDICAL "\xef\x83\xb9"                         // U+f0f9
#define ICON_FA_TRUCK_MONSTER "\xef\x98\xbb"                         // U+f63b
#define ICON_FA_TRUCK_MOVING "\xef\x93\x9f"                          // U+f4df
#define ICON_FA_TRUCK_PICKUP "\xef\x98\xbc"                          // U+f63c
#define ICON_FA_TRUCK_PLANE "\xee\x96\x8f"                           // U+e58f
#define ICON_FA_TRUCK_RAMP_BOX "\xef\x93\x9e"                        // U+f4de
#define ICON_FA_TTY "\xef\x87\xa4"                                   // U+f1e4
#define ICON_FA_TURKISH_LIRA_SIGN "\xee\x8a\xbb"                     // U+e2bb
#define ICON_FA_TURN_DOWN "\xef\x8e\xbe"                             // U+f3be
#define ICON_FA_TURN_UP "\xef\x8e\xbf"                               // U+f3bf
#define ICON_FA_TV "\xef\x89\xac"                                    // U+f26c
#define ICON_FA_U "U"                                                // U+0055
#define ICON_FA_UMBRELLA "\xef\x83\xa9"                              // U+f0e9
#define ICON_FA_UMBRELLA_BEACH "\xef\x97\x8a"                        // U+f5ca
#define ICON_FA_UNDERLINE "\xef\x83\x8d"                             // U+f0cd
#define ICON_FA_UNIVERSAL_ACCESS "\xef\x8a\x9a"                      // U+f29a
#define ICON_FA_UNLOCK "\xef\x82\x9c"                                // U+f09c
#define ICON_FA_UNLOCK_KEYHOLE "\xef\x84\xbe"                        // U+f13e
#define ICON_FA_UP_DOWN "\xef\x8c\xb8"                               // U+f338
#define ICON_FA_UP_DOWN_LEFT_RIGHT "\xef\x82\xb2"                    // U+f0b2
#define ICON_FA_UP_LONG "\xef\x8c\x8c"                               // U+f30c
#define ICON_FA_UP_RIGHT_AND_DOWN_LEFT_FROM_CENTER "\xef\x90\xa4"    // U+f424
#define ICON_FA_UP_RIGHT_FROM_SQUARE "\xef\x8d\x9d"                  // U+f35d
#define ICON_FA_UPLOAD "\xef\x82\x93"                                // U+f093
#define ICON_FA_USER "\xef\x80\x87"                                  // U+f007
#define ICON_FA_USER_ASTRONAUT "\xef\x93\xbb"                        // U+f4fb
#define ICON_FA_USER_CHECK "\xef\x93\xbc"                            // U+f4fc
#define ICON_FA_USER_CLOCK "\xef\x93\xbd"                            // U+f4fd
#define ICON_FA_USER_DOCTOR "\xef\x83\xb0"                           // U+f0f0
#define ICON_FA_USER_GEAR "\xef\x93\xbe"                             // U+f4fe
#define ICON_FA_USER_GRADUATE "\xef\x94\x81"                         // U+f501
#define ICON_FA_USER_GROUP "\xef\x94\x80"                            // U+f500
#define ICON_FA_USER_INJURED "\xef\x9c\xa8"                          // U+f728
#define ICON_FA_USER_LARGE "\xef\x90\x86"                            // U+f406
#define ICON_FA_USER_LARGE_SLASH "\xef\x93\xba"                      // U+f4fa
#define ICON_FA_USER_LOCK "\xef\x94\x82"                             // U+f502
#define ICON_FA_USER_MINUS "\xef\x94\x83"                            // U+f503
#define ICON_FA_USER_NINJA "\xef\x94\x84"                            // U+f504
#define ICON_FA_USER_NURSE "\xef\xa0\xaf"                            // U+f82f
#define ICON_FA_USER_PEN "\xef\x93\xbf"                              // U+f4ff
#define ICON_FA_USER_PLUS "\xef\x88\xb4"                             // U+f234
#define ICON_FA_USER_SECRET "\xef\x88\x9b"                           // U+f21b
#define ICON_FA_USER_SHIELD "\xef\x94\x85"                           // U+f505
#define ICON_FA_USER_SLASH "\xef\x94\x86"                            // U+f506
#define ICON_FA_USER_TAG "\xef\x94\x87"                              // U+f507
#define ICON_FA_USER_TIE "\xef\x94\x88"                              // U+f508
#define ICON_FA_USER_XMARK "\xef\x88\xb5"                            // U+f235
#define ICON_FA_USERS "\xef\x83\x80"                                 // U+f0c0
#define ICON_FA_USERS_BETWEEN_LINES "\xee\x96\x91"                   // U+e591
#define ICON_FA_USERS_GEAR "\xef\x94\x89"                            // U+f509
#define ICON_FA_USERS_LINE "\xee\x96\x92"                            // U+e592
#define ICON_FA_USERS_RAYS "\xee\x96\x93"                            // U+e593
#define ICON_FA_USERS_RECTANGLE "\xee\x96\x94"                       // U+e594
#define ICON_FA_USERS_SLASH "\xee\x81\xb3"                           // U+e073
#define ICON_FA_USERS_VIEWFINDER "\xee\x96\x95"                      // U+e595
#define ICON_FA_UTENSILS "\xef\x8b\xa7"                              // U+f2e7
#define ICON_FA_V "V"                                                // U+0056
#define ICON_FA_VAN_SHUTTLE "\xef\x96\xb6"                           // U+f5b6
#define ICON_FA_VAULT "\xee\x8b\x85"                                 // U+e2c5
#define ICON_FA_VECTOR_SQUARE "\xef\x97\x8b"                         // U+f5cb
#define ICON_FA_VENUS "\xef\x88\xa1"                                 // U+f221
#define ICON_FA_VENUS_DOUBLE "\xef\x88\xa6"                          // U+f226
#define ICON_FA_VENUS_MARS "\xef\x88\xa8"                            // U+f228
#define ICON_FA_VEST "\xee\x82\x85"                                  // U+e085
#define ICON_FA_VEST_PATCHES "\xee\x82\x86"                          // U+e086
#define ICON_FA_VIAL "\xef\x92\x92"                                  // U+f492
#define ICON_FA_VIAL_CIRCLE_CHECK "\xee\x96\x96"                     // U+e596
#define ICON_FA_VIAL_VIRUS "\xee\x96\x97"                            // U+e597
#define ICON_FA_VIALS "\xef\x92\x93"                                 // U+f493
#define ICON_FA_VIDEO "\xef\x80\xbd"                                 // U+f03d
#define ICON_FA_VIDEO_SLASH "\xef\x93\xa2"                           // U+f4e2
#define ICON_FA_VIHARA "\xef\x9a\xa7"                                // U+f6a7
#define ICON_FA_VIRUS "\xee\x81\xb4"                                 // U+e074
#define ICON_FA_VIRUS_COVID "\xee\x92\xa8"                           // U+e4a8
#define ICON_FA_VIRUS_COVID_SLASH "\xee\x92\xa9"                     // U+e4a9
#define ICON_FA_VIRUS_SLASH "\xee\x81\xb5"                           // U+e075
#define ICON_FA_VIRUSES "\xee\x81\xb6"                               // U+e076
#define ICON_FA_VOICEMAIL "\xef\xa2\x97"                             // U+f897
#define ICON_FA_VOLCANO "\xef\x9d\xb0"                               // U+f770
#define ICON_FA_VOLLEYBALL "\xef\x91\x9f"                            // U+f45f
#define ICON_FA_VOLUME_HIGH "\xef\x80\xa8"                           // U+f028
#define ICON_FA_VOLUME_LOW "\xef\x80\xa7"                            // U+f027
#define ICON_FA_VOLUME_OFF "\xef\x80\xa6"                            // U+f026
#define ICON_FA_VOLUME_XMARK "\xef\x9a\xa9"                          // U+f6a9
#define ICON_FA_VR_CARDBOARD "\xef\x9c\xa9"                          // U+f729
#define ICON_FA_W "W"                                                // U+0057
#define ICON_FA_WALKIE_TALKIE "\xef\xa3\xaf"                         // U+f8ef
#define ICON_FA_WALLET "\xef\x95\x95"                                // U+f555
#define ICON_FA_WAND_MAGIC "\xef\x83\x90"                            // U+f0d0
#define ICON_FA_WAND_MAGIC_SPARKLES "\xee\x8b\x8a"                   // U+e2ca
#define ICON_FA_WAND_SPARKLES "\xef\x9c\xab"                         // U+f72b
#define ICON_FA_WAREHOUSE "\xef\x92\x94"                             // U+f494
#define ICON_FA_WATER "\xef\x9d\xb3"                                 // U+f773
#define ICON_FA_WATER_LADDER "\xef\x97\x85"                          // U+f5c5
#define ICON_FA_WAVE_SQUARE "\xef\xa0\xbe"                           // U+f83e
#define ICON_FA_WEIGHT_HANGING "\xef\x97\x8d"                        // U+f5cd
#define ICON_FA_WEIGHT_SCALE "\xef\x92\x96"                          // U+f496
#define ICON_FA_WHEAT_AWN "\xee\x8b\x8d"                             // U+e2cd
#define ICON_FA_WHEAT_AWN_CIRCLE_EXCLAMATION "\xee\x96\x98"          // U+e598
#define ICON_FA_WHEELCHAIR "\xef\x86\x93"                            // U+f193
#define ICON_FA_WHEELCHAIR_MOVE "\xee\x8b\x8e"                       // U+e2ce
#define ICON_FA_WHISKEY_GLASS "\xef\x9e\xa0"                         // U+f7a0
#define ICON_FA_WIFI "\xef\x87\xab"                                  // U+f1eb
#define ICON_FA_WIND "\xef\x9c\xae"                                  // U+f72e
#define ICON_FA_WINDOW_MAXIMIZE "\xef\x8b\x90"                       // U+f2d0
#define ICON_FA_WINDOW_MINIMIZE "\xef\x8b\x91"                       // U+f2d1
#define ICON_FA_WINDOW_RESTORE "\xef\x8b\x92"                        // U+f2d2
#define ICON_FA_WINE_BOTTLE "\xef\x9c\xaf"                           // U+f72f
#define ICON_FA_WINE_GLASS "\xef\x93\xa3"                            // U+f4e3
#define ICON_FA_WINE_GLASS_EMPTY "\xef\x97\x8e"                      // U+f5ce
#define ICON_FA_WON_SIGN "\xef\x85\x99"                              // U+f159
#define ICON_FA_WORM "\xee\x96\x99"                                  // U+e599
#define ICON_FA_WRENCH "\xef\x82\xad"                                // U+f0ad
#define ICON_FA_X "X"                                                // U+0058
#define ICON_FA_X_RAY "\xef\x92\x97"                                 // U+f497
#define ICON_FA_XMARK "\xef\x80\x8d"                                 // U+f00d
#define ICON_FA_XMARKS_LINES "\xee\x96\x9a"                          // U+e59a
#define ICON_FA_Y "Y"                                                // U+0059
#define ICON_FA_YEN_SIGN "\xef\x85\x97"                              // U+f157
#define ICON_FA_YIN_YANG "\xef\x9a\xad"                              // U+f6ad
#define ICON_FA_Z "Z"                                                // U+005a

#endif// KOPERNIKUS_UI_H
