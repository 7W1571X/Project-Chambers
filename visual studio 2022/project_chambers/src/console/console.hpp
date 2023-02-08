#ifndef PROJECT_CHAMBERS_CONSOLE_HPP_
#define PROJECT_CHAMBERS_CONSOLE_HPP_
std::array<short, 2> get_console_res(void);
void show_console_cursor(bool show_flag);
void move_console_cursor(short x, short y);
void set_buffer_size(short x, short y);
#define up_input   GetAsyncKeyState(VK_UP) & 1 || GetAsyncKeyState(0x57) & 1
#define down_input GetAsyncKeyState(VK_DOWN) & 1 || GetAsyncKeyState(0x53) & 1

#endif // !PROJECT_CHAMBERS_CONSOLE_HPP_