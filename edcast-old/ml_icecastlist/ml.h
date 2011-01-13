/*
** Copyright (C) 2003 Nullsoft, Inc.
**
** This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held 
** liable for any damages arising from the use of this software. 
**
** Permission is granted to anyone to use this software for any purpose, including commercial applications, and to 
** alter it and redistribute it freely, subject to the following restrictions:
**
**   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. 
**      If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
**
**   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
**
**   3. This notice may not be removed or altered from any source distribution.
**
*/

#ifndef _ML_H_
#define _ML_H_


#define MLHDR_VER 0x15

typedef struct {
	int version;
	char *description;
	int (*init)();
	void (*quit)();


  // return NONZERO if you accept this message as yours, otherwise 0 to pass it 
  // to other plugins
  int (*MessageProc)(int message_type, int param1, int param2, int param3); 

  //all the following data is filled in by the library
	HWND hwndWinampParent;
	HWND hwndLibraryParent; // send this any of the WM_ML_IPC messages
	HINSTANCE hDllInstance;
} winampMediaLibraryPlugin;


// messages your plugin may receive on MessageProc()

#define ML_MSG_TREE_BEGIN 0x100
  #define ML_MSG_TREE_ONCREATEVIEW 0x100 // param1 = param of tree item, param2 is HWND of parent. return HWND if it is us

  #define ML_MSG_TREE_ONCLICK  0x101 // param1 = param of tree item, param2 = action type (below), param3 = HWND of main window
    #define ML_ACTION_RCLICK 0    // return value should be nonzero if ours
    #define ML_ACTION_DBLCLICK 1
    #define ML_ACTION_ENTER 2

  #define ML_MSG_TREE_ONDROPTARGET 0x102 // param1 = param of tree item, param2 = type of drop (ML_TYPE_*), param3 = pointer to data (or NULL if querying).
                                      // return -1 if not allowed, 1 if allowed, or 0 if not our tree item

  #define ML_MSG_TREE_ONDRAG 0x103 // param1 = param of tree item, param2 = POINT * to the mouse position, and param3 = (int *) to the type
                                  // set *(int*)param3 to the ML_TYPE you want and return 1, if you support drag&drop, or -1 to prevent d&d.

  #define ML_MSG_TREE_ONDROP 0x104 // param1 = param of tree item, param2 = POINT * to the mouse position
                                  // if you support dropping, send the appropriate ML_IPC_HANDLEDROP using SendMessage() and return 1, otherwise return -1.

#define ML_MSG_TREE_END 0x1FF // end of tree specific messages



// types for drag and drop

#define ML_TYPE_ITEMRECORDLIST 0 // if this, cast obj to itemRecordList
#define ML_TYPE_FILENAMES 1 // double NULL terminated char * to files, URLS, or playlists
#define ML_TYPE_STREAMNAMES 2 // double NULL terminated char * to URLS, or playlists ( effectively the same as ML_TYPE_FILENAMES, but not for files)
#define ML_TYPE_CDTRACKS 3 // if this, cast obj to itemRecordList (CD tracks) -- filenames should be cda://<drive letter>,<track index>. artist/album/title might be valid (CDDB)
#define ML_TYPE_QUERYSTRING 4 // char * to a query string
#define ML_TYPE_TREEITEM 69 // uhh?


// if you wish to put your tree items under "devices", use this constant for ML_IPC_ADDTREEITEM
#define ML_TREEVIEW_ID_DEVICES 10000



// children communicate back to the media library by SendMessage(plugin.hwndLibraryParent,WM_ML_IPC,param,ML_IPC_X);
#define WM_ML_IPC WM_USER+0x1000

#define ML_IPC_ADDTREEITEM 0x0101 // pass mlAddTreeItemStruct as the param
#define ML_IPC_SETTREEITEM 0x0102 // pass mlAddTreeItemStruct with this_id valid
#define ML_IPC_DELTREEITEM 0x0103 // pass param of tree item to remove
#define ML_IPC_GETCURTREEITEM 0x0104 // returns current tree item param or 0 if none
#define ML_IPC_SETCURTREEITEM 0x0105 // selects the tree item passed, returns 1 if found, 0 if not

typedef struct {
  int parent_id; //0=root, or ML_TREEVIEW_ID_*
  char *title;
  int has_children;

  int this_id; //filled in by the library on ML_IPC_ADDTREEITEM
} mlAddTreeItemStruct;

#define ML_IPC_GETFILEINFO 0x0200 // pass it a &itemRecord with a valid filename (and all other fields NULL), and it will try to fill in the rest

#define ML_IPC_HANDLEDRAG 0x0300 // pass it a &mlDropItemStruct it will handle cursors etc (unless flags has the lowest bit set), and it will set result appropriately:
#define ML_IPC_HANDLEDROP 0x0301 // pass it a &mlDropItemStruct with data on drop:

#define ML_IPC_SENDTOWINAMP 0x302 // send with a mlSendToWinampStruct:
typedef struct {
  int type; //ML_TYPE_ITEMRECORDLIST, etc
  void *data; // object to play

  int enqueue; // low bit set specifies enqueuing, and second bit NOT set specifies that 
               // the media library should use its default behavior as the user configured it (if 
               // enqueue is the default, the low bit will be flipped by the library)
} mlSendToWinampStruct;



#define ML_HANDLEDRAG_FLAG_NOCURSOR 1
#define ML_HANDLEDRAG_FLAG_NAME 2

typedef struct {
  int type; //ML_TYPE_ITEMRECORDLIST, etc
  void *data; // NULL if just querying

  int result; // filled in by client: -1 if dont allow, 0 if dont know, 1 if allow.

  POINT p; // cursor pos in screen coordinates
  int flags; 

  char *name; // only valid if ML_HANDLEDRAG_FLAG_NAME

} mlDropItemStruct;


#define ML_IPC_SKIN_LISTVIEW   0x0500 // pass the hwnd of your listview. returns a handle to use with ML_IPC_UNSKIN_LISTVIEW
#define ML_IPC_UNSKIN_LISTVIEW 0x0501 // pass the handle you got from ML_IPC_SKIN_LISTVIEW
#define ML_IPC_LISTVIEW_UPDATE 0x0502 // pass the handle you got from ML_IPC_SKIN_LISTVIEW
#define ML_IPC_LISTVIEW_DISABLEHSCROLL 0x0503 // pass the handle you got from ML_IPC_SKIN_LISTVIEW


#define ML_IPC_SKIN_WADLG_GETFUNC 0x0600 
    // 1: return int (*WADlg_getColor)(int idx); // pass this an index, returns a RGB value (passing 0 or > 3 returns NULL)
    // 2: return int (*WADlg_handleDialogMsgs)(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam); 
    // 3: return void (*WADlg_DrawChildWindowBorders)(HWND hwndDlg, int *tab, int tabsize); // each entry in tab would be the id | DCW_*
    // 32: return void (*childresize_init)(HWND hwndDlg, ChildWndResizeItem *list, int num);
    // 33: return void (*childresize_resize)(HWND hwndDlg, ChildWndResizeItem *list, int num);
   


// this gets sent to any child windows of the library windows, and then (if not
// handled) the library window itself

#define WM_ML_CHILDIPC WM_APP+0x800 // avoids conflicts with any windows controls
#define ML_CHILDIPC_DROPITEM 0x100         // lParam = 100, wParam = &mlDropItemStruct


// itemRecord type for use with ML_TYPE_ITEMRECORDLIST, as well as many other functions
typedef struct
{
  char *filename;
  char *title;
  char *album;
  char *artist;
  char *comment;
  char *genre;
  int year;
  int track;
  int length;
  char **extended_info;
} itemRecord;

typedef struct 
{
  itemRecord *Items;
  int Size;
  int Alloc;
} itemRecordList;

// utility functions in ml_lib.cpp
void freeRecordList(itemRecordList *obj);
void emptyRecordList(itemRecordList *obj); // does not free Items
void allocRecordList(itemRecordList *obj, int newsize, int granularity=1024);
void freeRecord(itemRecord *p);




#endif//_ML_H_