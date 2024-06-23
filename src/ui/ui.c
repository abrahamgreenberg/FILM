#include "ui.h"

void draw_ui(
    Diff *diffs, Folder *folders, int folder_count,
    const char *current_path, const char *debug, const char *message, int message_string_length, int edit, int write, int *highlight, int max_level, int *start

)
{
    clear();
    int j = 0;

    if (DEBUG_MODE == 1)
    {
        mvprintw(j++, 0, "Current path: %s", current_path);
        mvprintw(j++, 0, "Debug: %s", debug);
    }

    if (!edit)
    {
        mvprintw(j++, 0, "[Navigation mode]");
        mvprintw(j++, 0, "[Q]: Quit. [E]: Enter edit mode. [Up Arrow]: Go up. [Down Arrow]: Go down. [Enter]: Navigate.");
    }
    else if (edit && !write)
    {
        mvprintw(j++, 0, "[Edit mode]");
        mvprintw(j++, 0, "[Q]: Navigation mode. [U]: Move up. [D]: Move down. [R]: Rename. [W]: Write changes. [Up Arrow]: Go Up. [Down Arrow]: Go down.");
    }
    else if (write)
    {
        mvprintw(j++, 0, "[Write mode]");
        mvprintw(j++, 0, "Are you sure you want to make these changes (underlined) [C]: Confirm. [Q]: Back to Edit mode.");
    }

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

    for (int i = (*start); i < folder_count; i++)
    {
        h = i == *highlight;
        u = write ? (diffs[i].index == -1 || diffs[i].archive || strcmp(folders[i].folder_name, diffs[i].formatted_name) != 0) : 0;

        if (h && !write)
            attron(A_STANDOUT);

        if (u)
            attron(A_UNDERLINE);

        if (i == -1)

            mvprintw(j++, 0, "..");

        else

            mvprintw(i + j, 0, "%s%s", diffs[i].archive ? "* " : "", diffs[i].formatted_name);

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
    int *write

)
{
    if (strcmp(render_path, current_path) != 0)
    {
        if (*diffs != NULL)
            free(*diffs);

        list_folders(current_path, folders, folder_count, diffs);
    }
    strcpy(render_path, current_path);
    draw_ui(*diffs, folders, *folder_count, current_path, debug_string, message_string, *message_string_length, *edit, *write, highlight, max_level, start);

    debug_string[0] = '\0';
    *debug_string_length = 0;

    int ch = getch();

    switch (ch)
    {
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
    case 'e':
    case 'E':
        *edit = 1;
        break;
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
    case 'w':
    case 'W':
        if (!(*edit) || (*write))
            break;

        (*write) = 1;

        break;
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
        {
            strcpy((*diffs)[*highlight].name, new_name);
            FormatDiffName((*diffs)[*highlight].formatted_name, (*diffs)[*highlight].number, (*diffs)[*highlight].name);
        }
        break;
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
            int i;

            if (get_new_name(new_name))
            {
                for (i = 0; i <= (*folder_count - *highlight - 1); i++)
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

                (*diffs)[i].index = -1;
                (*diffs)[i].number = number;

                FormatDiffName(formatted_new_name, number, new_name);
                strcpy((*diffs)[i].name, new_name);
                strcpy((*diffs)[i].formatted_name, formatted_new_name);

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
    case 'a':
    case 'A':
        if (!(*edit) || (*write))
            break;

        if (strcmp((*diffs)[*highlight].formatted_name, "[99] Archive") == 0)
            break;

        if (!((*diffs)[*highlight]).archive)
            ((*diffs)[*highlight]).archive = 1;
        else
            ((*diffs)[*highlight]).archive = 0;

        break;
    case '-':
        if (!(*edit) || (*write))
            break;

        if ((*diffs)[*highlight].archive || (*diffs)[*highlight].number <= 1)
            break;

        (*diffs)[*highlight].number--;
        FormatDiffName((*diffs)[*highlight].formatted_name, (*diffs)[*highlight].number, (*diffs)[*highlight].name);

        while (*highlight > 0 && (*diffs)[*highlight].number < (*diffs)[*highlight - 1].number)
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

        if ((*diffs)[*highlight].archive || (*diffs)[*highlight].number >= 99)
            break;

        (*diffs)[*highlight].number++;
        FormatDiffName((*diffs)[*highlight].formatted_name, (*diffs)[*highlight].number, (*diffs)[*highlight].name);

        while (*highlight < *folder_count - 1 && (*diffs)[*highlight].number > (*diffs)[*highlight + 1].number)
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
        char formatted_new_name[OS_MAX_FILE_NAME_LENGTH];

        for (int i = 0; i < *folder_count; i++)
        {
            if ((*diffs)[i].archive || strcmp((*diffs)[i].formatted_name, "[99] Archive") == 0)
                continue;
            (*diffs)[i].number = j++;
            FormatDiffName(formatted_new_name, j, (*diffs)[i].name);
            strcpy((*diffs)[i].formatted_name, formatted_new_name);
        }
        break;

    case '\n':
        append_str(debug_string, debug_string_length, "ENTER ");

        append_frmt(debug_string, debug_string_length, "HL: %d ", *highlight);

        if (*edit)
            break;
        if (*highlight == -1)
        {
            append_str(debug_string, debug_string_length, "BACK ");

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
    }
}