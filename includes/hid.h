/* move_right AND move_down (same for left / up) are both identical functions */
/* They are seperated by name for readability sake. */

#ifndef _HID_H_
#define _HID_H_


int move_cursor_right(int cursor, int cursor_max);
int move_cursor_left(int cursor, int cursor_max);
int move_cursor_up(int cursor, int cursor_max);
int move_cursor_down(int cursor, int cursor_max);

#endif