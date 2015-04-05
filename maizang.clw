; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CFileManage
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "maizang.h"
LastPage=0

ClassCount=5
Class1=CMaizangApp
Class2=CAboutDlg
Class3=CMaizangDlg
Class4=CTrueColorToolBar

ResourceCount=8
Resource1=IDR_MENU_ONLINE
Resource2=IDR_TOOLBAR_MAIN
Resource3=IDD_MAIZANG_DIALOG
Resource4=IDR_MENU_FILE
Resource5=IDD_ABOUTBOX
Class5=CFileManage
Resource6=IDD_FILEMANAGE
Resource7=IDR_MENU_MAIN
Resource8=IDR_TOOLBAR_FILE

[CLS:CMaizangApp]
Type=0
BaseClass=CWinApp
HeaderFile=maizang.h
ImplementationFile=maizang.cpp
Filter=N
VirtualFilter=AC
LastObject=CMaizangApp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=maizangDlg.cpp
ImplementationFile=maizangDlg.cpp
LastObject=CAboutDlg

[CLS:CMaizangDlg]
Type=0
BaseClass=CDialog
HeaderFile=maizangDlg.h
ImplementationFile=maizangDlg.cpp
LastObject=ID_FILE_COPY

[CLS:CTrueColorToolBar]
Type=0
BaseClass=CToolBar
HeaderFile=TrueColorToolBar.h
ImplementationFile=TrueColorToolBar.cpp

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_MAIZANG_DIALOG]
Type=1
Class=CMaizangDlg
ControlCount=2
Control1=IDC_Message,SysListView32,1350631425
Control2=IDC_Online,SysListView32,1350631425

[MNU:IDR_MENU_ONLINE]
Type=1
Class=?
Command1=IDM_ONLINE_CHOSEALL
Command2=IDM_ONLINE_CMD
Command3=IDM_ONLINE_PROCESS
Command4=IDM_ONLINE_WINDOW
Command5=IDM_ONLINE_DESKTOP
Command6=IDM_ONLINE_FLIE
Command7=IDM_ONLINE_AUDIO
Command8=IDM_ONLINE_VIDIO
Command9=IDM_ONLINE_REGISTRY
Command10=IDM_ONLINE_SERVER
Command11=IDM_ONLINE_DELETE
CommandCount=11

[MNU:IDR_MENU_MAIN]
Type=1
Class=?
Command1=IDM_MAIN_CLOSE
Command2=IDM_MAIN_SET
Command3=IDM_MAIN_BUILD
Command4=IDM_MAIN_ABOUT
CommandCount=4

[TB:IDR_TOOLBAR_MAIN]
Type=1
Class=?
Command1=IDM_ONLINE_DESKTOP
Command2=IDM_ONLINE_VIDIO
Command3=IDM_ONLINE_PROCESS
Command4=IDM_ONLINE_AUDIO
Command5=IDM_ONLINE_FLIE
Command6=IDM_ONLINE_WINDOW
Command7=IDM_ONLINE_CMD
Command8=IDM_ONLINE_REGISTRY
Command9=IDM_ONLINE_SERVER
Command10=IDM_MAIN_ABOUT
Command11=IDM_MAIN_SET
Command12=IDM_MAIN_BUILD
Command13=IDM_MAIN_CLOSE
CommandCount=13

[DLG:IDD_FILEMANAGE]
Type=1
Class=CFileManage
ControlCount=5
Control1=IDC_STATIC,static,1342308352
Control2=IDC_EDIT1,edit,1350631552
Control3=IDC_BTN_ADDRGO,button,1342242816
Control4=IDC_FILETREE,SysTreeView32,1350631427
Control5=IDC_FILELIST,SysListView32,1350631429

[CLS:CFileManage]
Type=0
HeaderFile=FileManage.h
ImplementationFile=FileManage.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_FILETREE
VirtualFilter=dWC

[TB:IDR_TOOLBAR_FILE]
Type=1
Class=?
Command1=ID_FILE_UP
Command2=ID_FILE_COPY
Command3=ID_FILE_PASTE
Command4=ID_FILE_DELETE
Command5=ID_FILE_UPLAOD
Command6=ID_FILE_DOWNLOAD
Command7=ID_FILE_FRESH
Command8=ID_FILE_VIEW
CommandCount=8

[MNU:IDR_MENU_FILE]
Type=1
Class=?
Command1=ID_FILE_RUNNOMAL
Command2=ID_FILE_RUNHIDE
Command3=ID_FILE_COPY
Command4=ID_FILE_PASTE
Command5=ID_FILE_DELETE
Command6=ID_FILE_UPLOAD
Command7=ID_FILE_DOWNLOAD
Command8=ID_FILE_FRESH
CommandCount=8

