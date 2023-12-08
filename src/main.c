#include "core.h"
#include "render.h"
#include "input.h"

void loop () {
    bool running = true;
    while(running == true) {
        render();
        input(&running);
    }
}

int main (int argc, char * argv[]) {
    struct termios old_term_settings;
    char * base_dir;
    
    door.path = getcwd(NULL, 0);
    base_dir = (char *) malloc(strlen(door.path) * sizeof(char *) + 1);
    strcpy(base_dir, door.path);
    door.crsr_row = 1;
    door.crsr_col = 1;

    get_term_settings(&old_term_settings);
    set_term_raw_mode(&old_term_settings);
    get_term_size();

    clr_scr();
    hide_dflt_crsr();
    set_crsr(1, strlen(door.path));

    loop();    

    clr_scr();
    set_crsr(1, 1);
    reset_term_settings(&old_term_settings);
    chdir(base_dir);
    free(door.path);
    free(base_dir);
    free(door.dir_cntnt);

    return 0;
}