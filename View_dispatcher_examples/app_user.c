/**
 * This program works to create a counter in Flipper Zero
 */

#include <furi.h>
#include <furi_hal.h>
#include <gui/gui.h>
#include <gui/view_dispatcher.h>

// This library is included for the icons
// The name will be named by the app id and
// Adding "_icons"
#include "views_example_icons.h"

#define NEXT_VIEW 1 // Define to know if there is a change of view
#define BACK_VIEW 0 // Define used for return of view

// TAG to show in the LOGS
static char* TAG = "FLIPPERZERO:";

// To get the view
static uint32_t current_view = 0;

// Create my struct to controlate my GUIs
typedef struct {
    ViewDispatcher* view_dispatcher;
    View* first_view;
    View* second_view;
} App;

// Enumerate the views
typedef enum {
    FirstView,
    SecondView,
} views_enum;

// This function Works to draw the start scene
static void first_view_draw_callback(Canvas* canvas, void* context) {
    UNUSED(context);

    // Draw an icon
    canvas_draw_icon(canvas, 50, 5, &I_cat);

    // Set the font of the text
    canvas_set_font(canvas, FontPrimary);

    // draw a string
    canvas_draw_str_aligned(canvas, 60, 40, AlignCenter, AlignCenter, "ELECTRONIC CATS");

    // canvas_set_font(canvas, FontSecondary);
    canvas_draw_str_aligned(canvas, 60, 50, AlignCenter, AlignCenter, "Simple App");
}

// This function works for the inpust on the view
static bool first_view_input_event(InputEvent* input_event, void* context) {
    furi_assert(context);

    App* app = context;

    if(input_event->key == InputKeyRight) {
        if(input_event->type == InputTypeRelease) {
            view_dispatcher_send_custom_event(app->view_dispatcher, NEXT_VIEW);
        }
    }

    return false;
}

// This function works to draw the icon logo
static void second_view_draw_callback(Canvas* canvas, void* context) {
    UNUSED(context);

    // Draw an icon
    canvas_draw_icon(canvas, 20, 0, &I_donut);
}

// Input events in the second view
static bool second_view_input_event(InputEvent* input_event, void* context) {
    furi_assert(context);

    App* app = context;

    if(input_event->key == InputKeyLeft) {
        if(input_event->type == InputTypeRelease) {
            view_dispatcher_send_custom_event(app->view_dispatcher, BACK_VIEW);
        }
    }

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
    App* app = context;
    UNUSED(app);

    bool handle = false;

    if(event == NEXT_VIEW) {
        current_view++;
        view_dispatcher_switch_to_view(app->view_dispatcher, current_view);
        handle = true;

        // Print LOG for the CLI
        FURI_LOG_I(TAG, "NEXT VIEW");

    } else if(event == BACK_VIEW) {
        current_view--;
        view_dispatcher_switch_to_view(app->view_dispatcher, current_view);
        handle = true;

        // Print LOG for the CLI
        FURI_LOG_I(TAG, "PREVIOUS VIEW");
    }

    return handle;
}

// This function works to allocate the GUI
App* app_alloc() {
    // Allocating the memory of the App Struct
    App* app = (App*)malloc(sizeof(App));

    // Allocating the memory of view_dispatcher
    app->view_dispatcher = view_dispatcher_alloc();

    // Allocating the memory of view
    app->first_view = view_alloc();

    // Set the context in the view
    view_set_context(app->first_view, app);

    // Attaching the callback for the draw
    view_set_draw_callback(app->first_view, first_view_draw_callback);

    // Attaching the callback for the input events
    view_set_input_callback(app->first_view, first_view_input_event);

    // Set the orientation of the view
    view_set_orientation(app->first_view, ViewOrientationHorizontal);

    // Setting the second view
    app->second_view = view_alloc();
    view_set_context(app->second_view, app);
    view_set_draw_callback(app->second_view, second_view_draw_callback);
    view_set_input_callback(app->second_view, second_view_input_event);
    view_set_orientation(app->second_view, ViewOrientationHorizontal);

    // We set the context in the view dispatcher
    view_dispatcher_set_event_callback_context(app->view_dispatcher, app);

    // Set the function of custom event
    view_dispatcher_set_custom_event_callback(
        app->view_dispatcher, view_dispatcher_custom_event_callback);

    // Set the navegation event callback
    view_dispatcher_set_navigation_event_callback(
        app->view_dispatcher, view_dispatcher_navegation_event_callback);

    // Add the view to a view dispatcher
    view_dispatcher_add_view(app->view_dispatcher, FirstView, app->first_view);
    view_dispatcher_add_view(app->view_dispatcher, SecondView, app->second_view);

    return app;
}

void app_free(App* app) {
    // Remove the views from the view dispatcher
    view_dispatcher_remove_view(app->view_dispatcher, FirstView);

    // Remove second view
    view_dispatcher_remove_view(app->view_dispatcher, SecondView);

    // Free view memory
    view_free(app->first_view);

    // Free second view memory
    view_free(app->second_view);

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
    view_dispatcher_switch_to_view(app->view_dispatcher, current_view);

    view_dispatcher_run(app->view_dispatcher);

    app_free(app);

    return 0;
}
