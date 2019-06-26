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
#include "utils/util.h"
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

//icon size
u32 iconH = 0;
u32 iconW = 0;
u32 iconY = 0;
u32 iconX = 0;

u32 buttonH = 0;
u32 buttonW = 0;
u32 buttonY = 0;
u32 buttonX = 0;

//menus
u64 main_menu = 0;
u32 submenu = 0;
u32 filemgr = 0;

//sub menus
u32 permsubY = 0;
u32 permsubX = 0;
u32 sub_buttonW = 0;
u32 sub_buttonH = 0;

//some indicators
u32 onemi = 0;
u32 retir = 0;

//dinamic directory
char *directory = "";

//funtions
static int tool_reboot_rcm(void* param);
static int tool_power_off(void* param);
static int tool_emu(u32 param);
int tool_dir(char *param);
int tool_Menus(u32 param);
int tool_servises(u32 param);
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

if (sd_file_exists("emummc/emummc.ini"))
{

char *str = sd_file_read("emummc/emummc.ini");
if(retir == 0)
{
	if(strlen(str) != strlen(str_replace(str, " ", "")))
	{
	str = str_replace(str, " ", "");
	u32 size = strlen(str)-1;
	sd_save_to_file(str,size,"emummc/emummc.ini");
	}
retir = 1;
	if(strlen(str) != strlen(str_replace(str, "mummc_enabled=1", "")))
	{retir = 2;}
}
//	gui_menu_append_entry(menu,gui_create_menu_entry_no_bitmap(str, 10, 100, 150, 200, NULL, NULL)); //- 3, + 260//debug
	if(retir == 2)
	{
		gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/button.bmp"),139,620, buttonH, buttonW,(int (*)(void *))tool_emu, (void*)0)); //630
	}else{
        gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/buttoff.bmp"),139,620, buttonH, buttonW,(int (*)(void *))tool_emu, (void*)1)); //630
	}
free(str);
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

gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/filem.bmp"),1000, 20, 200, 75,(int (*)(void *))tool_Menus, (void*)66));
//gui_menu_append_entry(menu,gui_create_menu_entry_no_bitmap("File MGR", 1000+20, 20+30, 150, 100, NULL, NULL));

permsubY = permsubY+80;
gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/services.bmp"),permsubX,permsubY, sub_buttonW, sub_buttonH,(int (*)(void *))tool_Menus, (void*)1));
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
gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/back.bmp"),permsubX,625, sub_buttonW, sub_buttonH,(int (*)(void *))tool_Menus, (void*)5));
//gui_menu_append_entry(menu,gui_create_menu_entry_no_bitmap("Back", permsubX+20, permsubY+30, 150, 100, NULL, NULL));

//remove
gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/screenshot.bmp"), 500, 625, sub_buttonW, sub_buttonH,(int (*)(void *))screenshot, NULL));
//remove
gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/bar.bmp"),0, 100, 1280, 2, NULL, NULL));
gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/bar.bmp"),0, 610, 1280, 2, NULL, NULL));
display_backlight_brightness(100, 1000);

if(submenu == 0)
{
gui_menu_append_entry(menu,gui_create_menu_entry_no_bitmap("welcome to the option menu",700, 300, 150, 100, NULL, NULL));

}

if(submenu == 1)
{
u32 linw=0;
u32 linealY = 100;
u32 linealX = 500;
u32 separ = 80;

//themas
linw++;
linealY = linealY + separ;
if(linw == 4)
linealX=1000;
	if (sd_file_exists("atmosphere/titles/0100000000001000/fsmitm.flag"))
	{
gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/gray_button.bmp"),linealX, linealY, 200, 75,(int (*)(void *))tool_servises, (void*)0));
	}else{
gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/boff.bmp"),linealX, linealY, 200, 75,(int (*)(void *))tool_servises, (void*)1));
	}
gui_menu_append_entry(menu,gui_create_menu_entry_no_bitmap("Themes",linealX+30, linealY+30, 150, 100, NULL, NULL));


//ftp
if (sd_file_exists("atmosphere/titles/420000000000000E/exefs.nsp"))
{
linw++;
if(linw == 4)
{
linealX=linealX+300;
linealY = 100;
linw=0;
}
linealY = linealY + separ;
	if (sd_file_exists("atmosphere/titles/420000000000000E/flags/boot2.flag"))
	{
gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/bon.bmp"),linealX, linealY, 200, 75,(int (*)(void *))tool_servises, (void*)2));
	}else{
gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/boff.bmp"),linealX, linealY, 200, 75,(int (*)(void *))tool_servises, (void*)3));
	}
gui_menu_append_entry(menu,gui_create_menu_entry_no_bitmap("FTP",linealX+30, linealY+30, 150, 100, NULL, NULL));
}

//Emuiibo
if (sd_file_exists("atmosphere/titles/0100000000000352/exefs.nsp"))
{
linw++;
if(linw == 4)
{
linealX=linealX+300;
linealY = 100;
linw=0;
}
linealY = linealY + separ;
	if (sd_file_exists("atmosphere/titles/0100000000000352/flags/boot2.flag"))
	{
gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/bon.bmp"),linealX, linealY, 200, 75,(int (*)(void *))tool_servises, (void*)4));
	}else{
gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/boff.bmp"),linealX, linealY, 200, 75,(int (*)(void *))tool_servises, (void*)5));
	}
gui_menu_append_entry(menu,gui_create_menu_entry_no_bitmap("Emuiio",linealX+30, linealY+30, 150, 100, NULL, NULL));
}

//Lan Play
if (sd_file_exists("atmosphere/titles/4200000000000010/exefs.nsp"))
{
linw++;
if(linw == 4)
{
linealX=linealX+300;
linealY = 100;
linw=0;
}
linealY = linealY + separ;
	if (sd_file_exists("atmosphere/titles/4200000000000010/flags/boot2.flag"))
	{
gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/bon.bmp"),linealX, linealY, 200, 75,(int (*)(void *))tool_servises, (void*)6));
	}else{
gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/boff.bmp"),linealX, linealY, 200, 75,(int (*)(void *))tool_servises, (void*)7));
	}
gui_menu_append_entry(menu,gui_create_menu_entry_no_bitmap("Lan Play",linealX+30, linealY+30, 150, 100, NULL, NULL));
}

//HDI
if (sd_file_exists("atmosphere/titles/0100000000000FAF/exefs.nsp"))
{
linw++;
if(linw == 4)
{
linealX=linealX+300;
linealY = 100;
linw=0;
}
linealY = linealY + separ;
	if (sd_file_exists("atmosphere/titles/0100000000000FAF/flags/boot2.flag"))
	{
gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/bon.bmp"),linealX, linealY, 200, 75,(int (*)(void *))tool_servises, (void*)8));
	}else{
gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/boff.bmp"),linealX, linealY, 200, 75,(int (*)(void *))tool_servises, (void*)9));
	}
gui_menu_append_entry(menu,gui_create_menu_entry_no_bitmap("HDI",linealX+30, linealY+30, 150, 100, NULL, NULL));
}

//sysplay
if (sd_file_exists("atmosphere/titles/420000000000000B/exefs.nsp"))
{
linw++;
if(linw == 4)
{
linealX=linealX+300;
linealY = 100;
linw=0;
}
linealY = linealY + separ;
	if (sd_file_exists("atmosphere/titles/420000000000000B/flags/boot2.flag"))
	{
gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/bon.bmp"),linealX, linealY, 200, 75,(int (*)(void *))tool_servises, (void*)10));
	}else{
gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/boff.bmp"),linealX, linealY, 200, 75,(int (*)(void *))tool_servises, (void*)11));
	}
gui_menu_append_entry(menu,gui_create_menu_entry_no_bitmap("sysplay",linealX+30, linealY+30, 150, 100, NULL, NULL));
}

//sysclock
if (sd_file_exists("atmosphere/titles/00FF0000636C6BFF/exefs.nsp"))
{
linw++;
if(linw == 4)
{
linealX=linealX+300;
linealY = 100;
linw=0;
}
linealY = linealY + separ;
	if (sd_file_exists("atmosphere/titles/00FF0000636C6BFF/flags/boot2.flag"))
	{
gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/bon.bmp"),linealX, linealY, 200, 75,(int (*)(void *))tool_servises, (void*)12));
	}else{
gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/boff.bmp"),linealX, linealY, 200, 75,(int (*)(void *))tool_servises, (void*)13));
	}
gui_menu_append_entry(menu,gui_create_menu_entry_no_bitmap("sysclock",linealX+30, linealY+30, 150, 100, NULL, NULL));
}


/*
gui_menu_append_entry(menu,gui_create_menu_entry_no_bitmap("AutoRCM",500, 120, 150, 100, NULL, NULL));
gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/autorcm1.bmp"),500,355, sub_buttonW, sub_buttonH,(int (*)(void *))tool_Menus, (void*)5));
gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/autorcm2.bmp"),900,355, sub_buttonW, sub_buttonH,(int (*)(void *))tool_Menus, (void*)5));
*/
}

if(submenu == 2)
{
gui_menu_append_entry(menu,gui_create_menu_entry_no_bitmap("Nand",500, 120, 150, 100, NULL, NULL));
gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/backup1.bmp"),500,355, sub_buttonW, sub_buttonH,(int (*)(void *))tool_Menus, (void*)20));
gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/backup2.bmp"),900,355, sub_buttonW, sub_buttonH,(int (*)(void *))tool_Menus, (void*)21));
gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/backup3.bmp"),700,455, sub_buttonW, sub_buttonH,(int (*)(void *))tool_Menus, (void*)22));
}

if(submenu == 3)
{
gui_menu_append_entry(menu,gui_create_menu_entry_no_bitmap("EmuMMC",500, 120, 150, 100, NULL, NULL));
gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/emummc1.bmp"),500,355, sub_buttonW, sub_buttonH,(int (*)(void *))tool_Menus, (void*)23));
gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/blank.bmp"),900,355, sub_buttonW, sub_buttonH,(int (*)(void *))tool_Menus, (void*)5));
}

if(submenu == 4)
{
gui_menu_append_entry(menu,gui_create_menu_entry_no_bitmap("Please select the payload you wish to launch below",650, 120, 150, 100, NULL, NULL));
char* payloads = dirlist("", "*.bin", false);
    u32 i = 0;
	u32 y = 180;
	u32 x = 500;
    while(payloads[i * 256])
    {
		if(strlen(&payloads[i * 256]) <= 100){			
		gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/files.bmp"),x, y+30, 500, 25,(int (*)(void *))launch_payload, (void*)&payloads[i * 256]));
		gui_menu_append_entry(menu,gui_create_menu_entry_no_bitmap(&payloads[i * 256], x+strlen(&payloads[i * 256])*8-40, y+35, 150, 100, NULL, NULL));						
//			gui_menu_append_entry(menu,gui_create_menu_entry_no_bitmap(&payloads[i * 256],600, y+30, 150, 100, (int (*)(void *))launch_payload, (void*)&payloads[i * 256]));

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
	u32 x = 10;
	u32 space = 50;
//	gui_menu_append_entry(menu,gui_create_menu_entry_no_bitmap(directory,600, y-20, 150, 100, (int (*)(void *))tool_Menus, (void*)33));
			gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read(""),1, 20, 300, 75,(int (*)(void *))tool_Menus, (void*)33));
		gui_menu_append_entry(menu,gui_create_menu_entry_no_bitmap(directory, strlen(directory)*8-40, 5, 150, 100, NULL, NULL));
	
    while(folder[r * 256])
    {
		if(strlen(&folder[r * 256]) <= 100){			
		gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/folder.bmp"),x, y+30, 500, 25,(int (*)(void *))tool_dir, (void*)&folder[r * 256]));
		gui_menu_append_entry(menu,gui_create_menu_entry_no_bitmap(&folder[r * 256], x+strlen(&folder[r * 256])*8-40, y+35, 150, 100, NULL, NULL));

		y = y + space;
		
		i++;
			if(i == 12){
			y = 90;
			x = 700;
			i = 0;
			}
		}
	r++;
	}

    while(files[w * 256])
    {
		if(strlen(&files[w * 256]) <= 100){
		gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/files.bmp"),x, y+30, 500, 25,NULL, NULL));
		gui_menu_append_entry(menu,gui_create_menu_entry_no_bitmap(&files[w * 256], x+strlen(&files[w * 256])*8-40, y+35, 150, 100, NULL, NULL));
		y = y + space;

	i++;			
			if(i == 12){
			y = 90;
			x = 600;
			i = 0;
			}	

		}
	w++;
	}


 }


}

//permanent icons for all
display_backlight_brightness(100, 1000);
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

int tool_servises(u32 param)
{
//Themes
if(param == 0)
f_unlink("atmosphere/titles/0100000000001000/fsmitm.flag");

if(param == 1)
sd_save_to_file("",0,"atmosphere/titles/0100000000001000/fsmitm.flag");

//ftp
if(param == 2)
f_unlink("atmosphere/titles/420000000000000E/flags/boot2.flag");

if(param == 3)
sd_save_to_file("",0,"atmosphere/titles/420000000000000E/flags/boot2.flag");

//Emuiibo
if(param == 4)
f_unlink("atmosphere/titles/0100000000000352/flags/boot2.flag");

if(param == 5)
sd_save_to_file("",0,"atmosphere/titles/0100000000000352/flags/boot2.flag");

//lan Play
if(param == 6)
f_unlink("atmosphere/titles/4200000000000010/flags/boot2.flag");

if(param == 7)
sd_save_to_file("",0,"atmosphere/titles/4200000000000010/flags/boot2.flag");

//HDI
if(param == 8)
f_unlink("atmosphere/titles/0100000000000FAF/flags/boot2.flag");

if(param == 9)
sd_save_to_file("",0,"atmosphere/titles/0100000000000FAF/flags/boot2.flag");

//sysplay
if(param == 10)
f_unlink("atmosphere/titles/420000000000000B/flags/boot2.flag");

if(param == 11)
sd_save_to_file("",0,"atmosphere/titles/420000000000000B/flags/boot2.flag");

//sysclock
if(param == 12)
f_unlink("atmosphere/titles/00FF0000636C6BFF/flags/boot2.flag");

if(param == 13)
sd_save_to_file("",0,"atmosphere/titles/00FF0000636C6BFF/flags/boot2.flag");

gui_init_argon_menu();
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
//iiii
if(param == 20)
{
display_backlight_brightness(1, 1000);
u8* buffer = (u8*)malloc(4);
sd_save_to_file(buffer, 4, "raw.bk");
launch_payload("atmosphere/boot_menu/bin/zbackup.bin");
return 0;
}

if(param == 21)
{
display_backlight_brightness(1, 1000);
u8* buffer = (u8*)malloc(4);
sd_save_to_file(buffer, 4, "syslite.bk");
launch_payload("atmosphere/boot_menu/bin/zbackup.bin");
return 0;
}

if(param == 22)
{
display_backlight_brightness(1, 1000);
u8* buffer = (u8*)malloc(4);
sd_save_to_file(buffer, 4, "boot.bk");
launch_payload("atmosphere/boot_menu/bin/zbackup.bin");
return 0;
}

if(param == 23)
{
display_backlight_brightness(1, 1000);
u8* buffer = (u8*)malloc(4);
sd_save_to_file(buffer, 4, "emummc.bk");
launch_payload("atmosphere/boot_menu/bin/zbackup.bin");
return 0;
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

static int tool_emu(u32 param)
{

if (!sd_mount()){BootStrapNX();}//check sd
		
if(param == 1)
{
char *str1 = sd_file_read("emummc/emummc.ini");
char* payload_wo_bin = str_replace(str1, "enabled=0", "enabled=1");
FIL op;
f_open(&op, "emummc/emummc.ini", FA_READ);
u32 size = f_size(&op);
f_close(&op);
sd_save_to_file(payload_wo_bin,size,"emummc/emummc.ini");
retir = 2;
}

if(param == 0)
{
char *str1 = sd_file_read("emummc/emummc.ini");
char* payload_wo_bin = str_replace(str1, "enabled=1", "enabled=0");
FIL op;
f_open(&op, "emummc/emummc.ini", FA_READ);
u32 size = f_size(&op);
f_close(&op);
sd_save_to_file(payload_wo_bin,size,"emummc/emummc.ini");
retir = 1;
}

/*	
if (sd_file_exists ("emummc/emummc.ini"))
{f_unlink("emummc/emummc.ini.bak");}

if (sd_file_exists ("emummc/emummc.ini"))
{
f_rename("emummc/emummc.ini","emummc/emummc.ini.bak");
}else{
f_rename("emummc/emummc.ini.bak","emummc/emummc.ini");

    FIL fp;
    f_open(&fp, "emummc/emummc.ini", FA_WRITE);
    f_puts(payload_wo_bin, &fp);
    f_close(&fp);
*/
gfx_swap_buffer(&g_gfx_ctxt);
	gui_init_argon_menu();
return 0;
}
