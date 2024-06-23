#include "controls.h"

void append_str(char *string, int *string_length, const char *append_string)
{
    strcat(string, append_string);
    *string_length += strlen(append_string);
}

void append_frmt(char *string, int *string_length, const char *format, ...)
{
    char temp_buffer[STRING_LENGTH / 2];

    va_list args;
    va_start(args, format);
    vsnprintf(temp_buffer, sizeof(temp_buffer), format, args);
    va_end(args);

    strcat(string, temp_buffer);

    *string_length += strlen(temp_buffer);
}

int get_new_name(char *new_name)
{
    int ch, pos = 0;
    memset(new_name, 0, MAX_FILE_NAME_LENGTH);

    mvprintw(LINES - 1, 0, "Enter new name: ");
    refresh();

    while ((ch = getch()) != '\n')
    {
        if (ch == 27)
        {
            return 0;
        }
        else if (ch == KEY_BACKSPACE || ch == 127 || ch == '\b')
        {
            if (pos > 0)
            {
                pos--;
                new_name[pos] = '\0';
                mvprintw(LINES - 1, 15 + pos, " ");
                move(LINES - 1, 15 + pos);
                refresh();
            }
        }
        else if (pos < MAX_FILE_NAME_LENGTH - 1 && ch != '\n')
        {
            new_name[pos++] = ch;
            mvprintw(LINES - 1, 15 + pos - 1, "%c", ch);
            refresh();
        }
    }
    return 1;
}
