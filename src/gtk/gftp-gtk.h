/*****************************************************************************/
/*  gftp-gtk.h - include file for the gftp gtk+ 1.2 port                     */
/*  Copyright (C) 1998-2002 Brian Masney <masneyb@gftp.org>                  */
/*                                                                           */
/*  This program is free software; you can redistribute it and/or modify     */
/*  it under the terms of the GNU General Public License as published by     */
/*  the Free Software Foundation; either version 2 of the License, or        */
/*  (at your option) any later version.                                      */
/*                                                                           */
/*  This program is distributed in the hope that it will be useful,          */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of           */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            */
/*  GNU General Public License for more details.                             */
/*                                                                           */
/*  You should have received a copy of the GNU General Public License        */
/*  along with this program; if not, write to the Free Software              */
/*  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.                */
/*****************************************************************************/

#ifndef __GFTP_GTK_H
#define __GFTP_GTK_H

#include "../../lib/gftp.h"
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <pthread.h>

#define IS_ONE_SELECTED(wdata)		(GTK_CLIST ((wdata)->listbox)->selection && GTK_CLIST ((wdata)->listbox)->selection->next == NULL)
#define IS_NONE_SELECTED(wdata)		(GTK_CLIST ((wdata)->listbox)->selection == NULL)

#if GTK_MAJOR_VERSION == 1 && GTK_MINOR_VERSION == 2
  #define gtk_widget_set_size_request(widget, width, height)	\
				gtk_widget_set_usize (widget, width, height)
#endif

typedef struct gftp_window_data_tag
{
  GtkWidget *combo, 		/* Entry widget/history for the user to enter 
   				   a directory */
            *hoststxt, 		/* Show which directory we're in */
            *listbox; 		/* Our listbox showing the files */
  unsigned int sortcol, 	/* Which column we are sorting by */
               sortasds : 1, 	/* Sorted ascending or descending */
               sorted : 1,	/* Is the output sorted? */
               show_selected : 1, /* Show only selected files */
               *histlen;	/* Pointer to length of history */
  char *filespec;		/* Filespec for the listbox */
  gftp_request * request;	/* The host that we are connected to */
  GList * files,		/* Files in the listbox */
        ** history;		/* History of the directories */
  GtkItemFactory *ifactory; 	/* This is for the menus that will
                                   come up when you right click */
  pthread_t tid;		/* Thread for the stop button */
  unsigned long gotbytes;
} gftp_window_data;


typedef struct gftp_graphic_tag
{
  char * filename;
  GdkPixmap * pixmap;
  GdkBitmap * bitmap;
} gftp_graphic;


typedef struct gftp_dialog_data_tag
{
  GtkWidget * dialog,		/* Pointer to the dialog */
            * checkbox,		/* Pointer to the checkbox widget */
            * edit; 		/* Pointer to the edit widget */
  GList * all_buttons;		/* All the buttons of this dialog */
  gpointer data;		/* Data pointer we'll pass to the function */
} gftp_dialog_data;


typedef struct gftp_viewedit_data_tag 
{
   char *filename,              /* File we are viewing/editing currently */
        *remote_filename;       /* The filename on the remote computer */
   struct stat st;              /* Vital file statistics */
   pid_t pid;                   /* Our process id */
   char **argv;                 /* Our arguments we passed to execvp. We will 
                                   free it when the process terminates. This 
                                   is the safest place to free this */
   unsigned int view : 1,       /* View or edit this file */
                rm : 1,         /* Delete this file after we're done with it */
                dontupload : 1; /* Don't upload this file after we're done
				   editing it */
   gftp_window_data * fromwdata, /* The window we are viewing this file in */
                    * towdata;
} gftp_viewedit_data;


typedef struct gftp_curtrans_data_tag
{
  gftp_transfer * transfer;
  GList * curfle;
} gftp_curtrans_data;


extern gftp_window_data window1, window2, * other_wdata, * current_wdata;
extern GtkWidget * stop_btn, * hostedit, * useredit, * passedit,
                 * portedit, * logwdw, * dlwdw, * protocol_menu, * optionmenu;
extern GtkAdjustment * logwdw_vadj;
#if GTK_MAJOR_VERSION == 2 && GTK_MINOR_VERSION == 0
extern GtkTextMark * logwdw_textmark;
#endif
extern int local_start, remote_start, trans_start, log_start, tools_start;
extern GHashTable * graphic_hash_table;
extern GtkItemFactoryEntry * menus;
extern GtkItemFactory * factory;
extern pthread_mutex_t transfer_mutex, log_mutex;

/* bookmarks.c */
void run_bookmark 				( gpointer data );

void add_bookmark 				( gpointer data );

void edit_bookmarks 				( gpointer data );

void build_bookmarks_menu			( void );

/* chmod_dialog.c */ 
void chmod_dialog 				( gpointer data );

/* delete_dialog.c */ 
void delete_dialog 				( gpointer data );

/* dnd.c */
void openurl_get_drag_data 			( GtkWidget * widget, 
						  GdkDragContext * context, 
						  gint x,
						  gint y, 
						  GtkSelectionData * selection_data, 
						  guint info, 
						  guint32 clk_time, 
						  gpointer data );

void listbox_drag 				( GtkWidget * widget, 
						  GdkDragContext * context,
						  GtkSelectionData * selection_data, 
						  guint info, 
						  guint32 clk_time, 
						  gpointer data );

void listbox_get_drag_data 			( GtkWidget * widget, 
						  GdkDragContext * context, 
						  gint x,
						  gint y, 
						  GtkSelectionData * selection_data, 
						  guint info, 
						  guint32 clk_time, 
						  gpointer data );

/* gftp-gtk.c */
void toolbar_hostedit 				( GtkWidget * widget, 
						  gpointer data );

void sortrows 					( GtkCList * clist, 
						  gint column, 
						  gpointer data );

void stop_button				( GtkWidget * widget,
						  gpointer data );

/* menu_items.c */
void change_setting 				( gftp_window_data *wdata,
						  int menuitem,
						  GtkWidget * checkmenu );

void tb_openurl_dialog 				( gpointer data );

void openurl_dialog 				( gpointer data );

void disconnect 				( gpointer data );

void change_filespec 				( gpointer data );

void show_selected				( gpointer data );

void selectall 					( gpointer data );

void selectallfiles 				( gpointer data );

void deselectall 				( gpointer data );

void site_dialog 				( gpointer data );

int chdir_edit					( GtkWidget * widget,
						  gpointer data );

int chdir_dialog 				( gpointer data );

void clearlog 					( gpointer data );

void viewlog 					( gpointer data );

void savelog 					( gpointer data );

void clear_cache				( gpointer data );

void options_dialog (gpointer data);
void compare_windows (gpointer data);
void about_dialog (gpointer data);

/* misc-gtk.c */
void fix_display				( void );

void remove_files_window			( gftp_window_data * wdata );

void ftp_log					( gftp_logging_level level,
						  void *ptr,
						  const char *string,
						  ... );

void refresh 					( gftp_window_data * wdata );

void update_window_info				( void );

void update_window				( gftp_window_data * wdata );

GtkWidget * toolbar_pixmap			( GtkWidget * widget,
						  char *filename );

gftp_graphic * open_xpm				( GtkWidget * widget,
						  char *filename );

void gftp_free_pixmap 				( char *filename ); 

void gftp_get_pixmap 				( GtkWidget * widget, 
						  char *filename, 
						  GdkPixmap ** pix,
						  GdkBitmap ** bitmap );

int check_status				( char *name,
						  gftp_window_data * wdata,
						  int check_other_stop,
						  int only_one,
						  int at_least_one,
						  int func );

GtkItemFactory *item_factory_new                ( GtkType	       container_type,
						  const char	      *path,
						  GtkAccelGroup       *accel_group,
						  const char          *strip_prefix );

void create_item_factory 			( GtkItemFactory * ifactory, 
						  guint n_entries, 
						  GtkItemFactoryEntry * entries,
						  gpointer callback_data );

GList * get_next_selection 			( GList * selection, 
						  GList ** list, 
						  int *curnum );

void add_history 				( GtkWidget * widget, 
						  GList ** history, 
						  unsigned int *histlen, 
						  const char *str );

int check_reconnect 				( gftp_window_data * wdata );

void add_file_listbox 				( gftp_window_data * wdata, 
						  gftp_file * fle );

void MakeEditDialog 				( char *diagtxt, 
						  char *infotxt, 
						  char *deftext, 
						  int edit_shown, 
						  int erase, 
                                                  char *checktext,
						  char *oktxt, 
						  void (*okfunc) (), 
						  void *okptr, 
						  char *canceltxt, 
						  void (*cancelfunc) (), 
						  void *cancelptr );

void MakeYesNoDialog 				( char *diagtxt, 
						  char *infotxt, 
						  int erase, 
						  int num, 
						  ... );

void update_directory_download_progress 	( gftp_transfer * transfer );

void *generic_thread 				( void * (*func) 
                                                         (void *), 
						  gftp_window_data * wdata );

int progress_timeout 				( gpointer data );

void display_cached_logs			( void );

/* mkdir_dialog.c */
void mkdir_dialog 				( gpointer data );

/* rename_dialog.c */
void rename_dialog				( gpointer data );

/* transfer.c */
int ftp_list_files				( gftp_window_data * wdata,
						  int usecache );

int ftp_connect					( gftp_window_data * wdata,
						  gftp_request * request,
						  int getdir );

gint update_downloads 				( gpointer data );

void * gftp_gtk_transfer_files 			( void *data );

void get_files 					( gpointer data );

void put_files 					( gpointer data );

void transfer_window_files 			( gftp_window_data * fromwdata,
						  gftp_window_data * towdata );

void *do_getdir_thread 				( void * data );

void add_file_transfer 				( gftp_request * fromreq, 
						  gftp_request * toreq, 
						  gftp_window_data * fromwdata,
						  gftp_window_data * towdata, 
						  GList * files,
						  int copy_req );

void start_transfer				( gpointer data );

void stop_transfer				( gpointer data );

void skip_transfer				( gpointer data );

void remove_file_transfer 			( gpointer data );

void move_transfer_up				( gpointer data );

void move_transfer_down				( gpointer data );

void gftp_gtk_ask_transfer 			( gftp_transfer * tdata );

/* view_dialog.c */
void edit_dialog 				( gpointer data );

void view_dialog 				( gpointer data );

void view_file 					( char *filename, 
						  int fd, 
						  int viewedit, 
						  int del_file, 
						  int start_pos, 
						  int dontupload,
						  char *remote_filename, 
						  gftp_window_data * wdata );

#endif
