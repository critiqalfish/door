#include "core.h"
#include "render.h"

void draw_header () {
    int extra_save = door.crsr_col;

    save_crsr();

    set_crsr(1, 1);
    write(STDOUT_FILENO, "\033[30m\033[47m", 10);

    char p;
    for (int c = 0; c < door.crsr_col_max; c++) {
        if (c < strlen(door.path)) p = door.path[c];
        else p = ' ';

        if (door.crsr_sel == 0 && c == extra_save - 1) {
            write(STDOUT_FILENO, "\033[37m\033[40m", 10);
            write(STDOUT_FILENO, &p, 1);
            write(STDOUT_FILENO, "\033[30m\033[47m", 10);
        }
        else {
            write(STDOUT_FILENO, &p, 1);
        }
        mv_crsr(0, 1);
    }

    printf("\033[0m");
    fflush(stdout);

    restore_crsr();
}

void clr_content () {
    save_crsr();

    set_crsr(3, 1);

    for (int r = 3; r < door.crsr_row_max - 3; r++) {
        printf("\033[2K");
        mv_crsr(1, 0);
    }

    fflush(stdout);

    restore_crsr();
}

void draw_content () {
    save_crsr();

    get_dir_cntnt();

    set_crsr(3, 1);

    for (int r = 3; r < door.crsr_row_max - 3 && r < door.dir_cntnt_len + 3; r++) {
        if (r - 2 == door.crsr_sel) printf("\033[30m\033[47m");
        printf("%s\033[0m", door.dir_cntnt[r - 3]);
        mv_crsr(1, 0);
    }

    fflush(stdout);

    restore_crsr();
}

void draw_footer () {
    save_crsr();

    set_crsr(door.crsr_row_max, 1);
    write(STDOUT_FILENO, "\033[30m\033[47m", 10);

    for (int c = 0; c < door.crsr_col_max; c++) {
        if (door.crsr_sel == door.crsr_sel_max && c == 0) {
            write(STDOUT_FILENO, "\033[37m\033[40m \033[30m\033[47m", 21);
        }
        else {
            write(STDOUT_FILENO, " ", 1);
        }
        mv_crsr(0, 1);
    }

    write(STDOUT_FILENO, "\033[0m", 4);

    restore_crsr();
}

void render () {
    draw_header();
    draw_content();
    draw_footer();
}
