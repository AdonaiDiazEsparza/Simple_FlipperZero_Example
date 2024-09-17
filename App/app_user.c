// Includes the necessary libraries

#include <furi.h>
#include <furi_hal.h>
#include <gui/gui.h>
#include <gui/scene_manager.h>
#include <gui/view_dispatcher.h>

// First define all the functons that every scene needs
// Every scene has three functions
void hello_world_scene_on_enter(void* context); // one function on enter
bool hello_world_scene_on_event(void* context, SceneManagerEvent event); // one function on event
void hello_world_scene_on_exit(void* context); // one function on exit

// Then we have to save all the handle functions in an array

// For the "on enter" functions
void (*const app_on_enter_handlers[])(void*) = {
    hello_world_scene_on_enter,
};

// For the "on event" functions
bool (*const app_on_event_handlers[])(void* context, SceneManagerEvent event) = {
    hello_world_scene_on_event,
};

// For the "on exit" functions
void (*const app_on_exit_handlers[])(void* context) = {
    hello_world_scene_on_exit,
};

int app_main(void* p) {
    UNUSED(p);

    return 0;
}
