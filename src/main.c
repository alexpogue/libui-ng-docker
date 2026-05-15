#include <ui.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    uiWindow *win;
} AboutState;

static void on_about_ok(uiButton *b, void *data)
{
    AboutState *s = (AboutState *)data;

    uiControlDestroy(uiControl(s->win));
    free(s);
}

static void on_button_clicked(uiButton *b, void *data)
{
    uiWindow *win = (uiWindow *)data;
    uiMsgBox(win, "Hello", "Hello from libui-ng!");
}

static void on_open(uiButton *b, void *data)
{
    uiWindow *win = (uiWindow *)data;

    char *filename = uiOpenFile(win);

    if (filename) {
        uiMsgBox(win, "Selected file", filename);
        uiFreeText(filename);
    } else {
        uiMsgBox(win, "Open", "No file selected");
    }
}

static void on_about(uiButton *b, void *data)
{
    //AboutState *s = uiAlloc(sizeof(AboutState));
    AboutState *s = malloc(sizeof(AboutState));
    if (!s) {
      fprintf(stderr, "Malloc failure allocating AboutState\n");
      exit(1);
    }
    s->win = uiNewWindow("About", 250, 120, 1);
    uiWindowSetMargined(s->win, 1);

    uiBox *box = uiNewVerticalBox();
    uiBoxSetPadded(box, 1);

    uiLabel *label = uiNewLabel("libui-ng demo app");
    uiBoxAppend(box, uiControl(label), 0);

    uiButton *ok = uiNewButton("OK");
    uiButtonOnClicked(ok, on_about_ok, s);
    uiBoxAppend(box, uiControl(ok), 0);

    uiWindowSetChild(s->win, uiControl(box));
    uiControlShow(uiControl(s->win));
}

int main(void)
{
    uiInitOptions o = {0};
    const char *err = uiInit(&o);
    if (err) return 1;

    uiWindow *win = uiNewWindow("Menu Test", 400, 200, 0);
    uiWindowSetMargined(win, 1);

    // ---- "toolbar" ----
    uiBox *menuBar = uiNewHorizontalBox();
    uiBoxSetPadded(menuBar, 1);

    uiButton *openBtn = uiNewButton("Open");
    uiButtonOnClicked(openBtn, on_open, win);

    uiButton *aboutBtn = uiNewButton("About");
    uiButtonOnClicked(aboutBtn, on_about, win);

    uiBoxAppend(menuBar, uiControl(openBtn), 0);
    uiBoxAppend(menuBar, uiControl(aboutBtn), 0);

    // ---- main content ----
    uiBox *root = uiNewVerticalBox();
    uiBoxSetPadded(root, 1);

    uiBoxAppend(root, uiControl(menuBar), 0);

    uiLabel *label = uiNewLabel("Hello world");
    uiBoxAppend(root, uiControl(label), 0);

    uiButton *btn = uiNewButton("Click me");
    uiButtonOnClicked(btn, on_button_clicked, win);
    uiBoxAppend(root, uiControl(btn), 0);

    uiWindowSetChild(win, uiControl(root));

    uiWindowOnClosing(win, (void *)uiQuit, NULL);

    uiControlShow(uiControl(win));
    uiMain();

    return 0;
}
