// Includes the necessary libraries

#include <furi.h>
#include <furi_hal.h>
#include <gui/gui.h>
#include <gui/scene_manager.h>
#include <gui/view_dispatcher.h>
#include <gui/modules/widget.h>

// Create an structure to work with our app
typedef struct {
    SceneManager* scene_manager;
    ViewDispatcher* view_dispatcher;
    Widget* widget;
} App;

// Create an enum to contain the order of the scenes
typedef enum {
    hello_world_scene,
    app_scene_num // At the end add the number of scenes that are used
} number_of_scenes;

// Create an enum to contain the order of the views
typedef enum {
    widget_view,
} number_of_views;

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

// We initialize the scene handler structure
const SceneManagerHandlers app_scene_handlers = {
    .on_enter_handlers = app_on_enter_handlers,
    .on_event_handlers = app_on_event_handlers,
    .on_exit_handlers = app_on_exit_handlers,
    .scene_num = app_scene_num,
};

//  The functions for the scene

void hello_world_scene_on_enter(void* context) {
    // Into this function it must be the things will be used in the main view. like buttons, list and titles.
    // Here you draw the elements
    App* app = context;

    // Reset the widget view
    widget_reset(app->widget);

    // Add an element to the view
    widget_add_string_element(
        app->widget, 60, 30, AlignCenter, AlignCenter, FontPrimary, "Hello World!");

    // Change the view, and display the elements
    view_dispatcher_switch_to_view(app->view_dispatcher, widget_view);
}

bool hello_world_scene_on_event(void* context, SceneManagerEvent event) {
    // In this function catches all the events that happen in the app
    App* app = context;
    bool consumed = false;
    UNUSED(app);
    UNUSED(event);
    return consumed;
}

void hello_world_scene_on_exit(void* context) {
    // In this function will execute all the actions just before it goes out from the scene
    App* app = context;

    //  Reset the view
    widget_reset(app->widget);
}

// This function works to send costum events in the app
static bool app_scene_costum_callback(void* context, uint32_t costum_event) {
    furi_assert(context);
    App* app = context;
    return scene_manager_handle_custom_event(app->scene_manager, costum_event);
}

// This function works to send a back evento in the app
static bool app_scene_back_event(void* context) {
    furi_assert(context);
    App* app = context;
    return scene_manager_handle_back_event(app->scene_manager);
}

// This function works to allocate the memory for the app
static App* app_alloc() {
    // Start allocating the memory of the structs
    App* app = malloc(sizeof(App)); // Allocating the app struct
    app->scene_manager =
        scene_manager_alloc(&app_scene_handlers, app); // Allocating memory for scene manager
    app->view_dispatcher = view_dispatcher_alloc(); // Allocating memory for the view dispatcher

    // Setting the context for the view dispatcher
    view_dispatcher_set_event_callback_context(app->view_dispatcher, app);

    // Setting the callback functions to send costum events
    view_dispatcher_set_custom_event_callback(app->view_dispatcher, app_scene_costum_callback);

    // Setting the back events for the view dispatcher
    view_dispatcher_set_navigation_event_callback(app->view_dispatcher, app_scene_back_event);

    app->widget = widget_alloc(); //  Allocating the memory for this element

    // Giving the number of the view dispatcher element
    view_dispatcher_add_view(app->view_dispatcher, widget_view, widget_get_view(app->widget));

    return app;
}

// This function works to free the allocated memory in the app
void app_free(App* app) {
    furi_assert(app);

    // Remove all the views in view dispatcher
    view_dispatcher_remove_view(app->view_dispatcher, widget_view);

    // Free the memory of the structures
    scene_manager_free(app->scene_manager);
    view_dispatcher_free(app->view_dispatcher);
}

// Main program
int app_main(void* p) {
    UNUSED(p);

    // Allocating the app struct
    App* app = app_alloc();

    // Open the GUI
    Gui* gui = furi_record_open(RECORD_GUI);

    // Attaching the view dispatcher with the GUI
    view_dispatcher_attach_to_gui(app->view_dispatcher, gui, ViewDispatcherTypeFullscreen);

    // Show the first scene on the flipper
    scene_manager_next_scene(app->scene_manager, hello_world_scene);

    // Run the view dispatcher
    view_dispatcher_run(app->view_dispatcher);

    // When the app finishes close the GUI record
    furi_record_close(RECORD_GUI);

    // Free the memory app
    app_free(app);

    return 0;
}
