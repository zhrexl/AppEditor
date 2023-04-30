#include "Window/window.h"

static GtkWidget*
createPopOver()
{
  GMenu* Menu = g_menu_new();
  GMenuItem* item1 = g_menu_item_new("Preferences","AppEditor::Preferences");
  GMenuItem* item2 = g_menu_item_new("About","AppEditor::About");

  g_menu_append_item (Menu, item1);
  g_menu_append_item (Menu, item2);

  GtkWidget* popover = gtk_popover_menu_new_from_model (G_MENU_MODEL(Menu));
  return popover;
}

static GtkWidget*
createEditorPage(p_mainWindow Window)
{
  /*  Adwaita Widgets */
  GtkWidget* row1           = adw_action_row_new ();
  GtkWidget* row2           = adw_action_row_new ();
  GtkWidget* row3           = adw_action_row_new ();
  GtkWidget* clamp          = adw_clamp_new ();
  GtkWidget* preferences    = adw_preferences_group_new ();
  /*  GTK4 Widgets  */
  GtkWidget* clampBox       = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
  GtkWidget* hBox           = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,6);
  GtkWidget* box            = gtk_box_new(GTK_ORIENTATION_VERTICAL,12);
  GtkWidget* btnBox         = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,6);
  GtkWidget* openIcon       = gtk_button_new();
  GtkWidget* applybtn       = gtk_button_new_with_label ("Apply");
  GtkWidget* deleteBtn      = gtk_button_new_with_label ("Delete");
  GtkWidget* btnImg         = gtk_image_new_from_icon_name("document-open-symbolic");
  GtkWidget* scrolledWindow = gtk_scrolled_window_new ();
  GtkWidget* viewPort       = gtk_viewport_new (NULL,NULL);


  Window->lblOpenIcon    = gtk_label_new ("(None)");
  Window->entryName      = gtk_entry_new();
  Window->showApp        = gtk_switch_new ();
  Window->appIconImg     = gtk_image_new();
  Window->nameLbl        = gtk_label_new("");
  Window->idLbl          = gtk_label_new("");

  adw_clamp_set_maximum_size (ADW_CLAMP(clamp), 500);
  adw_clamp_set_tightening_threshold (ADW_CLAMP(clamp),350);

  gtk_label_set_ellipsize (GTK_LABEL (Window->idLbl), PANGO_ELLIPSIZE_MIDDLE);
  gtk_label_set_ellipsize (GTK_LABEL (Window->nameLbl), PANGO_ELLIPSIZE_MIDDLE);
  gtk_label_set_ellipsize (GTK_LABEL (Window->lblOpenIcon), PANGO_ELLIPSIZE_MIDDLE);
  gtk_label_set_max_width_chars (GTK_LABEL (Window->lblOpenIcon), 20);

  gtk_image_set_pixel_size (GTK_IMAGE(Window->appIconImg), 64);

  gtk_box_append (GTK_BOX(clampBox),Window->nameLbl);
  gtk_box_append (GTK_BOX(clampBox),Window->idLbl);
  gtk_box_append (GTK_BOX(clampBox),preferences);
  gtk_box_append (GTK_BOX(clampBox),hBox);

  gtk_box_append (GTK_BOX(box), Window->appIconImg);
  gtk_box_append (GTK_BOX(box), clamp);

  gtk_widget_set_vexpand (viewPort, true);
  gtk_widget_set_margin_top (scrolledWindow, 20);
  gtk_widget_set_margin_bottom (scrolledWindow, 20);


  adw_clamp_set_child (ADW_CLAMP(clamp), clampBox);
  adw_preferences_row_set_title (ADW_PREFERENCES_ROW (row1), "Show on Launcher");
  adw_action_row_set_subtitle (ADW_ACTION_ROW (row1), "if enabled this app will be listed on launcher grid");
  adw_action_row_add_suffix (ADW_ACTION_ROW(row1), Window->showApp);
  gtk_widget_set_valign (Window->showApp, GTK_ALIGN_CENTER);

  adw_preferences_row_set_title (ADW_PREFERENCES_ROW (row2), "Icon");

  gtk_box_append (GTK_BOX(btnBox), btnImg);
  gtk_box_append (GTK_BOX(btnBox), Window->lblOpenIcon);
  gtk_button_set_child (GTK_BUTTON (openIcon), btnBox);
  gtk_widget_set_valign (openIcon, GTK_ALIGN_CENTER);
  adw_action_row_add_suffix(ADW_ACTION_ROW(row2), openIcon);

  Window->entryBuffer = gtk_entry_buffer_new ("", 0);

  gtk_entry_set_buffer (GTK_ENTRY(Window->entryName), GTK_ENTRY_BUFFER(Window->entryBuffer));

  adw_preferences_row_set_title (ADW_PREFERENCES_ROW (row3), "Name");
  adw_action_row_add_suffix(ADW_ACTION_ROW(row3), Window->entryName);
  gtk_widget_set_valign (Window->entryName, GTK_ALIGN_CENTER);

  adw_preferences_group_add (ADW_PREFERENCES_GROUP (preferences), row3);
  adw_preferences_group_add (ADW_PREFERENCES_GROUP (preferences), row1);
  adw_preferences_group_add (ADW_PREFERENCES_GROUP (preferences), row2);

  gtk_widget_set_halign (hBox, GTK_ALIGN_END);

  gtk_widget_add_css_class (applybtn, "suggested-action");
  gtk_widget_add_css_class (Window->nameLbl, "title-1");
  gtk_widget_add_css_class (deleteBtn, "destructive-action");
  gtk_widget_add_css_class (Window->idLbl, "dim-label");

  gtk_box_append(GTK_BOX(hBox),applybtn);
  gtk_box_append(GTK_BOX(hBox),deleteBtn);


  //gtk_box_append(GTK_BOX(box),hBox);
  gtk_viewport_set_child (GTK_VIEWPORT (viewPort), box);

  gtk_viewport_set_scroll_to_focus (GTK_VIEWPORT (viewPort), true);


  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledWindow), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
  gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolledWindow),viewPort);

  return scrolledWindow;
}

void
build_window(p_mainWindow Window, GtkApplication *app)
{
  Window->listbox                   = gtk_list_box_new();
  GtkWidget *window         = adw_application_window_new(app);
  GtkWidget* scrolledWindow = gtk_scrolled_window_new ();
  GtkWidget* viewPort       = gtk_viewport_new (NULL,NULL);

  GtkWidget *headerBar      = adw_header_bar_new();
  GtkWidget *leftheaderBar  = adw_header_bar_new ();
  GtkWidget *menuButton     = gtk_menu_button_new ();
  GtkWidget *leaflet         = adw_leaflet_new ();

  GtkWidget *gBox           = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
  GtkWidget *separator      = gtk_separator_new(GTK_ORIENTATION_VERTICAL);
  GtkWidget *gBox_2         = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
  GtkWidget *emptyBox       = gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
  GtkWidget *findBtn         = gtk_button_new();

  gtk_button_set_icon_name(GTK_BUTTON(findBtn), "edit-find-symbolic");
  gtk_menu_button_set_icon_name (GTK_MENU_BUTTON (menuButton), "open-menu-symbolic");

  adw_header_bar_pack_start (ADW_HEADER_BAR (headerBar), findBtn);
  adw_header_bar_pack_end (ADW_HEADER_BAR (headerBar), menuButton);
  adw_header_bar_set_show_start_title_buttons(ADW_HEADER_BAR (headerBar), true);
  adw_header_bar_set_show_end_title_buttons(ADW_HEADER_BAR (headerBar), false);
  adw_header_bar_set_show_end_title_buttons (ADW_HEADER_BAR (leftheaderBar), true);
  adw_header_bar_set_title_widget (ADW_HEADER_BAR (leftheaderBar), emptyBox);

  adw_leaflet_set_can_navigate_back (ADW_LEAFLET (leaflet), true);

  gtk_box_append (GTK_BOX (gBox), headerBar);

  /*  Search Bar  */

  GtkWidget *search_bar;
  GtkWidget *box;
  GtkWidget *entry;

  search_bar = gtk_search_bar_new ();
  gtk_widget_set_valign (search_bar, GTK_ALIGN_START);
  gtk_box_append (GTK_BOX(gBox), search_bar);
  //gtk_widget_show (search_bar);

  box = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 6);
  gtk_search_bar_set_child (GTK_SEARCH_BAR (search_bar), box);

  entry = gtk_search_entry_new ();
  gtk_widget_set_hexpand (entry, FALSE);
  gtk_box_append (GTK_BOX (box), entry);

  gtk_search_bar_connect_entry (GTK_SEARCH_BAR (search_bar), GTK_EDITABLE (entry));
  gtk_search_bar_set_key_capture_widget (GTK_SEARCH_BAR (search_bar), window);

  /*
   * ListBox ScrolledWindow
   * */
  gtk_widget_set_vexpand (viewPort, true);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledWindow), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
  gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolledWindow),viewPort);
  gtk_widget_set_halign (Window->listbox, GTK_ALIGN_START);
  gtk_viewport_set_child (GTK_VIEWPORT (viewPort), Window->listbox);
  gtk_viewport_set_scroll_to_focus (GTK_VIEWPORT (viewPort), true);

  gtk_box_append (GTK_BOX (gBox), scrolledWindow);


  gtk_widget_set_size_request (gBox, 220, -1);

  gtk_widget_set_hexpand (GTK_WIDGET (gBox_2), true);
  gtk_widget_set_vexpand (GTK_WIDGET (gBox_2), true);

  gtk_box_append(GTK_BOX (gBox_2), leftheaderBar);
  gtk_box_append(GTK_BOX(gBox_2), createEditorPage(Window));

  adw_leaflet_append (ADW_LEAFLET (leaflet), gBox);
  adw_leaflet_append (ADW_LEAFLET (leaflet), separator);
  adw_leaflet_append (ADW_LEAFLET (leaflet), gBox_2);


  gtk_menu_button_set_popover (GTK_MENU_BUTTON(menuButton), createPopOver ());

  gtk_window_set_title (GTK_WINDOW (window), "Launcher Editor");
  gtk_window_set_default_size (GTK_WINDOW (window), 750, 475);
  adw_application_window_set_content(ADW_APPLICATION_WINDOW(window), leaflet);

  gtk_widget_add_css_class(Window->listbox, "navigation-sidebar");

  //gtk_list_box_set_activate_on_single_click (GTK_LIST_BOX (listbox), TRUE);


  gtk_window_present (GTK_WINDOW (window));
}

void connect_row_activated(p_mainWindow Window, GCallback callback)
{
  g_signal_connect (Window->listbox, "row-selected", callback, NULL);
}
