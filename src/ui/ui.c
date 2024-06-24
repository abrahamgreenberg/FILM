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
    const char *current_path, const char *debug, const char *message, int message_string_length, int *highlight, int max_level, int *start, View view, int help_page

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
    if (view == NAVIGATE)
    {
        attron(COLOR_PAIR(1));
        mvprintw(j++, 0, "[Navigation mode]");
        attroff(COLOR_PAIR(1));

        if (help_page == 0)
            strcpy(help_msg, "Controls (1/2): [2] Page 2. [Q] Quit. [Up Arrow/K] Move up. [Down Arrow/J] Move down.");
        else
            strcpy(help_msg, "Controls (2/2): [1] Page 1. [Enter] Navigate. [E] Edit mode. [R] Refresh files.");
    }
    else if (view == EDIT)
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
    else if (view == WRITE)
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
    if (view != NAVIGATE)
        *start = 0;
    if ((*highlight) == -1 && (*start) == 0)
        (*highlight) = 0;

    int h = 0;
    int u = 0;
    int y = 0;

    for (int i = (*start); i < folder_count; i++)
    {
        h = i == *highlight;
        u = view == WRITE ? (!DiffHasAction(diffs[i], NAME) || strcmp(folders[diffs[i].index].folder_name, diffs[i].formatted_name) != 0) : 0;
        y = DiffHasAction(diffs[i], ARCHIVE);

        if (h && view != WRITE)
            attron(A_STANDOUT);
        if (u)
            attron(A_UNDERLINE);
        if (y)
            attron(COLOR_PAIR(2));

        if (i == -1)
            mvprintw(j++, 0, "..");
        else
            mvprintw(i + j, 0, "%s%s", DiffHasAction(diffs[i], ARCHIVE) ? "* " : "",
                     diffs[i].formatted_name);

        if (y)
            attroff(COLOR_PAIR(2));
        if (h && view != WRITE)
            attroff(A_STANDOUT);
        if (u)
            attroff(A_UNDERLINE);
    }

    refresh();
}

void file_manager_draw_loop(
    char *current_path,
    char *render_path,
    Diff **diffs,
    Folder *folders,
    int *folder_count,
    Settings *settings,
    char *debug_string,
    int *debug_string_length,
    char *message_string,
    int *message_string_length,
    int *start,
    int max_level,
    int *highlight,
    View *view,
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
    draw_ui(*diffs, folders, *folder_count, current_path, debug_string, message_string, *message_string_length, highlight, max_level, start, *view, *help_page);

    debug_string[0] = '\0';
    *debug_string_length = 0;

    int ch = getch();

    switch (ch)
    {
    case KEY_UP:
    case 'k':
    case 'K':
        if (*view != WRITE)
            if (*highlight > *start)
                (*highlight)--;
        break;
    case KEY_DOWN:
    case 'j':
    case 'J':
        if (*view != WRITE)
            if (*highlight < *folder_count - 1)
                (*highlight)++;
        break;
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

    switch (*view)
    {
    case NAVIGATE:
        navigate_controls(ch, highlight, current_path, render_path, folders, view);
        break;
    case EDIT:
        edit_controls(ch, &(*diffs)[*highlight], diffs, highlight, folders, folder_count, view, debug_string, debug_string_length);
        break;
    case WRITE:
        write_controls(ch, view, current_path, folders, *diffs, *folder_count, message_string, message_string_length, render_path, settings);
        break;
    default:
        break;
    }
}