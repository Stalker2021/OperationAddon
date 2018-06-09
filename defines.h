#ifndef DEFINES_H
#define DEFINES_H

#define APP_NOTEPAD         "notepad.exe "

#define PATH_APP            QApplication::applicationDirPath()

#define WINDOW_MIN_WIDTH    700
#define WINDOW_MIN_FONT     7

#define WIND_NAME_SETTING   "Setting for \""
#define WIND_NAME_FIND_FILE "Find File"
#define WIND_NAME_FIND_INFO "Find Information"
#define WIND_NAME_RESULT    "Result"

#define PATH_SETTING        PATH_APP + "/setting.bin"
#define PATH_CONFIG         PATH_APP + "/config.bin"
#define PATH_ICO_SETTING    PATH_APP + "/OAS.ico"
#define PATH_ICO_CHECKING   PATH_APP + "/OAC.ico"
#define PATH_WAITSCREEN     PATH_APP + "/PW.jpg"
#define PATH_BUGFILE        PATH_APP + "/bug_file.txt"

#define CODE_OF_G1          0
#define CODE_OF_G2          1
#define CODE_OF_G3          2

#define NAME_FILE_PIC_PNG   "!pic.png"
#define NAME_FILE_PIC_JPG   "!pic.jpg"
#define NAME_FILE_DEPEND    "!depend.txt"
#define NAME_FILE_DESCRIP   "!descrip.txt"

#define EXTENSION_ALL       "*"
#define EXTENSION_ADDON     "*.pbo"
#define EXTENSIONONLY_ADDON ".pbo"

#define KEY_MISSION         "Missions"
#define KEY_CAMPAING        "Campaigns"

#define KEY_MISSION_DIR     "/Missions/AddonMissions"
#define KEY_CAMPAING_DIR    "/Campaigns"
#define KEY_ADDON_DIR       "/AddOns"

#define ICON_ERROR          QMessageBox::Critical
#define ICON_QUESTION       QMessageBox::Question
#define ICON_INFORMATION    QMessageBox::Information
#define ICON_ATTENTION      ICON_INFORMATION

#define TITLE_ATTENTION     "Attention"
#define TITLE_ERROR         "ERROR"
#define TITLE_INFORMATION   "Information"

#define TEXT_MISSINGCONFIG  "Config file missing or crashed!"
#define TEXT_MISSINGEXE     "Game \".exe\" file is missed!"
#define TEXT_ADDONADDED     "Addon is already added:"
#define TEXT_NOADDONFILES   "There are no addon files (.pbo):"
#define TEXT_FILEEXISTS     "File already exists:\n"
#define TEXT_WANTREPLACE    "\nWant to replace?"
#define TEXT_CANTCOPYFILE   "Can not copy file:\n"
#define TEXT_CANTDELETEFILE "Can not delete old file:\n"
#define TEXT_CANTCREATEDIR  "Can not create directory:\n"
#define TEXT_UNKNOWNADDON   "Unknown addon:\n"
#define TEXT_DELETEFROMUSED "\nRemove it from Used list?"
#define TEXT_TOOMANYGAMES   "Too many games in Config file."
#define TEXT_CANNOTDELETE   "This file cannot be deleted:\n"
#define TEXT_MBREADONLY     "\nMaybe this file have 'ReadOnly' status."
#define TEXT_ADDONDIRWRONG  "Addons directory is absent or wrong!"
#define TEXT_GAMEDIRWRONG   "Game directory is absent or wrong!"
#define TEXT_SAMENOTFOUND   "Identical files not found!"
#define TEXT_NOSIMILARITY   "Similarity not found!"
#define TEXT_SAMEFOUND      "Identical files was found:\n\n"
#define TEXT_USEDEMPTY      "No one addon is not used!"
#define TEXT_ALLUSEDNORM    "All used addon successfully added!"
#define TEXT_NOSELECTEADDON "No one Addon selected!"
#define TEXT_NODEPENDING    "Selected Addon have not depending."

#endif // DEFINES_H
