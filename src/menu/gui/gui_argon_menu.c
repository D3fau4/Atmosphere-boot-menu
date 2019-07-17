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

//some indicators
u32 retir = 0;



//funtions
int tool_CFW(void* param);
static int tool_emu(u32 param);
static int tool_reboot_rcm(void* param);
static int tool_power_off(void* param);

/* Init needed menus for ArgonNX */
void gui_init_argon_boot(void)
{
    /* Init pool for menu */
    gui_menu_pool_init();

    gui_menu_t* menu = gui_menu_create("ArgonNX");
	
	//show display without icons
    gui_menu_open2(menu);
	if (sd_file_exists("emummc/emummc.ini"))
	{
			char *str = sd_file_read("emummc/emummc.ini");
			//delete incompatible prefix
			if(strstr(str,"emummc_") != NULL)
			{
			str = str_replace(str, "emummc_", "");
			u32 size = strlen(str)-1;
			sd_save_to_file(str,size,"emummc/emummc.ini");
			}
	
			//delete Spaces
			if(strstr(str," ") != NULL)
			{
			str = str_replace(str, " ", "");
			u32 size = strlen(str)-1;
			sd_save_to_file(str,size,"emummc/emummc.ini");
			}

			//indicate emummc ON
			if(strstr(str,"enabled=1") != NULL)
			{retir = 1;}
	}

	
	//waith user input
    bool cancel_auto_chainloading = btn_read() & BTN_VOL_UP;
    if (!cancel_auto_chainloading)
	{
	if(retir == 1)
	tool_CFW(NULL);
	
	if (sd_file_exists("license.txt"))
	tool_CFW(NULL);
	}

gui_menu_pool_cleanup();
gui_init_argon_menu();
}

void gui_init_argon_menu(void)
{
if (!sd_mount()){BootStrapNX();}//check sd
    /* Init pool for menu */
    gui_menu_pool_init();

    gui_menu_t* menu = gui_menu_create("ArgonNX");


	//control panel
	iconH = 300;
	iconW = 300;
	iconY = 210;
	iconX = 145;//antes 145 316

	buttonH = 289;
	buttonW = 65;
	buttonY = 625;
	buttonX = iconX;

	//create menu entries
	gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/boot-CFW.bmp"),iconX, iconY, iconW, iconH, tool_CFW ,NULL));
	gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/boot-Stock.bmp"),iconX + 350, iconY, iconW, iconH, (int (*)(void *))launch_payload, (void*)"atmosphere/boot_menu/bin/stock.bin"));
	gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/gear.bmp"),iconX + 700, iconY, iconW, iconH,(int (*)(void *))launch_payload, (void*)"atmosphere/boot_menu/bin/zBackup.bin"));

	gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/poweroff.bmp"),buttonX+5-30,buttonY, buttonH, buttonW,tool_power_off, NULL));
	gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/screenshot.bmp"), buttonX+5 + 350, buttonY, buttonH, buttonW,(int (*)(void *))screenshot, NULL));
	gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/reboot.bmp"),buttonX+5 + 730, buttonY, buttonH, buttonW, tool_reboot_rcm, NULL));//655

	if (sd_file_exists("emummc/emummc.ini"))
	{
		if(retir == 1)
		gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/button.bmp"),buttonX+5-30,120, buttonH, buttonW,(int (*)(void *))tool_emu, (void*)0)); //630
		else
		gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/buttoff.bmp"),buttonX+5-30,120, buttonH, buttonW,(int (*)(void *))tool_emu, (void*)1)); //630
	}


//gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/power.bmp"),900, 645,buttonH, 12, tool_power_off, NULL));//655

//menu int
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

int tool_CFW(void* param)
{
if (!sd_mount()){BootStrapNX();}//check sd

	//reactivate emummc
	if(retir == 0)
	{
		char *str1 = sd_file_read("emummc/emummc.ini");
		char* payload_wo_bin = str_replace(str1, "enabled=0", "enabled=1");
		FIL op;
		f_open(&op, "emummc/emummc.ini", FA_READ);
		u32 size = f_size(&op);
		f_close(&op);
		sd_save_to_file(payload_wo_bin,size,"emummc/emummc.ini");
		retir = 1;
	}
	if(sd_file_exists("license.txt"))
	sd_save_to_file("",0,"atmosphere/titles/0100000000001000/fsmitm.flag");
	
	//panic
	if (btn_read() & BTN_VOL_DOWN)
	{
	gfx_clear_color(&g_gfx_ctxt, 0xFFFFFFFF);
	gfx_swap_buffer(&g_gfx_ctxt);
		msleep(2000);
	f_unlink("/atmosphere/titles/0100000000001000/fsmitm.flag");	
	//this is a panic option so i will disable the servises also
	f_unlink("/atmosphere/titles/420000000000000E/flags/boot2.flag");
	f_unlink("/atmosphere/titles/0100000000000352/flags/boot2.flag");
	f_unlink("/atmosphere/titles/4200000000000010/flags/boot2.flag");
	f_unlink("/atmosphere/titles/420000000000000B/flags/boot2.flag");
	f_unlink("/atmosphere/titles/0100000000000FAF/flags/boot2.flag");
	}
	
	//Activate AutoBoot
	if(!sd_file_exists("license.txt"))
	{
				FIL fp;
				f_open(&fp, "license.txt", FA_WRITE | FA_CREATE_ALWAYS);
				f_puts("Borra este fichero para desactivar el Autoboot", &fp);
				f_puts("\n", &fp);
				f_puts("Delete this file to Disable Autoboot", &fp);
				f_close(&fp);
	}

//https://www.fullserieshd.com/series/descargar-serie-virtual-hero-por-mega/

if(sd_file_exists("device.keys"))
launch_payload("atmosphere/boot_menu/bin/Atmosphere.bin");
else
launch_payload("atmosphere/boot_menu/bin/Lockpick_RCM.bin");
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
	retir = 1;
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
	retir = 0;
	}

gfx_swap_buffer(&g_gfx_ctxt);
	gui_init_argon_menu();
return 0;
}
