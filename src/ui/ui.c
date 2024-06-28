#include "ui.h"

#define SET_HELP_MSG(S) (strcpy(help_msg, S))

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

void draw_ui(FOLDER_PARAMS(Diff *, Folder *, int), MESSAGE_PARAMS(char *, int), NAVIGATION_PARAMS(View, int *, int, Settings *), char *current_path, char *debug, int max_level, int *start)
{
    clear();

    int j = 0;
    int col1 = 0;
    int col2 = 0;
    int col3 = 0;

    if (DEBUG_MODE == 1)
    {
        attron(A_DIM);
        col1 = GET_COLOUR(settings, YELLOW);
        attron(col1);
        mvprintw(j++, 0, "Debug: %s", debug);
        attroff(col1);
        col1 = 0;
        attroff(A_DIM);
    }

    char status_string[STRING_LENGTH / 8];
    char help_msg[STRING_LENGTH];

    switch (view)
    {
    case NAVIGATE:
        strcpy(status_string, "[Navigation mode]");
        col1 = GET_COLOUR(settings, CYAN);

        if (help_page == 0)
            // strcpy(help_msg, "Controls (1/2): [2] Page 2. [Q] Quit. [Up Arrow/K] Move up. [Down Arrow/J] Move down.");
            SET_HELP_MSG("Controls (1/2): [2] Page 2. [Q] Quit. [Up Arrow/K] Move up. [Down Arrow/J] Move down.");
        else
            SET_HELP_MSG("Controls (2/2): [1] Page 1. [Enter] Navigate. [E] Edit mode. [R] Refresh files.");
        break;
    case EDIT:
        strcpy(status_string, "[Edit mode]");
        col1 = GET_COLOUR(settings, BLUE);

        if (help_page == 0)
            SET_HELP_MSG("Controls (1/5): [1] Page 1. [2] Page 2. [3] Page 3. [4] Page 4. [5] Page 5. ");
        else if (help_page == 1)
            SET_HELP_MSG("Controls (2/5): [Q] Navigation mode. [Up Arrow/K] Move up. [Down Arrow/J] Move down.");
        else if (help_page == 2)
            SET_HELP_MSG("Controls (3/5): [U] Move up. [D] Move Down. [C] Create folder. [R] Rename folder.");
        else if (help_page == 3)
            SET_HELP_MSG("Controls (4/5):  [A] Archive folder. [-] Decrement number. [=] Increment number.");
        else
            SET_HELP_MSG("Controls (5/5): [F] Fix numbering. [W] Write changes.");
        break;
    case WRITE:
        strcpy(status_string, "[Write mode]");
        col1 = GET_COLOUR(settings, MAGENTA);
        SET_HELP_MSG("Controls: [Q] Edit mode. [C] Confirm.");
        break;
    default:
        break;
    }

    attron(col1);
    mvprintw(j++, 0, "%s", status_string);
    attroff(col1);
    mvprintw(j++, 0, "Current folder: %s", current_path);

    mvprintw(++j, 0, "%s", help_msg);

    if (message_string_length > 0)
    {
        col1 = GET_COLOUR(settings, GREEN);
        attron(col1);
        mvprintw(j + 1, 0, "%s", message_string);
        attroff(col1);
    }

    j += 3;

    // TODO: FIX THE MESSAGE

    *start = path_level(current_path) == max_level && !DEBUG_MODE ? 0 : -1;
    if (view != NAVIGATE)
        *start = 0;
    if ((*highlight) == -1 && (*start) == 0)
        (*highlight) = 0;

    int h = 0;
    int u = 0;
    int y = 0;

    col1 = GET_COLOUR(settings, CYAN);
    col2 = GET_COLOUR(settings, CYAN);
    col3 = GET_COLOUR(settings, YELLOW);

    for (int i = (*start); i < folder_count; i++)
    {
        h = i == *highlight;
        u = view == WRITE ? (DiffHasAction(diffs[i], ARCHIVE) || DiffHasAction(diffs[i], CREATE) || strcmp(folders[diffs[i].index].folder_name, diffs[i].formatted_name) != 0) : 0;
        y = DiffHasAction(diffs[i], ARCHIVE);

        if (h && view != WRITE)
        {
            attron(A_STANDOUT);
            attron(col1);
        }
        if (u)
        {
            attron(A_UNDERLINE);
            attron(col2);
        }
        if (y)
            attron(col3);

        if (i == -1)
            mvprintw(j++, 0, "..");
        else
            mvprintw(i + j, 0, "%s%s", DiffHasAction(diffs[i], ARCHIVE) ? "* " : "",
                     diffs[i].formatted_name);

        if (y)
            attroff(col3);
        if (u)
        {
            attroff(A_UNDERLINE);
            attroff(col2);
        }
        if (h && view != WRITE)
        {
            attroff(A_STANDOUT);
            attroff(col1);
        }
    }

    refresh();
}

void file_manager_draw_loop(
    NAVIGATION_PARAMS(View *, int *, int *, Settings *), FOLDER_PARAMS(Diff **, Folder *, int *), PATH_PARAMS(char *), DEBUG_PARAMS(char *, int *), MESSAGE_PARAMS(char *, int *), int *start, int max_level)
{
    if (strcmp(render_path, current_path) != 0)
    {
        if (*diffs != NULL)
            free(*diffs);

        list_folders(current_path, folders, folder_count, diffs);
    }
    strcpy(render_path, current_path);
    draw_ui(FOLDER_PARAMS(*, , *), MESSAGE_PARAMS(, *), NAVIGATION_PARAMS(*, , *, ), current_path, debug_string, max_level, start);

    debug_string[0] = '\0';
    *debug_string_length = 0;

    int ch = getch();
    if (ch >= 'A' && ch <= 'Z')
        ch += ('a' - 'A');

    switch (ch)
    {
    case KEY_UP:
    case 'k':
        if (*view != WRITE)
            if (*highlight > *start)
                (*highlight)--;
        break;
    case KEY_DOWN:
    case 'j':
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
        navigate_controls(ch, NAVIGATION_PARAMS(, , , ), PATH_PARAMS(), folders);
        break;
    case EDIT:
        edit_controls(ch, FOLDER_PARAMS(, , ), NAVIGATION_PARAMS(, , , ), DEBUG_PARAMS(, ), &(*diffs)[*highlight]);
        break;
    case WRITE:
        write_controls(ch, NAVIGATION_PARAMS(, , , ), FOLDER_PARAMS(*, , *), MESSAGE_PARAMS(, ), PATH_PARAMS());
        break;
    default:
        break;
    }
}