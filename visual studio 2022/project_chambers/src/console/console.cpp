#include "../stdafx.h"

std::array<short, 2> get_console_res(void) {
	CONSOLE_SCREEN_BUFFER_INFO console_info;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &console_info);
	const std::array<short, 2> console_res{ console_info.srWindow.Right - console_info.srWindow.Left + 1, console_info.srWindow.Bottom - console_info.srWindow.Top + 1 };
	return console_res;
}

void show_console_cursor(bool show_flag) {
	CONSOLE_CURSOR_INFO cursor_info;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
	cursor_info.bVisible = show_flag;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
	return;
}

void move_console_cursor(short x, short y) {
	COORD pos{ x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
	return;
}

void set_buffer_size(short x, short y) {
	COORD new_buffer_size{ x, y };
	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), new_buffer_size);
	return;
}