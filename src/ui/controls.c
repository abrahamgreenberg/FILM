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

void navigate_controls(int ch, int *highlight, char *current_path, char *render_path, Folder *folders, View *view)
{
    switch (ch)
    {
    case '\n':
        if (*highlight == -1)
        {
            char *last_slash = strrchr(current_path, '/');
            if (last_slash != NULL)
            {
                *last_slash = '\0';
                if (strlen(current_path) == 0)
                {
                    strcpy(current_path, "/");
                }
            }
            else
                strcpy(current_path, ".");
        }
        else
        {
            char new_path[OS_MAX_PATH_LENGTH];
            snprintf(new_path, OS_MAX_PATH_LENGTH, "%s/%s", current_path, folders[*highlight].folder_name);
            strncpy(current_path, new_path, OS_MAX_PATH_LENGTH);
            current_path[MAX_PATH_LENGTH - 1] = '\0';
        }
        *highlight = -1;
        break;
    case 'r':
    case 'R':
        render_path[0] = '\0';
        break;
    case 'e':
    case 'E':
        *view = EDIT;
        break;
    case 's':
    case 'S':
        *highlight = 0;
        *view = SETTINGS;
        break;
    case 'q':
    case 'Q':
        endwin();
        exit(0);
        break;
    }
}

void edit_controls(int ch, Diff *diff, Diff **diffs, int *highlight, Folder *folders, int *folder_count, View *view, char *debug_string, int *debug_string_length)
{
    char new_name[MAX_FILE_NAME_LENGTH];

    switch (ch)
    {
    case 'r':
    case 'R':

        if (get_new_name(new_name))
            UpdateDiffName(diffs, highlight, diff->number, new_name);
        break;
    case 'w':
    case 'W':
        *view = WRITE;
        break;
    case 'q':
    case 'Q':
        *view = NAVIGATE;
        if (*diffs != NULL)
            free(*diffs);
        DiffArrayConstructor(diffs, folders, *folder_count);
        break;
    case 'u':
    case 'U':
        if ((*highlight) <= 0)
            break;

        SwapDiffs(*diffs, *highlight, *highlight - 1);
        (*highlight)--;
        break;
    case 'd':
    case 'D':
        if ((*highlight) >= (*folder_count) - 1)
            break;

        SwapDiffs(*diffs, *highlight, *highlight + 1);
        (*highlight)++;
        break;
    case 'c':
    case 'C':
        append_str(debug_string, debug_string_length, "CREATE ");

        if (*folder_count >= 100)
            break;
        int number = 0;
        int i = 0;

        if (get_new_name(new_name))
        {
            if (*folder_count == 0)
                number = 1;
            else if (*folder_count == 1)
                if ((*diffs)[0].number == 99)
                {
                    number = 1;
                    (*diffs)[1] = (*diffs)[0];
                    i = 0;
                }
                else
                {
                    number = (*diffs)[0].number + 1;
                    i = 1;
                }
            else
            {
                for (; i < (*folder_count - *highlight - 1); i++)
                    if ((*diffs)[*highlight + i + 1].number != (*diffs)[*highlight + i].number + 1)
                        break;
                number = (*diffs)[*highlight + i].number + 1;

                if (number == 100)
                    number = 99;
                i += *highlight + 1;
                for (int j = *folder_count; j > i; j--)
                {
                    (*diffs)[j] = (*diffs)[j - 1];
                }
            }
            (*diffs)[i].index = -1;
            (*diffs)[i].number = number;
            (*diffs)[i].action = CREATE;
            UpdateDiffName(diffs, &i, number, new_name);
            (*folder_count)++;
        }
        break;
    case 'a':
    case 'A':
        if (strcmp(diff->formatted_name, "[99] Archive") == 0)
            break;

        ToggleDiffAction(&(*diffs)[*highlight], ARCHIVE);

        break;
    case '-':
        if (DiffHasAction(*diff, ARCHIVE) || diff->number <= 1)
            break;

        diff->number--;
        UpdateDiffName(diffs, highlight, diff->number, diff->name);

        while (*highlight > 0 && diff->number < (*diffs)[*highlight - 1].number)
        {
            Diff temp = *diff;
            (*diffs)[*highlight] = (*diffs)[*highlight - 1];
            (*diffs)[*highlight - 1] = temp;

            (*highlight)--;
        }

        break;
    case '=':
        if (DiffHasAction(*diff, ARCHIVE) || diff->number >= 99)
            break;

        diff->number++;
        UpdateDiffName(diffs, highlight, diff->number, diff->name);

        while (*highlight < *folder_count - 1 && diff->number > (*diffs)[*highlight + 1].number)
        {
            Diff temp = *diff;
            (*diffs)[*highlight] = (*diffs)[*highlight + 1];
            (*diffs)[*highlight + 1] = temp;

            (*highlight)++;
        }

        break;
    case 'f':
    case 'F':
        int j = 0;
        for (int i = 0; i < *folder_count; i++)
        {
            if (DiffHasAction(*diff, ARCHIVE) || strcmp((*diffs)[i].formatted_name, "[99] Archive") == 0)
                continue;
            (*diffs)[i].number = j++;
            UpdateDiffName(diffs, &i, j, (*diffs)[i].name);
        }
    }
}

void write_controls(int ch, View *view, const char *current_path, Folder *folders, Diff *diffs, size_t folder_count, char *message_string, int *message_string_length, char *render_path, Settings *settings, int *highlight)
{
    switch (ch)
    {
    case 'q':
    case 'Q':
        *view = EDIT;
        break;
    case 'c':
    case 'C':
        *view = NAVIGATE;
        write_changes(current_path, folders, diffs, folder_count, message_string, message_string_length, settings);
        render_path[0] = '\0';
        *highlight = -1;
        break;
    }
}