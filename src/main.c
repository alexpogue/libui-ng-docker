#include <ui.h>
#include <stdio.h>

static void on_button_clicked(uiButton *b, void *data)
{
    uiWindow *win = (uiWindow *)data;

    uiMsgBox(win, "Hello", "Hello from libui-ng!");
}

int main(void)
{
    uiInitOptions o = {0};
    const char *err = uiInit(&o);

    if (err != NULL) {
        fprintf(stderr, "uiInit failed: %s\n", err);
        uiFreeInitError(err);
        return 1;
    }

    uiWindow *win = uiNewWindow("libui-ng Hello", 300, 150, 0);
    uiWindowSetMargined(win, 1);

    uiBox *vbox = uiNewVerticalBox();
    uiBoxSetPadded(vbox, 1);

    uiLabel *label = uiNewLabel("Hello, world!");
    uiBoxAppend(vbox, uiControl(label), 0);

    uiButton *btn = uiNewButton("Click me");
    uiBoxAppend(vbox, uiControl(btn), 0);

    uiWindowSetChild(win, uiControl(vbox));

    uiButtonOnClicked(btn, on_button_clicked, win);

    uiWindowOnClosing(win, (void *)uiQuit, NULL);

    uiControlShow(uiControl(win));

    uiMain();

    return 0;
}
