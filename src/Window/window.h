#ifndef WINDOW_H_
#define WINDOW_H_

#include <adwaita.h>

typedef struct mainWindow
{
  GtkWidget*      listbox;
  GtkWidget*      nameLbl;
  GtkWidget*      idLbl;
  GtkWidget*      appIconImg;
  GtkWidget*      entryName;
  GtkWidget*      showApp;
  GtkWidget*      lblOpenIcon;
  GtkEntryBuffer*  entryBuffer;
}*p_mainWindow;


void build_window(p_mainWindow Window, GtkApplication *app);
void connect_row_activated(p_mainWindow Window, GCallback callback);

#endif 
