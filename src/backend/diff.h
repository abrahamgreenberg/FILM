
#ifndef DIFF_H
#define DIFF_H
#include "folders.h"
#include "../opts.h"
#include <stdbool.h>
#include <string.h>

typedef enum
{
    NAME = 1,
    ARCHIVE = 2,
    CREATE = 4,
} DiffAction;

struct Diff
{
    int index;
    short number;
    char name[MAX_FILE_NAME_LENGTH];
    char formatted_name[OS_MAX_FILE_NAME_LENGTH];
    DiffAction action;
};

typedef struct Diff Diff;

void ToggleDiffAction(Diff *diff, DiffAction action);
bool DiffHasAction(Diff diff, DiffAction action);
void DiffArrayConstructor(Diff **diffs, Folder *folders, size_t folders_size);
void SwapDiffs(Diff *diffs, int i, int j);
void UpdateDiffName(Diff **diffs, int *index, int number, const char *name);

#endif
