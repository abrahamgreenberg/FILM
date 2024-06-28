#include "controls.h"

#define SHRT(S) (ch == GET_SHORTCUT(S))

void append_str(char *string, int *string_length, const char *append_string)
{
    strncat(string, append_string, STRING_LENGTH);
    *string_length += strlen(append_string);
}

void append_frmt(char *string, int *string_length, const char *format, ...)
{
    char temp_buffer[STRING_LENGTH / 2];

    va_list args;
    va_start(args, format);
    vsnprintf(temp_buffer, sizeof(temp_buffer), format, args);
    va_end(args);

    strncat(string, temp_buffer, STRING_LENGTH);

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

void navigate_controls(int ch, int *highlight, char *current_path, char *render_path, Folder *folders, View *view, Settings *settings)
{
    if (SHRT(NAVIGATE_KEY))
    {
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
    }
    else if (SHRT(RELOAD_DIRS_KEY))
        render_path[0] = '\0';
    else if (SHRT(OPEN_EDIT_MODE_KEY))
        *view = EDIT;
    else if (SHRT(OPEN_SETTINGS_KEY))
    {
        *highlight = 0;
        *view = SETTINGS;
    }
    else if (SHRT(EXIT_KEY))
    {
        endwin();
        exit(0);
    }
}

void edit_controls(int ch, Diff *diff, Diff **diffs, int *highlight, Folder *folders, int *folder_count, View *view, char *debug_string, int *debug_string_length, Settings *settings)
{
    char new_name[MAX_FILE_NAME_LENGTH];

    if (SHRT(RENAME_KEY))
    {
        if (get_new_name(new_name))
            UpdateDiffName(diffs, highlight, diff->number, new_name);
    }
    else if (SHRT(OPEN_WRITE_MODE_KEY))
        *view = WRITE;
    else if (SHRT(EXIT_KEY))
    {
        *view = NAVIGATE;
        if (*diffs != NULL)
            free(*diffs);
        DiffArrayConstructor(diffs, folders, *folder_count);
    }
    else if (SHRT(MOVE_UP_KEY))
    {
        if ((*highlight) <= 0)
            return;

        SwapDiffs(*diffs, *highlight, *highlight - 1);
        (*highlight)--;
    }
    else if (SHRT(MOVE_DOWN_KEY))
    {

        if ((*highlight) >= (*folder_count) - 1)
            return;

        SwapDiffs(*diffs, *highlight, *highlight + 1);
        (*highlight)++;
    }
    else if (SHRT(CREATE_DIR_KEY))
    {
        append_str(debug_string, debug_string_length, "CREATE ");

        if (*folder_count >= 100)
            return;
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
    }
    else if (SHRT(ARCHIVE_DIR_KEY))
    {
        if (strcmp(diff->formatted_name, "[99] Archive") == 0)
            return;

        ToggleDiffAction(&(*diffs)[*highlight], ARCHIVE);
    }
    else if (SHRT(DECREMENT_DIR_NUMBER_KEY))
    {

        if (DiffHasAction(*diff, ARCHIVE) || diff->number <= 1)
            return;

        diff->number--;
        UpdateDiffName(diffs, highlight, diff->number, diff->name);

        while (*highlight > 0 && diff->number < (*diffs)[*highlight - 1].number)
        {
            Diff temp = *diff;
            (*diffs)[*highlight] = (*diffs)[*highlight - 1];
            (*diffs)[*highlight - 1] = temp;

            (*highlight)--;
        }
    }
    else if (SHRT(INCREMENT_DIR_NUMBER_KEY))
    {
        if (DiffHasAction(*diff, ARCHIVE) || diff->number >= 99)
            return;

        diff->number++;
        UpdateDiffName(diffs, highlight, diff->number, diff->name);

        while (*highlight < *folder_count - 1 && diff->number > (*diffs)[*highlight + 1].number)
        {
            Diff temp = *diff;
            (*diffs)[*highlight] = (*diffs)[*highlight + 1];
            (*diffs)[*highlight + 1] = temp;

            (*highlight)++;
        }
    }
    else if (SHRT(FIX_DIR_NUMBERS_KEY))
    {
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
    if (SHRT(EXIT_KEY))
        *view = EDIT;
    else if (SHRT(CONFIRM_CHANGES_KEY))
    {
        *view = NAVIGATE;
        write_changes(current_path, folders, diffs, folder_count, message_string, message_string_length, settings);
        render_path[0] = '\0';
        *highlight = -1;
    }
}