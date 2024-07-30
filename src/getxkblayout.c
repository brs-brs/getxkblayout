#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <X11/XKBlib.h>
#include <X11/extensions/XKBrules.h>

void print_help() {
    printf("Usage: getxkblayout [OPTION]\n");
    printf("Options:\n");
    printf("  -h, --help     Show this help message and exit\n");
    printf("  -l, --long     Output the full name of the layout\n");
    printf("  -s, --short    Output the short name of the layout (default)\n");
    printf("  -n             Do not output the trailing newline\n");
}

int main(int argc, char **argv) {
    int long_name = 0;
    int no_newline = 0;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-l") == 0 || strcmp(argv[i], "--long") == 0) {
            long_name = 1;
        } else if (strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--short") == 0) {
            long_name = 0;
        } else if (strcmp(argv[i], "-n") == 0) {
            no_newline = 1;
        } else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            print_help();
            return 0;
        } else {
            fprintf(stderr, "Unknown option: %s\n", argv[i]);
            print_help();
            return 1;
        }
    }

    Display *dpy = XOpenDisplay(NULL);
    if (dpy == NULL) {
        fprintf(stderr, "Cannot open display\n");
        exit(1);
    }

    XkbStateRec state;
    if (XkbGetState(dpy, XkbUseCoreKbd, &state) != Success) {
        fprintf(stderr, "Failed to get keyboard state\n");
        XCloseDisplay(dpy);
        exit(1);
    }

    XkbDescPtr desc = XkbGetKeyboard(dpy, XkbAllComponentsMask, XkbUseCoreKbd);
    if (desc == NULL) {
        fprintf(stderr, "Failed to get keyboard description\n");
        XCloseDisplay(dpy);
        exit(1);
    }

    char *group = XGetAtomName(dpy, desc->names->groups[state.group]);
    if (group == NULL) {
        fprintf(stderr, "Failed to get group name\n");
        XkbFreeKeyboard(desc, 0, True);
        XCloseDisplay(dpy);
        exit(1);
    }

    XkbRF_VarDefsRec vd;
    if (!XkbRF_GetNamesProp(dpy, NULL, &vd)) {
        fprintf(stderr, "Failed to get names property\n");
        XFree(group);
        XkbFreeKeyboard(desc, 0, True);
        XCloseDisplay(dpy);
        exit(1);
    }

    char *tok = strtok(vd.layout, ",");
    for (int i = 0; i < state.group; i++) {
        tok = strtok(NULL, ",");
        if (tok == NULL) {
            fprintf(stderr, "Failed to get layout name\n");
            XFree(group);
            XkbFreeKeyboard(desc, 0, True);
            XCloseDisplay(dpy);
            return 1;
        }
    }

    if (long_name) {
        if (no_newline) {
            printf("%s", group);
        } else {
            printf("%s\n", group);
        }
    } else {
        if (no_newline) {
            printf("%s", tok);
        } else {
            printf("%s\n", tok);
        }
    }

    XFree(group);
    XkbFreeKeyboard(desc, 0, True);
    XCloseDisplay(dpy);
    return 0;
}

