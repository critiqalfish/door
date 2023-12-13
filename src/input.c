#include "core.h"
#include "input.h"

void proc_arr_key (char * key) {
    if (door.sel_mode == SEL_BROWSER) {
        switch (*key) {
        case 'A':
            mv_sel_crsr(-1);
            if (door.crsr_sel == 0) set_crsr(1, strlen(door.path));
            break;
        case 'B':
            mv_sel_crsr(1);
            if (door.crsr_sel == door.crsr_sel_max) set_crsr(door.crsr_sel_max, 1);
            break;
        case 'C':
            if (door.crsr_sel > 0 && door.crsr_sel < door.crsr_sel_max) {
                set_sel_crsr(door.crsr_sel_max - 1);
            }
            else {
                mv_crsr(door.crsr_row, 1);
            }
            break;
        case 'D':
            if (door.crsr_sel > 0 && door.crsr_sel < door.crsr_sel_max){
                set_sel_crsr(1);
            }
            else {
                mv_crsr(door.crsr_row, -1);
            }
            break;
        }
    }
}

void proc_ret () {
    if (door.crsr_sel == 0) {

    }
    else if (door.crsr_sel == door.crsr_sel_max) {

    }
    else if (conv_d_type(door.dir_cntnt[door.crsr_sel - 1]->type) == 'd') {
        chg_dir();
    }
    else if (conv_d_type(door.dir_cntnt[door.crsr_sel - 1]->type) == 'f') {
        door.sel_mode = SEL_ENTRY;
    }
}

void proc_esc_seq () {
    char seq[3];

    if (read(STDIN_FILENO, &seq[0], 1) == 0) return;
    if (read(STDIN_FILENO, &seq[1], 1) == 0) return;

    if (seq[1] >= 'A' && seq[1] <= 'D') {
        proc_arr_key(&seq[1]);
    }
}

void input (bool * running) {
    char c;
    if (read(STDIN_FILENO, &c, 1) == 1) {
        switch (c) {
        case 'q':
            *running = false;
            break;
        case 'b':
            if (door.sel_mode == SEL_BROWSER) chg_dir_back();
            else if (door.sel_mode == SEL_ENTRY) door.sel_mode = SEL_BROWSER;
            break;
        case 'p':
            door.crsr_sel = 0;
            break;
        case 'c':
            door.crsr_sel = door.crsr_sel_max;
            break;
        case '\012':
            proc_ret();
            break;
        case '\033':
            proc_esc_seq();
            break;
        }
    }
}
