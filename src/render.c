#include "core.h"
#include "render.h"
#include <stdio.h>
#include <unistd.h>

void draw_header () {
    set_crsr(1, 1);
    write(STDOUT_FILENO, "\033[30;47m", 9);

    char p;
    for (int c = 0; c < door.crsr_col_max; c++) {
        if (c < strlen(door.path)) p = door.path[c];
        else p = ' ';

        if (door.sel_mode == SEL_PATH && c == door.crsr_c_sv - 1) {
            write(STDOUT_FILENO, "\033[37;40m", 9);
            write(STDOUT_FILENO, &p, 1);
            write(STDOUT_FILENO, "\033[30;47m", 9);
        }
        else {
            write(STDOUT_FILENO, &p, 1);
        }
        mv_crsr(0, 1);
    }

    printf("\033[0m");
    fflush(stdout);
}

void clr_content () {
    set_crsr(3, 1);

    for (int r = 3; r < door.crsr_row_max - 3; r++) {
        printf("\033[2K");
        mv_crsr(1, 0);
    }

    fflush(stdout);
}

void draw_content_browser () {
    get_dir_cntnt();

    set_crsr(3, 1);

    int page_max = door.crsr_row_max - 7;
    int page = ceil((door.crsr_sel) / page_max); // NOLINT: because im smarter than clangd-tidy

    for (int r = 0; r < page_max && r < door.dir_cntnt_len - page * page_max; r++) {
        if (r == (door.crsr_sel) - page * page_max && door.sel_mode != SEL_PATH && door.sel_mode != SEL_COMMAND) printf("\033[1;32m");
        printf("%c %s\033[0m", conv_d_type(door.dir_cntnt[r + page * page_max]->type), door.dir_cntnt[r + page * page_max]->name);
        mv_crsr(1, 0);
    }

    fflush(stdout);
}

void draw_content_entry_sel () {
    get_dir_cntnt();

    set_crsr(4, 1);

    printf("\033[1;32m%s\033[0m", door.dir_cntnt[door.crsr_sel]->name);

    fflush(stdout);
}

void draw_content_entry_sel_box () {
    long name_len = strlen(door.dir_cntnt[door.crsr_sel]->name) + 4;
    int width = 20;
    int height = 3 + 2;

    set_crsr(3, name_len);

    for (int r = 0; r < height; r++) {
        if (r == 0) write(STDOUT_FILENO, "\u250C", 4);
        else if (r == height - 1) write(STDOUT_FILENO, "\u2514", 4);
        else write(STDOUT_FILENO, "\u2502", 4);

        mv_crsr(0, 1);

        for (int c = 1; c < width - 2; c++) {
            if (r == 0 || r == height - 1) write(STDOUT_FILENO, "\u2500", 4);

            mv_crsr(0, 1);
        }

        if (r == 0) write(STDOUT_FILENO, "\u2510", 4);
        else if (r == height - 1) write(STDOUT_FILENO, "\u2518", 4);
        else write(STDOUT_FILENO, "\u2502", 4);

        set_crsr(r + 4, name_len);
    }

    set_crsr(4, name_len + 2);
    printf("\033[1;35mopen in nano\033[0m");

    fflush(stdout);
}

void draw_content () {
    if (door.sel_mode == SEL_BROWSER || door.sel_mode == SEL_PATH || door.sel_mode == SEL_COMMAND) {
        draw_content_browser();
    }
    else if (door.sel_mode == SEL_ENTRY) {
        draw_content_entry_sel();
        draw_content_entry_sel_box();
    }
}

void draw_status () {
    set_crsr(door.crsr_row_max - 3, 1);
    
    for (int c = 0; c < door.crsr_col_max; c++) {
        write(STDOUT_FILENO, " ", 1);
        mv_crsr(0, 1);
    }

    set_crsr(door.crsr_row_max - 3, 1);

    int page_max = door.crsr_row_max - 7;
    int page = ceil((door.crsr_sel + 1) / (page_max + 1)) + 1; // NOLINT: because im smarter than clangd-tidy
    int pages = ceil(door.dir_cntnt_len / (page_max + 1)) + 1; // NOLINT: because im smarter than clangd-tidy

    if (door.sel_mode == SEL_BROWSER) {
        printf("page %d/%d - item \033[32m%d\033[0m/%d", page, pages, door.crsr_sel + 1, door.dir_cntnt_len);
    }
    else {
        printf("page %d/%d - item -/%d", page, pages, door.dir_cntnt_len);
    }

    fflush(stdout);
}

void draw_help () {
    set_crsr(door.crsr_row_max - 2, 1);

    printf("\033[35mq\033[0muit \033[35mb\033[0mack \033[35mp\033[0math \033[35mc\033[0mommand \033[35m\u2191\033[0mup \033[35m\u2193\033[0mdown \033[35m\u2190\033[0mleft \033[35m\u2192\033[0mright \033[35m\u21B5\033[0mgo");

    fflush(stdout);
}

void draw_footer () {
    set_crsr(door.crsr_row_max, 1);

    write(STDOUT_FILENO, "\033[30m\033[47m", 10);

    for (int c = 0; c < door.crsr_col_max; c++) {
        if (door.sel_mode == SEL_COMMAND && c == 0) {
            write(STDOUT_FILENO, "\033[37m\033[40m \033[30m\033[47m", 21);
        }
        else {
            write(STDOUT_FILENO, " ", 1);
        }
        mv_crsr(0, 1);
    }

    write(STDOUT_FILENO, "\033[0m", 4);
}

void render () {
    save_crsr();
    draw_header();
    clr_content();
    draw_content();
    draw_status();
    draw_help();
    draw_footer();
    restore_crsr();
}
