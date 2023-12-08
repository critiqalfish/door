#pragma once

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#include <stdbool.h>

extern struct door {
    int crsr_row;
    int crsr_col;
    int crsr_r_sv;
    int crsr_c_sv;
    int crsr_sel;
    int crsr_sel_max;
    int crsr_row_max;
    int crsr_col_max;

    char * path;
    char ** dir_cntnt;
    int dir_cntnt_len;
} door;

void pass_door(struct door * door_ref);
void get_term_settings (struct termios * term_settings);
void set_term_raw_mode (struct termios * old_term_settings);
void reset_term_settings (struct termios * old_term_settings);
void get_term_size ();
void clr_scr ();
void hide_dflt_crsr ();
void show_dflt_crsr ();
void set_crsr_style ();
void set_crsr (int row, int col);
void mv_crsr (int row, int col);
void set_sel_crsr (int sel);
void mv_sel_crsr (int sel);
void save_crsr ();
void restore_crsr ();
void chg_dir ();
void chg_dir_back ();
int alph_sort_cmp (const void * a, const void * b);
int get_dir_cntnt ();