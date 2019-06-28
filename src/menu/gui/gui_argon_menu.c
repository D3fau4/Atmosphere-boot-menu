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
	if(retir == 2)
	{
		gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/button.bmp"),139,620, buttonH, buttonW,(int (*)(void *))tool_emu, (void*)0)); //630
	}else{
        gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/buttoff.bmp"),139,620, buttonH, buttonW,(int (*)(void *))tool_emu, (void*)1)); //630
	}
free(str);
}
    /* Generate reboot rcm and shutdown entry */ 
//gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/power.bmp"),900, 645,buttonH, 12, tool_power_off, NULL));//655
gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/reboot.bmp"),900, 620, buttonH, buttonW, tool_reboot_rcm, NULL));//655

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

permsubY = permsubY+80;
gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/RCM.bmp"),permsubX,permsubY, sub_buttonW, sub_buttonH,(int (*)(void *))tool_Menus, (void*)1));

permsubY = permsubY+80;
gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/nand.bmp"),permsubX,permsubY, sub_buttonW, sub_buttonH,(int (*)(void *))tool_Menus, (void*)2));

permsubY = permsubY+80;
gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/emummc.bmp"),permsubX,permsubY, sub_buttonW, sub_buttonH,(int (*)(void *))tool_Menus, (void*)3));

permsubY = permsubY+80;
gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/payload.bmp"),permsubX,permsubY, sub_buttonW, sub_buttonH,(int (*)(void *))tool_Menus, (void*)4));

permsubY = permsubY+150;
gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/back.bmp"),permsubX,625, sub_buttonW, sub_buttonH,(int (*)(void *))tool_Menus, (void*)5));
//gui_menu_append_entry(menu,gui_create_menu_entry_no_bitmap("Back", permsubX+20, permsubY+30, 150, 100, NULL, NULL));

//remove
gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/screenshot.bmp"), 500, 625, sub_buttonW, sub_buttonH,(int (*)(void *))screenshot, NULL));
//remove
gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/bar.bmp"),0, 100, 1280, 2, NULL, NULL));
gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/bar.bmp"),0, 610, 1280, 2, NULL, NULL));
gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/barh.bmp"),610, 0, 1280, 2, NULL, NULL));
display_backlight_brightness(100, 1000);

if(submenu == 0)
{
gui_menu_append_entry(menu,gui_create_menu_entry_no_bitmap("welcome to the option menu.",700, 300, 150, 100, NULL, NULL));

}

if(submenu == 1)
{
u32 linw=0;
u32 linealY = 100;
u32 linealX = 500;
u32 separ = 80;

gui_menu_append_entry(menu,gui_create_menu_entry_no_bitmap("Please, if your console is a patched unit, DONT ACTIVATE AUTORCM.",500, 120, 150, 100, NULL, NULL));
gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/autorcm1.bmp"),500,355, sub_buttonW, sub_buttonH,(int (*)(void *))tool_Menus, (void*)5));
gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/autorcm2.bmp"),900,355, sub_buttonW, sub_buttonH,(int (*)(void *))tool_Menus, (void*)5));

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

gfx_swap_buffer(&g_gfx_ctxt);
	gui_init_argon_menu();
return 0;
}
