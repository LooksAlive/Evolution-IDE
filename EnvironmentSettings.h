#ifndef ENVIRONMENTSETTINGS_H
#define ENVIRONMENTSETTINGS_H


// tabs
constexpr auto NEW_TAB_NAME = "untitled";



// paths
constexpr auto FILE_DIALOG_PATH = "/home";



// fonts, margins,  positions
constexpr auto FONT_FAMILY = "Console";
constexpr auto FONT_SIZE = 16;

// files operation variables
static bool CHANGES_IN_PROJECT = false;

static bool ALWAYS_SAVE = false;         // set later in settings with one window message or in settings!!

static bool TABS_ACTIVE = false;
static bool BINARY_ACTIVE = false;
static bool DEBUGGER_ACTIVE = false;
static bool HEX_ACTIVE = false;


// C++ template examples to insert into code: check clangd
    /*
    main();
    templates,
    [[[[ FIX IT ]]]]
            
    void <name>()
    {
    
    }



*/





#endif // ENVIRONMENTSETTINGS_H
