#include <furi.h>
#include <furi_hal.h>
#include <gui/gui.h>
#include <gui/view_dispatcher.h>

// This library is included for the icons
// The name will be named by the app id and
// Adding "_icons"
#include "simple_app_view_disp_icons.h"

// Create my struct to controlate my GUIs
typedef struct {
    ViewDispatcher* view_dispatcher;
    View* my_view;
} App;

// Enumerate the views
typedef enum {
    FirstView,
} views_enum;

// This function Works to draw the hello world
static void my_draw_callback(Canvas* canvas, void* context) {
    furi_assert(context);
    UNUSED(context);

    // Draw an icon
    canvas_draw_icon(canvas, 60, 20, &I_cat);

    // Set the font of the text
    canvas_set_font(canvas, FontPrimary);

    // draw a string
    canvas_draw_str_aligned(canvas, 60, 50, AlignCenter, AlignCenter, "HelloWorld!");
}

// This function works for the inpust on the view
static bool input_callback(InputEvent* input_event, void* context) {
    furi_assert(context);
    UNUSED(context);
    UNUSED(input_event);
    return false;
}

// View dispathcher navegation event callback
bool view_dispatcher_navegation_event_callback(void* context) {
    furi_assert(context);
    UNUSED(context);
    return false;
}

// View dispatcher costum event callback
bool view_dispatcher_custom_event_callback(void* context, uint32_t event) {
    furi_assert(context);
    UNUSED(context);
    UNUSED(event);
    return false;
}

// This function works to allocate the GUI
App* app_alloc() {
    // Allocating the memory of the App Struct
    App* app = (App*)malloc(sizeof(App));

    // Allocating the memory of view_dispatcher
    app->view_dispatcher = view_dispatcher_alloc();

    // Allocating the memory of view
    app->my_view = view_alloc();

    // Set the context in the view
    view_set_context(app->my_view, app);

    // Attaching the callback for the draw
    view_set_draw_callback(app->my_view, my_draw_callback);

    // Attaching the callback for the input events
    view_set_input_callback(app->my_view, input_callback);

    // Set the orientation of the view
    view_set_orientation(app->my_view, ViewOrientationHorizontal);

    // We set the context in the view dispatcher
    view_dispatcher_set_event_callback_context(app->view_dispatcher, app);

    // Set the function of custom event
    view_dispatcher_set_custom_event_callback(
        app->view_dispatcher, view_dispatcher_custom_event_callback);

    // Set the navegation event callback
    view_dispatcher_set_navigation_event_callback(
        app->view_dispatcher, view_dispatcher_navegation_event_callback);

    // Add the view to a view dispatcher
    view_dispatcher_add_view(app->view_dispatcher, FirstView, app->my_view);

    return app;
}

void app_free(App* app) {
    // Remove the views from the view dispatcher
    view_dispatcher_remove_view(app->view_dispatcher, FirstView);

    // Free memory
    view_free(app->my_view);

    // Free viewdispatcher
    view_dispatcher_free(app->view_dispatcher);

    // Free struct
    free(app);
}

int32_t app_main(void* p) {
    UNUSED(p);

    App* app = app_alloc();
    Gui* gui = furi_record_open(RECORD_GUI);

    view_dispatcher_attach_to_gui(app->view_dispatcher, gui, ViewDispatcherTypeFullscreen);
    view_dispatcher_switch_to_view(app->view_dispatcher, FirstView);

    view_dispatcher_run(app->view_dispatcher);

    app_free(app);

    return 0;
}
