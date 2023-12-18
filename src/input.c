#include "core.h"
#include "input.h"

void proc_arr_key (char * key) {
    if (door.sel_mode == SEL_PATH) {
        switch (*key) {
        case 'A':
            break;
        case 'B':
            set_sel_crsr(0);
            door.sel_mode = SEL_BROWSER;
            break;
        case 'C':
            if (door.crsr_col < door.crsr_col_max && door.crsr_col < strlen(door.path)) mv_crsr(0, 1);
            break;
        case 'D':
            if (door.crsr_col > 1) mv_crsr(0, -1);
            break;
        }
    }
    else if (door.sel_mode == SEL_BROWSER) {
        switch (*key) {
        case 'A':
            if (door.crsr_sel == 0) door.sel_mode = SEL_PATH;
            mv_sel_crsr(-1);
            break;
        case 'B':
            if (door.crsr_sel == door.crsr_sel_max) door.sel_mode = SEL_COMMAND;
            mv_sel_crsr(1);
            break;
        case 'C':
            set_sel_crsr(door.crsr_sel_max);
            break;
        case 'D':
            set_sel_crsr(0);
            break;
        }
    }
    else if (door.sel_mode == SEL_ENTRY) {
        
    }
    else if (door.sel_mode == SEL_COMMAND) {
        switch (*key) {
        case 'A':
            set_sel_crsr(door.crsr_sel_max);
            door.sel_mode = SEL_BROWSER;
            break;
        case 'B':
            break;
        case 'C':
            break;
        case 'D':
            break;
        }
    }
}

void proc_ret () {
    if (door.sel_mode == SEL_BROWSER) {
        if (conv_d_type(door.dir_cntnt[door.crsr_sel]->type) == 'd') {
            chg_dir();
        }
        else if (conv_d_type(door.dir_cntnt[door.crsr_sel]->type) == 'f') {
            door.sel_mode = SEL_ENTRY;
            door.crsr_entry_sel = 0;
        }
    }
    else if (door.sel_mode == SEL_ENTRY) {
        entry_sel_action();
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
            door.sel_mode = SEL_PATH;
            break;
        case 'c':
            door.sel_mode = SEL_COMMAND;
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
