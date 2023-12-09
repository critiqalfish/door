#include "core.h"
#include "render.h"

struct door door;

void get_term_settings (struct termios * term_settings) {
    tcgetattr(STDIN_FILENO, term_settings);
}

void set_term_raw_mode (struct termios * old_term_settings) {
    struct termios new_term_settings = *old_term_settings;
    new_term_settings.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_term_settings);
}

void reset_term_settings (struct termios * old_term_settings) {
    tcsetattr(STDIN_FILENO, TCSANOW, old_term_settings);
}

void get_term_size () {
    struct winsize ws;

    ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);
    door.crsr_row_max = ws.ws_row;
    door.crsr_col_max = ws.ws_col;
}

void clr_scr () {
    printf("\033[2J");
    fflush(stdout);
}

void hide_dflt_crsr () {
    printf("\033[?25l");
    fflush(stdout);
}

void show_dflt_crsr () {
    printf("\033[?25h");
    fflush(stdout);
}

void set_crsr_style () {
    printf("\033[0 q");
    fflush(stdout);
}

void set_crsr (int row, int col) {
    door.crsr_row = row;
    door.crsr_col = col;
    printf("\033[%d;%dH", row, col);
    fflush(stdout);
}

void mv_crsr (int row, int col) {
    door.crsr_row += row;
    door.crsr_col += col;
    if (door.crsr_row < 1) door.crsr_row = 1;
    if (door.crsr_row > door.crsr_row_max) door.crsr_row = door.crsr_row_max;
    if (door.crsr_col < 1) door.crsr_col = 1;
    if (door.crsr_col > door.crsr_col_max) door.crsr_col = door.crsr_col_max;
    printf("\033[%d;%dH", door.crsr_row, door.crsr_col);
    fflush(stdout);
}

void set_sel_crsr (int sel) {
    door.crsr_sel = sel;
}

void mv_sel_crsr (int sel) {
    door.crsr_sel += sel;

    if (door.crsr_sel < 0) door.crsr_sel = 0;
    if (door.crsr_sel > door.crsr_sel_max) door.crsr_sel = door.crsr_sel_max;
}

void save_crsr () {
    door.crsr_r_sv = door.crsr_row;
    door.crsr_c_sv = door.crsr_col;
}

void restore_crsr () {
    set_crsr(door.crsr_r_sv, door.crsr_c_sv);
}

void chg_dir () {
    char * path_clone = malloc(strlen(door.path) + strlen(door.dir_cntnt[door.crsr_sel - 1]) + 2);

    strcpy(path_clone, door.path);
    strcat(path_clone, "/");
    strcat(path_clone, door.dir_cntnt[door.crsr_sel - 1]);

    if (chdir(path_clone) == 0) {
        free(door.path);
        door.path = getcwd(NULL, 0);
        door.crsr_sel = 1;
    }

    free(path_clone);
}

void chg_dir_back () {
    door.path = realloc(door.path, strlen(door.path) + 4);
    strcat(door.path, "/..");

    if (chdir(door.path) == 0) {
        free(door.path);
        door.path = getcwd(NULL, 0);
        door.crsr_sel = 1;
    }
}

int alph_sort_cmp (const void * a, const void * b) {
    const char * str1 = * (const char **) a;
    const char * str2 = * (const char **) b;
    return strcmp(str1, str2);
}

int get_dir_cntnt () {
    DIR * dir;
    struct dirent *entry;

    dir = opendir(door.path);
    door.dir_cntnt = NULL;
    door.dir_cntnt_len = 0;
    door.crsr_sel_max = 1;

    if (dir == NULL) {
        return -1;
    }

    while ((entry = readdir(dir)) != NULL) {
        door.dir_cntnt = realloc(door.dir_cntnt, (door.dir_cntnt_len + 1) * sizeof(char *));
        door.dir_cntnt[door.dir_cntnt_len] = strdup(entry->d_name);

        door.dir_cntnt_len++;
    }

    closedir(dir);

    char **temp = malloc(door.dir_cntnt_len * sizeof(char *));
    int temp_count = 0;
    for (int i = 0; i < door.dir_cntnt_len; ++i) {
        if (strcmp(door.dir_cntnt[i], ".") != 0 && strcmp(door.dir_cntnt[i], "..") != 0) {
            temp[temp_count++] = door.dir_cntnt[i];
        }
    }

    qsort(temp, temp_count, sizeof(char *), alph_sort_cmp);

    free(door.dir_cntnt);
    door.dir_cntnt = temp;
    door.dir_cntnt_len = temp_count;
    door.crsr_sel_max = door.dir_cntnt_len + 1;

    return 0;
}
