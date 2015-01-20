#include <pebble.h>
#include "TaskMenu.h"
#include "Timer.h"

static int index;
static int poms_remaining;
static int poms_target;
static char* task_name;
static int pom_completed = 0;
static int pom_completed_ref = 0;

Window *window;
MenuLayer *menu_layer;

int poms_achieved[7];
int buffer[7];

char* string_poms0 = "Complete: -1/-1";
char* string_poms1 = "Complete: -1/-1";
char* string_poms2 = "Complete: -1/-1";
char* string_poms3 = "Complete: -1/-1";
char* string_poms4 = "Complete: -1/-1";
char* string_poms5 = "Complete: -1/-1";
char* string_poms6 = "Complete: -1/-1";

uint32_t Old_Data_key = 11111;
uint32_t Data_key = 11112;
  
/////////Input your tasks and target pomos here
char task0[] = "PVC Planning";
static int pom_target0 = 5;

char task1[] = "Study China 101R";
int pom_target1 = 7;

char task2[] = "Talk to Fay";
int pom_target2 = 2;

char task3[] = "Meditate";
int pom_target3 = 1;

char task4[] = "Clean Up";
int pom_target4 = 1;

char task5[] = "Lunch";
int pom_target5 = 1;

char task6[] = "Dinner";
int pom_target6 = 1;


void draw_row_callback (GContext *ctx, Layer *cell_layer, MenuIndex *cell_index, void *callback_context) {
    // Which row is it?
    switch (cell_index->row) {
    case 0:
        snprintf(string_poms0, 20, "Complete: %d/%d", poms_achieved[0], pom_target0);
        menu_cell_basic_draw(ctx, cell_layer, task0, string_poms0, NULL);
        break;
    case 1:
        snprintf(string_poms1, 20, "Complete: %d/%d", poms_achieved[1], pom_target1);
        menu_cell_basic_draw(ctx, cell_layer, task1, string_poms1, NULL);
        break;
    case 2:
        snprintf(string_poms2, 20, "Complete: %d/%d", poms_achieved[2], pom_target2);
        menu_cell_basic_draw(ctx, cell_layer, task2, string_poms2, NULL);
        break;
    case 3:
        snprintf(string_poms3, 20, "Complete: %d/%d", poms_achieved[3], pom_target3);
        menu_cell_basic_draw(ctx, cell_layer, task3, string_poms3, NULL);
        break;
    case 4:
        snprintf(string_poms4, 20, "Complete: %d/%d", poms_achieved[4], pom_target4);
        menu_cell_basic_draw(ctx, cell_layer, task4, string_poms4, NULL);
        break;
    case 5:
        snprintf(string_poms5, 20, "Complete: %d/%d", poms_achieved[5], pom_target5);
        menu_cell_basic_draw(ctx, cell_layer, task5, string_poms5, NULL);
        break;
    case 6:
        snprintf(string_poms6, 20, "Complete: %d/%d", poms_achieved[6], pom_target6);
        menu_cell_basic_draw(ctx, cell_layer, task6, string_poms6, NULL);
        break;
    }
}

void select_click_callback (MenuLayer *menu_layer, MenuIndex *cell_index, void *callback_context) {
  int which = cell_index->row;
  switch(which) {
    case 0:
    poms_remaining = poms_achieved[0];
    poms_target = pom_target0;
    task_name = task0;
    index = 0;
        break;
    case 1:
    poms_remaining = poms_achieved[1];
    poms_target = pom_target1;
    task_name = task1;
    index = 1;
        break;
    case 2:
    poms_remaining = poms_achieved[2];
    poms_target = pom_target2;
    task_name = task2;
    index = 2;
        break;
    case 3:
    poms_remaining = poms_achieved[3];
    poms_target = pom_target3;
    task_name = task3;
    index = 3;
        break;
    case 4:
    poms_remaining = poms_achieved[4];
    poms_target = pom_target4;
    task_name = task4;
    index = 4;
        break;
    case 5:
    poms_remaining = poms_achieved[5];
    poms_target = pom_target5;
    task_name = task5;
    index = 5;
         break;
    case 6:
    poms_remaining = poms_achieved[6];
    poms_target = pom_target6;
    task_name = task6;
    index = 6;
        break;
  }
  pass_variables(poms_remaining, poms_target, task_name);
  timer_init();
  if (pom_completed > pom_completed_ref) {
    poms_achieved[index] += 1; 
    pom_completed_ref += 1;
    menu_layer_reload_data(menu_layer); //This won't actually refresh the menu!!!!
    menu_layer_reload_data(menu_layer); //This won't actually refresh the menu!!!!
  }
}

uint16_t num_rows_callback (MenuLayer *menu_layer, uint16_t section_index, void *callback_context) {
  return 7;
}


void window_load1 (Window *window){
  GRect bounds = layer_get_bounds(window_get_root_layer(window));
  menu_layer = menu_layer_create(bounds);
  menu_layer_set_click_config_onto_window(menu_layer, window);
  MenuLayerCallbacks callbacks = {
    .draw_row = (MenuLayerDrawRowCallback) draw_row_callback,
    .get_num_rows = (MenuLayerGetNumberOfRowsInSectionsCallback) num_rows_callback,
    .select_click = (MenuLayerSelectCallback) select_click_callback
  };
  menu_layer_set_callbacks(menu_layer, NULL, callbacks);
  layer_add_child(window_get_root_layer(window), menu_layer_get_layer(menu_layer));
}

void window_unload1 (Window *window) {
  menu_layer_destroy(menu_layer);
}

void init_menu() {
  if (persist_exists(Data_key)) {
    persist_read_data(Data_key, buffer, 32);
    int i;
    for ( i = 0; i <= 7; i++ ) {
      poms_achieved[i] = buffer[i];
    }
    persist_delete (Old_Data_key); //Is this necessary or can I just go to a new key and the old memory will be overwritten?
  }
  window = window_create();
  WindowHandlers handlers = {
    .load = window_load1,
    .unload = window_unload1
  };
  window_set_window_handlers (window, (WindowHandlers) handlers);
  window_stack_push (window, true);
}

void deinit_menu() {
  window_destroy(window);
  persist_write_data(Data_key, poms_achieved, 32);
}

void pass_pom_complete (int pom_complete){
  pom_completed = pom_complete;
}


