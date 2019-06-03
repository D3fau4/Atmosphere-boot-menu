/*  
 * Copyright (c) 2018 Guillem96
 *
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "utils/btn.h"
#include "menu/gui/gui_argon_menu.h"
#include "menu/gui/gui_menu.h"
#include "menu/gui/gui_menu_pool.h"

#include "gfx/gfx.h"

#include "utils/types.h"
#include "utils/fs_utils.h"
#include "utils/dirlist.h"
#include "utils/util.h"
#include "utils/touch.h"

#include "core/launcher.h"
#include "core/payloads.h"
#include "core/custom-gui.h"

#include "mem/heap.h"

#define COLUMNS 3  
#define ROWS 1
#define ELEM_SIZE 230
#define MARGIN_TOP 330
#define MARGIN_LEFT 45

static int tool_reboot_rcm(void* param);
static int tool_power_off(void* param);
//static int tool_ams(void* param);


/* Init needed menus for ArgonNX */
void gui_init_argon_menu(void)
{
		
    /* Init pool for menu */
    gui_menu_pool_init();

    gui_menu_t* menu = gui_menu_create("ArgonNX");
	
	//show display without icons
    gui_menu_open2(menu);
	
	//waith user input
        bool cancel_auto_chainloading = btn_read() & BTN_VOL_UP;
        if (!cancel_auto_chainloading)
		launch_payload("atmosphere/boot_menu/bin/fusee-primary.bin");



//create menu entries

u32 iconH = 300;
u32 iconW = 300;
u32 iconY = 180;
u32 iconX = 125;

u32 buttonH = 300;
u32 buttonW = 100;
u32 buttonY = iconY + 300;
u32 buttonX = iconX;

gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/boot-CFW.bmp"),iconX, iconY, iconW, iconH,(int (*)(void *))launch_payload, (void*)"atmosphere/boot_menu/bin/Atmosphere.bin"));
gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/boot-Stock.bmp"),iconX + 350, iconY, iconW, iconH,(int (*)(void *))launch_payload, (void*)"atmosphere/boot_menu/bin/stock.bin"));
gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/gear.bmp"),iconX + 700, iconY, iconW, iconH, NULL, NULL));

gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/button.bmp"),buttonX,buttonY, buttonH, buttonW, NULL, NULL));
gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/button.bmp"),buttonX + 350, buttonY, buttonH, buttonW, NULL, NULL));
gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/button.bmp"),buttonX + 700, buttonY, buttonH, buttonW, NULL, NULL));


     gui_menu_append_entry(menu, 
            gui_create_menu_entry_no_bitmap("Screenshot", 700, 680, 150, 100, (int (*)(void *))screenshot, NULL));

    /* Generate reboot rcm and shutdown entry */
    gui_menu_append_entry(menu, 
            gui_create_menu_entry_no_bitmap("Power off", 900, 680, 150, 100, tool_power_off, NULL));

    gui_menu_append_entry(menu, 
            gui_create_menu_entry_no_bitmap("Reboot RCM", 1100, 680, 150, 100, tool_reboot_rcm, NULL));

    /* Start menu */
    gui_menu_open(menu);

    /* Clear all entries and menus */
    gui_menu_pool_cleanup();
}
/*
static int tool_ams(void* param)
{
	display_backlight_brightness(1, 1000);
	copyfile("atmosphere/fusee-secondary.bin","sept/payload.bin");
	launch_payload("atmosphere/boot_menu/bin/Atmosphere.bin");
    return 0;
}
*/
static int tool_reboot_rcm(void* param)
{
    gui_menu_pool_cleanup();
    reboot_rcm();
    return 0;
}

static int tool_power_off(void* param)
{
    gui_menu_pool_cleanup();
    power_off();
    return 0;
}