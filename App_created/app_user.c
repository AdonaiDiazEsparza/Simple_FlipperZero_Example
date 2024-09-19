#include <furi.h>
#include <furi_hal.h>
#include <gui/gui.h>
#include <gui/view_dispatcher.h>

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
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str_aligned(canvas, 30, 60, AlignCenter, AlignCenter, "HelloWorld");
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

    return app;
}

int32_t app_main(void* p) {
    UNUSED(p);

    return 0;
}
