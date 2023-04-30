#include "Window/window.h"
#include <glib.h>
#include <gio/gio.h>
#include <gio/gdesktopappinfo.h>

static struct mainWindow Window;

void
row_activated ( GtkListBox* self, GtkListBoxRow* row )
{
  if (self)
    g_assert (self);

  if (row)
    g_assert(row);

  if(!ADW_IS_ACTION_ROW (row))
    return;

  AdwActionRow* actionRow = ADW_ACTION_ROW (row);

  const char* title = adw_preferences_row_get_title(ADW_PREFERENCES_ROW(actionRow));
  gtk_label_set_text (GTK_LABEL(Window.nameLbl), title);

  const char* id = adw_action_row_get_subtitle(actionRow);

  GDesktopAppInfo* desktopAppInfo = g_desktop_app_info_new (id);


  GIcon * icon = g_app_info_get_icon (G_APP_INFO (desktopAppInfo));
  gtk_image_set_from_gicon(GTK_IMAGE(Window.appIconImg),icon);
  const char* iconname = g_icon_to_string (icon);
  gtk_label_set_text (GTK_LABEL(Window.lblOpenIcon), iconname);
  //g_object_unref(icon);

  gtk_entry_buffer_set_text (Window.entryBuffer, title, strlen(title));

  const gchar* description =   g_app_info_get_description(G_APP_INFO (desktopAppInfo));
  gtk_label_set_text (GTK_LABEL(Window.idLbl), description);

  gboolean isEnabled = g_desktop_app_info_get_nodisplay(desktopAppInfo);

  gtk_switch_set_state (GTK_SWITCH(Window.showApp), !isEnabled);



  g_object_unref(desktopAppInfo);

}

static gboolean
addToList(gpointer data)
{
  GtkWidget *app = adw_action_row_new ();
  GAppInfo *info = (GAppInfo*)data;

  if (!G_IS_APP_INFO(info))
    return FALSE;


  GIcon* gIcon = g_app_info_get_icon (info);
  GtkWidget* gImg = gtk_image_new_from_gicon (gIcon);

  gtk_image_set_icon_size (GTK_IMAGE (gImg), GTK_ICON_SIZE_LARGE);
  const char* displayName = g_app_info_get_display_name(info);

  adw_preferences_row_set_title (ADW_PREFERENCES_ROW (app), displayName);
  adw_action_row_set_title_lines (ADW_ACTION_ROW (app),1);
  adw_action_row_set_subtitle_lines(ADW_ACTION_ROW (app),1);
  adw_action_row_set_subtitle(ADW_ACTION_ROW (app), g_app_info_get_id (info));
  adw_action_row_add_prefix (ADW_ACTION_ROW (app), gImg);

  gtk_widget_set_halign (app, GTK_ALIGN_FILL);
  gtk_widget_set_margin_start (app, 0);

  gtk_list_box_append (GTK_LIST_BOX(Window.listbox), app);

  g_object_unref (info);
  return FALSE;
}

static void
print_app_list (GList *list)
{
  guint len = g_list_length (list);
  GList* apps = list;
  for (guint i = 0; i < len; i++)
  {
    GAppInfo *info = apps->data;
    g_idle_add ((GSourceFunc)addToList, info);
    apps = apps->next;
    list = g_list_delete_link (list, list);
  }
}

gpointer teste()
{
  GList* Apps = g_app_info_get_all();//g_desktop_app_info_get_implementations("");
  print_app_list (Apps);
  return NULL;
}

static void
activate_cb (GtkApplication *app)
{
  build_window (&Window, app);
  connect_row_activated(&Window, G_CALLBACK(row_activated));
    // multithread listing files async
  g_thread_new("thread_list_desktop_files", (GThreadFunc)teste,NULL);
}

int main (int   argc, char *argv[])
{
  g_autoptr (AdwApplication) app = NULL;

  app = adw_application_new ("com.github.AppEditor", G_APPLICATION_DEFAULT_FLAGS);

  g_signal_connect (app, "activate", G_CALLBACK (activate_cb), NULL);

  int app_status = g_application_run (G_APPLICATION (app), argc, argv);
  //g_object_unref(app);

  return app_status;
}
