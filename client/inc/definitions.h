#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include "header.h"

typedef unsigned char * code;
#define QUERY_DELIM "@"

#define START_PAGE "0"

//GET REQUESTS
#define GET_CHATS "V"
#define GET_MESSAGES "T"
#define GET_COLLOCUTOR_INFO 'K'
#define GET_USER_INFO "H"

//POST REQUESTS
#define SEND_MESSAGE 'S'
#define CREATE_CHAT 'C'
#define EDIT_MESSAGE 'B'
#define DELETE_MESSAGE 'D'
#define LEAVE_CHAT 'E'
#define EXIT_APP 'Q'
#define RESET_MESSAGES_STATUS "G"
#define UPDATE_USER_INFO "A"

//STATUS CODES
#define ACTIVE_STATUS "A"
#define EDITED_STATUS "E"
#define DELETED_STATUS "D"

//OTHERS
#define LOGIN 'L'
#define SIGNUP 'R'

//CODES
#define WAIT_FOR_CODE "W"
#define OK_CODE "Y"
#define ERROR_CODE "N"
#define NO_DATA_CODE "E"
#define RECORD_EXISTS_CODE "J"

#define DB_ROWS_MAX 10000
#define MESSAGE_MAX_LEN 4096
#define KEY_LENGHT 4096
typedef unsigned char * code;

#define CONNECTED 0
#define NO_LOGIN -1
#define NO_CONNECTED -2
#define LOST_CONNECT -3

#define AVATAR_PATH "client/resources/icons/user_icons/%s.png"
#define SETTINGS_PATH "client/resources/settings.txt"
#define MY_MESSAGE_UI_PATH "client/resources/ui/my_message.glade"
#define OTHER_MESSAGE_UI_PATH "client/resources/ui/other_message.glade"
#define CHAT_LIST_UI_PATH "client/resources/ui/chat_list.glade"
#define CHAT_UI_PATH "client/resources/ui/chat.glade"
#define ERROR_UI_PATH "client/resources/ui/error.glade"
#define LOGIN_UI_PATH "client/resources/ui/login.glade"
#define SIGNUP_UI_PATH "client/resources/ui/signup.glade"
#define MY_OPTIONS_POPUP_PATH "client/resources/ui/my_options_popup.glade"
#define OTHER_OPTIONS_POPUP_PATH "client/resources/ui/other_options_popup.glade"
#define CHAT_OPTIONS_POPUP_PATH "client/resources/ui/chat_list_popup.glade"
#define RECONNECT_GIF_PATH "client/resources/images/reconnect.gif"

//THEMES
#define DARK_THEME_PATH "client/resources/css/dark/chat.css"
#define WHITE_THEME_PATH "client/resources/css/white/chat.css"
#define BLUE_THEME_PATH "client/resources/css/blue/chat.css"
#define PINK_THEME_PATH "client/resources/css/pink/chat.css"
#define YELLOW_THEME_PATH "client/resources/css/yellow/chat.css"
#define PURPLE_THEME_PATH "client/resources/css/purple/chat.css"


#endif
