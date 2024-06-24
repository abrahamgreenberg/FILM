#include "ui.h"

void truncate_string_from_position(const char *source, char *destination, size_t start_position, size_t new_length)
{
    size_t source_length = strlen(source);

    if (start_position >= source_length)
    {
        destination[0] = '\0';
        return;
    }

    strncpy(destination, source + start_position, new_length);
    destination[new_length] = '\0';
}

void draw_ui(
    Diff *diffs, Folder *folders, int folder_count,
    const char *current_path, const char *debug, const char *message, int message_string_length, int edit, int write, int *highlight, int max_level, int *start, int help_page

)
{
    clear();

    int j = 0;

    if (DEBUG_MODE == 1)
    {
        attron(A_DIM);
        attron(COLOR_PAIR(2));
        mvprintw(j++, 0, "Debug: %s", debug);
        attroff(COLOR_PAIR(2));
        attroff(A_DIM);
    }
    mvprintw(j++, 0, "%s", current_path);

    char help_msg[512];
    if (!edit)
    {
        attron(COLOR_PAIR(1));
        mvprintw(j++, 0, "[Navigation mode]");
        attroff(COLOR_PAIR(1));

        if (help_page == 0)
            strcpy(help_msg, "Controls (1/2): [2] Page 2. [Q] Quit. [Up Arrow/K] Move up. [Down Arrow/J] Move down.");
        else
            strcpy(help_msg, "Controls (2/2): [1] Page 1. [Enter] Navigate. [E] Edit mode. [R] Refresh files.");
    }
    else if (edit && !write)
    {
        attron(COLOR_PAIR(3));
        mvprintw(j++, 0, "[Edit mode]");
        attroff(COLOR_PAIR(3));

        if (help_page == 0)
            strcpy(help_msg, "Controls (1/5): [1] Page 1. [2] Page 2. [3] Page 3. [4] Page 4. [5] Page 5. ");
        else if (help_page == 1)
            strcpy(help_msg, "Controls (2/5): [Q] Navigation mode. [Up Arrow/K] Move up. [Down Arrow/J] Move down.");
        else if (help_page == 2)
            strcpy(help_msg, "Controls (3/5): [U] Move up. [D] Move Down. [C] Create folder. [R] Rename folder.");
        else if (help_page == 3)
            strcpy(help_msg, "Controls (4/5):  [A] Archive folder. [-] Decrement number. [=] Increment number.");
        else
            strcpy(help_msg, "Controls (5/5): [F] Fix numbering. [W] Write changes.");
    }
    else if (write)
    {
        attron(COLOR_PAIR(4));
        mvprintw(j++, 0, "[Write mode]");
        attroff(COLOR_PAIR(4));

        strcpy(help_msg, "Controls: [Q] Edit mode. [C] Confirm.");
    }

    mvprintw(j, 0, "%s", help_msg);

    j += 2;

    if (message_string_length > 0)
    {
        mvprintw(j + 1, 0, "%s", message);
        j += 2;
    }

    *start = path_level(current_path) == max_level && !DEBUG_MODE ? 0 : -1;
    if (edit)
        *start = 0;
    if ((*highlight) == -1 && (*start) == 0)
        (*highlight) = 0;

    int h = 0;
    int u = 0;
    int y = 0;

    for (int i = (*start); i < folder_count; i++)
    {
        h = i == *highlight;
        u = write ? (diffs[i].index == -1 || diffs[i].archive || strcmp(folders[i].folder_name, diffs[i].formatted_name) != 0) : 0;
        y = diffs[i].archive;

        if (h && !write)
            attron(A_STANDOUT);
        if (u)
            attron(A_UNDERLINE);
        if (y)
            attron(COLOR_PAIR(2));

        if (i == -1)
            mvprintw(j++, 0, "..");
        else
            mvprintw(i + j, 0, "%s%s", diffs[i].archive ? "* " : "",
                     diffs[i].formatted_name);

        if (y)
            attroff(COLOR_PAIR(2));
        if (h && !write)
            attroff(A_STANDOUT);
        if (u)
            attroff(A_UNDERLINE);
    }

    refresh();
}

void draw_loop(
    char *current_path,
    char *render_path,
    Diff **diffs,
    Folder *folders,
    int *folder_count,
    char *debug_string,
    int *debug_string_length,
    char *message_string,
    int *message_string_length,
    int *start,
    int max_level,
    int *highlight,
    int *edit,
    int *write,
    int *help_page

)
{
    if (strcmp(render_path, current_path) != 0)
    {
        if (*diffs != NULL)
            free(*diffs);

        list_folders(current_path, folders, folder_count, diffs);
    }
    strcpy(render_path, current_path);
    draw_ui(*diffs, folders, *folder_count, current_path, debug_string, message_string, *message_string_length, *edit, *write, highlight, max_level, start, *help_page);

    debug_string[0] = '\0';
    *debug_string_length = 0;

    int ch = getch();

    Diff diff = (*diffs)[*highlight];

    switch (ch)
    {
    /* NAVIGATION */
    case KEY_UP:
    case 'K':
    case 'k':
        if (*highlight > *start)
            (*highlight)--;
        break;
    case KEY_DOWN:
    case 'j':
    case 'J':
        if (*highlight < *folder_count - 1)
            (*highlight)++;
        break;
    case '\n':
        if (*edit)
            break;

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
    /* NAVIGATION MODE: REFRESH / EDIT MODE: RENAME */
    case 'r':
    case 'R':
        if (*write)
            break;
        if (!(*edit))
        {
            render_path[0] = '\0';
            break;
        }
        char new_name[MAX_FILE_NAME_LENGTH];

        if (get_new_name(new_name))
            UpdateDiffName(diffs, highlight, diff.number, new_name);
        break;
    /* SWITCH MODES */
    case 'e':
    case 'E':
        *edit = 1;
        break;
    case 'w':
    case 'W':
        if (!(*edit) || (*write))
            break;
        (*write) = 1;
        break;
    case 'q':
    case 'Q':
        if (*write)
            *write = 0;
        else if (*edit)
        {
            *edit = 0;
            if (*diffs != NULL)
                free(*diffs);
            DiffArrayConstructor(diffs, folders, *folder_count);
        }
        else
        {
            endwin();
            exit(0);
        }
        break;
    /* EDIT MODE: ORDER */
    case 'u':
    case 'U':
        if (!(*edit) || *write)
            break;

        if ((*highlight) <= 0)
            break;

        SwapDiffs(*diffs, *highlight, *highlight - 1);
        (*highlight)--;
        break;
    case 'd':
    case 'D':
        if (!(*edit) || (*write))
            break;

        if ((*highlight) >= (*folder_count) - 1)
            break;

        SwapDiffs(*diffs, *highlight, *highlight + 1);
        (*highlight)++;
        break;
    /* EDIT MODE: CREATE / WRITE MODE: CONFIRM */
    case 'c':
    case 'C':
        if (!(*edit))
            break;
        if (!(*write))
        {
            append_str(debug_string, debug_string_length, "CREATE ");

            if (*folder_count >= 100)
                break;
            char new_name[MAX_FILE_NAME_LENGTH];
            char formatted_new_name[OS_MAX_FILE_NAME_LENGTH];
            int number = 0;
            int i = 0;

            if (get_new_name(new_name))
            {
                if (*folder_count == 0)
                    number = 1;
                else
                {
                    for (; i <= (*folder_count - *highlight - 1); i++)
                        if ((*diffs)[*highlight + i + 1].number != (*diffs)[*highlight + i].number + 1)
                            break;
                    number = (*diffs)[*highlight + i].number + 1;

                    if (number == 0)
                        number == 99;
                    i += *highlight + 1;
                    for (int j = *folder_count; j > i; j--)
                    {
                        (*diffs)[j] = (*diffs)[j - 1];
                    }
                }
                (*diffs)[i].index = -1;
                (*diffs)[i].number = number;
                (*diffs)[i].archive = false;
                UpdateDiffName(diffs, &i, number, new_name);
                (*folder_count)++;
            }
        }
        else
        {
            *edit = *write = 0;
            write_changes(current_path, folders, *diffs, *folder_count, message_string, message_string_length);
            render_path[0] = '\0';
        }
        break;
    /* EDIT MODE: ARCHIVE */
    case 'a':
    case 'A':
        if (!(*edit) || (*write))
            break;

        if (strcmp(diff.formatted_name, "[99] Archive") == 0)
            break;

        if (!(*diffs)[*highlight].archive)
            (*diffs)[*highlight].archive = true;
        else
            (*diffs)[*highlight].archive = false;

        break;
    /* EDIT MODE: NUMBERS -WORKS*/
    case '-':
        if (!(*edit) || (*write))
            break;

        if (diff.archive || diff.number <= 1)
            break;

        diff.number--;
        (*diffs)[*highlight].number = diff.number;
        UpdateDiffName(diffs, highlight, diff.number, diff.name);

        while (*highlight > 0 && diff.number < (*diffs)[*highlight - 1].number)
        {
            Diff temp = (*diffs)[*highlight];
            (*diffs)[*highlight] = (*diffs)[*highlight - 1];
            (*diffs)[*highlight - 1] = temp;

            (*highlight)--;
        }

        break;
    case '=':
        if (!(*edit) || (*write))
            break;

        if (diff.archive || diff.number >= 99)
            break;

        diff.number++;
        (*diffs)[*highlight].number = diff.number;
        UpdateDiffName(diffs, highlight, diff.number, diff.name);

        while (*highlight < *folder_count - 1 && diff.number > (*diffs)[*highlight + 1].number)
        {
            Diff temp = (*diffs)[*highlight];
            (*diffs)[*highlight] = (*diffs)[*highlight + 1];
            (*diffs)[*highlight + 1] = temp;

            (*highlight)++;
        }

        break;
    case 'f':
    case 'F':
        if (!(*edit) || (*write))
            break;

        int j = 0;
        for (int i = 0; i < *folder_count; i++)
        {
            if ((*diffs)[i].archive || strcmp((*diffs)[i].formatted_name, "[99] Archive") == 0)
                continue;
            (*diffs)[i].number = j++;
            UpdateDiffName(diffs, &i, j, (*diffs)[i].name);
        }
        break;
    /* HELP PAGES */
    case '1':
        *help_page = 0;
        break;
    case '2':
        *help_page = 1;
        break;
    case '3':
        *help_page = 2;
        break;
    case '4':
        *help_page = 3;
        break;
    case '5':
        *help_page = 4;
        break;
    }
}