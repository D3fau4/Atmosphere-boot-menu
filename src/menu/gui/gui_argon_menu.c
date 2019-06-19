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
#include <string.h>
#include "gfx/gfx.h"
#include "gfx/di.h"
#include "utils/types.h"
#include "utils/fs_utils.h"
#include "utils/dirlist.h"
#include "utils/util.h"
#include "utils/touch.h"
#include "utils/ini.h"

#include "core/launcher.h"
#include "core/payloads.h"
#include "core/custom-gui.h"

#include "mem/heap.h"

#define COLUMNS 2  
#define ROWS 1
#define ELEM_SIZE 230
#define MARGIN_TOP 330
#define MARGIN_LEFT 45
//antes 45
//emunand trigger
u32 emuNAND = 1;
u32 iconH = 0;
u32 iconW = 0;
u32 iconY = 0;
u32 iconX = 0;

u32 buttonH = 0;
u32 buttonW = 0;
u32 buttonY = 0;
u32 buttonX = 0;

//menus
u64 main_menu = 1;
u32 submenu = 0;
u32 filemgr = 1;

//sub menus
u32 permsubY = 0;
u32 permsubX = 0;
u32 sub_buttonW = 0;
u32 sub_buttonH = 0;

//dinamic directory
char *directory = "";

//funtions
static int tool_reboot_rcm(void* param);
static int tool_power_off(void* param);
static int tool_emu(void* param);
int tool_dir(char *param);
int tool_Menus(u32 param);

/* Init needed menus for ArgonNX */
void gui_init_argon_boot(void)
{
    /* Init pool for menu */
    gui_menu_pool_init();

    gui_menu_t* menu = gui_menu_create("ArgonNX");
	
	//show display without icons
    gui_menu_open2(menu);
	
	//waith user input
    bool cancel_auto_chainloading = btn_read() & BTN_VOL_UP;
    if (!cancel_auto_chainloading)
	launch_payload("atmosphere/boot_menu/bin/Atmosphere.bin");

gui_menu_pool_cleanup();
gui_init_argon_menu();
}

void gui_init_argon_menu(void)
{

    /* Init pool for menu */
    gui_menu_pool_init();

    gui_menu_t* menu = gui_menu_create("ArgonNX");
	//main menu
	if(main_menu == 0)
	{
//control panel
iconH = 300;
iconW = 300;
iconY = 210;
//antes 145 316
iconX = 145;

buttonH = 289;
buttonW = 65;
buttonY = iconY + 300;
buttonX = iconX;
//create menu entries
gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/boot-CFW.bmp"),iconX, iconY, iconW, iconH, (int (*)(void *))launch_payload, (void*)"atmosphere/boot_menu/bin/Atmosphere.bin"));
gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/boot-Stock.bmp"),iconX + 350, iconY, iconW, iconH, (int (*)(void *))launch_payload, (void*)"atmosphere/boot_menu/bin/stock.bin"));
gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/gear.bmp"),iconX + 700, iconY, iconW, iconH, (int (*)(void *))tool_Menus, (void*)9));


/*
gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/button.bmp"),buttonX,buttonY, buttonH, buttonW, (int (*)(void *))launch_payload, (void*)"atmosphere/boot_menu/bin/Atmosphere.bin"));

gui_menu_append_entry(menu,gui_create_menu_entry_no_bitmap("Boot custom FW", buttonX + 70, buttonY + 35, 150, 100, NULL, NULL));

gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/button.bmp"),buttonX + 350, buttonY, buttonH, buttonW, (int (*)(void *))launch_payload, (void*)"atmosphere/boot_menu/bin/stock.bin"));

gui_menu_append_entry(menu,gui_create_menu_entry_no_bitmap("Boot original FW", buttonX + 430, buttonY + 35, 150, 100, NULL, NULL));

gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/button.bmp"),buttonX + 700, buttonY, buttonH, buttonW, NULL, NULL));
gui_menu_append_entry(menu,gui_create_menu_entry_no_bitmap("Options", buttonX + 770, buttonY + 35, 150, 100, NULL, NULL));
*/

	//check emunand Status
    /*
    if (sd_file_exists ("emummc/emummc.ini"))
{
        gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/button.bmp"),buttonX - 120,buttonY + 825, buttonH, buttonW,tool_emu, NULL)); //- 80, - 500
        gui_menu_append_entry(menu,gui_create_menu_entry_no_bitmap("EmuMMC enabled", buttonX - 45, buttonY + 865, 150, 100, NULL, NULL));
		if (sd_file_exists("emummc/emummc.ini"))
        {
        gui_menu_append_entry(menu,gui_create_menu_entry_no_bitmap("EmuMMC enabled", buttonX - 45, buttonY + 865, 150, 100, NULL, NULL));
        }

        if (sd_file_exists("emummc/emummc.ini.bak"))
        {
        gui_menu_append_entry(menu,gui_create_menu_entry_no_bitmap("EmuMMC disabled", buttonX - 45, buttonY + 865, 150, 100, NULL, NULL)); // - 45. + 865
        }
}*/

if (sd_file_exists("emummc/emummc.ini") || sd_file_exists("emummc/emummc.ini.bak"))
{
    if (sd_file_exists("emummc/emummc.ini"))
        {
		gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/button.bmp"),139,620, buttonH, buttonW,tool_emu, NULL)); //630
        }
        if (sd_file_exists("emummc/emummc.ini.bak"))
        {
        gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/buttoff.bmp"),139,620, buttonH, buttonW,tool_emu, NULL)); //630
        }
}

/*
	if (emuNAND != 0)
	{
		gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/button.bmp"),buttonX - 80,buttonY - 500, buttonH, buttonW,tool_emu, NULL));
	
		if(emuNAND == 1)
		{
		gui_menu_append_entry(menu,gui_create_menu_entry_no_bitmap("EmuNAND enabled", buttonX - 3, buttonY + 260, 150, 100, NULL, NULL));
		}else{
		gui_menu_append_entry(menu,gui_create_menu_entry_no_bitmap("EmuNAND disabled", buttonX - 3, buttonY + 260, 150, 100, NULL, NULL));
		}
	}
    */

     gui_menu_append_entry(menu, 
            gui_create_menu_entry_no_bitmap("Screenshot", 700, 680, 150, 100, (int (*)(void *))screenshot, NULL));

    /* Generate reboot rcm and shutdown entry */ 
           /* gui_create_menu_entry_no_bitmap("Power off", 900, 680, 150, 100, tool_power_off, NULL));71, 12,*/
gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/power.bmp"),900, 645,71, 12, tool_power_off, NULL));//655
gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/reboot.bmp"),1100, 645, 82, 13, tool_reboot_rcm, NULL));//655
    /*gui_menu_append_entry(menu,gui_create_menu_entry(("atmosphere/boot_menu/gfx/reboot.bmp", 1100, 680, 150, 100, tool_reboot_rcm, NULL));*/

//end of main menu
}else{
//second menu and sub menus
	if(filemgr == 0)
	{
//unchanched icons
permsubY = 110;
permsubX = 80;
sub_buttonW = 289;
sub_buttonH = 65;
gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/poweroff.bmp"),permsubX,20, sub_buttonW, sub_buttonH,tool_power_off, NULL));
/* gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/poweroff.bmp"),5,20, 75, 75, NULL, NULL));
gui_menu_append_entry(menu,gui_create_menu_entry_no_bitmap("Poweroff", permsubX+20, 50, 150, 100, NULL, NULL));*/

gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/gray_button.bmp"),1000, 20, 200, 75,(int (*)(void *))tool_Menus, (void*)66));
gui_menu_append_entry(menu,gui_create_menu_entry_no_bitmap("File MGR", 1000+20, 20+30, 150, 100, NULL, NULL));

permsubY = permsubY+80;
gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/RCM.bmp"),permsubX,permsubY, sub_buttonW, sub_buttonH,(int (*)(void *))tool_Menus, (void*)1));
/* gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/RCM.bmp"),5,permsubY, 75, 75, NULL, NULL));
gui_menu_append_entry(menu,gui_create_menu_entry_no_bitmap("AutoRCM", permsubX+20, permsubY+30, 150, 100, NULL, NULL));*/


permsubY = permsubY+80;
gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/nand.bmp"),permsubX,permsubY, sub_buttonW, sub_buttonH,(int (*)(void *))tool_Menus, (void*)2));
/* gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/nand.bmp"),5,permsubY, 75, 75, NULL, NULL));
gui_menu_append_entry(menu,gui_create_menu_entry_no_bitmap("NAND", permsubX+20, permsubY+30, 150, 100, NULL, NULL));*/


permsubY = permsubY+80;
gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/emummc.bmp"),permsubX,permsubY, sub_buttonW, sub_buttonH,(int (*)(void *))tool_Menus, (void*)3));
/*gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/emummc.bmp"),5,permsubY, 75, 75, NULL, NULL));
gui_menu_append_entry(menu,gui_create_menu_entry_no_bitmap("EmuMMC", permsubX+20, permsubY+30, 150, 100, NULL, NULL));*/


permsubY = permsubY+80;
gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/payload.bmp"),permsubX,permsubY, sub_buttonW, sub_buttonH,(int (*)(void *))tool_Menus, (void*)4));
/* gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/payload.bmp"),5,permsubY, 75, 75, NULL, NULL));
gui_menu_append_entry(menu,gui_create_menu_entry_no_bitmap("Payloads", permsubX+20, permsubY+30, 150, 100, NULL, NULL));*/


permsubY = permsubY+150;
gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/back.bmp"),permsubX,permsubY, sub_buttonW, sub_buttonH,(int (*)(void *))tool_Menus, (void*)5));
//gui_menu_append_entry(menu,gui_create_menu_entry_no_bitmap("Back", permsubX+20, permsubY+30, 150, 100, NULL, NULL));

//remove
gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/screenshot.bmp"), 500, 610, sub_buttonW, sub_buttonH,(int (*)(void *))screenshot, NULL));
//remove
gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/bar.bmp"),0, 100, 1280, 2, NULL, NULL));
gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/bar.bmp"),0, 650, 1280, 2, NULL, NULL));
display_backlight_brightness(100, 1000);

if(submenu == 0)
{

gui_menu_append_entry(menu,gui_create_menu_entry_no_bitmap("welcome to the option menu",700, 300, 150, 100, NULL, NULL));

}

if(submenu == 1)
{
gui_menu_append_entry(menu,gui_create_menu_entry_no_bitmap("AutoRCM",500, permsubY+30, 150, 100, NULL, NULL));

}

if(submenu == 2)
{
gui_menu_append_entry(menu,gui_create_menu_entry_no_bitmap("Nand",500, permsubY+30, 150, 100, NULL, NULL));

}

if(submenu == 3)
{
gui_menu_append_entry(menu,gui_create_menu_entry_no_bitmap("EmuMMC",500, permsubY+30, 150, 100, NULL, NULL));
}

if(submenu == 4)
{
gui_menu_append_entry(menu,gui_create_menu_entry_no_bitmap("Please select the payload you wish to launch below",650, 120, 150, 100, NULL, NULL));
char* payloads = dirlist("", "*.bin", false);
    u32 i = 0;
	u32 y = 180;
    while(payloads[i * 256])
    {
		if(strlen(&payloads[i * 256]) <= 100){			
/*		
		const char* payload_wo_bin = str_replace(&payloads[i * 256], ".bin", "");
			gui_menu_append_entry(menu, 
					gui_create_menu_entry(payload_wo_bin,
                                    sd_file_read("atmosphere/boot_menu/gfx/gray_button.bmp"), 
                                    500, y,
                                    200, 75,
                                    (int (*)(void *))launch_payload, (void*)&payloads[i * 256]));
*/									
			gui_menu_append_entry(menu,gui_create_menu_entry_no_bitmap(&payloads[i * 256],600, y+30, 150, 100, (int (*)(void *))launch_payload, (void*)&payloads[i * 256]));

			}
	y = y + 70;
	//limit payloads to 6
	if(i >= 6)
	break;
	i++;	
    }

}

if(submenu == 5)
{
display_backlight_brightness(50, 1000);
main_menu = 0;
submenu = 0;
gui_init_argon_menu();

}
 }else{
 //file manager
 gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/power.bmp"),900, 645,71, 12, tool_power_off, NULL));//655
gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/gray_button.bmp"),1000, 20, 200, 75,(int (*)(void *))tool_Menus, (void*)77));
gui_menu_append_entry(menu,gui_create_menu_entry_no_bitmap("Exit", 1000+20, 20+30, 150, 100, NULL, NULL));
char* files = listfil(directory, "*", true);
char* folder = listfol(directory, "*", true);
		
    u32 r = 0;
    u32 w = 0;
    u32 i = 0;
	u32 y = 90;
	u32 space = 50;
//	gui_menu_append_entry(menu,gui_create_menu_entry_no_bitmap(directory,600, y-20, 150, 100, (int (*)(void *))tool_Menus, (void*)33));
			gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read(""),1, 20, 300, 75,(int (*)(void *))tool_Menus, (void*)33));
		gui_menu_append_entry(menu,gui_create_menu_entry_no_bitmap(directory, 100+20, 20+30, 150, 100, NULL, NULL));
	
    while(folder[r * 256])
    {
			if(strlen(&folder[r * 256]) <= 100){			
//			gui_menu_append_entry(menu,gui_create_menu_entry_no_bitmap(&folder[r * 256],600, y+30, 150, 100, (int (*)(void *))tool_dir, &folder[r * 256]));
			
			
		gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/folder.bmp"),10, y+30, 500, 25,(int (*)(void *))tool_dir, (void*)&folder[r * 256]));
		gui_menu_append_entry(menu,gui_create_menu_entry_no_bitmap(&folder[r * 256], strlen(&folder[r * 256])*8-50, y+35, 150, 100, NULL, NULL));
	y = y + space;

			}
	i++;
	r++;
	}

   y = 90;
    while(files[w * 256])
    {
			if(strlen(&files[w * 256]) <= 100){
		gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/files.bmp"),600+10, y+30, 500, 25,(int (*)(void *))tool_dir, (void*)&files[w * 256]));
		gui_menu_append_entry(menu,gui_create_menu_entry_no_bitmap(&files[w * 256], 570+strlen(&files[w * 256])*8, y+35, 150, 100, NULL, NULL));
			
//			gui_menu_append_entry(menu,gui_create_menu_entry_no_bitmap(&files[w * 256],800, y+30, 150, 100, NULL, NULL));
	y = y + space;
			
			}
	i++;
	w++;
	}
space = 50;

 
 }


}

//permanent icons for all

    /* Start menu */
    gui_menu_open(menu);

    /* Clear all entries and menus */
    gui_menu_pool_cleanup();
}


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

int tool_Menus(u32 param)
{
if(param == 77){
filemgr = 0;
gui_init_argon_menu();
}

if(param == 66){
filemgr = 1;
gui_init_argon_menu();
}


if(param == 33){
directory = "";
gui_init_argon_menu();
}

//summom option menu
if(param == 9){
display_backlight_brightness(50, 1000);
main_menu = 1;
gui_init_argon_menu();
return 0;}

//check if is the same menu
if(submenu == param)
return 0;

//set menu number
submenu = param;
gui_init_argon_menu();
return 0;
}

int tool_dir(char *param)
{

if(strlen(directory) <= 1)
{
directory = param;
}else{
strcat(directory, "/");
strcat(directory, param);

}
gui_init_argon_menu();
return 0;
}

static int tool_emu(void* param)
{
sd_mount();
        if (!g_sd_mounted)
        {return 0;}
if (sd_file_exists ("emummc/emummc.ini"))
{f_unlink("emummc/emummc.ini.bak");}

if (sd_file_exists ("emummc/emummc.ini"))
{
f_rename("emummc/emummc.ini","emummc/emummc.ini.bak");
}else{
f_rename("emummc/emummc.ini.bak","emummc/emummc.ini");
}
    gui_init_argon_menu();
return 0;
}