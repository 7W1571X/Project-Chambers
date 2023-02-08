#ifndef PROJECT_CHAMBERS_MENU_HPP_
#define PROJECT_CHAMBERS_MENU_HPP_
void clear_rect(short x, short y, int width, int height);
void create_tab(short x, short y, int width, int height, const char* name);
void print_art(short x, short y, std::vector<const char*> art, const char* color = "\033[0m");

#endif // !PROJECT_CHAMBERS_MENU_HPP_
