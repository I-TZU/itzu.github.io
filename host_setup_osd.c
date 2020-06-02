
#pragma GCC diagnostic error "-Wall"

//*******************************************************************************
//                              I N C L U D E   F I L E
//*******************************************************************************
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include "hd_util.h"
#include "ab_os_api.h"
#include "ab_mm.h"
#include "ab_display.h"
#include "ab_osd.h"
#include "main.h"
#include "sysdef.h"
#include "host_setup_def.h"
#include "host_setup_lib.h"
#include "host_setup_menu_main.h"
#include "host_setup_factory_menu_main.h"
#include "host_ui_db.h"
#include "host_osd.h"
#include "host_view_lib.h"
#include "host_setup_osd.h"
#include "host_menu.h"
#include "host_mode.h"

#include "ui_lifetest.h"

//*******************************************************************************
//                              D A T A   T Y P E S
//*******************************************************************************

//*******************************************************************************
//                              C O N S T A N T S
//*******************************************************************************

extern HOST_SETUP_ST *gps_setup;
HOST_SETUP_MENU_OSD_INFO s_setup_osd;
AB_OSD_BUF_ST gs_osd_display_buf;
AB_OSD_BUF_ST gs_osd_enc_buf;

char *FactroySubItem_Restore[2] = {"RESTORE", "Long press enter key 2 sec to active"};
uint8_t ui_dbg_draw_times[2] = {0, 0};

//*******************************************************************************
//                  F U N C T I O N   D E C L A R A T I O N S
//*******************************************************************************
void host_setup_menu_osd_get_info(HOST_STREAM_ID eStreamId)
{
	uint32_t uiStreamW = 0;
	uint32_t uiStreamH = 0;

	if (eStreamId == HOST_STREAM_ID_UVC)
	{
		HostPipeline_GetStreamResolution(HOST_STREAM_ID_UVC, (UINT32 *)&uiStreamW, (UINT32 *)&uiStreamH);
		
		if(uiStreamW==7680 && uiStreamH==4320){
			s_setup_osd.ui_enc_resolution = HOST_OSD_MENU_RESOLUTION_8K;			
			s_setup_osd.ui_enc_w = 3456;
			s_setup_osd.ui_enc_h = 1680*1.2;
			s_setup_osd.ui_enc_x = ((uiStreamW-s_setup_osd.ui_enc_w)/2);
			s_setup_osd.ui_enc_y = ((uiStreamH-s_setup_osd.ui_enc_h)/2);
			s_setup_osd.ui_enc_font_size = 72;
		}else if(uiStreamW==3840 && uiStreamH==2160){
			s_setup_osd.ui_enc_resolution = HOST_OSD_MENU_RESOLUTION_4K;
			s_setup_osd.ui_enc_w = 1728;
			s_setup_osd.ui_enc_h = 840*1.2;
			s_setup_osd.ui_enc_x = ((uiStreamW-s_setup_osd.ui_enc_w)/2);
			s_setup_osd.ui_enc_y = ((uiStreamH-s_setup_osd.ui_enc_h)/2);
			s_setup_osd.ui_enc_font_size = 36;
		}else if(uiStreamW==1920 && uiStreamH==1080){
			s_setup_osd.ui_enc_resolution = HOST_OSD_MENU_RESOLUTION_FULLHD;
			s_setup_osd.ui_enc_w = 864;
			s_setup_osd.ui_enc_h = 420*1.2;
			s_setup_osd.ui_enc_x = ((uiStreamW-s_setup_osd.ui_enc_w)/2);
			s_setup_osd.ui_enc_y = ((uiStreamH-s_setup_osd.ui_enc_h)/2);
			s_setup_osd.ui_enc_font_size = 18;
		}else if(uiStreamW==1280 && uiStreamH==720){
			s_setup_osd.ui_enc_resolution = HOST_OSD_MENU_RESOLUTION_HD;
			s_setup_osd.ui_enc_w = 576;
			s_setup_osd.ui_enc_h = 280*1.2;
			s_setup_osd.ui_enc_x = ((uiStreamW-s_setup_osd.ui_enc_w)/2);
			s_setup_osd.ui_enc_y = ((uiStreamH-s_setup_osd.ui_enc_h)/2);
			s_setup_osd.ui_enc_font_size = 12;
		}else if(uiStreamW==720 && uiStreamH==480){
			s_setup_osd.ui_enc_resolution = HOST_OSD_MENU_RESOLUTION_HD;
			s_setup_osd.ui_enc_w = 468;
			s_setup_osd.ui_enc_h = 240*1.2;
			s_setup_osd.ui_enc_x = ((uiStreamW-s_setup_osd.ui_enc_w)/2);
			s_setup_osd.ui_enc_y = ((uiStreamH-s_setup_osd.ui_enc_h)/2);
			s_setup_osd.ui_enc_font_size = 10;
		}else{
			s_setup_osd.ui_enc_resolution = HOST_OSD_MENU_RESOLUTION_FULLHD;
			s_setup_osd.ui_enc_w = 864;
			s_setup_osd.ui_enc_h = 420*1.2;
			s_setup_osd.ui_enc_x = ((uiStreamW-s_setup_osd.ui_enc_w)/2);
			s_setup_osd.ui_enc_y = ((uiStreamH-s_setup_osd.ui_enc_h)/2);
			s_setup_osd.ui_enc_font_size = 18;
		}
		
		s_setup_osd.ui_enc_base_w = 576;
		s_setup_osd.ui_enc_base_h = 280*1.2;
		
		if(s_setup_osd.ui_enc_x%2==1)s_setup_osd.ui_enc_x = s_setup_osd.ui_enc_x-1;
		if(s_setup_osd.ui_enc_y%2==1)s_setup_osd.ui_enc_y = s_setup_osd.ui_enc_y-1;
		if(s_setup_osd.ui_enc_w%2==1)s_setup_osd.ui_enc_w = s_setup_osd.ui_enc_w-1;
		if(s_setup_osd.ui_enc_h%2==1)s_setup_osd.ui_enc_h = s_setup_osd.ui_enc_h-1;
		if(s_setup_osd.ui_enc_base_w%2==1)s_setup_osd.ui_enc_base_w = s_setup_osd.ui_enc_base_w-1;
		if(s_setup_osd.ui_enc_base_h%2==1)s_setup_osd.ui_enc_base_h = s_setup_osd.ui_enc_base_h-1;
		
		if(OSD_DEBUG) printf(BLUE("[SetupEncOsd] x:%d, y:%d, w:%d, h:%d, fontsize:%d\n"), s_setup_osd.ui_enc_x, s_setup_osd.ui_enc_y, s_setup_osd.ui_enc_w, s_setup_osd.ui_enc_h, s_setup_osd.ui_enc_font_size);
	}
	else  if (eStreamId == HOST_STREAM_ID_HDMI)
	{
		HostPipeline_GetStreamResolution(HOST_STREAM_ID_HDMI, (UINT32 *)&uiStreamW, (UINT32 *)&uiStreamH);

		if(uiStreamW==7680 && uiStreamH==4320){
			s_setup_osd.ui_display_resolution = HOST_OSD_MENU_RESOLUTION_8K;
			s_setup_osd.ui_display_w = 3456;
			s_setup_osd.ui_display_h = 1680*1.2;
			s_setup_osd.ui_display_x = ((uiStreamW-s_setup_osd.ui_display_w)/2);
			s_setup_osd.ui_display_y = ((uiStreamH-s_setup_osd.ui_display_h)/2);
			s_setup_osd.ui_display_font_size = 72;
		}else if(uiStreamW==3840 && uiStreamH==2160){
			s_setup_osd.ui_display_resolution = HOST_OSD_MENU_RESOLUTION_4K;
			s_setup_osd.ui_display_w = 1728;
			s_setup_osd.ui_display_h = 840*1.2;
			s_setup_osd.ui_display_x = ((uiStreamW-s_setup_osd.ui_display_w)/2);
			s_setup_osd.ui_display_y = ((uiStreamH-s_setup_osd.ui_display_h)/2);
			s_setup_osd.ui_display_font_size = 36;
		}else if(uiStreamW==1920 && uiStreamH==1080){
			s_setup_osd.ui_display_resolution = HOST_OSD_MENU_RESOLUTION_FULLHD;
			s_setup_osd.ui_display_w = 864;
			s_setup_osd.ui_display_h = 420*1.2;
			s_setup_osd.ui_display_x = ((uiStreamW-s_setup_osd.ui_display_w)/2);
			s_setup_osd.ui_display_y = ((uiStreamH-s_setup_osd.ui_display_h)/2);
			s_setup_osd.ui_display_font_size = 18;
		}else if(uiStreamW==1280 && uiStreamH==720){
			s_setup_osd.ui_display_resolution = HOST_OSD_MENU_RESOLUTION_HD;
			s_setup_osd.ui_display_w = 576;
			s_setup_osd.ui_display_h = 280*1.2;
			s_setup_osd.ui_display_x = ((uiStreamW-s_setup_osd.ui_display_w)/2);
			s_setup_osd.ui_display_y = ((uiStreamH-s_setup_osd.ui_display_h)/2);
			s_setup_osd.ui_display_font_size = 12;
		}else if(uiStreamW==720 && uiStreamH==480){
			s_setup_osd.ui_display_resolution = HOST_OSD_MENU_RESOLUTION_HD;
			s_setup_osd.ui_display_w = 468;
			s_setup_osd.ui_display_h = 240*1.2;
			s_setup_osd.ui_display_x = ((uiStreamW-s_setup_osd.ui_display_w)/2);
			s_setup_osd.ui_display_y = ((uiStreamH-s_setup_osd.ui_display_h)/2);
			s_setup_osd.ui_display_font_size = 10;
		}else{
			s_setup_osd.ui_display_resolution = HOST_OSD_MENU_RESOLUTION_FULLHD;
			s_setup_osd.ui_display_w = 864;
			s_setup_osd.ui_display_h = 420*1.2;
			s_setup_osd.ui_display_x = ((uiStreamW-s_setup_osd.ui_display_w)/2);
			s_setup_osd.ui_display_y = ((uiStreamH-s_setup_osd.ui_display_h)/2);
			s_setup_osd.ui_display_font_size = 18;
		}

		s_setup_osd.ui_display_base_w = 576;
		s_setup_osd.ui_display_base_h = 280*1.2;

		if(s_setup_osd.ui_display_x%2==1)s_setup_osd.ui_display_x = s_setup_osd.ui_display_x-1;
		if(s_setup_osd.ui_display_y%2==1)s_setup_osd.ui_display_y = s_setup_osd.ui_display_y-1;
		if(s_setup_osd.ui_display_w%2==1)s_setup_osd.ui_display_w = s_setup_osd.ui_display_w-1;
		if(s_setup_osd.ui_display_h%2==1)s_setup_osd.ui_display_h = s_setup_osd.ui_display_h-1;
		if(s_setup_osd.ui_display_base_w%2==1)s_setup_osd.ui_display_base_w = s_setup_osd.ui_display_base_w-1;
		if(s_setup_osd.ui_display_base_h%2==1)s_setup_osd.ui_display_base_h = s_setup_osd.ui_display_base_h-1;

		if(OSD_DEBUG) printf(BLUE("[SetupDisplayOsd] x:%d, y:%d, w:%d, h:%d, fontsize:%d\n"), s_setup_osd.ui_display_x, s_setup_osd.ui_display_y, s_setup_osd.ui_display_w, s_setup_osd.ui_display_h, s_setup_osd.ui_display_font_size);
	}
}

#if 1
void OSDLib_DrawMenuBG(AB_OSD_BUF_ST s_dst_buf, HOST_OSD_MENU_RESOLUTION nResolution)
{
	uint32_t ui_w = 0;
	uint32_t ui_h = 0;
	uint32_t ui_base_x = 0;
	uint32_t ui_base_h = 0;	
	uint32_t ui_gap = 0;
	uint32_t ui_times = 30;
	uint32_t divisor = 10;

	switch(nResolution){
		case HOST_OSD_MENU_RESOLUTION_8K:
			ui_times = 60;
			break;
			
		default:
		case HOST_OSD_MENU_RESOLUTION_4K:
			ui_times = 30;
			break;
			
		case HOST_OSD_MENU_RESOLUTION_FULLHD:
			ui_times = 15;
			break;
			
		case HOST_OSD_MENU_RESOLUTION_HD:
			ui_times = 10;
			break;		
	}

	if(s_setup_osd.ui_osd_type == HOST_OSD_TYPE_ENC){
		ui_w = s_setup_osd.ui_enc_w;
		ui_h = s_setup_osd.ui_enc_h;
		ui_base_x = s_setup_osd.ui_enc_base_w/3+30;
		ui_base_h = s_setup_osd.ui_enc_base_h;
		ui_gap = ui_base_h/12;
	}else{
		ui_w = s_setup_osd.ui_display_w;
		ui_h = s_setup_osd.ui_display_h;
		ui_base_x = s_setup_osd.ui_display_base_w/3+30;
		ui_base_h = s_setup_osd.ui_display_base_h;
		ui_gap = ui_base_h/12;
	}

	// Draw Menu
	ab_osd_draw_rectangle(&s_dst_buf, COLOR_ARGB8888_DARKGRAY, 0, 0, ui_w, ui_gap*ui_times/divisor);
	ab_osd_draw_rectangle(&s_dst_buf, COLOR_ARGB8888_MEDIUMGRAY, 0, ui_gap*ui_times/divisor, ui_w, ui_h-ui_gap*ui_times/divisor);
	ab_osd_draw_rectangle(&s_dst_buf, COLOR_ARGB8888_PICKGRAY, (ui_base_x-1)*ui_times/divisor, ui_gap*ui_times/divisor, 2*ui_times/divisor, (ui_base_h-ui_gap)*ui_times/divisor);
}

void OSDLib_DrawMainMenu(AB_OSD_BUF_ST s_dst_buf, uint16_t wIndex, HOST_OSD_MENU_RESOLUTION nResolution)
{
	uint32_t i=0, j=0;
	AB_OSD_RECT_ST s_dst_rect = {0};
	uint32_t ui_base_w = 0;
	uint32_t ui_base_h = 0;	
	uint32_t ui_base_title_w = 0;
	uint32_t ui_gap = 0;
	uint32_t ui_times = 30;
	uint32_t divisor = 10;
	uint32_t ui_font_size = 0;
	uint32_t ui_str_id[SETUPMENU_TOTAL] = {_STR_IMAGE_, _STR_HDR_, _STR_WHITE_BALANCE_, _STR_HDMI_OUTPUT_, _STR_AUDIO_, 
		_STR_PIR_DETECTOR_, _STR_TIME_OPTION_, _STR_LANGUAGE_, _STR_AI_SETTING_, _STR_STITCHING_CALIBRATION_, _STR_SAVE_OR_EXIT};

	switch(nResolution){
		case HOST_OSD_MENU_RESOLUTION_8K:
			ui_times = 60;
			break;
			
		default:
		case HOST_OSD_MENU_RESOLUTION_4K:
			ui_times = 30;
			break;
			
		case HOST_OSD_MENU_RESOLUTION_FULLHD:
			ui_times = 15;
			break;
			
		case HOST_OSD_MENU_RESOLUTION_HD:
			ui_times = 10;
			break;		
	}

	if(s_setup_osd.ui_osd_type == HOST_OSD_TYPE_ENC){
		ui_font_size = s_setup_osd.ui_enc_font_size;
		ui_base_title_w = s_setup_osd.ui_enc_base_w;
		ui_base_w = s_setup_osd.ui_enc_base_w/3+30;
		ui_base_h = s_setup_osd.ui_enc_base_h;
		ui_gap = ui_base_h/12;
	}else{
		ui_font_size = s_setup_osd.ui_display_font_size;
		ui_base_title_w = s_setup_osd.ui_display_base_w;
		ui_base_w = s_setup_osd.ui_display_base_w/3+30;
		ui_base_h = s_setup_osd.ui_display_base_h;
		ui_gap = ui_base_h/12;
	}

	//title
	host_osd_draw_text_utf8(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_WHITE, COLOR_ARGB8888_DARKGRAY, ui_font_size, _STR_MENU_, (ui_base_title_w/2)*ui_times/divisor, (ui_gap/2+2)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_CENTER);
	
	// item
	if(ui_menu_item==HOST_OSD_MENU_MAIN){
		ab_osd_draw_rectangle(&s_dst_buf, COLOR_ARGB8888_LIGHTGRAY, 0, ui_gap*ui_times/divisor, (ui_base_w-1)*ui_times/divisor, (ui_base_h-ui_gap)*ui_times/divisor);
		for(j=0; j<SETUPMENU_TOTAL; j++){
			if(s_setup_osd.ui_main_turnoff[j] == 0){
				if(s_setup_osd.ui_main_disabled[j] == 1){
					host_osd_draw_text_utf8(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_DISABLE, COLOR_ARGB8888_LIGHTGRAY, ui_font_size, ui_str_id[j], 10*ui_times/divisor, (ui_gap*3/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
				}else{
					if(j==(wIndex%SETUPMENU_TOTAL)){
						ab_osd_draw_rectangle(&s_dst_buf, COLOR_ARGB8888_PICKGRAY, 0, (ui_gap+i*ui_gap)*ui_times/divisor, (ui_base_w-1)*ui_times/divisor, ui_gap*ui_times/divisor);
						host_osd_draw_text_utf8(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_WHITE, COLOR_ARGB8888_PICKGRAY, ui_font_size, ui_str_id[j], 10*ui_times/divisor, (ui_gap*3/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
					}else{
						host_osd_draw_text_utf8(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_NAVY, COLOR_ARGB8888_LIGHTGRAY, ui_font_size, ui_str_id[j], 10*ui_times/divisor, (ui_gap*3/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
					}
				}
				i++;
			}
		}
	}else{
		for(j=0; j<SETUPMENU_TOTAL; j++){
			if(s_setup_osd.ui_main_turnoff[j] == 0){
				if(s_setup_osd.ui_main_disabled[j] == 1){
					host_osd_draw_text_utf8(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_DISABLE, COLOR_ARGB8888_MEDIUMGRAY, ui_font_size, ui_str_id[j], 10*ui_times/divisor, (ui_gap*3/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
				}else{
					if(j==(wIndex%SETUPMENU_TOTAL)){
						ab_osd_draw_rectangle(&s_dst_buf, COLOR_ARGB8888_PICKGRAY, 0, (ui_gap+i*ui_gap)*ui_times/divisor, (ui_base_w-1)*ui_times/divisor, ui_gap*ui_times/divisor);
						host_osd_draw_text_utf8(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_WHITE, COLOR_ARGB8888_PICKGRAY, ui_font_size, ui_str_id[j], 10*ui_times/divisor, (ui_gap*3/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
					}else{
						host_osd_draw_text_utf8(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_NAVY, COLOR_ARGB8888_MEDIUMGRAY, ui_font_size, ui_str_id[j], 10*ui_times/divisor, (ui_gap*3/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
					}
				}
				i++;
			}
		}
	}
}

void OSDLib_DrawImageMenu(AB_OSD_BUF_ST s_dst_buf, uint16_t wIndex, HOST_OSD_MENU_RESOLUTION nResolution)
{
	uint32_t i=0;
	AB_OSD_RECT_ST s_dst_rect = {0};
	uint32_t ui_base_x = 0;
	uint32_t ui_base_w = 0;
	uint32_t ui_base_h = 0;
	uint32_t ui_base_title_w = 0;
	uint32_t ui_gap = 0;
	uint32_t ui_times = 30;
	uint32_t divisor = 10;
	uint32_t ui_font_size = 0;
	uint32_t ui_str_id[HOSTMI_IMAGE_TOTAL] = {_STR_BRIGHTNESS_, _STR_CONTRAST_, _STR_SHARPNESS_,
			_STR_COLOR_GAIN_, _STR_V_FLIP_, _STR_H_FLIP_, _STR_SAVE_RETURN_};

	switch(nResolution){
		case HOST_OSD_MENU_RESOLUTION_8K:
			ui_times = 60;
			break;
			
		default:
		case HOST_OSD_MENU_RESOLUTION_4K:
			ui_times = 30;
			break;
			
		case HOST_OSD_MENU_RESOLUTION_FULLHD:
			ui_times = 15;
			break;
			
		case HOST_OSD_MENU_RESOLUTION_HD:
			ui_times = 10;
			break;		
	}

	if(s_setup_osd.ui_osd_type == HOST_OSD_TYPE_ENC){
		ui_font_size = s_setup_osd.ui_enc_font_size;
		ui_base_title_w = s_setup_osd.ui_enc_base_w;
		ui_base_x = 10;
		ui_base_w = s_setup_osd.ui_enc_base_w/3+30;
		ui_base_h = s_setup_osd.ui_enc_base_h;
		ui_gap = ui_base_h/12;
	}else{
		ui_font_size = s_setup_osd.ui_display_font_size;
		ui_base_title_w = s_setup_osd.ui_display_base_w;
		ui_base_x = 10;
		ui_base_w = s_setup_osd.ui_display_base_w/3+30;
		ui_base_h = s_setup_osd.ui_display_base_h;
		ui_gap = ui_base_h/12;
	}

	//title
	host_osd_draw_text_utf8(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_WHITE, COLOR_ARGB8888_DARKGRAY, ui_font_size, _STR_IMAGE_, (ui_base_title_w/2)*ui_times/divisor, (ui_gap/2+2)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_CENTER);
	
	// item
	if(ui_menu_item==HOST_OSD_MENU_IMAGE){
		ab_osd_draw_rectangle(&s_dst_buf, COLOR_ARGB8888_LIGHTGRAY, 0, ui_gap*ui_times/divisor, (ui_base_w-1)*ui_times/divisor, (ui_base_h-ui_gap)*ui_times/divisor);
		for(i=0; i<HOSTMI_IMAGE_TOTAL; i++){
			if(s_setup_osd.ui_image_disabled[i] == 1){
				host_osd_draw_text_utf8(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_DISABLE, COLOR_ARGB8888_LIGHTGRAY, ui_font_size, ui_str_id[i], ui_base_x*ui_times/divisor, (ui_gap*3/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
			}else{
				if(i==(wIndex%HOSTMI_IMAGE_TOTAL)){
					ab_osd_draw_rectangle(&s_dst_buf, COLOR_ARGB8888_PICKGRAY, 0, (ui_gap+i*ui_gap)*ui_times/divisor, (ui_base_w-1)*ui_times/divisor, ui_gap*ui_times/divisor);
					host_osd_draw_text_utf8(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_WHITE, COLOR_ARGB8888_PICKGRAY, ui_font_size, ui_str_id[i], ui_base_x*ui_times/divisor, (ui_gap*3/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
				}else{
					host_osd_draw_text_utf8(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_NAVY, COLOR_ARGB8888_LIGHTGRAY, ui_font_size, ui_str_id[i], ui_base_x*ui_times/divisor, (ui_gap*3/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
				}
			}
		}
	}else{
		for(i=0; i<HOSTMI_IMAGE_TOTAL; i++){
			if(s_setup_osd.ui_image_disabled[i] == 1){
				host_osd_draw_text_utf8(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_DISABLE, COLOR_ARGB8888_MEDIUMGRAY, ui_font_size, ui_str_id[i], ui_base_x*ui_times/divisor, (ui_gap*3/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
			}else{
				if(i==(wIndex%HOSTMI_IMAGE_TOTAL)){
					ab_osd_draw_rectangle(&s_dst_buf, COLOR_ARGB8888_PICKGRAY, 0, (ui_gap+i*ui_gap)*ui_times/divisor, (ui_base_w-1)*ui_times/divisor, ui_gap*ui_times/divisor);
					host_osd_draw_text_utf8(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_WHITE, COLOR_ARGB8888_PICKGRAY, ui_font_size, ui_str_id[i], ui_base_x*ui_times/divisor, (ui_gap*3/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
				}else{
					host_osd_draw_text_utf8(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_NAVY, COLOR_ARGB8888_MEDIUMGRAY, ui_font_size, ui_str_id[i], ui_base_x*ui_times/divisor, (ui_gap*3/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
				}
			}
		}
	}
}

void OSDLib_DrawHdmiMenu(AB_OSD_BUF_ST s_dst_buf, uint16_t wIndex, HOST_OSD_MENU_RESOLUTION nResolution)
{
	uint32_t i=0;
	AB_OSD_RECT_ST s_dst_rect = {0};
	uint32_t ui_base_x = 0;
	uint32_t ui_base_w = 0;
	uint32_t ui_base_h = 0;
	uint32_t ui_base_title_w = 0;
	uint32_t ui_gap = 0;
	uint32_t ui_times = 30;
	uint32_t divisor = 10;
	uint32_t ui_font_size = 0;
	uint32_t ui_str_id[HOSTMI_HDMI_TOTAL] = {_STR_RESOLUTION_, _STR_SAVE_RETURN_};

	switch(nResolution){
		case HOST_OSD_MENU_RESOLUTION_8K:
			ui_times = 60;
			break;
			
		default:
		case HOST_OSD_MENU_RESOLUTION_4K:
			ui_times = 30;
			break;
			
		case HOST_OSD_MENU_RESOLUTION_FULLHD:
			ui_times = 15;
			break;
			
		case HOST_OSD_MENU_RESOLUTION_HD:
			ui_times = 10;
			break;		
	}

	if(s_setup_osd.ui_osd_type == HOST_OSD_TYPE_ENC){
		ui_font_size = s_setup_osd.ui_enc_font_size;
		ui_base_title_w = s_setup_osd.ui_enc_base_w;
		ui_base_x = 10;
		ui_base_w = s_setup_osd.ui_enc_base_w/3+30;
		ui_base_h = s_setup_osd.ui_enc_base_h;
		ui_gap = ui_base_h/12;
	}else{
		ui_font_size = s_setup_osd.ui_display_font_size;
		ui_base_title_w = s_setup_osd.ui_display_base_w;
		ui_base_x = 10;
		ui_base_w = s_setup_osd.ui_display_base_w/3+30;
		ui_base_h = s_setup_osd.ui_display_base_h;
		ui_gap = ui_base_h/12;
	}

	//title
	host_osd_draw_text_utf8(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_WHITE, COLOR_ARGB8888_DARKGRAY, ui_font_size, _STR_HDMI_OUTPUT_, (ui_base_title_w/2)*ui_times/divisor, (ui_gap/2+2)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_CENTER);
	
	// item
	if(ui_menu_item==HOST_OSD_MENU_HDMI){
		ab_osd_draw_rectangle(&s_dst_buf, COLOR_ARGB8888_LIGHTGRAY, 0, ui_gap*ui_times/divisor, (ui_base_w-1)*ui_times/divisor, (ui_base_h-ui_gap)*ui_times/divisor);
		for(i=0; i<HOSTMI_HDMI_TOTAL; i++){
			if(s_setup_osd.ui_hdmi_disabled[i] == 1){
				host_osd_draw_text_utf8(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_DISABLE, COLOR_ARGB8888_LIGHTGRAY, ui_font_size, ui_str_id[i], ui_base_x*ui_times/divisor, (ui_gap*3/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
			}else{
				if(i==(wIndex%HOSTMI_HDMI_TOTAL)){
					ab_osd_draw_rectangle(&s_dst_buf, COLOR_ARGB8888_PICKGRAY, 0, (ui_gap+i*ui_gap)*ui_times/divisor, (ui_base_w-1)*ui_times/divisor, ui_gap*ui_times/divisor);
					host_osd_draw_text_utf8(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_WHITE, COLOR_ARGB8888_PICKGRAY, ui_font_size, ui_str_id[i], ui_base_x*ui_times/divisor, (ui_gap*3/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
				}else{
					host_osd_draw_text_utf8(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_NAVY, COLOR_ARGB8888_LIGHTGRAY, ui_font_size, ui_str_id[i], ui_base_x*ui_times/divisor, (ui_gap*3/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
				}
			}
		}
	}else{
		for(i=0; i<HOSTMI_HDMI_TOTAL; i++){
			if(s_setup_osd.ui_hdmi_disabled[i] == 1){
				host_osd_draw_text_utf8(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_DISABLE, COLOR_ARGB8888_MEDIUMGRAY, ui_font_size, ui_str_id[i], ui_base_x*ui_times/divisor, (ui_gap*3/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
			}else{
				if(i==(wIndex%HOSTMI_HDMI_TOTAL)){
					ab_osd_draw_rectangle(&s_dst_buf, COLOR_ARGB8888_PICKGRAY, 0, (ui_gap+i*ui_gap)*ui_times/divisor, (ui_base_w-1)*ui_times/divisor, ui_gap*ui_times/divisor);
					host_osd_draw_text_utf8(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_WHITE, COLOR_ARGB8888_PICKGRAY, ui_font_size, ui_str_id[i], ui_base_x*ui_times/divisor, (ui_gap*3/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
				}else{
					host_osd_draw_text_utf8(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_NAVY, COLOR_ARGB8888_MEDIUMGRAY, ui_font_size, ui_str_id[i], ui_base_x*ui_times/divisor, (ui_gap*3/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
				}
			}
		}
	}
}

void OSDLib_DrawHdmiResolutionMenu(AB_OSD_BUF_ST s_dst_buf, uint16_t wIndex, HOST_OSD_MENU_RESOLUTION nResolution)
{
	uint32_t i=0;
	AB_OSD_RECT_ST s_dst_rect = {0};
	uint32_t ui_base_x = 0;
	uint32_t ui_base_w = 0;
	uint32_t ui_base_h = 0;
	uint32_t ui_gap = 0;
	uint32_t ui_times = 30;
	uint32_t divisor = 10;
	uint32_t ui_font_size = 0;
	uint32_t ui_str_id[HOSTMI_HDMI_RESOLUTION_TOTAL] = {_STR_4K_30FPS_, _STR_4K_25FPS_, _STR_FHD_60FPS_,
		_STR_FHD_50FPS_, _STR_FHD_30FPS_, _STR_FHD_25FPS_, _STR_HD_60FPS_, _STR_HD_50FPS_
	#ifdef ENABLE_HDMI_720X48060FPS_ITEM
	, _STR_SD_60FPS_
	#endif
	};

	switch(nResolution){
		case HOST_OSD_MENU_RESOLUTION_8K:
			ui_times = 60;
			break;
			
		default:
		case HOST_OSD_MENU_RESOLUTION_4K:
			ui_times = 30;
			break;
			
		case HOST_OSD_MENU_RESOLUTION_FULLHD:
			ui_times = 15;
			break;
			
		case HOST_OSD_MENU_RESOLUTION_HD:
			ui_times = 10;
			break;		
	}

	if(s_setup_osd.ui_osd_type == HOST_OSD_TYPE_ENC){
		ui_font_size = s_setup_osd.ui_enc_font_size;
		ui_base_x = s_setup_osd.ui_enc_base_w/3+30;
		ui_base_w = s_setup_osd.ui_enc_base_w/3+20;
		ui_base_h = s_setup_osd.ui_enc_base_h;
		ui_gap = s_setup_osd.ui_enc_base_h/12;
	}else{
		ui_font_size = s_setup_osd.ui_display_font_size;
		ui_base_x = s_setup_osd.ui_display_base_w/3+30;
		ui_base_w = s_setup_osd.ui_display_base_w/3+20;
		ui_base_h = s_setup_osd.ui_display_base_h;
		ui_gap = s_setup_osd.ui_display_base_h/12;
	}

	ab_osd_draw_rectangle(&s_dst_buf, COLOR_ARGB8888_PICKGRAY, (ui_base_x+ui_base_w-1)*ui_times/divisor, ui_gap*ui_times/divisor, 2*ui_times/divisor, (ui_base_h-ui_gap)*ui_times/divisor);

	if(ui_menu_item==HOST_OSD_MENU_HDMI_RESOLUTION){
		ab_osd_draw_rectangle(&s_dst_buf, COLOR_ARGB8888_LIGHTGRAY, (ui_base_x+1)*ui_times/divisor, ui_gap*ui_times/divisor, (ui_base_w-2)*ui_times/divisor, (ui_base_h-ui_gap)*ui_times/divisor);
		for(i=0; i<HOSTMI_HDMI_RESOLUTION_TOTAL; i++){
			if(s_setup_osd.ui_hdmi_resolution_disabled[i]==1){
				host_osd_draw_text_utf8(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_DISABLE, COLOR_ARGB8888_LIGHTGRAY, ui_font_size, ui_str_id[i], (ui_base_x+10)*ui_times/divisor, (ui_gap*3/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
			}else{		
				if(i==(wIndex%HOSTMI_HDMI_RESOLUTION_TOTAL)){
					ab_osd_draw_rectangle(&s_dst_buf, COLOR_ARGB8888_PICKGRAY, (ui_base_x+1)*ui_times/divisor, (ui_gap+i*ui_gap)*ui_times/divisor, (ui_base_w-2)*ui_times/divisor, ui_gap*ui_times/divisor);
					host_osd_draw_text_utf8(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_WHITE, COLOR_ARGB8888_PICKGRAY, ui_font_size, ui_str_id[i], (ui_base_x+10)*ui_times/divisor, (ui_gap*3/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
				}else{
					host_osd_draw_text_utf8(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_NAVY, COLOR_ARGB8888_LIGHTGRAY, ui_font_size, ui_str_id[i], (ui_base_x+10)*ui_times/divisor, (ui_gap*3/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
				}
			}
		}
	}else{
		for(i=0; i<HOSTMI_HDMI_RESOLUTION_TOTAL; i++){
			if(s_setup_osd.ui_hdmi_resolution_disabled[i]==1){
				host_osd_draw_text_utf8(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_DISABLE, COLOR_ARGB8888_MEDIUMGRAY, ui_font_size, ui_str_id[i], (ui_base_x+10)*ui_times/divisor, (ui_gap*3/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
			}else{		
				if(i==(wIndex%HOSTMI_HDMI_RESOLUTION_TOTAL)){
					ab_osd_draw_rectangle(&s_dst_buf, COLOR_ARGB8888_PICKGRAY, (ui_base_x+1)*ui_times/divisor, (ui_gap+i*ui_gap)*ui_times/divisor, (ui_base_w-2)*ui_times/divisor, ui_gap*ui_times/divisor);
					host_osd_draw_text_utf8(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_WHITE, COLOR_ARGB8888_PICKGRAY, ui_font_size, ui_str_id[i], (ui_base_x+10)*ui_times/divisor, (ui_gap*3/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
				}else{
					host_osd_draw_text_utf8(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_NAVY, COLOR_ARGB8888_MEDIUMGRAY, ui_font_size, ui_str_id[i], (ui_base_x+10)*ui_times/divisor, (ui_gap*3/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
				}
			}
		}
	}
}

void OSDLib_DrawTimeMunualMenu(AB_OSD_BUF_ST s_dst_buf, uint16_t wIndex, HOST_OSD_MENU_RESOLUTION nResolution)
{
	uint32_t i = 0;
	AB_OSD_RECT_ST s_dst_rect = {0};
	uint32_t ui_base_x = 0;
	uint32_t ui_base_w = 0;
	uint32_t ui_base_h = 0;
	uint32_t ui_gap = 0;
	uint32_t ui_times = 30;
	uint32_t divisor = 10;
	uint32_t ui_font_size = 0;
	uint32_t ui_length = 0;
	char year[8];
	char month[8];
	char day[8];
	char hour[8];
	char min[8];
	char sec[8];

	switch(nResolution){
		case HOST_OSD_MENU_RESOLUTION_8K:
			ui_times = 60;
			break;
			
		default:
		case HOST_OSD_MENU_RESOLUTION_4K:
			ui_times = 30;
			break;
			
		case HOST_OSD_MENU_RESOLUTION_FULLHD:
			ui_times = 15;
			break;
			
		case HOST_OSD_MENU_RESOLUTION_HD:
			ui_times = 10;
			break;		
	}
	
	if(s_setup_osd.ui_osd_type == HOST_OSD_TYPE_ENC){
		ui_font_size = s_setup_osd.ui_enc_font_size;
		ui_base_x = s_setup_osd.ui_enc_base_w/3+30;
		ui_base_w = s_setup_osd.ui_enc_base_w/3+20;
		ui_base_h = s_setup_osd.ui_enc_base_h;
		ui_gap = s_setup_osd.ui_enc_base_h/12;
	}else{
		ui_font_size = s_setup_osd.ui_display_font_size;
		ui_base_x = s_setup_osd.ui_display_base_w/3+30;
		ui_base_w = s_setup_osd.ui_display_base_w/3+20;
		ui_base_h = s_setup_osd.ui_display_base_h;
		ui_gap = s_setup_osd.ui_display_base_h/12;
	}
	
	ab_osd_draw_rectangle(&s_dst_buf, COLOR_ARGB8888_PICKGRAY, (ui_base_x+ui_base_w-1)*ui_times/divisor, ui_gap*ui_times/divisor, 2*ui_times/divisor, (ui_base_h-ui_gap)*ui_times/divisor);	
	ab_osd_draw_rectangle(&s_dst_buf, COLOR_ARGB8888_LIGHTGRAY, (ui_base_x+1)*ui_times/divisor, ui_gap*ui_times/divisor, (ui_base_w-2)*ui_times/divisor, (ui_base_h-ui_gap)*ui_times/divisor);
	
	if(ui_menu_item == HOST_OSD_MENU_TIME_TIMESETUP_MANUAL_TIME_HOUR){
		i = 0;
		snprintf(hour, sizeof(hour), "%02d", s_setup_time.i_hour);
		snprintf(min, sizeof(min), "%02d", s_setup_time.i_min);
		snprintf(sec, sizeof(sec), "%02d", s_setup_time.i_sec);
		ab_osd_draw_rectangle(&s_dst_buf, COLOR_ARGB8888_PICKGRAY, (ui_base_x+1)*ui_times/divisor, (ui_gap+i*ui_gap)*ui_times/divisor, (ui_base_w-1)*ui_times/divisor, ui_gap*ui_times/divisor);
		ab_osd_draw_text(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_WHITE, COLOR_ARGB8888_PICKGRAY, ui_font_size, hour, (ui_base_x+10)*ui_times/divisor, (ui_gap*3/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
		ui_length = ui_length + s_dst_rect.ui_w;
		ab_osd_draw_text(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_MEDIUMGRAY, COLOR_ARGB8888_PICKGRAY, ui_font_size, " : ", (ui_base_x+10)*ui_times/divisor+ui_length, (ui_gap*3/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
		ui_length = ui_length + s_dst_rect.ui_w;
		ab_osd_draw_text(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_MEDIUMGRAY, COLOR_ARGB8888_PICKGRAY, ui_font_size, min, (ui_base_x+10)*ui_times/divisor+ui_length, (ui_gap*3/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
		#if 0
		ui_length = ui_length + s_dst_rect.ui_w;
		ab_osd_draw_text(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_MEDIUMGRAY, COLOR_ARGB8888_PICKGRAY, ui_font_size, " : ", (ui_base_x+10)*ui_times/divisor+ui_length, (ui_gap*3/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
		ui_length = ui_length + s_dst_rect.ui_w;
		ab_osd_draw_text(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_MEDIUMGRAY, COLOR_ARGB8888_PICKGRAY, ui_font_size, sec, (ui_base_x+10)*ui_times/divisor+ui_length, (ui_gap*3/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
		#endif
	}else if(ui_menu_item == HOST_OSD_MENU_TIME_TIMESETUP_MANUAL_TIME_MIN){
		i = 0;
		snprintf(hour, sizeof(hour), "%02d", s_setup_time.i_hour);
		snprintf(min, sizeof(min), "%02d", s_setup_time.i_min);
		snprintf(sec, sizeof(sec), "%02d", s_setup_time.i_sec);
		ab_osd_draw_rectangle(&s_dst_buf, COLOR_ARGB8888_PICKGRAY, (ui_base_x+1)*ui_times/divisor, (ui_gap+i*ui_gap)*ui_times/divisor, (ui_base_w-1)*ui_times/divisor, ui_gap*ui_times/divisor);
		ab_osd_draw_text(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_MEDIUMGRAY, COLOR_ARGB8888_PICKGRAY, ui_font_size, hour, (ui_base_x+10)*ui_times/divisor, (ui_gap*3/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
		ui_length = ui_length + s_dst_rect.ui_w;
		ab_osd_draw_text(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_MEDIUMGRAY, COLOR_ARGB8888_PICKGRAY, ui_font_size, " : ", (ui_base_x+10)*ui_times/divisor+ui_length, (ui_gap*3/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
		ui_length = ui_length + s_dst_rect.ui_w;
		ab_osd_draw_text(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_WHITE, COLOR_ARGB8888_PICKGRAY, ui_font_size, min, (ui_base_x+10)*ui_times/divisor+ui_length, (ui_gap*3/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
		#if 0
		ui_length = ui_length + s_dst_rect.ui_w;
		ab_osd_draw_text(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_MEDIUMGRAY, COLOR_ARGB8888_PICKGRAY, ui_font_size, " : ", (ui_base_x+10)*ui_times/divisor+ui_length, (ui_gap*3/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
		ui_length = ui_length + s_dst_rect.ui_w;
		ab_osd_draw_text(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_MEDIUMGRAY, COLOR_ARGB8888_PICKGRAY, ui_font_size, sec, (ui_base_x+10)*ui_times/divisor+ui_length, (ui_gap*3/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
		#endif
	#if 0
	}else if(ui_menu_item == HOST_OSD_MENU_TIME_TIMESETUP_MANUAL_TIME_SEC){
		i = 0;
		snprintf(hour, sizeof(hour), "%02d", s_setup_time.i_hour);
		snprintf(min, sizeof(min), "%02d", s_setup_time.i_min);
		snprintf(sec, sizeof(sec), "%02d", s_setup_time.i_sec);
		ab_osd_draw_rectangle(&s_dst_buf, COLOR_ARGB8888_PICKGRAY, (ui_base_x+1)*ui_times/divisor, (ui_gap+i*ui_gap)*ui_times/divisor, (ui_base_w-1)*ui_times/divisor, ui_gap*ui_times/divisor);
		ab_osd_draw_text(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_MEDIUMGRAY, COLOR_ARGB8888_PICKGRAY, ui_font_size, hour, (ui_base_x+10)*ui_times/divisor, (ui_gap*3/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
		ui_length = ui_length + s_dst_rect.ui_w;
		ab_osd_draw_text(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_MEDIUMGRAY, COLOR_ARGB8888_PICKGRAY, ui_font_size, " : ", (ui_base_x+10)*ui_times/divisor+ui_length, (ui_gap*3/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
		ui_length = ui_length + s_dst_rect.ui_w;
		ab_osd_draw_text(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_MEDIUMGRAY, COLOR_ARGB8888_PICKGRAY, ui_font_size, min, (ui_base_x+10)*ui_times/divisor+ui_length, (ui_gap*3/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
		ui_length = ui_length + s_dst_rect.ui_w;
		ab_osd_draw_text(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_MEDIUMGRAY, COLOR_ARGB8888_PICKGRAY, ui_font_size, " : ", (ui_base_x+10)*ui_times/divisor+ui_length, (ui_gap*3/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
		ui_length = ui_length + s_dst_rect.ui_w;
		ab_osd_draw_text(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_WHITE, COLOR_ARGB8888_PICKGRAY, ui_font_size, sec, (ui_base_x+10)*ui_times/divisor+ui_length, (ui_gap*3/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
	#endif
	}else if(ui_menu_item == HOST_OSD_MENU_TIME_TIMESETUP_MANUAL_DATE_YEAR){
		i = 1;
		snprintf(year, sizeof(year), "%04d", s_setup_time.i_year);
		snprintf(month, sizeof(month), "%02d", s_setup_time.i_mon);
		snprintf(day, sizeof(day), "%02d", s_setup_time.i_day);
		ab_osd_draw_rectangle(&s_dst_buf, COLOR_ARGB8888_PICKGRAY, (ui_base_x+1)*ui_times/divisor, (ui_gap+i*ui_gap)*ui_times/divisor, (ui_base_w-1)*ui_times/divisor, ui_gap*ui_times/divisor);
		ab_osd_draw_text(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_WHITE, COLOR_ARGB8888_PICKGRAY, ui_font_size, year, (ui_base_x+10)*ui_times/divisor, (ui_gap*3/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
		ui_length = ui_length + s_dst_rect.ui_w;
		ab_osd_draw_text(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_MEDIUMGRAY, COLOR_ARGB8888_PICKGRAY, ui_font_size, " / ", (ui_base_x+10)*ui_times/divisor+ui_length, (ui_gap*3/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
		ui_length = ui_length + s_dst_rect.ui_w;
		ab_osd_draw_text(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_MEDIUMGRAY, COLOR_ARGB8888_PICKGRAY, ui_font_size, month, (ui_base_x+10)*ui_times/divisor+ui_length, (ui_gap*3/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
		ui_length = ui_length + s_dst_rect.ui_w;
		ab_osd_draw_text(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_MEDIUMGRAY, COLOR_ARGB8888_PICKGRAY, ui_font_size, " / ", (ui_base_x+10)*ui_times/divisor+ui_length, (ui_gap*3/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
		ui_length = ui_length + s_dst_rect.ui_w;
		ab_osd_draw_text(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_MEDIUMGRAY, COLOR_ARGB8888_PICKGRAY, ui_font_size, day, (ui_base_x+10)*ui_times/divisor+ui_length, (ui_gap*3/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
	}else if(ui_menu_item == HOST_OSD_MENU_TIME_TIMESETUP_MANUAL_DATE_MONTH){
		i = 1;
		snprintf(year, sizeof(year), "%04d", s_setup_time.i_year);
		snprintf(month, sizeof(month), "%02d", s_setup_time.i_mon);
		snprintf(day, sizeof(day), "%02d", s_setup_time.i_day);
		ab_osd_draw_rectangle(&s_dst_buf, COLOR_ARGB8888_PICKGRAY, (ui_base_x+1)*ui_times/divisor, (ui_gap+i*ui_gap)*ui_times/divisor, (ui_base_w-1)*ui_times/divisor, ui_gap*ui_times/divisor);
		ab_osd_draw_text(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_MEDIUMGRAY, COLOR_ARGB8888_PICKGRAY, ui_font_size, year, (ui_base_x+10)*ui_times/divisor, (ui_gap*3/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
		ui_length = ui_length + s_dst_rect.ui_w;
		ab_osd_draw_text(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_MEDIUMGRAY, COLOR_ARGB8888_PICKGRAY, ui_font_size, " / ", (ui_base_x+10)*ui_times/divisor+ui_length, (ui_gap*3/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
		ui_length = ui_length + s_dst_rect.ui_w;
		ab_osd_draw_text(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_WHITE, COLOR_ARGB8888_PICKGRAY, ui_font_size, month, (ui_base_x+10)*ui_times/divisor+ui_length, (ui_gap*3/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
		ui_length = ui_length + s_dst_rect.ui_w;
		ab_osd_draw_text(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_MEDIUMGRAY, COLOR_ARGB8888_PICKGRAY, ui_font_size, " / ", (ui_base_x+10)*ui_times/divisor+ui_length, (ui_gap*3/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
		ui_length = ui_length + s_dst_rect.ui_w;
		ab_osd_draw_text(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_MEDIUMGRAY, COLOR_ARGB8888_PICKGRAY, ui_font_size, day, (ui_base_x+10)*ui_times/divisor+ui_length, (ui_gap*3/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
	}else if(ui_menu_item == HOST_OSD_MENU_TIME_TIMESETUP_MANUAL_DATE_DAY){
		i = 1;
		snprintf(year, sizeof(year), "%04d", s_setup_time.i_year);
		snprintf(month, sizeof(month), "%02d", s_setup_time.i_mon);
		snprintf(day, sizeof(day), "%02d", s_setup_time.i_day);
		ab_osd_draw_rectangle(&s_dst_buf, COLOR_ARGB8888_PICKGRAY, (ui_base_x+1)*ui_times/divisor, (ui_gap+i*ui_gap)*ui_times/divisor, (ui_base_w-1)*ui_times/divisor, ui_gap*ui_times/divisor);
		ab_osd_draw_text(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_MEDIUMGRAY, COLOR_ARGB8888_PICKGRAY, ui_font_size, year, (ui_base_x+10)*ui_times/divisor, (ui_gap*3/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
		ui_length = ui_length + s_dst_rect.ui_w;
		ab_osd_draw_text(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_MEDIUMGRAY, COLOR_ARGB8888_PICKGRAY, ui_font_size, " / ", (ui_base_x+10)*ui_times/divisor+ui_length, (ui_gap*3/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
		ui_length = ui_length + s_dst_rect.ui_w;
		ab_osd_draw_text(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_MEDIUMGRAY, COLOR_ARGB8888_PICKGRAY, ui_font_size, month, (ui_base_x+10)*ui_times/divisor+ui_length, (ui_gap*3/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
		ui_length = ui_length + s_dst_rect.ui_w;
		ab_osd_draw_text(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_MEDIUMGRAY, COLOR_ARGB8888_PICKGRAY, ui_font_size, " / ", (ui_base_x+10)*ui_times/divisor+ui_length, (ui_gap*3/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
		ui_length = ui_length + s_dst_rect.ui_w;
		ab_osd_draw_text(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_WHITE, COLOR_ARGB8888_PICKGRAY, ui_font_size, day, (ui_base_x+10)*ui_times/divisor+ui_length, (ui_gap*3/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
	}
}

void OSDLib_DrawLanguageMenu(AB_OSD_BUF_ST s_dst_buf, uint16_t wIndex, HOST_OSD_MENU_RESOLUTION nResolution)
{
	uint32_t i=0;
	AB_OSD_RECT_ST s_dst_rect = {0};
	uint32_t ui_base_x = 0;
	uint32_t ui_base_w = 0;
	uint32_t ui_base_h = 0;
	uint32_t ui_gap = 0;
	uint32_t ui_times = 30;
	uint32_t divisor = 10;
	uint32_t ui_font_size = 0;
	uint32_t ui_str_id[HOSTMI_LANGUAGE_TOTAL] = {_STR_LANG_ENG_, _STR_LANG_GER_, _STR_LANG_SPA_, _STR_LANG_TCH_, _STR_LANG_FRA_};

	switch(nResolution){
		case HOST_OSD_MENU_RESOLUTION_8K:
			ui_times = 60;
			break;
			
		default:
		case HOST_OSD_MENU_RESOLUTION_4K:
			ui_times = 30;
			break;
			
		case HOST_OSD_MENU_RESOLUTION_FULLHD:
			ui_times = 15;
			break;
			
		case HOST_OSD_MENU_RESOLUTION_HD:
			ui_times = 10;
			break;		
	}
	
	if(s_setup_osd.ui_osd_type == HOST_OSD_TYPE_ENC){
		ui_font_size = s_setup_osd.ui_enc_font_size;
		ui_base_x = s_setup_osd.ui_enc_base_w/3+30;
		ui_base_w = s_setup_osd.ui_enc_base_w/3+20;
		ui_base_h = s_setup_osd.ui_enc_base_h;
		ui_gap = s_setup_osd.ui_enc_base_h/12;
	}else{
		ui_font_size = s_setup_osd.ui_display_font_size;
		ui_base_x = s_setup_osd.ui_display_base_w/3+30;
		ui_base_w = s_setup_osd.ui_display_base_w/3+20;
		ui_base_h = s_setup_osd.ui_display_base_h;
		ui_gap = s_setup_osd.ui_display_base_h/12;
	}

	ab_osd_draw_rectangle(&s_dst_buf, COLOR_ARGB8888_PICKGRAY, (ui_base_x+ui_base_w-1)*ui_times/divisor, ui_gap*ui_times/divisor, 2*ui_times/divisor, (ui_base_h-ui_gap)*ui_times/divisor);

	if(ui_menu_item==HOST_OSD_MENU_LANGUAGE){
		ab_osd_draw_rectangle(&s_dst_buf, COLOR_ARGB8888_LIGHTGRAY, (ui_base_x+1)*ui_times/divisor, ui_gap*ui_times/divisor, (ui_base_w-2)*ui_times/divisor, (ui_base_h-ui_gap)*ui_times/divisor);
		for(i=0; i<HOSTMI_LANGUAGE_TOTAL; i++){		
			host_setup_set_language(i);
			if(i==(wIndex%HOSTMI_LANGUAGE_TOTAL)){
				ab_osd_draw_rectangle(&s_dst_buf, COLOR_ARGB8888_PICKGRAY, (ui_base_x+1)*ui_times/divisor, (ui_gap+i*ui_gap)*ui_times/divisor, (ui_base_w-2)*ui_times/divisor, ui_gap*ui_times/divisor);
				host_osd_draw_text_utf8(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_WHITE, COLOR_ARGB8888_PICKGRAY, ui_font_size, ui_str_id[i], (ui_base_x+10)*ui_times/divisor, (ui_gap*3/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
			}else{
				host_osd_draw_text_utf8(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_NAVY, COLOR_ARGB8888_LIGHTGRAY, ui_font_size, ui_str_id[i], (ui_base_x+10)*ui_times/divisor, (ui_gap*3/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
			}
		}
	}else{
		for(i=0; i<HOSTMI_LANGUAGE_TOTAL; i++){		
			host_setup_set_language(i);
			if(i==(wIndex%HOSTMI_LANGUAGE_TOTAL)){
				ab_osd_draw_rectangle(&s_dst_buf, COLOR_ARGB8888_PICKGRAY, (ui_base_x+1)*ui_times/divisor, (ui_gap+i*ui_gap)*ui_times/divisor, (ui_base_w-2)*ui_times/divisor, ui_gap*ui_times/divisor);
				host_osd_draw_text_utf8(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_WHITE, COLOR_ARGB8888_PICKGRAY, ui_font_size, ui_str_id[i], (ui_base_x+10)*ui_times/divisor, (ui_gap*3/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
			}else{
				host_osd_draw_text_utf8(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_NAVY, COLOR_ARGB8888_MEDIUMGRAY, ui_font_size, ui_str_id[i], (ui_base_x+10)*ui_times/divisor, (ui_gap*3/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
			}
		}
	}
}

void OSDLib_Draw1stMenu(AB_OSD_BUF_ST s_dst_buf, uint32_t ui_str_title, uint32_t ui_str_id[], uint16_t wIndex, uint16_t wTotalIndex, HOST_OSD_MENU_RESOLUTION nResolution)
{
	uint32_t i=0;
	AB_OSD_RECT_ST s_dst_rect = {0};
	uint32_t ui_base_w = 0;
	uint32_t ui_base_h = 0;	
	uint32_t ui_base_title_w = 0;
	uint32_t ui_gap = 0;
	uint32_t ui_times = 30;
	uint32_t divisor = 10;
	uint32_t ui_font_size = 0;

	switch(nResolution){
		case HOST_OSD_MENU_RESOLUTION_8K:
			ui_times = 60;
			break;
			
		default:
		case HOST_OSD_MENU_RESOLUTION_4K:
			ui_times = 30;
			break;
			
		case HOST_OSD_MENU_RESOLUTION_FULLHD:
			ui_times = 15;
			break;
			
		case HOST_OSD_MENU_RESOLUTION_HD:
			ui_times = 10;
			break;		
	}

	if(s_setup_osd.ui_osd_type == HOST_OSD_TYPE_ENC){
		ui_font_size = s_setup_osd.ui_enc_font_size;
		ui_base_title_w = s_setup_osd.ui_enc_base_w;
		ui_base_w = s_setup_osd.ui_enc_base_w/3+30;
		ui_base_h = s_setup_osd.ui_enc_base_h;
		ui_gap = ui_base_h/12;
	}else{
		ui_font_size = s_setup_osd.ui_display_font_size;
		ui_base_title_w = s_setup_osd.ui_display_base_w;
		ui_base_w = s_setup_osd.ui_display_base_w/3+30;
		ui_base_h = s_setup_osd.ui_display_base_h;
		ui_gap = ui_base_h/12;
	}

	//title
	host_osd_draw_text_utf8(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_WHITE, COLOR_ARGB8888_DARKGRAY, ui_font_size, ui_str_title, (ui_base_title_w/2)*ui_times/divisor, (ui_gap/2+2)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_CENTER);
	
	// item
	if(ui_menu_item==HOST_OSD_MENU_TIME_TIMESETUP_MANUAL){
		ab_osd_draw_rectangle(&s_dst_buf, COLOR_ARGB8888_LIGHTGRAY, 0, ui_gap*ui_times/divisor, (ui_base_w-1)*ui_times/divisor, (ui_base_h-ui_gap)*ui_times/divisor);
		for(i=0; i<wTotalIndex; i++){
			if(i==(wIndex%wTotalIndex)){
				ab_osd_draw_rectangle(&s_dst_buf, COLOR_ARGB8888_PICKGRAY, 0, (ui_gap+i*ui_gap)*ui_times/divisor, (ui_base_w-1)*ui_times/divisor, ui_gap*ui_times/divisor);
				host_osd_draw_text_utf8(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_WHITE, COLOR_ARGB8888_PICKGRAY, ui_font_size, ui_str_id[i], 10*ui_times/divisor, (ui_gap*3/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
			}else{
				host_osd_draw_text_utf8(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_NAVY, COLOR_ARGB8888_LIGHTGRAY, ui_font_size, ui_str_id[i], 10*ui_times/divisor, (ui_gap*3/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
			}
		}
	}else{
		for(i=0; i<wTotalIndex; i++){
			if(i==(wIndex%wTotalIndex)){
				ab_osd_draw_rectangle(&s_dst_buf, COLOR_ARGB8888_PICKGRAY, 0, (ui_gap+i*ui_gap)*ui_times/divisor, (ui_base_w-1)*ui_times/divisor, ui_gap*ui_times/divisor);
				host_osd_draw_text_utf8(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_WHITE, COLOR_ARGB8888_PICKGRAY, ui_font_size, ui_str_id[i], 10*ui_times/divisor, (ui_gap*3/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
			}else{
				host_osd_draw_text_utf8(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_NAVY, COLOR_ARGB8888_MEDIUMGRAY, ui_font_size, ui_str_id[i], 10*ui_times/divisor, (ui_gap*3/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
			}
		}
	}
}

void OSDLib_Draw2ndMenu(AB_OSD_BUF_ST s_dst_buf, uint32_t ui_str_id[], uint16_t wIndex, uint16_t wTotalIndex, HOST_OSD_MENU_RESOLUTION nResolution)
{
	uint32_t i=0;
	AB_OSD_RECT_ST s_dst_rect = {0};
	uint32_t ui_base_x = 0;
	uint32_t ui_base_w = 0;
	uint32_t ui_base_h = 0;
	uint32_t ui_gap = 0;
	uint32_t ui_times = 30;
	uint32_t divisor = 10;
	uint32_t ui_font_size = 0;

	switch(nResolution){
		case HOST_OSD_MENU_RESOLUTION_8K:
			ui_times = 60;
			break;
			
		default:
		case HOST_OSD_MENU_RESOLUTION_4K:
			ui_times = 30;
			break;
			
		case HOST_OSD_MENU_RESOLUTION_FULLHD:
			ui_times = 15;
			break;
			
		case HOST_OSD_MENU_RESOLUTION_HD:
			ui_times = 10;
			break;		
	}
	
	if(s_setup_osd.ui_osd_type == HOST_OSD_TYPE_ENC){
		ui_font_size = s_setup_osd.ui_enc_font_size;
		ui_base_x = s_setup_osd.ui_enc_base_w/3+30;
		ui_base_h = s_setup_osd.ui_enc_base_h;
		if((ui_menu_item == HOST_OSD_MENU_WB) || (ui_menu_item == HOST_OSD_MENU_STITCHALG)){
			ui_base_w = s_setup_osd.ui_enc_base_w-ui_base_x;
		}else{
			ui_base_w = s_setup_osd.ui_enc_base_w/3+20;
		}
		ui_gap = s_setup_osd.ui_enc_base_h/12;
	}else{
		ui_font_size = s_setup_osd.ui_display_font_size;
		ui_base_x = s_setup_osd.ui_display_base_w/3+30;
		ui_base_h = s_setup_osd.ui_display_base_h;
		if((ui_menu_item == HOST_OSD_MENU_WB) || (ui_menu_item == HOST_OSD_MENU_STITCHALG)){
			ui_base_w = s_setup_osd.ui_display_base_w-ui_base_x;
		}else{
			ui_base_w = s_setup_osd.ui_display_base_w/3+20;
		}
		ui_gap = s_setup_osd.ui_display_base_h/12;
	}

	if(!(ui_menu_item == HOST_OSD_MENU_WB) || (ui_menu_item == HOST_OSD_MENU_STITCHALG)){
		ab_osd_draw_rectangle(&s_dst_buf, COLOR_ARGB8888_PICKGRAY, (ui_base_x+ui_base_w-1)*ui_times/divisor, ui_gap*ui_times/divisor, 2*ui_times/divisor, (ui_base_h-ui_gap)*ui_times/divisor);
	}
	
	if(ui_menu_item==HOST_OSD_MENU_TIME_DISPLAY || ui_menu_item==HOST_OSD_MENU_TIME_TIMESETUP){
		for(i=0; i<wTotalIndex; i++){
			if(i==(wIndex%wTotalIndex)){
				ab_osd_draw_rectangle(&s_dst_buf, COLOR_ARGB8888_PICKGRAY, (ui_base_x+1)*ui_times/divisor, (ui_gap+i*ui_gap)*ui_times/divisor, (ui_base_w-2)*ui_times/divisor, ui_gap*ui_times/divisor);
				host_osd_draw_text_utf8(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_WHITE, COLOR_ARGB8888_PICKGRAY, ui_font_size, ui_str_id[i], (ui_base_x+10)*ui_times/divisor, (ui_gap*3/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
			}else{
				host_osd_draw_text_utf8(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_NAVY, COLOR_ARGB8888_MEDIUMGRAY, ui_font_size, ui_str_id[i], (ui_base_x+10)*ui_times/divisor, (ui_gap*3/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
			}
		}   
	}else{
		if((ui_menu_item == HOST_OSD_MENU_WB) || (ui_menu_item == HOST_OSD_MENU_STITCHALG)){
			ab_osd_draw_rectangle(&s_dst_buf, COLOR_ARGB8888_LIGHTGRAY, (ui_base_x+1)*ui_times/divisor, ui_gap*ui_times/divisor, (ui_base_w-1)*ui_times/divisor, (ui_base_h-ui_gap)*ui_times/divisor);
		}else{
			ab_osd_draw_rectangle(&s_dst_buf, COLOR_ARGB8888_LIGHTGRAY, (ui_base_x+1)*ui_times/divisor, ui_gap*ui_times/divisor, (ui_base_w-2)*ui_times/divisor, (ui_base_h-ui_gap)*ui_times/divisor);
		}
		
		for(i=0; i<wTotalIndex; i++){
			if(i==(wIndex%wTotalIndex)){
				if((ui_menu_item == HOST_OSD_MENU_WB) || (ui_menu_item == HOST_OSD_MENU_STITCHALG)){
					ab_osd_draw_rectangle(&s_dst_buf, COLOR_ARGB8888_PICKGRAY, (ui_base_x+1)*ui_times/divisor, (ui_gap+i*ui_gap)*ui_times/divisor, (ui_base_w-1)*ui_times/divisor, ui_gap*ui_times/divisor);
				}else{
					ab_osd_draw_rectangle(&s_dst_buf, COLOR_ARGB8888_PICKGRAY, (ui_base_x+1)*ui_times/divisor, (ui_gap+i*ui_gap)*ui_times/divisor, (ui_base_w-2)*ui_times/divisor, ui_gap*ui_times/divisor);
				}
				host_osd_draw_text_utf8(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_WHITE, COLOR_ARGB8888_PICKGRAY, ui_font_size, ui_str_id[i], (ui_base_x+10)*ui_times/divisor, (ui_gap*3/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
			}else{
				host_osd_draw_text_utf8(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_NAVY, COLOR_ARGB8888_LIGHTGRAY, ui_font_size, ui_str_id[i], (ui_base_x+10)*ui_times/divisor, (ui_gap*3/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
			}
		}  
	}
}

void OSDLib_Draw3rdMenu(AB_OSD_BUF_ST s_dst_buf, uint32_t ui_str_id[], uint16_t wIndex, uint16_t wTotalIndex, HOST_OSD_MENU_RESOLUTION nResolution)
{
	uint32_t i=0;
	AB_OSD_RECT_ST s_dst_rect = {0};
	uint32_t ui_base_x = 0;
	uint32_t ui_base_w = 0;
	uint32_t ui_base_h = 0;
	uint32_t ui_gap = 0;
	uint32_t ui_times = 30;
	uint32_t divisor = 10;
	uint32_t ui_font_size = 0;

	switch(nResolution){
		case HOST_OSD_MENU_RESOLUTION_8K:
			ui_times = 60;
			break;
			
		default:
		case HOST_OSD_MENU_RESOLUTION_4K:
			ui_times = 30;
			break;
			
		case HOST_OSD_MENU_RESOLUTION_FULLHD:
			ui_times = 15;
			break;
			
		case HOST_OSD_MENU_RESOLUTION_HD:
			ui_times = 10;
			break;		
	}
	
	if(s_setup_osd.ui_osd_type == HOST_OSD_TYPE_ENC){
		ui_font_size = s_setup_osd.ui_enc_font_size;
		ui_base_x = s_setup_osd.ui_enc_base_w*2/3+50;
		ui_base_w = s_setup_osd.ui_enc_base_w/3-50;
		ui_base_h = s_setup_osd.ui_enc_base_h;
		ui_gap = s_setup_osd.ui_enc_base_h/12;
	}else{
		ui_font_size = s_setup_osd.ui_display_font_size;
		ui_base_x = s_setup_osd.ui_display_base_w*2/3+50;
		ui_base_w = s_setup_osd.ui_display_base_w/3-50;
		ui_base_h = s_setup_osd.ui_display_base_h;
		ui_gap = s_setup_osd.ui_display_base_h/12;
	}

	ab_osd_draw_rectangle(&s_dst_buf, COLOR_ARGB8888_LIGHTGRAY, (ui_base_x+1)*ui_times/divisor, ui_gap*ui_times/divisor, (ui_base_w-1)*ui_times/divisor, (ui_base_h-ui_gap)*ui_times/divisor);
	if(nResolution == HOST_OSD_MENU_RESOLUTION_FULLHD) ab_osd_draw_rectangle(&s_dst_buf, COLOR_ARGB8888_LIGHTGRAY, 
	                                                   (ui_base_x+1)*ui_times/divisor+(ui_base_w-1)*ui_times/divisor, ui_gap*ui_times/divisor, 
	                                                   s_setup_osd.ui_enc_w-((ui_base_x+1)*ui_times/divisor+(ui_base_w-1)*ui_times/divisor), (ui_base_h-ui_gap)*ui_times/divisor);
	for(i=0; i<wTotalIndex; i++){
		if(i==(wIndex%wTotalIndex)){
			ab_osd_draw_rectangle(&s_dst_buf, COLOR_ARGB8888_PICKGRAY, (ui_base_x+1)*ui_times/divisor, (ui_gap+i*ui_gap)*ui_times/divisor, (ui_base_w-1)*ui_times/divisor, ui_gap*ui_times/divisor);
			if(nResolution == HOST_OSD_MENU_RESOLUTION_FULLHD) ab_osd_draw_rectangle(&s_dst_buf, COLOR_ARGB8888_PICKGRAY, 
			                                                   (ui_base_x+1)*ui_times/divisor+(ui_base_w-1)*ui_times/divisor, (ui_gap+i*ui_gap)*ui_times/divisor, 
			                                                   s_setup_osd.ui_enc_w-((ui_base_x+1)*ui_times/divisor+(ui_base_w-1)*ui_times/divisor), ui_gap*ui_times/divisor);
			host_osd_draw_text_utf8(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_WHITE, COLOR_ARGB8888_PICKGRAY, ui_font_size, ui_str_id[i], (ui_base_x+10)*ui_times/divisor, (ui_gap*3/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
		}else{
			host_osd_draw_text_utf8(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_NAVY, COLOR_ARGB8888_LIGHTGRAY, ui_font_size, ui_str_id[i], (ui_base_x+10)*ui_times/divisor, (ui_gap*3/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
		}
	}
}

void OSDLib_DrawMenuAdjustValue(AB_OSD_BUF_ST s_dst_buf, uint32_t nCurIndex, uint32_t nStartIndex, uint32_t nTotalIndex, HOST_OSD_MENU_RESOLUTION nResolution)
{
	char nValue1[10];
	char nValue2[10];
	char nValue3[10];
	AB_OSD_RECT_ST s_dst_rect = {0};
	uint32_t ui_times = 30;
	uint32_t divisor = 10;
	uint32_t ui_font_size = 0;
	uint32_t ui_base_w = 0;
	uint32_t ui_base_h = 0;
	uint32_t ui_base_bg_x = 0;
	uint32_t ui_base_bg_y = 0;
	uint32_t ui_base_bg_w = 0;
	uint32_t ui_base_bg_h = 0;
	uint32_t ui_bar_x = 0;
	uint32_t ui_bar_y = 0;
	uint32_t ui_bar_w = 0;
	uint32_t ui_bar_h = 0;
	uint32_t ui_gap = 0;
	uint32_t ui_str_id[2] = {_STR_AI_SETTING_NOTE_1_, _STR_AI_SETTING_NOTE_2_};
	
	snprintf(nValue1, sizeof(nValue1), "%d", nStartIndex+nCurIndex);
	snprintf(nValue2, sizeof(nValue2), "%d", nStartIndex+nTotalIndex);
	snprintf(nValue3, sizeof(nValue3), "%d", nStartIndex);

	switch(nResolution){
		case HOST_OSD_MENU_RESOLUTION_8K:
			ui_times = 60;
			break;
			
		default:
		case HOST_OSD_MENU_RESOLUTION_4K:
			ui_times = 30;
			break;
			
		case HOST_OSD_MENU_RESOLUTION_FULLHD:
			ui_times = 15;
			break;
			
		case HOST_OSD_MENU_RESOLUTION_HD:
			ui_times = 10;
			break;		
	}
	
	if(s_setup_osd.ui_osd_type == HOST_OSD_TYPE_ENC){
		ui_font_size = s_setup_osd.ui_enc_font_size;
		if(gs_ui.uw_mode == UI_MODE_SETUP_MENU_MAIN){
			ui_bar_x = s_setup_osd.ui_enc_base_w*2/3+4;
			
			ui_base_bg_x = s_setup_osd.ui_enc_base_w/3+30;
			ui_base_bg_y = s_setup_osd.ui_enc_base_h/12;
			ui_base_bg_w = s_setup_osd.ui_enc_base_w*2/3-30;
			ui_base_bg_h = s_setup_osd.ui_enc_base_h-ui_base_bg_y;
		}else{
			ui_bar_x = s_setup_osd.ui_enc_base_w*3/4-10;
			
			ui_base_bg_x = s_setup_osd.ui_enc_base_w/2;
			ui_base_bg_y = s_setup_osd.ui_enc_base_h/12;
			ui_base_bg_w = s_setup_osd.ui_enc_base_w/2;
			ui_base_bg_h = s_setup_osd.ui_enc_base_h-ui_base_bg_y;
		}
		ui_bar_w = 20;
		ui_base_w = s_setup_osd.ui_enc_base_w;
		ui_base_h = s_setup_osd.ui_enc_base_h;
		ui_gap = s_setup_osd.ui_enc_base_h/12;
		if(ui_menu_item == HOST_OSD_MENU_AI){
			ui_bar_h = 120;
			ui_bar_y = ui_gap*5-ui_bar_h/2;
		}else{
			ui_bar_h = 160;
			ui_bar_y = ui_gap*6+ui_gap/2-ui_bar_h/2;
		}
	}else{
		ui_font_size = s_setup_osd.ui_display_font_size;
		if(gs_ui.uw_mode == UI_MODE_SETUP_MENU_MAIN){
			ui_bar_x = s_setup_osd.ui_display_base_w*2/3+4;
			
			ui_base_bg_x = s_setup_osd.ui_display_base_w/3+30;
			ui_base_bg_y = s_setup_osd.ui_display_base_h/12;
			ui_base_bg_w = s_setup_osd.ui_display_base_w*2/3-30;
			ui_base_bg_h = s_setup_osd.ui_display_base_h-ui_base_bg_y;
		}else{
			ui_bar_x = s_setup_osd.ui_display_base_w*3/4-10;
			
			ui_base_bg_x = s_setup_osd.ui_display_base_w/2;
			ui_base_bg_y = s_setup_osd.ui_display_base_h/12;
			ui_base_bg_w = s_setup_osd.ui_display_base_w/2;
			ui_base_bg_h = s_setup_osd.ui_display_base_h-ui_base_bg_y;
		}
		ui_bar_w = 20;
		ui_base_w = s_setup_osd.ui_display_base_w;
		ui_base_h = s_setup_osd.ui_display_base_h;
		ui_gap = s_setup_osd.ui_display_base_h/12;
		if(ui_menu_item == HOST_OSD_MENU_AI){
			ui_bar_h = 120;
			ui_bar_y = ui_gap*5-ui_bar_h/2;
		}else{
			ui_bar_h = 160;
			ui_bar_y = ui_gap*6+ui_gap/2-ui_bar_h/2;
		}
	}

	
	// Draw Adjust Menu
	ab_osd_draw_rectangle(&s_dst_buf, COLOR_ARGB8888_LIGHTGRAY, (ui_base_bg_x+1)*ui_times/divisor, ui_base_bg_y*ui_times/divisor, (ui_base_bg_w-1)*ui_times/divisor, ui_base_bg_h*ui_times/divisor);
	if(nResolution == HOST_OSD_MENU_RESOLUTION_FULLHD) ab_osd_draw_rectangle(&s_dst_buf, COLOR_ARGB8888_LIGHTGRAY, 
	                                                   (ui_base_bg_x+1)*ui_times/divisor+(ui_base_bg_w-1)*ui_times/divisor, ui_base_bg_y*ui_times/divisor, 
	                                                   s_setup_osd.ui_enc_w-((ui_base_bg_x+1)*ui_times/divisor+(ui_base_bg_w-1)*ui_times/divisor), ui_base_bg_h*ui_times/divisor);

	ab_osd_draw_rectangle(&s_dst_buf, COLOR_ARGB8888_MEDIUMGRAY, ui_bar_x*ui_times/divisor, ui_bar_y*ui_times/divisor, ui_bar_w*ui_times/divisor, ui_bar_h*ui_times/divisor);

	ab_osd_draw_line(&s_dst_buf, COLOR_ARGB8888_MEDIUMGRAY, 2*ui_times/divisor, (ui_bar_x+ui_bar_w+14)*ui_times/divisor, (ui_bar_y+ui_bar_h/2-20)  *ui_times/divisor, (ui_bar_x+ui_bar_w+20)*ui_times/divisor, (ui_bar_y+ui_bar_h/2-20-6)*ui_times/divisor);
	ab_osd_draw_line(&s_dst_buf, COLOR_ARGB8888_MEDIUMGRAY, 2*ui_times/divisor, (ui_bar_x+ui_bar_w+20)*ui_times/divisor, (ui_bar_y+ui_bar_h/2-20-6)*ui_times/divisor, (ui_bar_x+ui_bar_w+26)*ui_times/divisor, (ui_bar_y+ui_bar_h/2-20)  *ui_times/divisor);
	ab_osd_draw_line(&s_dst_buf, COLOR_ARGB8888_MEDIUMGRAY, 2*ui_times/divisor, (ui_bar_x+ui_bar_w+14)*ui_times/divisor, (ui_bar_y+ui_bar_h/2+20)  *ui_times/divisor, (ui_bar_x+ui_bar_w+20)*ui_times/divisor, (ui_bar_y+ui_bar_h/2+20+6)*ui_times/divisor);
	ab_osd_draw_line(&s_dst_buf, COLOR_ARGB8888_MEDIUMGRAY, 2*ui_times/divisor, (ui_bar_x+ui_bar_w+20)*ui_times/divisor, (ui_bar_y+ui_bar_h/2+20+6)*ui_times/divisor, (ui_bar_x+ui_bar_w+26)*ui_times/divisor, (ui_bar_y+ui_bar_h/2+20)  *ui_times/divisor);
	
	ab_osd_draw_text(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_PICKGRAY, COLOR_ARGB8888_LIGHTGRAY, ui_font_size, nValue2,	(ui_bar_x+ui_bar_w/2)*ui_times/divisor, (ui_bar_y-20)         *ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_CENTER);
	ab_osd_draw_text(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_PICKGRAY, COLOR_ARGB8888_LIGHTGRAY, ui_font_size, nValue3, (ui_bar_x+ui_bar_w/2)*ui_times/divisor, (ui_bar_y+ui_bar_h+20+2)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_CENTER);

	//Draw Adjust Value
	ab_osd_draw_text(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_NAVY, COLOR_ARGB8888_LIGHTGRAY, ui_font_size, nValue1,	(ui_bar_x+ui_bar_w+20)*ui_times/divisor, (ui_bar_y+ui_bar_h/2+2)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_CENTER);
	if(nCurIndex>0){
		ab_osd_draw_rectangle(&s_dst_buf, COLOR_ARGB8888_PICKGRAY, ui_bar_x*ui_times/divisor, (ui_bar_y+(ui_bar_h-ui_bar_h*nCurIndex/nTotalIndex))*ui_times/divisor, ui_bar_w*ui_times/divisor, ui_bar_h*nCurIndex/nTotalIndex*ui_times/divisor);
		//if(nResolution == HOST_OSD_MENU_RESOLUTION_FULLHD)ab_osd_draw_rectangle(&s_dst_buf, 0xFF00FFFF, (ui_base_x+2)*ui_times/divisor, (ui_base_y+ui_base_h-4-2)*ui_times/divisor, (20-4)*ui_times/divisor, 4*ui_times/divisor);
	}

	// Draw Note
	if(ui_menu_item == HOST_OSD_MENU_AI){
		ab_osd_draw_rectangle(&s_dst_buf, COLOR_ARGB8888_PICKGRAY, ui_base_bg_x*ui_times/divisor, ui_gap*9*ui_times/divisor, (ui_base_w-ui_base_bg_x)*ui_times/divisor, (ui_base_h-ui_gap*9)*ui_times/divisor);
		host_osd_draw_text_utf8(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_WHITE, COLOR_ARGB8888_PICKGRAY, ui_font_size, ui_str_id[0], (ui_bar_x+ui_bar_w/2)*ui_times/divisor, (ui_gap*10)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_CENTER);
		host_osd_draw_text_utf8(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_WHITE, COLOR_ARGB8888_PICKGRAY, ui_font_size, ui_str_id[1], (ui_bar_x+ui_bar_w/2)*ui_times/divisor, (ui_gap*11)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_CENTER);
	}
}

#if 1
void host_setup_menu_display_osd_main_draw(uint16_t wIndex, MENU_PARAM eMenuParam, PMENU_STATUS pStatus)
{
	s_setup_osd.ui_osd_type = HOST_OSD_TYPE_DISPLAY;
	s_setup_osd.b_display_need_update = FALSE;
	
	ab_osd_clear(&gs_osd_display_buf, COLOR_ARGB8888_TRANSPARENT, 0, 0, gs_osd_display_buf.ui_width, gs_osd_display_buf.ui_height);

	// draw BG
	OSDLib_DrawMenuBG(gs_osd_display_buf, s_setup_osd.ui_display_resolution);

	// draw main menu
	OSDLib_DrawMainMenu(gs_osd_display_buf, wIndex, s_setup_osd.ui_display_resolution);

	s_setup_osd.b_display_need_update = TRUE;
}

void host_setup_menu_display_osd_image_draw(uint16_t wIndex, MENU_PARAM eMenuParam, PMENU_STATUS pStatus)
{
	s_setup_osd.ui_osd_type = HOST_OSD_TYPE_DISPLAY;
	s_setup_osd.b_display_need_update = FALSE;
	
	ab_osd_clear(&gs_osd_display_buf, COLOR_ARGB8888_TRANSPARENT, 0, 0, gs_osd_display_buf.ui_width, gs_osd_display_buf.ui_height);

	// draw BG
	OSDLib_DrawMenuBG(gs_osd_display_buf, s_setup_osd.ui_display_resolution);

	// draw image menu
	OSDLib_DrawImageMenu(gs_osd_display_buf, wIndex, s_setup_osd.ui_display_resolution);

	s_setup_osd.b_display_need_update = TRUE;
}

void host_setup_menu_display_osd_image_Brightness_draw(uint16_t wIndex, MENU_PARAM eMenuParam, PMENU_STATUS pStatus)
{
	s_setup_osd.ui_osd_type = HOST_OSD_TYPE_DISPLAY;
	s_setup_osd.b_display_need_update = FALSE;
	
	ab_osd_clear(&gs_osd_display_buf, COLOR_ARGB8888_TRANSPARENT, 0, 0, gs_osd_display_buf.ui_width, gs_osd_display_buf.ui_height);

	// draw BG
	OSDLib_DrawMenuBG(gs_osd_display_buf, s_setup_osd.ui_display_resolution);

	// draw image menu
	OSDLib_DrawImageMenu(gs_osd_display_buf, HOSTMI_IMAGE_BRIGHTNESS, s_setup_osd.ui_display_resolution);

	// draw Brightness bar
	OSDLib_DrawMenuAdjustValue(gs_osd_display_buf, wIndex, HOSTMI_IMAGE_BRIGHTNESS_OSD_START_VAL, HOSTMI_IMAGE_BRIGHTNESS_TOTAL-1, s_setup_osd.ui_display_resolution);

	s_setup_osd.b_display_need_update = TRUE;
}

void host_setup_menu_display_osd_image_Contrast_draw(uint16_t wIndex, MENU_PARAM eMenuParam, PMENU_STATUS pStatus)
{
	s_setup_osd.ui_osd_type = HOST_OSD_TYPE_DISPLAY;
	s_setup_osd.b_display_need_update = FALSE;
	
	ab_osd_clear(&gs_osd_display_buf, COLOR_ARGB8888_TRANSPARENT, 0, 0, gs_osd_display_buf.ui_width, gs_osd_display_buf.ui_height);

	// draw BG
	OSDLib_DrawMenuBG(gs_osd_display_buf, s_setup_osd.ui_display_resolution);

	// draw image menu
	OSDLib_DrawImageMenu(gs_osd_display_buf, HOSTMI_IMAGE_CONTRAST, s_setup_osd.ui_display_resolution);

	// draw Contrast bar
	OSDLib_DrawMenuAdjustValue(gs_osd_display_buf, wIndex, HOSTMI_IMAGE_CONTRAST_OSD_START_VAL, HOSTMI_IMAGE_CONTRAST_TOTAL-1, s_setup_osd.ui_display_resolution);

	s_setup_osd.b_display_need_update = TRUE;
}

void host_setup_menu_display_osd_image_Sharpness_draw(uint16_t wIndex, MENU_PARAM eMenuParam, PMENU_STATUS pStatus)
{
	s_setup_osd.ui_osd_type = HOST_OSD_TYPE_DISPLAY;
	s_setup_osd.b_display_need_update = FALSE;
	
	ab_osd_clear(&gs_osd_display_buf, COLOR_ARGB8888_TRANSPARENT, 0, 0, gs_osd_display_buf.ui_width, gs_osd_display_buf.ui_height);

	// draw BG
	OSDLib_DrawMenuBG(gs_osd_display_buf, s_setup_osd.ui_display_resolution);

	// draw image menu
	OSDLib_DrawImageMenu(gs_osd_display_buf, HOSTMI_IMAGE_SHARPNESS, s_setup_osd.ui_display_resolution);

	// draw Sharpness bar
	OSDLib_DrawMenuAdjustValue(gs_osd_display_buf, wIndex, HOSTMI_IMAGE_SHARPNESS_OSD_START_VAL, HOSTMI_IMAGE_SHARPNESS_TOTAL-1, s_setup_osd.ui_display_resolution);

	s_setup_osd.b_display_need_update = TRUE;
}

void host_setup_menu_display_osd_image_Saturation_draw(uint16_t wIndex, MENU_PARAM eMenuParam, PMENU_STATUS pStatus)
{
	s_setup_osd.ui_osd_type = HOST_OSD_TYPE_DISPLAY;

	s_setup_osd.b_display_need_update = FALSE;
	
	ab_osd_clear(&gs_osd_display_buf, COLOR_ARGB8888_TRANSPARENT, 0, 0, gs_osd_display_buf.ui_width, gs_osd_display_buf.ui_height);

	// draw BG
	OSDLib_DrawMenuBG(gs_osd_display_buf, s_setup_osd.ui_display_resolution);

	// draw image menu
	OSDLib_DrawImageMenu(gs_osd_display_buf, HOSTMI_IMAGE_SATURATION, s_setup_osd.ui_display_resolution);

	// draw Saturation bar
	OSDLib_DrawMenuAdjustValue(gs_osd_display_buf, wIndex, HOSTMI_IMAGE_SATURATION_OSD_START_VAL, HOSTMI_IMAGE_SATURATION_TOTAL-1, s_setup_osd.ui_display_resolution);

	s_setup_osd.b_display_need_update = TRUE;
}

void host_setup_menu_display_osd_image_VFlip_draw(uint16_t wIndex, MENU_PARAM eMenuParam, PMENU_STATUS pStatus)
{
	uint32_t ui_str_id[HOSTMI_IMAGE_VFLIP_TOTAL] = {_STR_ON_, _STR_OFF_};
	
	s_setup_osd.ui_osd_type = HOST_OSD_TYPE_DISPLAY;
	s_setup_osd.b_display_need_update = FALSE;
	
	ab_osd_clear(&gs_osd_display_buf, COLOR_ARGB8888_TRANSPARENT, 0, 0, gs_osd_display_buf.ui_width, gs_osd_display_buf.ui_height);

	// draw BG
	OSDLib_DrawMenuBG(gs_osd_display_buf, s_setup_osd.ui_display_resolution);

	// draw image menu
	OSDLib_DrawImageMenu(gs_osd_display_buf, HOSTMI_IMAGE_VFLIP, s_setup_osd.ui_display_resolution);

	// draw VFlip item
	OSDLib_Draw2ndMenu(gs_osd_display_buf, ui_str_id, wIndex, HOSTMI_IMAGE_VFLIP_TOTAL, s_setup_osd.ui_display_resolution);
	
	s_setup_osd.b_display_need_update = TRUE;
}

void host_setup_menu_display_osd_image_HFlip_draw(uint16_t wIndex, MENU_PARAM eMenuParam, PMENU_STATUS pStatus)
{
	uint32_t ui_str_id[HOSTMI_IMAGE_HFLIP_TOTAL] = {_STR_ON_, _STR_OFF_};
	
	s_setup_osd.ui_osd_type = HOST_OSD_TYPE_DISPLAY;
	s_setup_osd.b_display_need_update = FALSE;
	
	ab_osd_clear(&gs_osd_display_buf, COLOR_ARGB8888_TRANSPARENT, 0, 0, gs_osd_display_buf.ui_width, gs_osd_display_buf.ui_height);

	// draw BG
	OSDLib_DrawMenuBG(gs_osd_display_buf, s_setup_osd.ui_display_resolution);

	// draw image menu
	OSDLib_DrawImageMenu(gs_osd_display_buf, HOSTMI_IMAGE_HFLIP, s_setup_osd.ui_display_resolution);

	// draw HFlip item
	OSDLib_Draw2ndMenu(gs_osd_display_buf, ui_str_id, wIndex, HOSTMI_IMAGE_HFLIP_TOTAL, s_setup_osd.ui_display_resolution);

	s_setup_osd.b_display_need_update = TRUE;
}

void host_setup_menu_display_osd_image_Leave_draw(uint16_t wIndex, MENU_PARAM eMenuParam, PMENU_STATUS pStatus)
{
	uint32_t ui_str_id[HOSTMI_IMAGE_LEAVE_TOTAL] = {_STR_RETURN_, _STR_SAVE_, _STR_EXIT_};
	
	s_setup_osd.ui_osd_type = HOST_OSD_TYPE_DISPLAY;
	s_setup_osd.b_display_need_update = FALSE;
	
	ab_osd_clear(&gs_osd_display_buf, COLOR_ARGB8888_TRANSPARENT, 0, 0, gs_osd_display_buf.ui_width, gs_osd_display_buf.ui_height);

	// draw BG
	OSDLib_DrawMenuBG(gs_osd_display_buf, s_setup_osd.ui_display_resolution);

	// draw image menu
	OSDLib_DrawImageMenu(gs_osd_display_buf, HOSTMI_IMAGE_LEAVE, s_setup_osd.ui_display_resolution);

	// draw leave item
	OSDLib_Draw2ndMenu(gs_osd_display_buf, ui_str_id, wIndex, HOSTMI_IMAGE_LEAVE_TOTAL, s_setup_osd.ui_display_resolution);
	
	s_setup_osd.b_display_need_update = TRUE;
}

void host_setup_menu_display_osd_Hdr_draw(uint16_t wIndex, MENU_PARAM eMenuParam, PMENU_STATUS pStatus)
{
	uint32_t ui_str_id[HOSTMI_HDR_TOTAL] = {_STR_HIGH_, _STR_MIDDLE_,  _STR_LOW_, _STR_OFF_};
	
	s_setup_osd.ui_osd_type = HOST_OSD_TYPE_DISPLAY;
	s_setup_osd.b_display_need_update = FALSE;
	
	ab_osd_clear(&gs_osd_display_buf, COLOR_ARGB8888_TRANSPARENT, 0, 0, gs_osd_display_buf.ui_width, gs_osd_display_buf.ui_height);

	// draw BG
	OSDLib_DrawMenuBG(gs_osd_display_buf, s_setup_osd.ui_display_resolution);

	// draw main menu
	OSDLib_DrawMainMenu(gs_osd_display_buf, SETUPMENU_HDR, s_setup_osd.ui_display_resolution);

	// draw hdr item
	OSDLib_Draw2ndMenu(gs_osd_display_buf, ui_str_id, wIndex, HOSTMI_HDR_TOTAL, s_setup_osd.ui_display_resolution);
	
	s_setup_osd.b_display_need_update = TRUE;
}

void host_setup_menu_display_osd_WB_draw(uint16_t wIndex, MENU_PARAM eMenuParam, PMENU_STATUS pStatus)
{
	uint32_t ui_str_id[HOSTMI_WB_TOTAL] = {_STR_COOL_LIGHTING_ENVIRONMENT_, _STR_WARM_LIGHTING_ENVIRONMENT_,  _STR_AUTO_TRACKING_, _STR_FIXED_WHITE_BALANCE_};	
	
	s_setup_osd.ui_osd_type = HOST_OSD_TYPE_DISPLAY;
	s_setup_osd.b_display_need_update = FALSE;
	
	ab_osd_clear(&gs_osd_display_buf, COLOR_ARGB8888_TRANSPARENT, 0, 0, gs_osd_display_buf.ui_width, gs_osd_display_buf.ui_height);

	// draw BG
	OSDLib_DrawMenuBG(gs_osd_display_buf, s_setup_osd.ui_display_resolution);

	// draw main menu
	OSDLib_DrawMainMenu(gs_osd_display_buf, SETUPMENU_WB, s_setup_osd.ui_display_resolution);

	// draw WB item
	OSDLib_Draw2ndMenu(gs_osd_display_buf, ui_str_id, wIndex, HOSTMI_WB_TOTAL, s_setup_osd.ui_display_resolution);
	
	s_setup_osd.b_display_need_update = TRUE;
}

void host_setup_menu_display_osd_Hdmi_draw(uint16_t wIndex, MENU_PARAM eMenuParam, PMENU_STATUS pStatus)
{
	s_setup_osd.ui_osd_type = HOST_OSD_TYPE_DISPLAY;
	s_setup_osd.b_display_need_update = FALSE;
	
	ab_osd_clear(&gs_osd_display_buf, COLOR_ARGB8888_TRANSPARENT, 0, 0, gs_osd_display_buf.ui_width, gs_osd_display_buf.ui_height);

	// draw BG
	OSDLib_DrawMenuBG(gs_osd_display_buf, s_setup_osd.ui_display_resolution);

	// draw hdmi menu
	OSDLib_DrawHdmiMenu(gs_osd_display_buf, wIndex, s_setup_osd.ui_display_resolution);

	s_setup_osd.b_display_need_update = TRUE;
}

void host_setup_menu_display_osd_Hdmi_Resolution_draw(uint16_t wIndex, MENU_PARAM eMenuParam, PMENU_STATUS pStatus)
{
	s_setup_osd.ui_osd_type = HOST_OSD_TYPE_DISPLAY;
	s_setup_osd.b_display_need_update = FALSE;
	
	ab_osd_clear(&gs_osd_display_buf, COLOR_ARGB8888_TRANSPARENT, 0, 0, gs_osd_display_buf.ui_width, gs_osd_display_buf.ui_height);

	// draw BG
	OSDLib_DrawMenuBG(gs_osd_display_buf, s_setup_osd.ui_display_resolution);

	// draw hdmi menu
	OSDLib_DrawHdmiMenu(gs_osd_display_buf, HOSTMI_HDMI_RESOLUTION, s_setup_osd.ui_display_resolution);

	// draw resolution item
	OSDLib_DrawHdmiResolutionMenu(gs_osd_display_buf, wIndex, s_setup_osd.ui_display_resolution);

	s_setup_osd.b_display_need_update = TRUE;
}

void host_setup_menu_display_osd_Hdmi_Leave_draw(uint16_t wIndex, MENU_PARAM eMenuParam, PMENU_STATUS pStatus)
{
	uint32_t ui_str_id[HOSTMI_HDMI_LEAVE_TOTAL] = {_STR_RETURN_, _STR_SAVE_, _STR_EXIT_};
	
	s_setup_osd.ui_osd_type = HOST_OSD_TYPE_DISPLAY;
	s_setup_osd.b_display_need_update = FALSE;

	ab_osd_clear(&gs_osd_display_buf, COLOR_ARGB8888_TRANSPARENT, 0, 0, gs_osd_display_buf.ui_width, gs_osd_display_buf.ui_height);

	// draw BG
	OSDLib_DrawMenuBG(gs_osd_display_buf, s_setup_osd.ui_display_resolution);

	// draw hdmi menu
	OSDLib_DrawHdmiMenu(gs_osd_display_buf, HOSTMI_HDMI_LEAVE, s_setup_osd.ui_display_resolution);
	
	// draw leave item
	OSDLib_Draw2ndMenu(gs_osd_display_buf, ui_str_id, wIndex, HOSTMI_HDMI_LEAVE_TOTAL, s_setup_osd.ui_display_resolution);
	
	s_setup_osd.b_display_need_update = TRUE;
}


void host_setup_menu_display_osd_Audio_draw(uint16_t wIndex, MENU_PARAM eMenuParam, PMENU_STATUS pStatus)
{
	s_setup_osd.ui_osd_type = HOST_OSD_TYPE_DISPLAY;
	s_setup_osd.b_display_need_update = FALSE;
	
	ab_osd_clear(&gs_osd_display_buf, COLOR_ARGB8888_TRANSPARENT, 0, 0, gs_osd_display_buf.ui_width, gs_osd_display_buf.ui_height);

	// draw BG
	OSDLib_DrawMenuBG(gs_osd_display_buf, s_setup_osd.ui_display_resolution);

	// draw main menu
	OSDLib_DrawMainMenu(gs_osd_display_buf, SETUPMENU_AUDIO, s_setup_osd.ui_display_resolution);

	// draw audio bar
	OSDLib_DrawMenuAdjustValue(gs_osd_display_buf, wIndex, HOSTMI_AUDIO_OSD_START_VAL, HOSTMI_AUDIO_TOTAL-1, s_setup_osd.ui_display_resolution);

	s_setup_osd.b_display_need_update = TRUE;
}

void host_setup_menu_display_osd_Pir_draw(uint16_t wIndex, MENU_PARAM eMenuParam, PMENU_STATUS pStatus)
{
	uint32_t ui_str_id[HOSTMI_PIR_TOTAL] = {_STR_HIGH_, _STR_MIDDLE_, _STR_LOW_};
	
	s_setup_osd.ui_osd_type = HOST_OSD_TYPE_DISPLAY;
	s_setup_osd.b_display_need_update = FALSE;
	
	ab_osd_clear(&gs_osd_display_buf, COLOR_ARGB8888_TRANSPARENT, 0, 0, gs_osd_display_buf.ui_width, gs_osd_display_buf.ui_height);

	// draw BG
	OSDLib_DrawMenuBG(gs_osd_display_buf, s_setup_osd.ui_display_resolution);

	// draw main menu
	OSDLib_DrawMainMenu(gs_osd_display_buf, SETUPMENU_PIR, s_setup_osd.ui_display_resolution);

	// draw pir item
	OSDLib_Draw2ndMenu(gs_osd_display_buf, ui_str_id, wIndex, HOSTMI_PIR_TOTAL, s_setup_osd.ui_display_resolution);

	s_setup_osd.b_display_need_update = TRUE;
}

void host_setup_menu_display_osd_Time_draw(uint16_t wIndex, MENU_PARAM eMenuParam, PMENU_STATUS pStatus)
{
	uint32_t ui_str_id[HOSTMI_TIME_TOTAL] = {_STR_DISPLAY_, _STR_TIME_SETUP_};
	
	s_setup_osd.ui_osd_type = HOST_OSD_TYPE_DISPLAY;
	s_setup_osd.b_display_need_update = FALSE;
	
	ab_osd_clear(&gs_osd_display_buf, COLOR_ARGB8888_TRANSPARENT, 0, 0, gs_osd_display_buf.ui_width, gs_osd_display_buf.ui_height);

	// draw BG
	OSDLib_DrawMenuBG(gs_osd_display_buf, s_setup_osd.ui_display_resolution);

	// draw main menu
	OSDLib_DrawMainMenu(gs_osd_display_buf, SETUPMENU_TIME, s_setup_osd.ui_display_resolution);

	// draw time item
	OSDLib_Draw2ndMenu(gs_osd_display_buf, ui_str_id, wIndex, HOSTMI_TIME_TOTAL, s_setup_osd.ui_display_resolution);

	s_setup_osd.b_display_need_update = TRUE;
}

void host_setup_menu_display_osd_Time_Display_draw(uint16_t wIndex, MENU_PARAM eMenuParam, PMENU_STATUS pStatus)
{
	uint32_t ui_str_id_2nd[HOSTMI_TIME_TOTAL] = {_STR_DISPLAY_, _STR_TIME_SETUP_};
	uint32_t ui_str_id_3rd[HOSTMI_TIME_DISPLAY_TOTAL] = {_STR_TIME_, _STR_TIME_DATE_, _STR_TIME_OFF_};
	
	s_setup_osd.ui_osd_type = HOST_OSD_TYPE_DISPLAY;
	s_setup_osd.b_display_need_update = FALSE;
	
	ab_osd_clear(&gs_osd_display_buf, COLOR_ARGB8888_TRANSPARENT, 0, 0, gs_osd_display_buf.ui_width, gs_osd_display_buf.ui_height);

	// draw BG
	OSDLib_DrawMenuBG(gs_osd_display_buf, s_setup_osd.ui_display_resolution);

	// draw main menu
	OSDLib_DrawMainMenu(gs_osd_display_buf, SETUPMENU_TIME, s_setup_osd.ui_display_resolution);

	// draw time item
	OSDLib_Draw2ndMenu(gs_osd_display_buf, ui_str_id_2nd, HOSTMI_TIME_DISPLAY, HOSTMI_TIME_TOTAL, s_setup_osd.ui_display_resolution);

	// draw time display item
	OSDLib_Draw3rdMenu(gs_osd_display_buf, ui_str_id_3rd, wIndex, HOSTMI_TIME_DISPLAY_TOTAL, s_setup_osd.ui_display_resolution);

	s_setup_osd.b_display_need_update = TRUE;
}

void host_setup_menu_display_osd_Time_Timesetup_draw(uint16_t wIndex, MENU_PARAM eMenuParam, PMENU_STATUS pStatus)
{
	uint32_t ui_str_id_2nd[HOSTMI_TIME_TOTAL] = {_STR_DISPLAY_, _STR_TIME_SETUP_};
	uint32_t ui_str_id_3rd[HOSTMI_TIME_TIMESETUP_TOTAL] = {_STR_AUTO_, _STR_MANUAL_};
	
	s_setup_osd.ui_osd_type = HOST_OSD_TYPE_DISPLAY;
	s_setup_osd.b_display_need_update = FALSE;
	
	ab_osd_clear(&gs_osd_display_buf, COLOR_ARGB8888_TRANSPARENT, 0, 0, gs_osd_display_buf.ui_width, gs_osd_display_buf.ui_height);

	// draw BG
	OSDLib_DrawMenuBG(gs_osd_display_buf, s_setup_osd.ui_display_resolution);

	// draw main menu
	OSDLib_DrawMainMenu(gs_osd_display_buf, SETUPMENU_TIME, s_setup_osd.ui_display_resolution);

	// draw time item
	OSDLib_Draw2ndMenu(gs_osd_display_buf, ui_str_id_2nd, HOSTMI_TIME_TIMESETUP, HOSTMI_TIME_TOTAL, s_setup_osd.ui_display_resolution);

	// draw time display item
	OSDLib_Draw3rdMenu(gs_osd_display_buf, ui_str_id_3rd, wIndex, HOSTMI_TIME_TIMESETUP_TOTAL, s_setup_osd.ui_display_resolution);

	s_setup_osd.b_display_need_update = TRUE;
}

void host_setup_menu_display_osd_Time_Timesetup_Munual_draw(uint16_t wIndex, MENU_PARAM eMenuParam, PMENU_STATUS pStatus)
{
	uint32_t ui_str_id[HOSTMI_TIME_TIMESETUP_MANUAL_TOTAL] = {_STR_TIME_, _STR_DATE_};
	
	s_setup_osd.ui_osd_type = HOST_OSD_TYPE_DISPLAY;
	s_setup_osd.b_display_need_update = FALSE;
	
	ab_osd_clear(&gs_osd_display_buf, COLOR_ARGB8888_TRANSPARENT, 0, 0, gs_osd_display_buf.ui_width, gs_osd_display_buf.ui_height);

	// draw BG
	OSDLib_DrawMenuBG(gs_osd_display_buf, s_setup_osd.ui_display_resolution);

	// draw time timesetup munual menu
	OSDLib_Draw1stMenu(gs_osd_display_buf, _STR_TIME_OPTION_, ui_str_id, wIndex, HOSTMI_TIME_TIMESETUP_MANUAL_TOTAL, s_setup_osd.ui_display_resolution);

	s_setup_osd.b_display_need_update = TRUE;
}

void host_setup_menu_display_osd_Time_Timesetup_Munual_Time_draw(uint16_t wIndex, MENU_PARAM eMenuParam, PMENU_STATUS pStatus)
{
	uint32_t ui_str_id[HOSTMI_TIME_TIMESETUP_MANUAL_TOTAL] = {_STR_TIME_, _STR_DATE_};
	
	s_setup_osd.ui_osd_type = HOST_OSD_TYPE_DISPLAY;
	s_setup_osd.b_display_need_update = FALSE;
	
	ab_osd_clear(&gs_osd_display_buf, COLOR_ARGB8888_TRANSPARENT, 0, 0, gs_osd_display_buf.ui_width, gs_osd_display_buf.ui_height);

	// draw BG
	OSDLib_DrawMenuBG(gs_osd_display_buf, s_setup_osd.ui_display_resolution);

	// draw time timesetup munual menu
	OSDLib_Draw1stMenu(gs_osd_display_buf, _STR_TIME_OPTION_, ui_str_id, HOSTMI_TIME_TIMESETUP_MANUAL_TIME, HOSTMI_TIME_TIMESETUP_MANUAL_TOTAL, s_setup_osd.ui_display_resolution);

	// draw munual time 
	OSDLib_DrawTimeMunualMenu(gs_osd_display_buf, wIndex, s_setup_osd.ui_display_resolution);

	s_setup_osd.b_display_need_update = TRUE;
}

void host_setup_menu_display_osd_Time_Timesetup_Munual_Date_draw(uint16_t wIndex, MENU_PARAM eMenuParam, PMENU_STATUS pStatus)
{
	uint32_t ui_str_id[HOSTMI_TIME_TIMESETUP_MANUAL_TOTAL] = {_STR_TIME_, _STR_DATE_};
	
	s_setup_osd.ui_osd_type = HOST_OSD_TYPE_DISPLAY;
	s_setup_osd.b_display_need_update = FALSE;
	
	ab_osd_clear(&gs_osd_display_buf, COLOR_ARGB8888_TRANSPARENT, 0, 0, gs_osd_display_buf.ui_width, gs_osd_display_buf.ui_height);

	// draw BG
	OSDLib_DrawMenuBG(gs_osd_display_buf, s_setup_osd.ui_display_resolution);

	// draw time timesetup munual menu
	OSDLib_Draw1stMenu(gs_osd_display_buf, _STR_TIME_OPTION_, ui_str_id, HOSTMI_TIME_TIMESETUP_MANUAL_DATE, HOSTMI_TIME_TIMESETUP_MANUAL_TOTAL, s_setup_osd.ui_display_resolution);

	// draw munual date
	OSDLib_DrawTimeMunualMenu(gs_osd_display_buf, wIndex, s_setup_osd.ui_display_resolution);

	s_setup_osd.b_display_need_update = TRUE;
}

void host_setup_menu_display_osd_Language_draw(uint16_t wIndex, MENU_PARAM eMenuParam, PMENU_STATUS pStatus)
{
	s_setup_osd.ui_osd_type = HOST_OSD_TYPE_DISPLAY;
	s_setup_osd.b_display_need_update = FALSE;
	
	ab_osd_clear(&gs_osd_display_buf, COLOR_ARGB8888_TRANSPARENT, 0, 0, gs_osd_display_buf.ui_width, gs_osd_display_buf.ui_height);

	// draw BG
	OSDLib_DrawMenuBG(gs_osd_display_buf, s_setup_osd.ui_display_resolution);

	// draw main menu
	host_setup_set_language(wIndex);
	OSDLib_DrawMainMenu(gs_osd_display_buf, SETUPMENU_LANGUAGE, s_setup_osd.ui_display_resolution);
	
	// draw language item
	OSDLib_DrawLanguageMenu(gs_osd_display_buf, wIndex, s_setup_osd.ui_display_resolution);

	s_setup_osd.b_display_need_update = TRUE;
}

void host_setup_menu_display_osd_Ai_draw(uint16_t wIndex, MENU_PARAM eMenuParam, PMENU_STATUS pStatus)
{
	s_setup_osd.ui_osd_type = HOST_OSD_TYPE_DISPLAY;
	s_setup_osd.b_display_need_update = FALSE;
	
	ab_osd_clear(&gs_osd_display_buf, COLOR_ARGB8888_TRANSPARENT, 0, 0, gs_osd_display_buf.ui_width, gs_osd_display_buf.ui_height);

	// draw BG
	OSDLib_DrawMenuBG(gs_osd_display_buf, s_setup_osd.ui_display_resolution);

	// draw main menu
	OSDLib_DrawMainMenu(gs_osd_display_buf, SETUPMENU_AI, s_setup_osd.ui_display_resolution);

	// draw ai bar
	OSDLib_DrawMenuAdjustValue(gs_osd_display_buf, wIndex, HOSTMI_AI_OSD_START_VAL, HOSTMI_AI_TOTAL-1, s_setup_osd.ui_display_resolution);

	s_setup_osd.b_display_need_update = TRUE;
}

void host_setup_menu_display_osd_StitchAlg_draw(uint16_t wIndex, MENU_PARAM eMenuParam, PMENU_STATUS pStatus)
{
	uint32_t ui_str_id[HOSTMI_STITCHALG_TOTAL] = {_STR_RESTORE_FACTORY_SETTINGS_, _STR_ACTIVE_};
	
	s_setup_osd.ui_osd_type = HOST_OSD_TYPE_DISPLAY;
	s_setup_osd.b_display_need_update = FALSE;
	
	ab_osd_clear(&gs_osd_display_buf, COLOR_ARGB8888_TRANSPARENT, 0, 0, gs_osd_display_buf.ui_width, gs_osd_display_buf.ui_height);

	// draw BG
	OSDLib_DrawMenuBG(gs_osd_display_buf, s_setup_osd.ui_display_resolution);

	// draw main menu
	OSDLib_DrawMainMenu(gs_osd_display_buf, SETUPMENU_STITCHALG, s_setup_osd.ui_display_resolution);

	// draw StitchAlg item
	OSDLib_Draw2ndMenu(gs_osd_display_buf, ui_str_id, wIndex, HOSTMI_STITCHALG_TOTAL, s_setup_osd.ui_display_resolution);

	s_setup_osd.b_display_need_update = TRUE;
}

void host_setup_menu_display_osd_Check_draw(uint16_t wIndex, MENU_PARAM eMenuParam, PMENU_STATUS pStatus)
{
	uint32_t ui_str_id[HOSTMI_CHECK_TOTAL] = {_STR_SAVE_, _STR_RESTORE_, _STR_EXIT_};
	
	s_setup_osd.ui_osd_type = HOST_OSD_TYPE_DISPLAY;
	s_setup_osd.b_display_need_update = FALSE;
	
	ab_osd_clear(&gs_osd_display_buf, COLOR_ARGB8888_TRANSPARENT, 0, 0, gs_osd_display_buf.ui_width, gs_osd_display_buf.ui_height);

	// draw BG
	OSDLib_DrawMenuBG(gs_osd_display_buf, s_setup_osd.ui_display_resolution);

	// draw main menu
	OSDLib_DrawMainMenu(gs_osd_display_buf, SETUPMENU_CHECK, s_setup_osd.ui_display_resolution);

	// draw check item
	OSDLib_Draw2ndMenu(gs_osd_display_buf, ui_str_id, wIndex, HOSTMI_CHECK_TOTAL, s_setup_osd.ui_display_resolution);

	s_setup_osd.b_display_need_update = TRUE;
}

void host_setup_menu_display_osd_StitchAlg_Check_draw(char* sz_text[], uint8_t ntotaltext, uint32_t ui_argb8888_color, uint32_t ui_argb8888_bg_color)
{
	if(s_setup_osd.b_is_init == 0){
		printf("OSD Buffer not created!\n");
		return;
	}

	AB_OSD_RECT_ST s_dst_rect = {0};
	uint8_t i = 0;
	uint32_t ui_x = 0;
	uint32_t ui_base_w = 0;
	uint32_t ui_base_h = 0;	
	uint32_t ui_gap = 0;
	uint32_t ui_times = 30;
	uint32_t divisor = 10;
	uint32_t ui_font_size = 0;

	switch(s_setup_osd.ui_display_resolution){
		case HOST_OSD_MENU_RESOLUTION_8K:
			ui_times = 60;
			break;
			
		default:
		case HOST_OSD_MENU_RESOLUTION_4K:
			ui_times = 30;
			break;
			
		case HOST_OSD_MENU_RESOLUTION_FULLHD:
			ui_times = 15;
			break;
			
		case HOST_OSD_MENU_RESOLUTION_HD:
			ui_times = 10;
			break;		
	}

	s_setup_osd.b_display_need_update = FALSE;

	ui_font_size = s_setup_osd.ui_display_font_size;
	ui_base_w = s_setup_osd.ui_display_base_w;
	ui_base_h = s_setup_osd.ui_display_base_h;
	ui_gap = ui_base_h/5;

	ui_x = ui_base_w/2;

	ab_osd_clear(&gs_osd_display_buf, COLOR_ARGB8888_TRANSPARENT, 0, 0, gs_osd_display_buf.ui_width, gs_osd_display_buf.ui_height);
	
	if(ntotaltext>5)ntotaltext = 5;
	for(i=0; i<ntotaltext; i++){
		ab_osd_draw_text(&gs_osd_display_buf, &s_dst_rect, ui_argb8888_color, ui_argb8888_bg_color, ui_font_size+10, sz_text[i], ui_x*ui_times/divisor, (ui_gap/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_CENTER);
	}
	
	s_setup_osd.b_display_need_update = TRUE;
}

void host_setup_menu_display_osd_Clear_draw(void)
{
	s_setup_osd.b_display_need_clear = FALSE;
	
	ab_osd_clear(&gs_osd_display_buf, COLOR_ARGB8888_TRANSPARENT, 0, 0, gs_osd_display_buf.ui_width, gs_osd_display_buf.ui_height);

	s_setup_osd.b_display_need_clear = TRUE;
}
#endif
#if 1
void host_setup_menu_enc_osd_main_draw(uint16_t wIndex, MENU_PARAM eMenuParam, PMENU_STATUS pStatus)
{
	s_setup_osd.ui_osd_type = HOST_OSD_TYPE_ENC;
	s_setup_osd.b_enc_need_update = FALSE;
	
	ab_osd_clear(&gs_osd_enc_buf, COLOR_ARGB8888_TRANSPARENT, 0, 0, gs_osd_enc_buf.ui_width, gs_osd_enc_buf.ui_height);

	// draw BG
	OSDLib_DrawMenuBG(gs_osd_enc_buf, s_setup_osd.ui_enc_resolution);

	// draw main menu
	OSDLib_DrawMainMenu(gs_osd_enc_buf, wIndex, s_setup_osd.ui_enc_resolution);
	
	s_setup_osd.b_enc_need_update = TRUE;
}

void host_setup_menu_enc_osd_image_draw(uint16_t wIndex, MENU_PARAM eMenuParam, PMENU_STATUS pStatus)
{
	s_setup_osd.ui_osd_type = HOST_OSD_TYPE_ENC;
	s_setup_osd.b_enc_need_update = FALSE;
	
	ab_osd_clear(&gs_osd_enc_buf, COLOR_ARGB8888_TRANSPARENT, 0, 0, gs_osd_enc_buf.ui_width, gs_osd_enc_buf.ui_height);

	// draw BG
	OSDLib_DrawMenuBG(gs_osd_enc_buf, s_setup_osd.ui_enc_resolution);

	// draw main menu
	OSDLib_DrawImageMenu(gs_osd_enc_buf, wIndex, s_setup_osd.ui_enc_resolution);

	s_setup_osd.b_enc_need_update = TRUE;
}

void host_setup_menu_enc_osd_image_Brightness_draw(uint16_t wIndex, MENU_PARAM eMenuParam, PMENU_STATUS pStatus)
{
	s_setup_osd.ui_osd_type = HOST_OSD_TYPE_ENC;
	s_setup_osd.b_enc_need_update = FALSE;
	
	ab_osd_clear(&gs_osd_enc_buf, COLOR_ARGB8888_TRANSPARENT, 0, 0, gs_osd_enc_buf.ui_width, gs_osd_enc_buf.ui_height);

	// draw BG
	OSDLib_DrawMenuBG(gs_osd_enc_buf, s_setup_osd.ui_enc_resolution);

	// draw image menu
	OSDLib_DrawImageMenu(gs_osd_enc_buf, HOSTMI_IMAGE_BRIGHTNESS, s_setup_osd.ui_enc_resolution);

	// draw Brightness bar
	OSDLib_DrawMenuAdjustValue(gs_osd_enc_buf, wIndex, HOSTMI_IMAGE_BRIGHTNESS_OSD_START_VAL, HOSTMI_IMAGE_BRIGHTNESS_TOTAL-1, s_setup_osd.ui_enc_resolution);

	s_setup_osd.b_enc_need_update = TRUE;
}

void host_setup_menu_enc_osd_image_Contrast_draw(uint16_t wIndex, MENU_PARAM eMenuParam, PMENU_STATUS pStatus)
{
	s_setup_osd.ui_osd_type = HOST_OSD_TYPE_ENC;
	s_setup_osd.b_enc_need_update = FALSE;
	
	ab_osd_clear(&gs_osd_enc_buf, COLOR_ARGB8888_TRANSPARENT, 0, 0, gs_osd_enc_buf.ui_width, gs_osd_enc_buf.ui_height);

	// draw BG
	OSDLib_DrawMenuBG(gs_osd_enc_buf, s_setup_osd.ui_enc_resolution);

	// draw image menu
	OSDLib_DrawImageMenu(gs_osd_enc_buf, HOSTMI_IMAGE_CONTRAST, s_setup_osd.ui_enc_resolution);

	// draw Contrast bar
	OSDLib_DrawMenuAdjustValue(gs_osd_enc_buf, wIndex, HOSTMI_IMAGE_CONTRAST_OSD_START_VAL, HOSTMI_IMAGE_CONTRAST_TOTAL-1, s_setup_osd.ui_enc_resolution);

	s_setup_osd.b_enc_need_update = TRUE;
}

void host_setup_menu_enc_osd_image_Sharpness_draw(uint16_t wIndex, MENU_PARAM eMenuParam, PMENU_STATUS pStatus)
{
	s_setup_osd.ui_osd_type = HOST_OSD_TYPE_ENC;
	s_setup_osd.b_enc_need_update = FALSE;
	
	ab_osd_clear(&gs_osd_enc_buf, COLOR_ARGB8888_TRANSPARENT, 0, 0, gs_osd_enc_buf.ui_width, gs_osd_enc_buf.ui_height);

	// draw BG
	OSDLib_DrawMenuBG(gs_osd_enc_buf, s_setup_osd.ui_enc_resolution);

	// draw image menu
	OSDLib_DrawImageMenu(gs_osd_enc_buf, HOSTMI_IMAGE_SHARPNESS, s_setup_osd.ui_enc_resolution);

	// draw Sharpness bar
	OSDLib_DrawMenuAdjustValue(gs_osd_enc_buf, wIndex, HOSTMI_IMAGE_SHARPNESS_OSD_START_VAL, HOSTMI_IMAGE_SHARPNESS_TOTAL-1, s_setup_osd.ui_enc_resolution);

	s_setup_osd.b_enc_need_update = TRUE;
}

void host_setup_menu_enc_osd_image_Saturation_draw(uint16_t wIndex, MENU_PARAM eMenuParam, PMENU_STATUS pStatus)
{
	s_setup_osd.ui_osd_type = HOST_OSD_TYPE_ENC;
	s_setup_osd.b_enc_need_update = FALSE;
	
	ab_osd_clear(&gs_osd_enc_buf, COLOR_ARGB8888_TRANSPARENT, 0, 0, gs_osd_enc_buf.ui_width, gs_osd_enc_buf.ui_height);

	// draw BG
	OSDLib_DrawMenuBG(gs_osd_enc_buf, s_setup_osd.ui_enc_resolution);

	// draw image menu
	OSDLib_DrawImageMenu(gs_osd_enc_buf, HOSTMI_IMAGE_SATURATION, s_setup_osd.ui_enc_resolution);

	// draw Saturation bar
	OSDLib_DrawMenuAdjustValue(gs_osd_enc_buf, wIndex, HOSTMI_IMAGE_SATURATION_OSD_START_VAL, HOSTMI_IMAGE_SATURATION_TOTAL-1, s_setup_osd.ui_enc_resolution);

	s_setup_osd.b_enc_need_update = TRUE;
}

void host_setup_menu_enc_osd_image_VFlip_draw(uint16_t wIndex, MENU_PARAM eMenuParam, PMENU_STATUS pStatus)
{
	uint32_t ui_str_id[HOSTMI_IMAGE_VFLIP_TOTAL] = {_STR_ON_, _STR_OFF_};
	
	s_setup_osd.ui_osd_type = HOST_OSD_TYPE_ENC;
	s_setup_osd.b_enc_need_update = FALSE;
	
	ab_osd_clear(&gs_osd_enc_buf, COLOR_ARGB8888_TRANSPARENT, 0, 0, gs_osd_enc_buf.ui_width, gs_osd_enc_buf.ui_height);

	// draw BG
	OSDLib_DrawMenuBG(gs_osd_enc_buf, s_setup_osd.ui_enc_resolution);

	// draw image menu
	OSDLib_DrawImageMenu(gs_osd_enc_buf, HOSTMI_IMAGE_VFLIP, s_setup_osd.ui_enc_resolution);

	// draw VFlip item
	OSDLib_Draw2ndMenu(gs_osd_enc_buf, ui_str_id, wIndex, HOSTMI_IMAGE_VFLIP_TOTAL, s_setup_osd.ui_enc_resolution);

	s_setup_osd.b_enc_need_update = TRUE;
}

void host_setup_menu_enc_osd_image_HFlip_draw(uint16_t wIndex, MENU_PARAM eMenuParam, PMENU_STATUS pStatus)
{
	uint32_t ui_str_id[HOSTMI_IMAGE_HFLIP_TOTAL] = {_STR_ON_, _STR_OFF_};
	
	s_setup_osd.ui_osd_type = HOST_OSD_TYPE_ENC;
	s_setup_osd.b_enc_need_update = FALSE;
	
	ab_osd_clear(&gs_osd_enc_buf, COLOR_ARGB8888_TRANSPARENT, 0, 0, gs_osd_enc_buf.ui_width, gs_osd_enc_buf.ui_height);

	// draw BG
	OSDLib_DrawMenuBG(gs_osd_enc_buf, s_setup_osd.ui_enc_resolution);

	// draw image menu
	OSDLib_DrawImageMenu(gs_osd_enc_buf, HOSTMI_IMAGE_HFLIP, s_setup_osd.ui_enc_resolution);

	// draw HFlip item
	OSDLib_Draw2ndMenu(gs_osd_enc_buf, ui_str_id, wIndex, HOSTMI_IMAGE_HFLIP_TOTAL, s_setup_osd.ui_enc_resolution);

	s_setup_osd.b_enc_need_update = TRUE;
}

void host_setup_menu_enc_osd_image_Leave_draw(uint16_t wIndex, MENU_PARAM eMenuParam, PMENU_STATUS pStatus)
{
	uint32_t ui_str_id[HOSTMI_IMAGE_LEAVE_TOTAL] = {_STR_RETURN_, _STR_SAVE_, _STR_EXIT_};
	
	s_setup_osd.ui_osd_type = HOST_OSD_TYPE_ENC;
	s_setup_osd.b_enc_need_update = FALSE;
	
	ab_osd_clear(&gs_osd_enc_buf, COLOR_ARGB8888_TRANSPARENT, 0, 0, gs_osd_enc_buf.ui_width, gs_osd_enc_buf.ui_height);

	// draw BG
	OSDLib_DrawMenuBG(gs_osd_enc_buf, s_setup_osd.ui_enc_resolution);

	// draw image menu
	OSDLib_DrawImageMenu(gs_osd_enc_buf, HOSTMI_IMAGE_LEAVE, s_setup_osd.ui_enc_resolution);

	// draw leave item
	OSDLib_Draw2ndMenu(gs_osd_enc_buf, ui_str_id, wIndex, HOSTMI_IMAGE_LEAVE_TOTAL, s_setup_osd.ui_enc_resolution);

	s_setup_osd.b_enc_need_update = TRUE;
}

void host_setup_menu_enc_osd_Hdr_draw(uint16_t wIndex, MENU_PARAM eMenuParam, PMENU_STATUS pStatus)
{
	uint32_t ui_str_id[HOSTMI_HDR_TOTAL] = {_STR_HIGH_, _STR_MIDDLE_,  _STR_LOW_, _STR_OFF_};
	
	s_setup_osd.ui_osd_type = HOST_OSD_TYPE_ENC;
	s_setup_osd.b_enc_need_update = FALSE;
	
	ab_osd_clear(&gs_osd_enc_buf, COLOR_ARGB8888_TRANSPARENT, 0, 0, gs_osd_enc_buf.ui_width, gs_osd_enc_buf.ui_height);

	// draw BG
	OSDLib_DrawMenuBG(gs_osd_enc_buf, s_setup_osd.ui_enc_resolution);

	// draw main menu
	OSDLib_DrawMainMenu(gs_osd_enc_buf, SETUPMENU_HDR, s_setup_osd.ui_enc_resolution);
	
	// draw hdr item
	OSDLib_Draw2ndMenu(gs_osd_enc_buf, ui_str_id, wIndex, HOSTMI_HDR_TOTAL, s_setup_osd.ui_enc_resolution);

	s_setup_osd.b_enc_need_update = TRUE;
}

void host_setup_menu_enc_osd_WB_draw(uint16_t wIndex, MENU_PARAM eMenuParam, PMENU_STATUS pStatus)
{
	uint32_t ui_str_id[HOSTMI_WB_TOTAL] = {_STR_COOL_LIGHTING_ENVIRONMENT_, _STR_WARM_LIGHTING_ENVIRONMENT_,  _STR_AUTO_TRACKING_, _STR_FIXED_WHITE_BALANCE_};
	
	s_setup_osd.ui_osd_type = HOST_OSD_TYPE_ENC;
	s_setup_osd.b_enc_need_update = FALSE;
	
	ab_osd_clear(&gs_osd_enc_buf, COLOR_ARGB8888_TRANSPARENT, 0, 0, gs_osd_enc_buf.ui_width, gs_osd_enc_buf.ui_height);

	// draw BG
	OSDLib_DrawMenuBG(gs_osd_enc_buf, s_setup_osd.ui_enc_resolution);

	// draw main menu
	OSDLib_DrawMainMenu(gs_osd_enc_buf, SETUPMENU_WB, s_setup_osd.ui_enc_resolution);

	// draw WB item
	OSDLib_Draw2ndMenu(gs_osd_enc_buf, ui_str_id, wIndex, HOSTMI_WB_TOTAL, s_setup_osd.ui_enc_resolution);

	s_setup_osd.b_enc_need_update = TRUE;
}

void host_setup_menu_enc_osd_Hdmi_draw(uint16_t wIndex, MENU_PARAM eMenuParam, PMENU_STATUS pStatus)
{
	s_setup_osd.ui_osd_type = HOST_OSD_TYPE_ENC;
	s_setup_osd.b_enc_need_update = FALSE;
	
	ab_osd_clear(&gs_osd_enc_buf, COLOR_ARGB8888_TRANSPARENT, 0, 0, gs_osd_enc_buf.ui_width, gs_osd_enc_buf.ui_height);

	// draw BG
	OSDLib_DrawMenuBG(gs_osd_enc_buf, s_setup_osd.ui_enc_resolution);

	// draw hdmi item
	OSDLib_DrawHdmiMenu(gs_osd_enc_buf, wIndex, s_setup_osd.ui_enc_resolution);

	s_setup_osd.b_enc_need_update = TRUE;
}

void host_setup_menu_enc_osd_Hdmi_Resolution_draw(uint16_t wIndex, MENU_PARAM eMenuParam, PMENU_STATUS pStatus)
{
	s_setup_osd.ui_osd_type = HOST_OSD_TYPE_ENC;
	s_setup_osd.b_enc_need_update = FALSE;
	
	ab_osd_clear(&gs_osd_enc_buf, COLOR_ARGB8888_TRANSPARENT, 0, 0, gs_osd_enc_buf.ui_width, gs_osd_enc_buf.ui_height);

	// draw BG
	OSDLib_DrawMenuBG(gs_osd_enc_buf, s_setup_osd.ui_enc_resolution);

	// draw hdmi menu
	OSDLib_DrawHdmiMenu(gs_osd_enc_buf, HOSTMI_HDMI_RESOLUTION, s_setup_osd.ui_enc_resolution);

	// draw resolution item
	OSDLib_DrawHdmiResolutionMenu(gs_osd_enc_buf, wIndex, s_setup_osd.ui_enc_resolution);
	
	s_setup_osd.b_enc_need_update = TRUE;
}

void host_setup_menu_enc_osd_Hdmi_Leave_draw(uint16_t wIndex, MENU_PARAM eMenuParam, PMENU_STATUS pStatus)
{
	uint32_t ui_str_id[HOSTMI_HDMI_LEAVE_TOTAL] = {_STR_RETURN_, _STR_SAVE_, _STR_EXIT_};
	
	s_setup_osd.ui_osd_type = HOST_OSD_TYPE_ENC;
	s_setup_osd.b_enc_need_update = FALSE;

	ab_osd_clear(&gs_osd_enc_buf, COLOR_ARGB8888_TRANSPARENT, 0, 0, gs_osd_enc_buf.ui_width, gs_osd_enc_buf.ui_height);

	// draw BG
	OSDLib_DrawMenuBG(gs_osd_enc_buf, s_setup_osd.ui_enc_resolution);

	// draw hdmi menu
	OSDLib_DrawHdmiMenu(gs_osd_enc_buf, HOSTMI_HDMI_LEAVE, s_setup_osd.ui_enc_resolution);
	
	// draw leave item
	OSDLib_Draw2ndMenu(gs_osd_enc_buf, ui_str_id, wIndex, HOSTMI_HDMI_LEAVE_TOTAL, s_setup_osd.ui_enc_resolution);

	s_setup_osd.b_enc_need_update = TRUE;
}


void host_setup_menu_enc_osd_Audio_draw(uint16_t wIndex, MENU_PARAM eMenuParam, PMENU_STATUS pStatus)
{
	s_setup_osd.ui_osd_type = HOST_OSD_TYPE_ENC;
	s_setup_osd.b_enc_need_update = FALSE;
	
	ab_osd_clear(&gs_osd_enc_buf, COLOR_ARGB8888_TRANSPARENT, 0, 0, gs_osd_enc_buf.ui_width, gs_osd_enc_buf.ui_height);

	// draw BG
	OSDLib_DrawMenuBG(gs_osd_enc_buf, s_setup_osd.ui_enc_resolution);

	// draw main menu
	OSDLib_DrawMainMenu(gs_osd_enc_buf, SETUPMENU_AUDIO, s_setup_osd.ui_enc_resolution);

	// draw audio bar
	OSDLib_DrawMenuAdjustValue(gs_osd_enc_buf, wIndex, HOSTMI_AUDIO_OSD_START_VAL, HOSTMI_AUDIO_TOTAL-1, s_setup_osd.ui_enc_resolution);

	s_setup_osd.b_enc_need_update = TRUE;
}

void host_setup_menu_enc_osd_Pir_draw(uint16_t wIndex, MENU_PARAM eMenuParam, PMENU_STATUS pStatus)
{
	uint32_t ui_str_id[HOSTMI_PIR_TOTAL] = {_STR_HIGH_, _STR_MIDDLE_, _STR_LOW_};

	s_setup_osd.ui_osd_type = HOST_OSD_TYPE_ENC;
	s_setup_osd.b_enc_need_update = FALSE;
	
	ab_osd_clear(&gs_osd_enc_buf, COLOR_ARGB8888_TRANSPARENT, 0, 0, gs_osd_enc_buf.ui_width, gs_osd_enc_buf.ui_height);

	// draw BG
	OSDLib_DrawMenuBG(gs_osd_enc_buf, s_setup_osd.ui_enc_resolution);

	// draw main menu
	OSDLib_DrawMainMenu(gs_osd_enc_buf, SETUPMENU_PIR, s_setup_osd.ui_enc_resolution);

	// draw pir item
	OSDLib_Draw2ndMenu(gs_osd_enc_buf, ui_str_id, wIndex, HOSTMI_PIR_TOTAL, s_setup_osd.ui_enc_resolution);

	s_setup_osd.b_enc_need_update = TRUE;
}

void host_setup_menu_enc_osd_Time_draw(uint16_t wIndex, MENU_PARAM eMenuParam, PMENU_STATUS pStatus)
{
	uint32_t ui_str_id[HOSTMI_TIME_TOTAL] = {_STR_DISPLAY_, _STR_TIME_SETUP_};

	s_setup_osd.ui_osd_type = HOST_OSD_TYPE_ENC;
	s_setup_osd.b_enc_need_update = FALSE;
	
	ab_osd_clear(&gs_osd_enc_buf, COLOR_ARGB8888_TRANSPARENT, 0, 0, gs_osd_enc_buf.ui_width, gs_osd_enc_buf.ui_height);

	// draw BG
	OSDLib_DrawMenuBG(gs_osd_enc_buf, s_setup_osd.ui_enc_resolution);

	// draw main menu
	OSDLib_DrawMainMenu(gs_osd_enc_buf, SETUPMENU_TIME, s_setup_osd.ui_enc_resolution);

	// draw time item
	OSDLib_Draw2ndMenu(gs_osd_enc_buf, ui_str_id, wIndex, HOSTMI_TIME_TOTAL, s_setup_osd.ui_enc_resolution);

	s_setup_osd.b_enc_need_update = TRUE;
}

void host_setup_menu_enc_osd_Time_Display_draw(uint16_t wIndex, MENU_PARAM eMenuParam, PMENU_STATUS pStatus)
{
	uint32_t ui_str_id_2nd[HOSTMI_TIME_TOTAL] = {_STR_DISPLAY_, _STR_TIME_SETUP_};
	uint32_t ui_str_id_3rd[HOSTMI_TIME_DISPLAY_TOTAL] = {_STR_TIME_, _STR_TIME_DATE_, _STR_TIME_OFF_};
	
	s_setup_osd.ui_osd_type = HOST_OSD_TYPE_ENC;
	s_setup_osd.b_enc_need_update = FALSE;
	
	ab_osd_clear(&gs_osd_enc_buf, COLOR_ARGB8888_TRANSPARENT, 0, 0, gs_osd_enc_buf.ui_width, gs_osd_enc_buf.ui_height);

	// draw BG
	OSDLib_DrawMenuBG(gs_osd_enc_buf, s_setup_osd.ui_enc_resolution);

	// draw main menu
	OSDLib_DrawMainMenu(gs_osd_enc_buf, SETUPMENU_TIME, s_setup_osd.ui_enc_resolution);

	// draw time item
	OSDLib_Draw2ndMenu(gs_osd_enc_buf, ui_str_id_2nd, HOSTMI_TIME_DISPLAY, HOSTMI_TIME_TOTAL, s_setup_osd.ui_enc_resolution);

	// draw time display item
	OSDLib_Draw3rdMenu(gs_osd_enc_buf, ui_str_id_3rd, wIndex, HOSTMI_TIME_DISPLAY_TOTAL, s_setup_osd.ui_enc_resolution);

	s_setup_osd.b_enc_need_update = TRUE;
}

void host_setup_menu_enc_osd_Time_Timesetup_draw(uint16_t wIndex, MENU_PARAM eMenuParam, PMENU_STATUS pStatus)
{
	uint32_t ui_str_id_2nd[HOSTMI_TIME_TOTAL] = {_STR_DISPLAY_, _STR_TIME_SETUP_};
	uint32_t ui_str_id_3rd[HOSTMI_TIME_TIMESETUP_TOTAL] = {_STR_AUTO_, _STR_MANUAL_};
	
	s_setup_osd.ui_osd_type = HOST_OSD_TYPE_ENC;
	s_setup_osd.b_enc_need_update = FALSE;
	
	ab_osd_clear(&gs_osd_enc_buf, COLOR_ARGB8888_TRANSPARENT, 0, 0, gs_osd_enc_buf.ui_width, gs_osd_enc_buf.ui_height);

	// draw BG
	OSDLib_DrawMenuBG(gs_osd_enc_buf, s_setup_osd.ui_enc_resolution);

	// draw main menu
	OSDLib_DrawMainMenu(gs_osd_enc_buf, SETUPMENU_TIME, s_setup_osd.ui_enc_resolution);

	// draw time item
	OSDLib_Draw2ndMenu(gs_osd_enc_buf, ui_str_id_2nd, HOSTMI_TIME_TIMESETUP, HOSTMI_TIME_TOTAL, s_setup_osd.ui_enc_resolution);

	// draw time display item
	OSDLib_Draw3rdMenu(gs_osd_enc_buf, ui_str_id_3rd, wIndex, HOSTMI_TIME_TIMESETUP_TOTAL, s_setup_osd.ui_enc_resolution);

	s_setup_osd.b_enc_need_update = TRUE;
}

void host_setup_menu_enc_osd_Time_Timesetup_Munual_draw(uint16_t wIndex, MENU_PARAM eMenuParam, PMENU_STATUS pStatus)
{
	uint32_t ui_str_id[HOSTMI_TIME_TIMESETUP_MANUAL_TOTAL] = {_STR_TIME_, _STR_DATE_};
	
	s_setup_osd.ui_osd_type = HOST_OSD_TYPE_ENC;
	s_setup_osd.b_enc_need_update = FALSE;
	
	ab_osd_clear(&gs_osd_enc_buf, COLOR_ARGB8888_TRANSPARENT, 0, 0, gs_osd_enc_buf.ui_width, gs_osd_enc_buf.ui_height);

	// draw BG
	OSDLib_DrawMenuBG(gs_osd_enc_buf, s_setup_osd.ui_enc_resolution);

	// draw time timesetup munual menu
	OSDLib_Draw1stMenu(gs_osd_enc_buf, _STR_TIME_OPTION_, ui_str_id, wIndex, HOSTMI_TIME_TIMESETUP_MANUAL_TOTAL, s_setup_osd.ui_enc_resolution);

	s_setup_osd.b_enc_need_update = TRUE;
}

void host_setup_menu_enc_osd_Time_Timesetup_Munual_Time_draw(uint16_t wIndex, MENU_PARAM eMenuParam, PMENU_STATUS pStatus)
{
	uint32_t ui_str_id[HOSTMI_TIME_TIMESETUP_MANUAL_TOTAL] = {_STR_TIME_, _STR_DATE_};
	
	s_setup_osd.ui_osd_type = HOST_OSD_TYPE_ENC;
	s_setup_osd.b_enc_need_update = FALSE;
	
	ab_osd_clear(&gs_osd_enc_buf, COLOR_ARGB8888_TRANSPARENT, 0, 0, gs_osd_enc_buf.ui_width, gs_osd_enc_buf.ui_height);

	// draw BG
	OSDLib_DrawMenuBG(gs_osd_enc_buf, s_setup_osd.ui_enc_resolution);

	// draw time timesetup munual menu
	OSDLib_Draw1stMenu(gs_osd_enc_buf, _STR_TIME_OPTION_, ui_str_id, HOSTMI_TIME_TIMESETUP_MANUAL_TIME, HOSTMI_TIME_TIMESETUP_MANUAL_TOTAL, s_setup_osd.ui_enc_resolution);

	// draw munual time 
	OSDLib_DrawTimeMunualMenu(gs_osd_enc_buf, wIndex, s_setup_osd.ui_enc_resolution);

	s_setup_osd.b_enc_need_update = TRUE;
}

void host_setup_menu_enc_osd_Time_Timesetup_Munual_Date_draw(uint16_t wIndex, MENU_PARAM eMenuParam, PMENU_STATUS pStatus)
{
	uint32_t ui_str_id[HOSTMI_TIME_TIMESETUP_MANUAL_TOTAL] = {_STR_TIME_, _STR_DATE_};
	
	s_setup_osd.ui_osd_type = HOST_OSD_TYPE_ENC;
	s_setup_osd.b_enc_need_update = FALSE;
	
	ab_osd_clear(&gs_osd_enc_buf, COLOR_ARGB8888_TRANSPARENT, 0, 0, gs_osd_enc_buf.ui_width, gs_osd_enc_buf.ui_height);

	// draw BG
	OSDLib_DrawMenuBG(gs_osd_enc_buf, s_setup_osd.ui_enc_resolution);

	// draw time timesetup munual menu
	OSDLib_Draw1stMenu(gs_osd_enc_buf, _STR_TIME_OPTION_, ui_str_id, HOSTMI_TIME_TIMESETUP_MANUAL_DATE, HOSTMI_TIME_TIMESETUP_MANUAL_TOTAL, s_setup_osd.ui_enc_resolution);

	// draw munual date
	OSDLib_DrawTimeMunualMenu(gs_osd_enc_buf, wIndex, s_setup_osd.ui_enc_resolution);

	s_setup_osd.b_enc_need_update = TRUE;
}

void host_setup_menu_enc_osd_Language_draw(uint16_t wIndex, MENU_PARAM eMenuParam, PMENU_STATUS pStatus)
{
	s_setup_osd.ui_osd_type = HOST_OSD_TYPE_ENC;	
	s_setup_osd.b_enc_need_update = FALSE;
	
	ab_osd_clear(&gs_osd_enc_buf, COLOR_ARGB8888_TRANSPARENT, 0, 0, gs_osd_enc_buf.ui_width, gs_osd_enc_buf.ui_height);

	// draw BG
	OSDLib_DrawMenuBG(gs_osd_enc_buf, s_setup_osd.ui_enc_resolution);

	// draw main menu
	host_setup_set_language(wIndex);
	OSDLib_DrawMainMenu(gs_osd_enc_buf, SETUPMENU_LANGUAGE, s_setup_osd.ui_enc_resolution);

	// draw language item
	OSDLib_DrawLanguageMenu(gs_osd_enc_buf, wIndex, s_setup_osd.ui_enc_resolution);

	s_setup_osd.b_enc_need_update = TRUE;
}

void host_setup_menu_enc_osd_Ai_draw(uint16_t wIndex, MENU_PARAM eMenuParam, PMENU_STATUS pStatus)
{
	s_setup_osd.ui_osd_type = HOST_OSD_TYPE_ENC;
	s_setup_osd.b_enc_need_update = FALSE;
	
	ab_osd_clear(&gs_osd_enc_buf, COLOR_ARGB8888_TRANSPARENT, 0, 0, gs_osd_enc_buf.ui_width, gs_osd_enc_buf.ui_height);

	// draw BG
	OSDLib_DrawMenuBG(gs_osd_enc_buf, s_setup_osd.ui_enc_resolution);

	// draw main menu
	OSDLib_DrawMainMenu(gs_osd_enc_buf, SETUPMENU_AI, s_setup_osd.ui_enc_resolution);

	// draw audio bar
	OSDLib_DrawMenuAdjustValue(gs_osd_enc_buf, wIndex, HOSTMI_AI_OSD_START_VAL, HOSTMI_AI_TOTAL-1, s_setup_osd.ui_enc_resolution);

	s_setup_osd.b_enc_need_update = TRUE;
}

void host_setup_menu_enc_osd_StitchAlg_draw(uint16_t wIndex, MENU_PARAM eMenuParam, PMENU_STATUS pStatus)
{
	uint32_t ui_str_id[HOSTMI_STITCHALG_TOTAL] = {_STR_RESTORE_FACTORY_SETTINGS_, _STR_ACTIVE_};
	
	s_setup_osd.ui_osd_type = HOST_OSD_TYPE_ENC;
	s_setup_osd.b_enc_need_update = FALSE;
	
	ab_osd_clear(&gs_osd_enc_buf, COLOR_ARGB8888_TRANSPARENT, 0, 0, gs_osd_enc_buf.ui_width, gs_osd_enc_buf.ui_height);

	// draw BG
	OSDLib_DrawMenuBG(gs_osd_enc_buf, s_setup_osd.ui_enc_resolution);

	// draw main menu
	OSDLib_DrawMainMenu(gs_osd_enc_buf, SETUPMENU_STITCHALG, s_setup_osd.ui_enc_resolution);

	// draw StitchAlg item
	OSDLib_Draw2ndMenu(gs_osd_enc_buf, ui_str_id, wIndex, HOSTMI_STITCHALG_TOTAL, s_setup_osd.ui_enc_resolution);

	s_setup_osd.b_enc_need_update = TRUE;
}

void host_setup_menu_enc_osd_Check_draw(uint16_t wIndex, MENU_PARAM eMenuParam, PMENU_STATUS pStatus)
{
	uint32_t ui_str_id[HOSTMI_CHECK_TOTAL] = {_STR_SAVE_, _STR_RESTORE_, _STR_EXIT_};
	
	s_setup_osd.ui_osd_type = HOST_OSD_TYPE_ENC;
	s_setup_osd.b_enc_need_update = FALSE;
	
	ab_osd_clear(&gs_osd_enc_buf, COLOR_ARGB8888_TRANSPARENT, 0, 0, gs_osd_enc_buf.ui_width, gs_osd_enc_buf.ui_height);

	// draw BG
	OSDLib_DrawMenuBG(gs_osd_enc_buf, s_setup_osd.ui_enc_resolution);

	// draw main menu
	OSDLib_DrawMainMenu(gs_osd_enc_buf, SETUPMENU_CHECK, s_setup_osd.ui_enc_resolution);

	// draw check item
	OSDLib_Draw2ndMenu(gs_osd_enc_buf, ui_str_id, wIndex, HOSTMI_CHECK_TOTAL, s_setup_osd.ui_enc_resolution);

	s_setup_osd.b_enc_need_update = TRUE;
}

void host_setup_menu_enc_osd_StitchAlg_Check_draw(char* sz_text[], uint8_t ntotaltext, uint32_t ui_argb8888_color, uint32_t ui_argb8888_bg_color)
{	
	if(s_setup_osd.b_is_init == 0){
		printf("OSD Buffer not created!\n");
		return;
	}
	
	AB_OSD_RECT_ST s_dst_rect = {0};
	uint8_t i = 0;
	uint32_t ui_x = 0;
	uint32_t ui_base_w = 0;
	uint32_t ui_base_h = 0;	
	uint32_t ui_gap = 0;
	uint32_t ui_times = 30;
	uint32_t divisor = 10;
	uint32_t ui_font_size = 0;

	switch(s_setup_osd.ui_enc_resolution){
		case HOST_OSD_MENU_RESOLUTION_8K:
			ui_times = 60;
			break;
			
		default:
		case HOST_OSD_MENU_RESOLUTION_4K:
			ui_times = 30;
			break;
			
		case HOST_OSD_MENU_RESOLUTION_FULLHD:
			ui_times = 15;
			break;
			
		case HOST_OSD_MENU_RESOLUTION_HD:
			ui_times = 10;
			break;		
	}
	s_setup_osd.b_enc_need_update = FALSE;

	ui_font_size = s_setup_osd.ui_enc_font_size;
	ui_base_w = s_setup_osd.ui_enc_base_w;
	ui_base_h = s_setup_osd.ui_enc_base_h;
	ui_gap = ui_base_h/5;

	ui_x = ui_base_w/2;
	
	ab_osd_clear(&gs_osd_enc_buf, COLOR_ARGB8888_TRANSPARENT, 0, 0, gs_osd_enc_buf.ui_width, gs_osd_enc_buf.ui_height);
	
	if(ntotaltext>5)ntotaltext = 5;
	for(i=0; i<ntotaltext; i++){
		ab_osd_draw_text(&gs_osd_enc_buf, &s_dst_rect, ui_argb8888_color, ui_argb8888_bg_color, ui_font_size+10, sz_text[i], ui_x*ui_times/divisor, (ui_gap/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_CENTER);
	}

	s_setup_osd.b_enc_need_update = TRUE;
}

void host_setup_menu_enc_osd_Clear_draw(void)
{
	s_setup_osd.b_enc_need_clear = FALSE;
	
	ab_osd_clear(&gs_osd_enc_buf, COLOR_ARGB8888_TRANSPARENT, 0, 0, gs_osd_enc_buf.ui_width, gs_osd_enc_buf.ui_height);

	s_setup_osd.b_enc_need_clear = TRUE;
}

#endif
#endif

#if 1
void OSDLib_DrawFactoryMenuBG(AB_OSD_BUF_ST s_dst_buf, HOST_OSD_MENU_RESOLUTION nResolution)
{
	AB_OSD_RECT_ST s_dst_rect1 = {0};
	AB_OSD_RECT_ST s_dst_rect2 = {0};
	uint32_t ui_w = 0;
	uint32_t ui_h = 0;
	uint32_t ui_base_w = 0;
	uint32_t ui_base_h = 0;	
	uint32_t ui_gap = 0;
	uint32_t ui_times = 30;
	uint32_t divisor = 10;
	uint32_t ui_font_size = 30;
	
	char SW_ver[30];
	char FW_ver[30];
	uint32_t ui_formal_version = 0, ui_major_version= 0, ui_minor_revision= 0;
	uint32_t ui_ai_formal_version= 0, ui_ai_major_version= 0, ui_ai_minor_revision= 0;

	switch(nResolution){
		case HOST_OSD_MENU_RESOLUTION_8K:
			ui_times = 60;
			break;
			
		default:
		case HOST_OSD_MENU_RESOLUTION_4K:
			ui_times = 30;
			break;
			
		case HOST_OSD_MENU_RESOLUTION_FULLHD:
			ui_times = 15;
			break;
			
		case HOST_OSD_MENU_RESOLUTION_HD:
			ui_font_size = 12;
			ui_times = 10;
			break;		
	}

	if(s_setup_osd.ui_osd_type == HOST_OSD_TYPE_ENC){
		ui_w = s_setup_osd.ui_enc_w;
		ui_h = s_setup_osd.ui_enc_h;
		ui_font_size = s_setup_osd.ui_enc_font_size;
		ui_base_w = s_setup_osd.ui_enc_base_w/2;
		ui_base_h = s_setup_osd.ui_enc_base_h;
		ui_gap = ui_base_h/12;

	}else{
		ui_w = s_setup_osd.ui_display_w;
		ui_h = s_setup_osd.ui_display_h;
		ui_font_size = s_setup_osd.ui_display_font_size;
		ui_base_w = s_setup_osd.ui_display_base_w/2;
		ui_base_h = s_setup_osd.ui_display_base_h;
		ui_gap = ui_base_h/12;
	}

	// Draw Menu
	ab_osd_draw_rectangle(&s_dst_buf, COLOR_ARGB8888_DARKGRAY, 0, 0, ui_w, ui_gap*ui_times/divisor);
	ab_osd_draw_rectangle(&s_dst_buf, COLOR_ARGB8888_MEDIUMGRAY, 0, ui_gap*ui_times/divisor, ui_w, ui_h-ui_gap*ui_times/divisor);

	ab_osd_draw_rectangle(&s_dst_buf, COLOR_ARGB8888_PICKGRAY, (ui_base_w-1)*ui_times/divisor, ui_gap*3*ui_times/divisor, 2*ui_times/divisor, (ui_base_h-ui_gap*3)*ui_times/divisor);

	ab_osd_draw_rectangle(&s_dst_buf, COLOR_ARGB8888_PICKGRAY, 0, (ui_gap*3-2)*ui_times/divisor, ui_w, 2*ui_times/divisor);


	//information	
	host_get_version(&ui_formal_version, &ui_major_version, &ui_minor_revision);
	host_get_ai_version(&ui_ai_formal_version, &ui_ai_major_version, &ui_ai_minor_revision);
	snprintf(SW_ver, sizeof(SW_ver), "%s%02d%s%03d%s%03d", ": ", ui_ai_formal_version, ".", ui_ai_major_version, ".", ui_ai_minor_revision);
	snprintf(FW_ver, sizeof(FW_ver), "%s%02d%s%03d%s%03d", ": ", ui_formal_version, ".", ui_major_version, ".", ui_minor_revision);
	
	host_osd_draw_text_utf8(&s_dst_buf, &s_dst_rect1, COLOR_ARGB8888_NAVY, COLOR_ARGB8888_MEDIUMGRAY, ui_font_size, _STR_SOFTWARE_VERSION_, (ui_base_w-5)*ui_times/divisor, (ui_gap*3/2+0*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_RIGHT);
	ab_osd_draw_text(&s_dst_buf, &s_dst_rect2, COLOR_ARGB8888_NAVY, COLOR_ARGB8888_MEDIUMGRAY, ui_font_size, SW_ver, ui_base_w*ui_times/divisor, (ui_gap*3/2+0*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
	host_osd_draw_text_utf8(&s_dst_buf, &s_dst_rect1, COLOR_ARGB8888_NAVY, COLOR_ARGB8888_MEDIUMGRAY, ui_font_size, _STR_FIRMWARE_VERSION_, (ui_base_w-5)*ui_times/divisor, (ui_gap*3/2+1*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_RIGHT);
	ab_osd_draw_text(&s_dst_buf, &s_dst_rect2, COLOR_ARGB8888_NAVY, COLOR_ARGB8888_MEDIUMGRAY, ui_font_size, FW_ver, ui_base_w*ui_times/divisor, (ui_gap*3/2+1*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
}

void OSDLib_DrawFactoryMenu2pageBG(AB_OSD_BUF_ST s_dst_buf, HOST_OSD_MENU_RESOLUTION nResolution)
{
	uint32_t ui_w = 0;
	uint32_t ui_h = 0;
	uint32_t ui_base_w = 0;
	uint32_t ui_base_h = 0;	
	uint32_t ui_gap = 0;
	uint32_t ui_times = 30;
	uint32_t divisor = 10;

	switch(nResolution){
		case HOST_OSD_MENU_RESOLUTION_8K:
			ui_times = 60;
			break;
			
		default:
		case HOST_OSD_MENU_RESOLUTION_4K:
			ui_times = 30;
			break;
			
		case HOST_OSD_MENU_RESOLUTION_FULLHD:
			ui_times = 15;
			break;
			
		case HOST_OSD_MENU_RESOLUTION_HD:
			ui_times = 10;
			break;		
	}

	if(s_setup_osd.ui_osd_type == HOST_OSD_TYPE_ENC){
		ui_w = s_setup_osd.ui_enc_w;
		ui_h = s_setup_osd.ui_enc_h;
		ui_base_w = s_setup_osd.ui_enc_base_w/2;
		ui_base_h = s_setup_osd.ui_enc_base_h;
		ui_gap = ui_base_h/12;

	}else{
		ui_w = s_setup_osd.ui_display_w;
		ui_h = s_setup_osd.ui_display_h;
		ui_base_w = s_setup_osd.ui_display_base_w/2;
		ui_base_h = s_setup_osd.ui_display_base_h;
		ui_gap = ui_base_h/12;
	}

	// Draw Menu
	ab_osd_draw_rectangle(&s_dst_buf, COLOR_ARGB8888_DARKGRAY, 0, 0, ui_w, ui_gap*ui_times/divisor);
	ab_osd_draw_rectangle(&s_dst_buf, COLOR_ARGB8888_MEDIUMGRAY, 0, ui_gap*ui_times/divisor, ui_w, ui_h-ui_gap*ui_times/divisor);

	ab_osd_draw_rectangle(&s_dst_buf, COLOR_ARGB8888_PICKGRAY, (ui_base_w-1)*ui_times/divisor, ui_gap*ui_times/divisor, 2*ui_times/divisor, (ui_base_h-ui_gap)*ui_times/divisor);
}

void OSDLib_DrawFactoryMainMenu(AB_OSD_BUF_ST s_dst_buf, uint16_t wIndex, HOST_OSD_MENU_RESOLUTION nResolution)
{
	uint32_t i=0;
	AB_OSD_RECT_ST s_dst_rect = {0};
	uint32_t ui_times = 30;
	uint32_t divisor = 10;
	uint32_t ui_base_w = 0;
	uint32_t ui_base_h = 0; 
	uint32_t ui_base_title_w = 0;
	uint32_t ui_gap = 0;
	uint32_t ui_font_size = 14;
	uint32_t ui_str_id[SETUP_FACTORY_MENU_TOTAL] = {_STR_DEMO_MODE_, _STR_IMAGE_SOURCE_, _STR_SAVE_STREAM_, _STR_ADVANCE_IMAGE_, _STR_BURN_IN_MODE_, _STR_RESTORE_FACTORY_SETTING_};
	
	switch(nResolution){
		case HOST_OSD_MENU_RESOLUTION_8K:
			ui_times = 60;
			break;
			
		case HOST_OSD_MENU_RESOLUTION_4K:
			ui_times = 30;
			break;
			
		default:
		case HOST_OSD_MENU_RESOLUTION_FULLHD:
			ui_times = 15;
			break;
			
		case HOST_OSD_MENU_RESOLUTION_HD:
			ui_times = 10;
			break;		
	}

	if(s_setup_osd.ui_osd_type == HOST_OSD_TYPE_ENC){
		ui_font_size  = s_setup_osd.ui_enc_font_size;
		ui_font_size = s_setup_osd.ui_enc_font_size;
		ui_base_title_w = s_setup_osd.ui_enc_base_w;
		ui_base_w = s_setup_osd.ui_enc_base_w/2;
		ui_base_h = s_setup_osd.ui_enc_base_h;
		ui_gap = ui_base_h/12;
	}else{
		ui_font_size  = s_setup_osd.ui_display_font_size;	
		ui_font_size = s_setup_osd.ui_display_font_size;
		ui_base_title_w = s_setup_osd.ui_display_base_w;
		ui_base_w = s_setup_osd.ui_display_base_w/2;
		ui_base_h = s_setup_osd.ui_display_base_h;
		ui_gap = ui_base_h/12;
	}

	//title
	host_osd_draw_text_utf8(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_WHITE, COLOR_ARGB8888_DARKGRAY, ui_font_size, _STR_FACTORY_MENU_, (ui_base_title_w/2)*ui_times/divisor, (ui_gap/2+2)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_CENTER); //14->(ui_gap/2+2)

	// item
#ifdef ENABLE_RESTORE_OLD_DATA
	if(ui_factory_menu_item == HOST_OSD_FACTORY_MENU_MAIN){
		ab_osd_draw_rectangle(&s_dst_buf, COLOR_ARGB8888_LIGHTGRAY, 0, ui_gap*3*ui_times/divisor, (ui_base_w-1)*ui_times/divisor, (ui_base_h-ui_gap*3)*ui_times/divisor);
		for(i=0; i<SETUP_FACTORY_MENU_TOTAL; i++){
			if(s_setup_osd.ui_factory_main_disabled[i] == 1){
				host_osd_draw_text_utf8(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_DISABLE, COLOR_ARGB8888_LIGHTGRAY, ui_font_size, ui_str_id[i], 10*ui_times/divisor, (ui_gap*7/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
			}else{
				if(i==(wIndex%SETUP_FACTORY_MENU_TOTAL)){
					ab_osd_draw_rectangle(&s_dst_buf, COLOR_ARGB8888_PICKGRAY, 0, (ui_gap*3+i*ui_gap)*ui_times/divisor, (ui_base_w-1)*ui_times/divisor, ui_gap*ui_times/divisor);
					if(i == SETUP_FACTORY_MENU_RESTORE_OLD_DATA){
						ab_osd_draw_text(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_WHITE, COLOR_ARGB8888_PICKGRAY, ui_font_size, "Restore Old Data", 10*ui_times/divisor, (ui_gap*7/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
					}else if(i == SETUP_FACTORY_MENU_BACKUP){
						ab_osd_draw_text(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_WHITE, COLOR_ARGB8888_PICKGRAY, ui_font_size, "Back Up", 10*ui_times/divisor, (ui_gap*7/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
					}else{
						host_osd_draw_text_utf8(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_WHITE, COLOR_ARGB8888_PICKGRAY, ui_font_size, ui_str_id[i], 10*ui_times/divisor, (ui_gap*7/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
					}
				}else{
					if(i == SETUP_FACTORY_MENU_RESTORE_OLD_DATA){
						ab_osd_draw_text(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_NAVY, COLOR_ARGB8888_LIGHTGRAY, ui_font_size, "Restore Old Data", 10*ui_times/divisor, (ui_gap*7/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
					}else if(i == SETUP_FACTORY_MENU_BACKUP){
						ab_osd_draw_text(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_NAVY, COLOR_ARGB8888_LIGHTGRAY, ui_font_size, "Back Up", 10*ui_times/divisor, (ui_gap*7/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
					}else{
						host_osd_draw_text_utf8(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_NAVY, COLOR_ARGB8888_LIGHTGRAY, ui_font_size, ui_str_id[i], 10*ui_times/divisor, (ui_gap*7/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
					}
				}
			}
		}
	}else{
		for(i=0; i<SETUP_FACTORY_MENU_TOTAL; i++){
			if(s_setup_osd.ui_factory_main_disabled[i] == 1){
				host_osd_draw_text_utf8(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_DISABLE, COLOR_ARGB8888_MEDIUMGRAY, ui_font_size, ui_str_id[i], 10*ui_times/divisor, (ui_gap*7/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
			}else{
				if(i==(wIndex%SETUP_FACTORY_MENU_TOTAL)){
					ab_osd_draw_rectangle(&s_dst_buf, COLOR_ARGB8888_PICKGRAY, 0, (ui_gap*3+i*ui_gap)*ui_times/divisor, (ui_base_w-1)*ui_times/divisor, ui_gap*ui_times/divisor);
					if(i == SETUP_FACTORY_MENU_RESTORE_OLD_DATA){
						ab_osd_draw_text(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_WHITE, COLOR_ARGB8888_PICKGRAY, ui_font_size, "Restore Old Data", 10*ui_times/divisor, (ui_gap*7/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
					}else if(i == SETUP_FACTORY_MENU_BACKUP){
						ab_osd_draw_text(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_WHITE, COLOR_ARGB8888_PICKGRAY, ui_font_size, "Back Up", 10*ui_times/divisor, (ui_gap*7/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
					}else{
						host_osd_draw_text_utf8(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_WHITE, COLOR_ARGB8888_PICKGRAY, ui_font_size, ui_str_id[i], 10*ui_times/divisor, (ui_gap*7/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
					}
				}else{
					if(i == SETUP_FACTORY_MENU_RESTORE_OLD_DATA){
						ab_osd_draw_text(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_NAVY, COLOR_ARGB8888_MEDIUMGRAY, ui_font_size, "Restore Old Data", 10*ui_times/divisor, (ui_gap*7/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
					}else if(i == SETUP_FACTORY_MENU_BACKUP){
						ab_osd_draw_text(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_NAVY, COLOR_ARGB8888_MEDIUMGRAY, ui_font_size, "Back Up", 10*ui_times/divisor, (ui_gap*7/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
					}else{
						host_osd_draw_text_utf8(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_NAVY, COLOR_ARGB8888_MEDIUMGRAY, ui_font_size, ui_str_id[i], 10*ui_times/divisor, (ui_gap*7/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
					}
				}
			}
		}
	}
#else
	if(ui_factory_menu_item == HOST_OSD_FACTORY_MENU_MAIN){
		ab_osd_draw_rectangle(&s_dst_buf, COLOR_ARGB8888_LIGHTGRAY, 0, ui_gap*3*ui_times/divisor, (ui_base_w-1)*ui_times/divisor, (ui_base_h-ui_gap*3)*ui_times/divisor);
		for(i=0; i<SETUP_FACTORY_MENU_TOTAL; i++){
			if(s_setup_osd.ui_factory_main_disabled[i] == 1){
				host_osd_draw_text_utf8(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_DISABLE, COLOR_ARGB8888_LIGHTGRAY, ui_font_size, ui_str_id[i], 10*ui_times/divisor, (ui_gap*7/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
			}else{
				if(i==(wIndex%SETUP_FACTORY_MENU_TOTAL)){
					ab_osd_draw_rectangle(&s_dst_buf, COLOR_ARGB8888_PICKGRAY, 0, (ui_gap*3+i*ui_gap)*ui_times/divisor, (ui_base_w-1)*ui_times/divisor, ui_gap*ui_times/divisor);
					host_osd_draw_text_utf8(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_WHITE, COLOR_ARGB8888_PICKGRAY, ui_font_size, ui_str_id[i], 10*ui_times/divisor, (ui_gap*7/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
				}else{
					host_osd_draw_text_utf8(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_NAVY, COLOR_ARGB8888_LIGHTGRAY, ui_font_size, ui_str_id[i], 10*ui_times/divisor, (ui_gap*7/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
				}
			}
		}
	}else{
		for(i=0; i<SETUP_FACTORY_MENU_TOTAL; i++){
			if(s_setup_osd.ui_factory_main_disabled[i] == 1){
				host_osd_draw_text_utf8(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_DISABLE, COLOR_ARGB8888_MEDIUMGRAY, ui_font_size, ui_str_id[i], 10*ui_times/divisor, (ui_gap*7/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
			}else{
				if(i==(wIndex%SETUP_FACTORY_MENU_TOTAL)){
					ab_osd_draw_rectangle(&s_dst_buf, COLOR_ARGB8888_PICKGRAY, 0, (ui_gap*3+i*ui_gap)*ui_times/divisor, (ui_base_w-1)*ui_times/divisor, ui_gap*ui_times/divisor);
					host_osd_draw_text_utf8(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_WHITE, COLOR_ARGB8888_PICKGRAY, ui_font_size, ui_str_id[i], 10*ui_times/divisor, (ui_gap*7/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
				}else{
					host_osd_draw_text_utf8(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_NAVY, COLOR_ARGB8888_MEDIUMGRAY, ui_font_size, ui_str_id[i], 10*ui_times/divisor, (ui_gap*7/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
				}
			}
		}
	}
#endif
}

void OSDLib_DrawFactoryAdImageMenu(AB_OSD_BUF_ST s_dst_buf, uint16_t wIndex, HOST_OSD_MENU_RESOLUTION nResolution)
{
	uint32_t i=0;
	AB_OSD_RECT_ST s_dst_rect = {0};
	uint32_t ui_times = 30;
	uint32_t divisor = 10;
	uint32_t ui_base_w = 0;
	uint32_t ui_base_h = 0; 
	uint32_t ui_base_title_w = 0;
	uint32_t ui_gap = 0;
	uint32_t ui_font_size = 14;
	uint32_t ui_str_id[HOSTMI_ADIMAGE_TOTAL] = {_STR_SHUTTER_, _STR_AGC_, _STR_BACKLIGHT_, _STR_COLOR_SHIFT_, _STR_NOISE_REDUCTION_};
	
	switch(nResolution){
		case HOST_OSD_MENU_RESOLUTION_8K:
			ui_times = 60;
			break;
			
		default:
		case HOST_OSD_MENU_RESOLUTION_4K:
			ui_times = 30;
			break;
			
		case HOST_OSD_MENU_RESOLUTION_FULLHD:
			ui_times = 15;
			break;
			
		case HOST_OSD_MENU_RESOLUTION_HD:
			ui_times = 10;
			break;		
	}
	
	if(s_setup_osd.ui_osd_type == HOST_OSD_TYPE_ENC){
		ui_font_size  = s_setup_osd.ui_enc_font_size;
		ui_font_size = s_setup_osd.ui_enc_font_size;
		ui_base_title_w = s_setup_osd.ui_enc_base_w;
		ui_base_w = s_setup_osd.ui_enc_base_w/2;
		ui_base_h = s_setup_osd.ui_enc_base_h;
		ui_gap = ui_base_h/12;
	}else{
		ui_font_size  = s_setup_osd.ui_display_font_size;	
		ui_font_size = s_setup_osd.ui_display_font_size;
		ui_base_title_w = s_setup_osd.ui_display_base_w;
		ui_base_w = s_setup_osd.ui_display_base_w/2;
		ui_base_h = s_setup_osd.ui_display_base_h;
		ui_gap = ui_base_h/12;
	}

	//title
	host_osd_draw_text_utf8(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_WHITE, COLOR_ARGB8888_DARKGRAY, ui_font_size, _STR_ADVANCE_IMAGE_, (ui_base_title_w/2)*ui_times/divisor, (ui_gap/2+2)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_CENTER); //14->(ui_gap/2+2)

	// item
	if(ui_factory_menu_item == HOST_OSD_FACTORY_MENU_ADIMAGE){
		ab_osd_draw_rectangle(&s_dst_buf, COLOR_ARGB8888_LIGHTGRAY, 0, ui_gap*ui_times/divisor, (ui_base_w-1)*ui_times/divisor, (ui_base_h-ui_gap)*ui_times/divisor);
		for(i=0; i<HOSTMI_ADIMAGE_TOTAL; i++){
			if(s_setup_osd.ui_adimage_disabled[i] == 1){
				host_osd_draw_text_utf8(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_DISABLE, COLOR_ARGB8888_LIGHTGRAY, ui_font_size, ui_str_id[i], 10*ui_times/divisor, (ui_gap*3/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
			}else{
				if(i==(wIndex%HOSTMI_ADIMAGE_TOTAL)){
					ab_osd_draw_rectangle(&s_dst_buf, COLOR_ARGB8888_PICKGRAY, 0, (ui_gap+i*ui_gap)*ui_times/divisor, (ui_base_w-1)*ui_times/divisor, ui_gap*ui_times/divisor);
					host_osd_draw_text_utf8(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_WHITE, COLOR_ARGB8888_PICKGRAY, ui_font_size, ui_str_id[i], 10*ui_times/divisor, (ui_gap*3/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
				}else{
					host_osd_draw_text_utf8(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_NAVY, COLOR_ARGB8888_LIGHTGRAY, ui_font_size, ui_str_id[i], 10*ui_times/divisor, (ui_gap*3/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
				}
			}
		}
	}else{
		for(i=0; i<HOSTMI_ADIMAGE_TOTAL; i++){
			if(s_setup_osd.ui_adimage_disabled[i] == 1){
				host_osd_draw_text_utf8(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_DISABLE, COLOR_ARGB8888_MEDIUMGRAY, ui_font_size, ui_str_id[i], 10*ui_times/divisor, (ui_gap*3/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
			}else{
				if(i==(wIndex%HOSTMI_ADIMAGE_TOTAL)){
					ab_osd_draw_rectangle(&s_dst_buf, COLOR_ARGB8888_PICKGRAY, 0, (ui_gap+i*ui_gap)*ui_times/divisor, (ui_base_w-1)*ui_times/divisor, ui_gap*ui_times/divisor);
					host_osd_draw_text_utf8(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_WHITE, COLOR_ARGB8888_PICKGRAY, ui_font_size, ui_str_id[i], 10*ui_times/divisor, (ui_gap*3/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
				}else{
					host_osd_draw_text_utf8(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_NAVY, COLOR_ARGB8888_MEDIUMGRAY, ui_font_size, ui_str_id[i], 10*ui_times/divisor, (ui_gap*3/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
				}
			}
		}
	}
}

void OSDLib_Draw2ndFactoryMenu(AB_OSD_BUF_ST s_dst_buf, uint32_t ui_str_id[], uint16_t wIndex, uint16_t wTotalIndex, HOST_OSD_MENU_RESOLUTION nResolution)
{
	uint32_t i=0;
	AB_OSD_RECT_ST s_dst_rect = {0};
	uint32_t ui_base_x = 0;
	uint32_t ui_base_w = 0;
	uint32_t ui_base_h = 0;
	uint32_t ui_gap = 0;
	uint32_t ui_times = 30;
	uint32_t divisor = 10;
	uint32_t ui_font_size = 0;

	switch(nResolution){
		case HOST_OSD_MENU_RESOLUTION_8K:
			ui_times = 60;
			break;
			
		default:
		case HOST_OSD_MENU_RESOLUTION_4K:
			ui_times = 30;
			break;
			
		case HOST_OSD_MENU_RESOLUTION_FULLHD:
			ui_times = 15;
			break;
			
		case HOST_OSD_MENU_RESOLUTION_HD:
			ui_times = 10;
			break;		
	}
	
	if(s_setup_osd.ui_osd_type == HOST_OSD_TYPE_ENC){
		ui_font_size = s_setup_osd.ui_enc_font_size;
		ui_base_x = s_setup_osd.ui_enc_base_w/2;
		ui_base_w = s_setup_osd.ui_enc_base_w/2;
		ui_base_h = s_setup_osd.ui_enc_base_h;
		ui_gap = ui_base_h/12; 
	}else{
		ui_font_size = s_setup_osd.ui_display_font_size;
		ui_base_x = s_setup_osd.ui_display_base_w/2;
		ui_base_w = s_setup_osd.ui_display_base_w/2;
		ui_base_h = s_setup_osd.ui_display_base_h;
		ui_gap = ui_base_h/12;
	}

	ab_osd_draw_rectangle(&s_dst_buf, COLOR_ARGB8888_LIGHTGRAY, (ui_base_x+1)*ui_times/divisor, ui_gap*3*ui_times/divisor, (ui_base_w-1)*ui_times/divisor, (ui_base_h-ui_gap*3)*ui_times/divisor);
	if(nResolution == HOST_OSD_MENU_RESOLUTION_FULLHD) ab_osd_draw_rectangle(&s_dst_buf, COLOR_ARGB8888_LIGHTGRAY, 
	                                                   (ui_base_x+1)*ui_times/divisor+(ui_base_w-1)*ui_times/divisor, ui_gap*3*ui_times/divisor, 
	                                                   s_setup_osd.ui_enc_w-((ui_base_x+1)*ui_times/divisor+(ui_base_w-1)*ui_times/divisor), (ui_base_h-ui_gap*3)*ui_times/divisor);
	for(i=0; i<wTotalIndex; i++){
		if(i==(wIndex%wTotalIndex)){
			ab_osd_draw_rectangle(&s_dst_buf, COLOR_ARGB8888_PICKGRAY, (ui_base_x+1)*ui_times/divisor, (ui_gap*3+i*ui_gap)*ui_times/divisor, (ui_base_w-1)*ui_times/divisor, ui_gap*ui_times/divisor);
			if(nResolution == HOST_OSD_MENU_RESOLUTION_FULLHD) ab_osd_draw_rectangle(&s_dst_buf, COLOR_ARGB8888_PICKGRAY, 
	                                                           (ui_base_x+1)*ui_times/divisor+(ui_base_w-1)*ui_times/divisor, (ui_gap*3+i*ui_gap)*ui_times/divisor, 
	                                                           s_setup_osd.ui_enc_w-((ui_base_x+1)*ui_times/divisor+(ui_base_w-1)*ui_times/divisor), ui_gap*ui_times/divisor);
			host_osd_draw_text_utf8(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_WHITE, COLOR_ARGB8888_PICKGRAY, ui_font_size, ui_str_id[i], (ui_base_x+10)*ui_times/divisor, (ui_gap*7/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
		}else{
			host_osd_draw_text_utf8(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_NAVY, COLOR_ARGB8888_LIGHTGRAY, ui_font_size, ui_str_id[i], (ui_base_x+10)*ui_times/divisor, (ui_gap*7/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
		}
	}
}

void OSDLib_Draw2page2ndFactoryMenu(AB_OSD_BUF_ST s_dst_buf, uint32_t ui_str_id[], uint16_t wIndex, uint16_t wTotalIndex, HOST_OSD_MENU_RESOLUTION nResolution)
{
	uint32_t i=0;
	AB_OSD_RECT_ST s_dst_rect = {0};
	uint32_t ui_base_x = 0;
	uint32_t ui_base_w = 0;
	uint32_t ui_base_h = 0;
	uint32_t ui_gap = 0;
	uint32_t ui_times = 30;
	uint32_t divisor = 10;
	uint32_t ui_font_size = 0;

	switch(nResolution){
		case HOST_OSD_MENU_RESOLUTION_8K:
			ui_times = 60;
			break;
			
		default:
		case HOST_OSD_MENU_RESOLUTION_4K:
			ui_times = 30;
			break;
			
		case HOST_OSD_MENU_RESOLUTION_FULLHD:
			ui_times = 15;
			break;
			
		case HOST_OSD_MENU_RESOLUTION_HD:
			ui_times = 10;
			break;		
	}
	
	if(s_setup_osd.ui_osd_type == HOST_OSD_TYPE_ENC){
		ui_font_size = s_setup_osd.ui_enc_font_size;
		ui_base_x = s_setup_osd.ui_enc_base_w/2;
		ui_base_w = s_setup_osd.ui_enc_base_w/2;
		ui_base_h = s_setup_osd.ui_enc_base_h;
		ui_gap = ui_base_h/12;
	}else{
		ui_font_size = s_setup_osd.ui_display_font_size;
		ui_base_x = s_setup_osd.ui_display_base_w/2;
		ui_base_w = s_setup_osd.ui_display_base_w/2;
		ui_base_h = s_setup_osd.ui_display_base_h;
		ui_gap = ui_base_h/12;
	}

	ab_osd_draw_rectangle(&s_dst_buf, COLOR_ARGB8888_LIGHTGRAY, (ui_base_x+1)*ui_times/divisor, ui_gap*ui_times/divisor, (ui_base_w-1)*ui_times/divisor, (ui_base_h-ui_gap)*ui_times/divisor);
	if(nResolution == HOST_OSD_MENU_RESOLUTION_FULLHD) ab_osd_draw_rectangle(&s_dst_buf, COLOR_ARGB8888_LIGHTGRAY, 
	                                                   (ui_base_x+1)*ui_times/divisor+(ui_base_w-1)*ui_times/divisor, ui_gap*ui_times/divisor, 
	                                                   s_setup_osd.ui_enc_w-((ui_base_x+1)*ui_times/divisor+(ui_base_w-1)*ui_times/divisor), (ui_base_h-ui_gap)*ui_times/divisor);
	for(i=0; i<wTotalIndex; i++){
		if(i==(wIndex%wTotalIndex)){
			ab_osd_draw_rectangle(&s_dst_buf, COLOR_ARGB8888_PICKGRAY, (ui_base_x+1)*ui_times/divisor, (ui_gap+i*ui_gap)*ui_times/divisor, (ui_base_w-1)*ui_times/divisor, ui_gap*ui_times/divisor);
			if(nResolution == HOST_OSD_MENU_RESOLUTION_FULLHD) ab_osd_draw_rectangle(&s_dst_buf, COLOR_ARGB8888_PICKGRAY, 
	                                                           (ui_base_x+1)*ui_times/divisor+(ui_base_w-1)*ui_times/divisor, (ui_gap+i*ui_gap)*ui_times/divisor, 
	                                                           s_setup_osd.ui_enc_w-((ui_base_x+1)*ui_times/divisor+(ui_base_w-1)*ui_times/divisor), ui_gap*ui_times/divisor);
			host_osd_draw_text_utf8(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_WHITE, COLOR_ARGB8888_PICKGRAY, ui_font_size, ui_str_id[i], (ui_base_x+10)*ui_times/divisor, (ui_gap*3/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
		}else{
			host_osd_draw_text_utf8(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_NAVY, COLOR_ARGB8888_LIGHTGRAY, ui_font_size, ui_str_id[i], (ui_base_x+10)*ui_times/divisor, (ui_gap*3/2+i*ui_gap)*ui_times/divisor, AB_OSD_ALIGN_V_CENTER|AB_OSD_ALIGN_H_LEFT);
		}
	}
}

void OSDLib_DrawRestoreMenu(AB_OSD_BUF_ST s_dst_buf, HOST_OSD_MENU_RESOLUTION nResolution)
{
	AB_OSD_RECT_ST s_dst_rect = {0};
	uint32_t ui_base_w = 0;
	uint32_t ui_base_h = 0;
	uint32_t ui_gap = 0;
	uint32_t ui_times = 30;
	uint32_t divisor = 10;
	uint32_t ui_font_size = 0;
	uint32_t ui_str_id[3] = {_STR_RESTORE_FACTORY_SETTING_, _STR_RESTORE_WAN_1_, _STR_RESTORE_WAN_2_};

	switch(nResolution){
		case HOST_OSD_MENU_RESOLUTION_8K:
			ui_times = 60;
			break;
			
		default:
		case HOST_OSD_MENU_RESOLUTION_4K:
			ui_times = 30;
			break;
			
		case HOST_OSD_MENU_RESOLUTION_FULLHD:
			ui_times = 15;
			break;
			
		case HOST_OSD_MENU_RESOLUTION_HD:
			ui_times = 10;
			break;		
	}
	
	if(s_setup_osd.ui_osd_type == HOST_OSD_TYPE_ENC){
		ui_font_size = s_setup_osd.ui_enc_font_size;
		ui_base_w = s_setup_osd.ui_enc_base_w;
		ui_base_h = s_setup_osd.ui_enc_base_h;
		ui_gap = s_setup_osd.ui_enc_base_h/12;
	}else{
		ui_font_size = s_setup_osd.ui_display_font_size;
		ui_base_w = s_setup_osd.ui_display_base_w;
		ui_base_h = s_setup_osd.ui_display_base_h;
		ui_gap = s_setup_osd.ui_display_base_h/12;
	}

	// restore item
	ab_osd_draw_rectangle(&s_dst_buf, COLOR_ARGB8888_DARKGRAY, ui_base_w/8*ui_times/divisor, ui_base_h/6*ui_times/divisor, ui_base_w*6/8*ui_times/divisor, ui_base_h*4/6*ui_times/divisor);
	host_osd_draw_text_utf8(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_WHITE, COLOR_ARGB8888_DARKGRAY, ui_font_size, ui_str_id[0], (ui_base_w/2)*ui_times/divisor, ui_base_h/3*ui_times/divisor, AB_OSD_ALIGN_V_DOWN|AB_OSD_ALIGN_H_CENTER);
	host_osd_draw_text_utf8(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_WHITE, COLOR_ARGB8888_DARKGRAY, ui_font_size, ui_str_id[1], (ui_base_w/2)*ui_times/divisor, (ui_base_h/2+ui_gap*0)*ui_times/divisor, AB_OSD_ALIGN_V_TOP|AB_OSD_ALIGN_H_CENTER);
	host_osd_draw_text_utf8(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_WHITE, COLOR_ARGB8888_DARKGRAY, ui_font_size, ui_str_id[2], (ui_base_w/2)*ui_times/divisor, (ui_base_h/2+ui_gap*1)*ui_times/divisor, AB_OSD_ALIGN_V_TOP|AB_OSD_ALIGN_H_CENTER);
}

#if 1
void host_setup_factory_menu_display_osd_main_draw(uint16_t wIndex, MENU_PARAM eMenuParam, PMENU_STATUS pStatus)
{
	s_setup_osd.ui_osd_type = HOST_OSD_TYPE_DISPLAY;
	s_setup_osd.b_display_need_update = FALSE;
	
	ab_osd_clear(&gs_osd_display_buf, COLOR_ARGB8888_TRANSPARENT, 0, 0, gs_osd_display_buf.ui_width, gs_osd_display_buf.ui_height);

	// draw BG
	OSDLib_DrawFactoryMenuBG(gs_osd_display_buf, s_setup_osd.ui_display_resolution);

	// draw main menu
	OSDLib_DrawFactoryMainMenu(gs_osd_display_buf, wIndex, s_setup_osd.ui_display_resolution);
	
	s_setup_osd.b_display_need_update = TRUE;
}

void host_setup_factory_menu_display_osd_Demo_draw(uint16_t wIndex, MENU_PARAM eMenuParam, PMENU_STATUS pStatus)
{
	uint32_t ui_str_id[HOSTMI_DEMO_TOTAL] = {_STR_ON_, _STR_OFF_};
	
	s_setup_osd.ui_osd_type = HOST_OSD_TYPE_DISPLAY;
	s_setup_osd.b_display_need_update = FALSE;
	
	ab_osd_clear(&gs_osd_display_buf, COLOR_ARGB8888_TRANSPARENT, 0, 0, gs_osd_display_buf.ui_width, gs_osd_display_buf.ui_height);

	// draw BG
	OSDLib_DrawFactoryMenuBG(gs_osd_display_buf, s_setup_osd.ui_display_resolution);

	// draw main menu
	OSDLib_DrawFactoryMainMenu(gs_osd_display_buf, SETUP_FACTORY_MENU_DEMO_MODE, s_setup_osd.ui_display_resolution);

	// draw demo item
	OSDLib_Draw2ndFactoryMenu(gs_osd_display_buf, ui_str_id, wIndex, HOSTMI_DEMO_TOTAL, s_setup_osd.ui_display_resolution);
	
	s_setup_osd.b_display_need_update = TRUE;
}

void host_setup_factory_menu_display_osd_ImageSource_draw(uint16_t wIndex, MENU_PARAM eMenuParam, PMENU_STATUS pStatus)
{
	uint32_t ui_str_id[HOSTMI_IMAGE_SOURCE_TOTAL] = {_STR_STITCHING_, _STR_TELE_, _STR_WIDE_};
	
	s_setup_osd.ui_osd_type = HOST_OSD_TYPE_DISPLAY;
	s_setup_osd.b_display_need_update = FALSE;
	
	ab_osd_clear(&gs_osd_display_buf, COLOR_ARGB8888_TRANSPARENT, 0, 0, gs_osd_display_buf.ui_width, gs_osd_display_buf.ui_height);

	// draw BG
	OSDLib_DrawFactoryMenuBG(gs_osd_display_buf, s_setup_osd.ui_display_resolution);

	// draw main menu
	OSDLib_DrawFactoryMainMenu(gs_osd_display_buf, SETUP_FACTORY_MENU_IMAGE_SOURCE, s_setup_osd.ui_display_resolution);

	// draw demo item
	OSDLib_Draw2ndFactoryMenu(gs_osd_display_buf, ui_str_id, wIndex, HOSTMI_IMAGE_SOURCE_TOTAL, s_setup_osd.ui_display_resolution);
	
	s_setup_osd.b_display_need_update = TRUE;
}

void host_setup_factory_menu_display_osd_SaveStream_draw(uint16_t wIndex, MENU_PARAM eMenuParam, PMENU_STATUS pStatus)
{
#ifdef ENABLE_SAVE_STREAM
	uint32_t ui_str_id[HOSTMI_SAVESTREAM_TOTAL] = {_STR_ON_, _STR_OFF_};
	
	s_setup_osd.ui_osd_type = HOST_OSD_TYPE_DISPLAY;
	s_setup_osd.b_display_need_update = FALSE;
	
	ab_osd_clear(&gs_osd_display_buf, COLOR_ARGB8888_TRANSPARENT, 0, 0, gs_osd_display_buf.ui_width, gs_osd_display_buf.ui_height);

	// draw BG
	OSDLib_DrawFactoryMenuBG(gs_osd_display_buf, s_setup_osd.ui_display_resolution);

	// draw main menu
	OSDLib_DrawFactoryMainMenu(gs_osd_display_buf, SETUP_FACTORY_MENU_SAVESTREAM, s_setup_osd.ui_display_resolution);

	// draw save stream item
	OSDLib_Draw2ndFactoryMenu(gs_osd_display_buf, ui_str_id, wIndex, HOSTMI_DEMO_TOTAL, s_setup_osd.ui_display_resolution);
	
	s_setup_osd.b_display_need_update = TRUE;
#endif
}

void host_setup_factory_menu_display_osd_AdImage_draw(uint16_t wIndex, MENU_PARAM eMenuParam, PMENU_STATUS pStatus)
{
	s_setup_osd.ui_osd_type = HOST_OSD_TYPE_DISPLAY;
	s_setup_osd.b_display_need_update = FALSE;
	
	ab_osd_clear(&gs_osd_display_buf, COLOR_ARGB8888_TRANSPARENT, 0, 0, gs_osd_display_buf.ui_width, gs_osd_display_buf.ui_height);

	// draw BG
	OSDLib_DrawFactoryMenu2pageBG(gs_osd_display_buf, s_setup_osd.ui_display_resolution);

	// draw  advance image menu
	OSDLib_DrawFactoryAdImageMenu(gs_osd_display_buf, wIndex, s_setup_osd.ui_display_resolution);
	
	s_setup_osd.b_display_need_update = TRUE;
}

void host_setup_factory_menu_display_osd_AdImage_Shutter_draw(uint16_t wIndex, MENU_PARAM eMenuParam, PMENU_STATUS pStatus)
{
	uint32_t ui_str_id[HOSTMI_ADIMAGE_SHUTTER_TOTAL] = {_STR_NORMAL_MODE_, _STR_SLOW_MODE_, _STR_HIGH_SPEED_MODE_};
	
	s_setup_osd.ui_osd_type = HOST_OSD_TYPE_DISPLAY;
	s_setup_osd.b_display_need_update = FALSE;
	
	ab_osd_clear(&gs_osd_display_buf, COLOR_ARGB8888_TRANSPARENT, 0, 0, gs_osd_display_buf.ui_width, gs_osd_display_buf.ui_height);

	// draw BG
	OSDLib_DrawFactoryMenu2pageBG(gs_osd_display_buf, s_setup_osd.ui_display_resolution);

	// draw  advance image menu
	OSDLib_DrawFactoryAdImageMenu(gs_osd_display_buf, HOSTMI_ADIMAGE_SHUTTER, s_setup_osd.ui_display_resolution);

	// draw Shutter item
	OSDLib_Draw2page2ndFactoryMenu(gs_osd_display_buf, ui_str_id, wIndex, HOSTMI_ADIMAGE_SHUTTER_TOTAL, s_setup_osd.ui_display_resolution);

	s_setup_osd.b_display_need_update = TRUE;
}

void host_setup_factory_menu_display_osd_AdImage_AGC_draw(uint16_t wIndex, MENU_PARAM eMenuParam, PMENU_STATUS pStatus)
{
	s_setup_osd.ui_osd_type = HOST_OSD_TYPE_DISPLAY;
	s_setup_osd.b_display_need_update = FALSE;
	
	ab_osd_clear(&gs_osd_display_buf, COLOR_ARGB8888_TRANSPARENT, 0, 0, gs_osd_display_buf.ui_width, gs_osd_display_buf.ui_height);

	// draw BG
	OSDLib_DrawFactoryMenu2pageBG(gs_osd_display_buf, s_setup_osd.ui_display_resolution);

	// draw  advance image menu
	OSDLib_DrawFactoryAdImageMenu(gs_osd_display_buf, HOSTMI_ADIMAGE_AGC, s_setup_osd.ui_display_resolution);

	// draw agc bar
	OSDLib_DrawMenuAdjustValue(gs_osd_display_buf, wIndex, HOSTMI_ADIMAGE_AGC_OSD_START_VAL, HOSTMI_ADIMAGE_AGC_TOTAL-1, s_setup_osd.ui_display_resolution);

	s_setup_osd.b_display_need_update = TRUE;
}

void host_setup_factory_menu_display_osd_AdImage_Backlight_draw(uint16_t wIndex, MENU_PARAM eMenuParam, PMENU_STATUS pStatus)
{
	uint32_t ui_str_id[HOSTMI_ADIMAGE_BACKLIGHT_TOTAL] = {_STR_ON_, _STR_OFF_};
	
	s_setup_osd.ui_osd_type = HOST_OSD_TYPE_DISPLAY;
	s_setup_osd.b_display_need_update = FALSE;
	
	ab_osd_clear(&gs_osd_display_buf, COLOR_ARGB8888_TRANSPARENT, 0, 0, gs_osd_display_buf.ui_width, gs_osd_display_buf.ui_height);

	// draw BG
	OSDLib_DrawFactoryMenu2pageBG(gs_osd_display_buf, s_setup_osd.ui_display_resolution);

	// draw  advance image menu
	OSDLib_DrawFactoryAdImageMenu(gs_osd_display_buf, HOSTMI_ADIMAGE_BACKLIGHT, s_setup_osd.ui_display_resolution);

	// draw Shutter item
	OSDLib_Draw2page2ndFactoryMenu(gs_osd_display_buf, ui_str_id, wIndex, HOSTMI_ADIMAGE_BACKLIGHT_TOTAL, s_setup_osd.ui_display_resolution);

	s_setup_osd.b_display_need_update = TRUE;
}

void host_setup_factory_menu_display_osd_AdImage_Hue_draw(uint16_t wIndex, MENU_PARAM eMenuParam, PMENU_STATUS pStatus)
{
	s_setup_osd.ui_osd_type = HOST_OSD_TYPE_DISPLAY;
	s_setup_osd.b_display_need_update = FALSE;
	
	ab_osd_clear(&gs_osd_display_buf, COLOR_ARGB8888_TRANSPARENT, 0, 0, gs_osd_display_buf.ui_width, gs_osd_display_buf.ui_height);

	// draw BG
	OSDLib_DrawFactoryMenu2pageBG(gs_osd_display_buf, s_setup_osd.ui_display_resolution);

	// draw  advance image menu
	OSDLib_DrawFactoryAdImageMenu(gs_osd_display_buf, HOSTMI_ADIMAGE_HUE, s_setup_osd.ui_display_resolution);

	// draw agc bar
	OSDLib_DrawMenuAdjustValue(gs_osd_display_buf, wIndex, HOSTMI_ADIMAGE_HUE_OSD_START_VAL, HOSTMI_ADIMAGE_HUE_TOTAL-1, s_setup_osd.ui_display_resolution);

	s_setup_osd.b_display_need_update = TRUE;
}

void host_setup_factory_menu_display_osd_AdImage_NR_draw(uint16_t wIndex, MENU_PARAM eMenuParam, PMENU_STATUS pStatus)
{
	s_setup_osd.ui_osd_type = HOST_OSD_TYPE_DISPLAY;
	s_setup_osd.b_display_need_update = FALSE;
	
	ab_osd_clear(&gs_osd_display_buf, COLOR_ARGB8888_TRANSPARENT, 0, 0, gs_osd_display_buf.ui_width, gs_osd_display_buf.ui_height);

	// draw BG
	OSDLib_DrawFactoryMenu2pageBG(gs_osd_display_buf, s_setup_osd.ui_display_resolution);

	// draw  advance image menu
	OSDLib_DrawFactoryAdImageMenu(gs_osd_display_buf, HOSTMI_ADIMAGE_NR, s_setup_osd.ui_display_resolution);

	// draw agc bar
	OSDLib_DrawMenuAdjustValue(gs_osd_display_buf, wIndex, HOSTMI_ADIMAGE_NR_OSD_START_VAL, HOSTMI_ADIMAGE_NR_TOTAL-1, s_setup_osd.ui_display_resolution);

	s_setup_osd.b_display_need_update = TRUE;
}

void host_setup_factory_menu_display_osd_BurnIn_draw(uint16_t wIndex, MENU_PARAM eMenuParam, PMENU_STATUS pStatus)
{
	uint32_t ui_str_id[HOSTMI_BURNIN_TOTAL] = {_STR_ON_, _STR_OFF_};
	
	s_setup_osd.ui_osd_type = HOST_OSD_TYPE_DISPLAY;
	s_setup_osd.b_display_need_update = FALSE;
	
	ab_osd_clear(&gs_osd_display_buf, COLOR_ARGB8888_TRANSPARENT, 0, 0, gs_osd_display_buf.ui_width, gs_osd_display_buf.ui_height);

	// draw BG
	OSDLib_DrawFactoryMenuBG(gs_osd_display_buf, s_setup_osd.ui_display_resolution);

	// draw main menu
	OSDLib_DrawFactoryMainMenu(gs_osd_display_buf, SETUP_FACTORY_MENU_BURNIN, s_setup_osd.ui_display_resolution);

	// draw demo item
	OSDLib_Draw2ndFactoryMenu(gs_osd_display_buf, ui_str_id, wIndex, HOSTMI_BURNIN_TOTAL, s_setup_osd.ui_display_resolution);
	
	s_setup_osd.b_display_need_update = TRUE;
}

void host_setup_factory_menu_display_osd_Restore_draw(uint16_t wIndex, MENU_PARAM eMenuParam, PMENU_STATUS pStatus)
{
	s_setup_osd.ui_osd_type = HOST_OSD_TYPE_DISPLAY;
	s_setup_osd.b_display_need_update = FALSE;
	
	ab_osd_clear(&gs_osd_display_buf, COLOR_ARGB8888_TRANSPARENT, 0, 0, gs_osd_display_buf.ui_width, gs_osd_display_buf.ui_height);

	// draw BG
	OSDLib_DrawFactoryMenuBG(gs_osd_display_buf, s_setup_osd.ui_display_resolution);

	// draw main menu
	OSDLib_DrawFactoryMainMenu(gs_osd_display_buf, SETUP_FACTORY_MENU_RESTORE, s_setup_osd.ui_display_resolution);

	// restore item
	OSDLib_DrawRestoreMenu(gs_osd_display_buf, s_setup_osd.ui_display_resolution);

	s_setup_osd.b_display_need_update = TRUE;
}

void host_setup_factory_menu_display_osd_Restore_Old_Data_draw(uint16_t wIndex, MENU_PARAM eMenuParam, PMENU_STATUS pStatus)
{
#ifdef ENABLE_RESTORE_OLD_DATA
	uint32_t ui_str_id[HOSTMI_RESTORE_OLD_DATA_TOTAL] = {_STR_ON_, _STR_OFF_};
	
	s_setup_osd.ui_osd_type = HOST_OSD_TYPE_DISPLAY;
	s_setup_osd.b_display_need_update = FALSE;
	
	ab_osd_clear(&gs_osd_display_buf, COLOR_ARGB8888_TRANSPARENT, 0, 0, gs_osd_display_buf.ui_width, gs_osd_display_buf.ui_height);
	
	// draw BG
	OSDLib_DrawFactoryMenuBG(gs_osd_display_buf, s_setup_osd.ui_display_resolution);
	
	// draw main menu
	OSDLib_DrawFactoryMainMenu(gs_osd_display_buf, SETUP_FACTORY_MENU_RESTORE_OLD_DATA, s_setup_osd.ui_display_resolution);
	
	// draw demo item
	OSDLib_Draw2ndFactoryMenu(gs_osd_display_buf, ui_str_id, wIndex, HOSTMI_RESTORE_OLD_DATA_TOTAL, s_setup_osd.ui_display_resolution);
	
	s_setup_osd.b_display_need_update = TRUE;
#endif
}

void host_setup_factory_menu_display_osd_Backup_draw(uint16_t wIndex, MENU_PARAM eMenuParam, PMENU_STATUS pStatus)
{
#ifdef ENABLE_RESTORE_OLD_DATA
	uint32_t ui_str_id[HOSTMI_BACKUP_TOTAL] = {_STR_ON_, _STR_OFF_};
	
	s_setup_osd.ui_osd_type = HOST_OSD_TYPE_DISPLAY;
	s_setup_osd.b_display_need_update = FALSE;
	
	ab_osd_clear(&gs_osd_display_buf, COLOR_ARGB8888_TRANSPARENT, 0, 0, gs_osd_display_buf.ui_width, gs_osd_display_buf.ui_height);
	
	// draw BG
	OSDLib_DrawFactoryMenuBG(gs_osd_display_buf, s_setup_osd.ui_display_resolution);
	
	// draw main menu
	OSDLib_DrawFactoryMainMenu(gs_osd_display_buf, SETUP_FACTORY_MENU_BACKUP, s_setup_osd.ui_display_resolution);
	
	// draw demo item
	OSDLib_Draw2ndFactoryMenu(gs_osd_display_buf, ui_str_id, wIndex, HOSTMI_BACKUP_TOTAL, s_setup_osd.ui_display_resolution);
	
	s_setup_osd.b_display_need_update = TRUE;
#endif
}

void host_setup_factory_menu_display_osd_CaliCap_draw(uint16_t wIndex, MENU_PARAM eMenuParam, PMENU_STATUS pStatus)
{
#ifdef ENABLE_DEMO_CALIBRATION
	s_setup_osd.ui_osd_type = HOST_OSD_TYPE_DISPLAY;
	s_setup_osd.b_display_need_update = FALSE;
	
	ab_osd_clear(&gs_osd_display_buf, COLOR_ARGB8888_TRANSPARENT, 0, 0, gs_osd_display_buf.ui_width, gs_osd_display_buf.ui_height);

	// draw BG
	OSDLib_DrawFactoryMenuBG(gs_osd_display_buf, s_setup_osd.ui_display_resolution);

	// draw main menu
	OSDLib_DrawFactoryMainMenu(gs_osd_display_buf, SETUP_FACTORY_MENU_DEMO_CALI_CAP, s_setup_osd.ui_display_resolution);
	
	s_setup_osd.b_display_need_update = TRUE;
#endif
}

void host_setup_factory_menu_display_osd_CaliRestore_draw(uint16_t wIndex, MENU_PARAM eMenuParam, PMENU_STATUS pStatus)
{
#ifdef ENABLE_DEMO_CALIBRATION
	s_setup_osd.ui_osd_type = HOST_OSD_TYPE_DISPLAY;
	s_setup_osd.b_display_need_update = FALSE;
	
	ab_osd_clear(&gs_osd_display_buf, COLOR_ARGB8888_TRANSPARENT, 0, 0, gs_osd_display_buf.ui_width, gs_osd_display_buf.ui_height);

	// draw BG
	OSDLib_DrawFactoryMenuBG(gs_osd_display_buf, s_setup_osd.ui_display_resolution);

	// draw main menu
	OSDLib_DrawFactoryMainMenu(gs_osd_display_buf, SETUP_FACTORY_MENU_DEMO_CALI_RESTORE, s_setup_osd.ui_display_resolution);
	
	s_setup_osd.b_display_need_update = TRUE;
#endif
}

void host_setup_factory_menu_display_osd_Uvc_draw(uint16_t wIndex, MENU_PARAM eMenuParam, PMENU_STATUS pStatus)
{
#ifdef ENABLE_SEND_UVC_FRAME
	uint32_t ui_str_id[HOSTMI_UVC_FRAME_TOTAL] = {_STR_ON_, _STR_OFF_};
	
	s_setup_osd.ui_osd_type = HOST_OSD_TYPE_DISPLAY;
	s_setup_osd.b_display_need_update = FALSE;
	
	ab_osd_clear(&gs_osd_display_buf, COLOR_ARGB8888_TRANSPARENT, 0, 0, gs_osd_display_buf.ui_width, gs_osd_display_buf.ui_height);

	// draw BG
	OSDLib_DrawFactoryMenuBG(gs_osd_display_buf, s_setup_osd.ui_display_resolution);

	// draw main menu
	OSDLib_DrawFactoryMainMenu(gs_osd_display_buf, SETUP_FACTORY_MENU_UVCFRAME, s_setup_osd.ui_display_resolution);

	// draw demo item
	OSDLib_Draw2ndFactoryMenu(gs_osd_display_buf, ui_str_id, wIndex, HOSTMI_UVC_FRAME_TOTAL, s_setup_osd.ui_display_resolution);
	
	s_setup_osd.b_display_need_update = TRUE;
#endif
}
#endif
#if 1
void host_setup_factory_menu_enc_osd_main_draw(uint16_t wIndex, MENU_PARAM eMenuParam, PMENU_STATUS pStatus)
{
	s_setup_osd.ui_osd_type = HOST_OSD_TYPE_ENC;
	s_setup_osd.b_enc_need_update = FALSE;
	
	ab_osd_clear(&gs_osd_enc_buf, COLOR_ARGB8888_TRANSPARENT, 0, 0, gs_osd_enc_buf.ui_width, gs_osd_enc_buf.ui_height);

	// draw BG
	OSDLib_DrawFactoryMenuBG(gs_osd_enc_buf, s_setup_osd.ui_enc_resolution);

	// draw main menu
	OSDLib_DrawFactoryMainMenu(gs_osd_enc_buf, wIndex, s_setup_osd.ui_enc_resolution);
	
	s_setup_osd.b_enc_need_update = TRUE;
}

void host_setup_factory_menu_enc_osd_Demo_draw(uint16_t wIndex, MENU_PARAM eMenuParam, PMENU_STATUS pStatus)
{
	uint32_t ui_str_id[HOSTMI_DEMO_TOTAL] = {_STR_ON_, _STR_OFF_};
	
	s_setup_osd.ui_osd_type = HOST_OSD_TYPE_ENC;
	s_setup_osd.b_enc_need_update = FALSE;
	
	ab_osd_clear(&gs_osd_enc_buf, COLOR_ARGB8888_TRANSPARENT, 0, 0, gs_osd_enc_buf.ui_width, gs_osd_enc_buf.ui_height);

	// draw BG
	OSDLib_DrawFactoryMenuBG(gs_osd_enc_buf, s_setup_osd.ui_enc_resolution);

	// draw main menu
	OSDLib_DrawFactoryMainMenu(gs_osd_enc_buf, SETUP_FACTORY_MENU_DEMO_MODE, s_setup_osd.ui_enc_resolution);

	// draw demo item
	OSDLib_Draw2ndFactoryMenu(gs_osd_enc_buf, ui_str_id, wIndex, HOSTMI_DEMO_TOTAL, s_setup_osd.ui_enc_resolution);

	s_setup_osd.b_enc_need_update = TRUE;
}

void host_setup_factory_menu_enc_osd_ImageSource_draw(uint16_t wIndex, MENU_PARAM eMenuParam, PMENU_STATUS pStatus)
{
	uint32_t ui_str_id[HOSTMI_IMAGE_SOURCE_TOTAL] = {_STR_STITCHING_, _STR_TELE_, _STR_WIDE_};
	
	s_setup_osd.ui_osd_type = HOST_OSD_TYPE_ENC;
	s_setup_osd.b_enc_need_update = FALSE;
	
	ab_osd_clear(&gs_osd_enc_buf, COLOR_ARGB8888_TRANSPARENT, 0, 0, gs_osd_enc_buf.ui_width, gs_osd_enc_buf.ui_height);

	// draw BG
	OSDLib_DrawFactoryMenuBG(gs_osd_enc_buf, s_setup_osd.ui_enc_resolution);

	// draw main menu
	OSDLib_DrawFactoryMainMenu(gs_osd_enc_buf, SETUP_FACTORY_MENU_IMAGE_SOURCE, s_setup_osd.ui_enc_resolution);

	// draw demo item
	OSDLib_Draw2ndFactoryMenu(gs_osd_enc_buf, ui_str_id, wIndex, HOSTMI_IMAGE_SOURCE_TOTAL, s_setup_osd.ui_enc_resolution);

	s_setup_osd.b_enc_need_update = TRUE;
}

void host_setup_factory_menu_enc_osd_SaveStream_draw(uint16_t wIndex, MENU_PARAM eMenuParam, PMENU_STATUS pStatus)
{
#ifdef ENABLE_SAVE_STREAM
	uint32_t ui_str_id[HOSTMI_SAVESTREAM_TOTAL] = {_STR_ON_, _STR_OFF_};
	
	s_setup_osd.ui_osd_type = HOST_OSD_TYPE_ENC;
	s_setup_osd.b_enc_need_update = FALSE;
	
	ab_osd_clear(&gs_osd_enc_buf, COLOR_ARGB8888_TRANSPARENT, 0, 0, gs_osd_enc_buf.ui_width, gs_osd_enc_buf.ui_height);

	// draw BG
	OSDLib_DrawFactoryMenuBG(gs_osd_enc_buf, s_setup_osd.ui_enc_resolution);

	// draw main menu
	OSDLib_DrawFactoryMainMenu(gs_osd_enc_buf, SETUP_FACTORY_MENU_SAVESTREAM, s_setup_osd.ui_enc_resolution);
	
	// draw save stream item
	OSDLib_Draw2ndFactoryMenu(gs_osd_enc_buf, ui_str_id, wIndex, HOSTMI_DEMO_TOTAL, s_setup_osd.ui_enc_resolution);

	s_setup_osd.b_enc_need_update = TRUE;
#endif
}

void host_setup_factory_menu_enc_osd_AdImage_draw(uint16_t wIndex, MENU_PARAM eMenuParam, PMENU_STATUS pStatus)
{
	s_setup_osd.ui_osd_type = HOST_OSD_TYPE_ENC;
	s_setup_osd.b_enc_need_update = FALSE;
	
	ab_osd_clear(&gs_osd_enc_buf, COLOR_ARGB8888_TRANSPARENT, 0, 0, gs_osd_enc_buf.ui_width, gs_osd_enc_buf.ui_height);

	// draw BG
	OSDLib_DrawFactoryMenu2pageBG(gs_osd_enc_buf, s_setup_osd.ui_enc_resolution);

	// draw advance image menu
	OSDLib_DrawFactoryAdImageMenu(gs_osd_enc_buf, wIndex, s_setup_osd.ui_enc_resolution);

	s_setup_osd.b_enc_need_update = TRUE;
}

void host_setup_factory_menu_enc_osd_AdImage_Shutter_draw(uint16_t wIndex, MENU_PARAM eMenuParam, PMENU_STATUS pStatus)
{
	uint32_t ui_str_id[HOSTMI_ADIMAGE_SHUTTER_TOTAL] = {_STR_NORMAL_MODE_, _STR_SLOW_MODE_, _STR_HIGH_SPEED_MODE_};
	
	s_setup_osd.ui_osd_type = HOST_OSD_TYPE_ENC;
	s_setup_osd.b_enc_need_update = FALSE;
	
	ab_osd_clear(&gs_osd_enc_buf, COLOR_ARGB8888_TRANSPARENT, 0, 0, gs_osd_enc_buf.ui_width, gs_osd_enc_buf.ui_height);

	// draw BG
	OSDLib_DrawFactoryMenu2pageBG(gs_osd_enc_buf, s_setup_osd.ui_enc_resolution);

	// draw  advance image menu
	OSDLib_DrawFactoryAdImageMenu(gs_osd_enc_buf, HOSTMI_ADIMAGE_SHUTTER, s_setup_osd.ui_enc_resolution);

	// draw Shutter item
	OSDLib_Draw2page2ndFactoryMenu(gs_osd_enc_buf, ui_str_id, wIndex, HOSTMI_ADIMAGE_SHUTTER_TOTAL, s_setup_osd.ui_enc_resolution);

	s_setup_osd.b_enc_need_update = TRUE;
}

void host_setup_factory_menu_enc_osd_AdImage_AGC_draw(uint16_t wIndex, MENU_PARAM eMenuParam, PMENU_STATUS pStatus)
{
	s_setup_osd.ui_osd_type = HOST_OSD_TYPE_ENC;
	s_setup_osd.b_enc_need_update = FALSE;
	
	ab_osd_clear(&gs_osd_enc_buf, COLOR_ARGB8888_TRANSPARENT, 0, 0, gs_osd_enc_buf.ui_width, gs_osd_enc_buf.ui_height);

	// draw BG
	OSDLib_DrawFactoryMenu2pageBG(gs_osd_enc_buf, s_setup_osd.ui_enc_resolution);

	// draw  advance image menu
	OSDLib_DrawFactoryAdImageMenu(gs_osd_enc_buf, HOSTMI_ADIMAGE_AGC, s_setup_osd.ui_enc_resolution);

	// draw agc bar
	OSDLib_DrawMenuAdjustValue(gs_osd_enc_buf, wIndex, HOSTMI_ADIMAGE_AGC_OSD_START_VAL, HOSTMI_ADIMAGE_AGC_TOTAL-1, s_setup_osd.ui_enc_resolution);

	s_setup_osd.b_enc_need_update = TRUE;
}

void host_setup_factory_menu_enc_osd_AdImage_Backlight_draw(uint16_t wIndex, MENU_PARAM eMenuParam, PMENU_STATUS pStatus)
{
	uint32_t ui_str_id[HOSTMI_ADIMAGE_BACKLIGHT_TOTAL] = {_STR_ON_, _STR_OFF_};
	
	s_setup_osd.ui_osd_type = HOST_OSD_TYPE_ENC;
	s_setup_osd.b_enc_need_update = FALSE;
	
	ab_osd_clear(&gs_osd_enc_buf, COLOR_ARGB8888_TRANSPARENT, 0, 0, gs_osd_enc_buf.ui_width, gs_osd_enc_buf.ui_height);

	// draw BG
	OSDLib_DrawFactoryMenu2pageBG(gs_osd_enc_buf, s_setup_osd.ui_enc_resolution);

	// draw  advance image menu
	OSDLib_DrawFactoryAdImageMenu(gs_osd_enc_buf, HOSTMI_ADIMAGE_BACKLIGHT, s_setup_osd.ui_enc_resolution);

	// draw Shutter item
	OSDLib_Draw2page2ndFactoryMenu(gs_osd_enc_buf, ui_str_id, wIndex, HOSTMI_ADIMAGE_BACKLIGHT_TOTAL, s_setup_osd.ui_enc_resolution);

	s_setup_osd.b_enc_need_update = TRUE;
}

void host_setup_factory_menu_enc_osd_AdImage_Hue_draw(uint16_t wIndex, MENU_PARAM eMenuParam, PMENU_STATUS pStatus)
{
	s_setup_osd.ui_osd_type = HOST_OSD_TYPE_ENC;
	s_setup_osd.b_enc_need_update = FALSE;
	
	ab_osd_clear(&gs_osd_enc_buf, COLOR_ARGB8888_TRANSPARENT, 0, 0, gs_osd_enc_buf.ui_width, gs_osd_enc_buf.ui_height);

	// draw BG
	OSDLib_DrawFactoryMenu2pageBG(gs_osd_enc_buf, s_setup_osd.ui_enc_resolution);

	// draw  advance image menu
	OSDLib_DrawFactoryAdImageMenu(gs_osd_enc_buf, HOSTMI_ADIMAGE_HUE, s_setup_osd.ui_enc_resolution);

	// draw agc bar
	OSDLib_DrawMenuAdjustValue(gs_osd_enc_buf, wIndex, HOSTMI_ADIMAGE_HUE_OSD_START_VAL, HOSTMI_ADIMAGE_HUE_TOTAL-1, s_setup_osd.ui_enc_resolution);

	s_setup_osd.b_enc_need_update = TRUE;
}

void host_setup_factory_menu_enc_osd_AdImage_NR_draw(uint16_t wIndex, MENU_PARAM eMenuParam, PMENU_STATUS pStatus)
{
	s_setup_osd.ui_osd_type = HOST_OSD_TYPE_ENC;
	s_setup_osd.b_enc_need_update = FALSE;
	
	ab_osd_clear(&gs_osd_enc_buf, COLOR_ARGB8888_TRANSPARENT, 0, 0, gs_osd_enc_buf.ui_width, gs_osd_enc_buf.ui_height);

	// draw BG
	OSDLib_DrawFactoryMenu2pageBG(gs_osd_enc_buf, s_setup_osd.ui_enc_resolution);

	// draw  advance image menu
	OSDLib_DrawFactoryAdImageMenu(gs_osd_enc_buf, HOSTMI_ADIMAGE_NR, s_setup_osd.ui_enc_resolution);

	// draw agc bar
	OSDLib_DrawMenuAdjustValue(gs_osd_enc_buf, wIndex, HOSTMI_ADIMAGE_NR_OSD_START_VAL, HOSTMI_ADIMAGE_NR_TOTAL-1, s_setup_osd.ui_enc_resolution);

	s_setup_osd.b_enc_need_update = TRUE;
}

void host_setup_factory_menu_enc_osd_BurnIn_draw(uint16_t wIndex, MENU_PARAM eMenuParam, PMENU_STATUS pStatus)
{
	uint32_t ui_str_id[HOSTMI_BURNIN_TOTAL] = {_STR_ON_, _STR_OFF_};
	
	s_setup_osd.ui_osd_type = HOST_OSD_TYPE_ENC;
	s_setup_osd.b_enc_need_update = FALSE;
	
	ab_osd_clear(&gs_osd_enc_buf, COLOR_ARGB8888_TRANSPARENT, 0, 0, gs_osd_enc_buf.ui_width, gs_osd_enc_buf.ui_height);

	// draw BG
	OSDLib_DrawFactoryMenuBG(gs_osd_enc_buf, s_setup_osd.ui_enc_resolution);

	// draw main menu
	OSDLib_DrawFactoryMainMenu(gs_osd_enc_buf, SETUP_FACTORY_MENU_BURNIN, s_setup_osd.ui_enc_resolution);

	// draw demo item
	OSDLib_Draw2ndFactoryMenu(gs_osd_enc_buf, ui_str_id, wIndex, HOSTMI_BURNIN_TOTAL, s_setup_osd.ui_enc_resolution);

	s_setup_osd.b_enc_need_update = TRUE;
}

void host_setup_factory_menu_enc_osd_Restore_draw(uint16_t wIndex, MENU_PARAM eMenuParam, PMENU_STATUS pStatus)
{
	s_setup_osd.ui_osd_type = HOST_OSD_TYPE_ENC;
	s_setup_osd.b_enc_need_update = FALSE;
	
	ab_osd_clear(&gs_osd_enc_buf, COLOR_ARGB8888_TRANSPARENT, 0, 0, gs_osd_enc_buf.ui_width, gs_osd_enc_buf.ui_height);

	// draw BG
	OSDLib_DrawFactoryMenuBG(gs_osd_enc_buf, s_setup_osd.ui_enc_resolution);

	// draw main menu
	OSDLib_DrawFactoryMainMenu(gs_osd_enc_buf, SETUP_FACTORY_MENU_RESTORE, s_setup_osd.ui_enc_resolution);

	// restore item
	OSDLib_DrawRestoreMenu(gs_osd_enc_buf, s_setup_osd.ui_enc_resolution);

	s_setup_osd.b_enc_need_update = TRUE;
}

void host_setup_factory_menu_enc_osd_Restore_Old_Data_draw(uint16_t wIndex, MENU_PARAM eMenuParam, PMENU_STATUS pStatus)
{
#ifdef ENABLE_RESTORE_OLD_DATA
	uint32_t ui_str_id[HOSTMI_RESTORE_OLD_DATA_TOTAL] = {_STR_ON_, _STR_OFF_};
	
	s_setup_osd.ui_osd_type = HOST_OSD_TYPE_ENC;
	s_setup_osd.b_enc_need_update = FALSE;
	
	ab_osd_clear(&gs_osd_enc_buf, COLOR_ARGB8888_TRANSPARENT, 0, 0, gs_osd_enc_buf.ui_width, gs_osd_enc_buf.ui_height);

	// draw BG
	OSDLib_DrawFactoryMenuBG(gs_osd_enc_buf, s_setup_osd.ui_enc_resolution);

	// draw main menu
	OSDLib_DrawFactoryMainMenu(gs_osd_enc_buf, SETUP_FACTORY_MENU_RESTORE_OLD_DATA, s_setup_osd.ui_enc_resolution);

	// draw demo item
	OSDLib_Draw2ndFactoryMenu(gs_osd_enc_buf, ui_str_id, wIndex, HOSTMI_RESTORE_OLD_DATA_TOTAL, s_setup_osd.ui_enc_resolution);

	s_setup_osd.b_enc_need_update = TRUE;
#endif
}

void host_setup_factory_menu_enc_osd_Backup_draw(uint16_t wIndex, MENU_PARAM eMenuParam, PMENU_STATUS pStatus)
{
#ifdef ENABLE_RESTORE_OLD_DATA
	uint32_t ui_str_id[HOSTMI_BACKUP_TOTAL] = {_STR_ON_, _STR_OFF_};
	
	s_setup_osd.ui_osd_type = HOST_OSD_TYPE_ENC;
	s_setup_osd.b_enc_need_update = FALSE;
	
	ab_osd_clear(&gs_osd_enc_buf, COLOR_ARGB8888_TRANSPARENT, 0, 0, gs_osd_enc_buf.ui_width, gs_osd_enc_buf.ui_height);

	// draw BG
	OSDLib_DrawFactoryMenuBG(gs_osd_enc_buf, s_setup_osd.ui_enc_resolution);

	// draw main menu
	OSDLib_DrawFactoryMainMenu(gs_osd_enc_buf, SETUP_FACTORY_MENU_BACKUP, s_setup_osd.ui_enc_resolution);

	// draw demo item
	OSDLib_Draw2ndFactoryMenu(gs_osd_enc_buf, ui_str_id, wIndex, HOSTMI_BACKUP_TOTAL, s_setup_osd.ui_enc_resolution);

	s_setup_osd.b_enc_need_update = TRUE;
#endif
}

void host_setup_factory_menu_enc_osd_CaliCap_draw(uint16_t wIndex, MENU_PARAM eMenuParam, PMENU_STATUS pStatus)
{
#ifdef ENABLE_DEMO_CALIBRATION
	s_setup_osd.ui_osd_type = HOST_OSD_TYPE_ENC;
	s_setup_osd.b_enc_need_update = FALSE;
	
	ab_osd_clear(&gs_osd_enc_buf, COLOR_ARGB8888_TRANSPARENT, 0, 0, gs_osd_enc_buf.ui_width, gs_osd_enc_buf.ui_height);

	// draw BG
	OSDLib_DrawFactoryMenuBG(gs_osd_enc_buf, s_setup_osd.ui_enc_resolution);

	// draw main menu
	OSDLib_DrawFactoryMainMenu(gs_osd_enc_buf, SETUP_FACTORY_MENU_DEMO_CALI_CAP, s_setup_osd.ui_enc_resolution);
	
	s_setup_osd.b_enc_need_update = TRUE;
#endif
}

void host_setup_factory_menu_enc_osd_CaliRestore_draw(uint16_t wIndex, MENU_PARAM eMenuParam, PMENU_STATUS pStatus)
{
#ifdef ENABLE_DEMO_CALIBRATION
	s_setup_osd.ui_osd_type = HOST_OSD_TYPE_ENC;
	s_setup_osd.b_enc_need_update = FALSE;
	
	ab_osd_clear(&gs_osd_enc_buf, COLOR_ARGB8888_TRANSPARENT, 0, 0, gs_osd_enc_buf.ui_width, gs_osd_enc_buf.ui_height);

	// draw BG
	OSDLib_DrawFactoryMenuBG(gs_osd_enc_buf, s_setup_osd.ui_enc_resolution);

	// draw main menu
	OSDLib_DrawFactoryMainMenu(gs_osd_enc_buf, SETUP_FACTORY_MENU_DEMO_CALI_RESTORE, s_setup_osd.ui_enc_resolution);
	
	s_setup_osd.b_enc_need_update = TRUE;
#endif
}

void host_setup_factory_menu_enc_osd_Uvc_draw(uint16_t wIndex, MENU_PARAM eMenuParam, PMENU_STATUS pStatus)
{
#ifdef ENABLE_SEND_UVC_FRAME
	uint32_t ui_str_id[HOSTMI_UVC_FRAME_TOTAL] = {_STR_ON_, _STR_OFF_};
	
	s_setup_osd.ui_osd_type = HOST_OSD_TYPE_ENC;
	s_setup_osd.b_enc_need_update = FALSE;
	
	ab_osd_clear(&gs_osd_enc_buf, COLOR_ARGB8888_TRANSPARENT, 0, 0, gs_osd_enc_buf.ui_width, gs_osd_enc_buf.ui_height);

	// draw BG
	OSDLib_DrawFactoryMenuBG(gs_osd_enc_buf, s_setup_osd.ui_enc_resolution);

	// draw main menu
	OSDLib_DrawFactoryMainMenu(gs_osd_enc_buf, SETUP_FACTORY_MENU_UVCFRAME, s_setup_osd.ui_enc_resolution);

	// draw demo item
	OSDLib_Draw2ndFactoryMenu(gs_osd_enc_buf, ui_str_id, wIndex, HOSTMI_UVC_FRAME_TOTAL, s_setup_osd.ui_enc_resolution);
	
	s_setup_osd.b_enc_need_update = TRUE;
#endif
}

#endif
#endif

void host_setup_osd_display_draw(uint32_t ui_idx, AB_DISPLAY_STAMP_CB_PARA_ST* ps_para)
{
	ps_para->as_list[ui_idx].b_need_update = false;
	ps_para->as_list[ui_idx].b_lock_buffer = true;

	if(ps_para->as_list[ui_idx].puc_virtual_addr != NULL){
		if(s_setup_osd.b_display_need_update == TRUE){
			memcpy(ps_para->as_list[ui_idx].puc_virtual_addr, gs_osd_display_buf.puc_buf_vir_addr, gs_osd_display_buf.ui_width*gs_osd_display_buf.ui_height*2);
			
			s_setup_osd.b_display_need_update = false;
			ps_para->as_list[ui_idx].b_need_update = true;
			ps_para->as_list[ui_idx].b_lock_buffer = false;
		}else if(s_setup_osd.b_display_need_clear == TRUE){
			memset(ps_para->as_list[ui_idx].puc_virtual_addr, 0x00, ps_para->as_list[ui_idx].ui_w*ps_para->as_list[ui_idx].ui_h*2);

			s_setup_osd.b_display_need_clear = false;
			ps_para->as_list[ui_idx].b_need_update = true;
			ps_para->as_list[ui_idx].b_lock_buffer = false;
		}
	}
}

void host_setup_osd_display_dbg_draw(uint32_t ui_idx, AB_DISPLAY_STAMP_CB_PARA_ST* ps_para)
{
	AB_OSD_BUF_ST s_dst_buf = {0};
	AB_OSD_RECT_ST s_dst_rect = {0};

	ps_para->as_list[ui_idx].b_need_update = false;
	ps_para->as_list[ui_idx].b_lock_buffer = true;
	
	if(ps_para->as_list[ui_idx].puc_virtual_addr != NULL){
		if((hd_gettime_ms() - s_setup_osd.ui_display_draw_time_intervel) > 3000){
			s_dst_buf.e_color_fmt = AB_OSD_COLOR_FORMAT_ARGB1555;
			s_dst_buf.ui_buf_phy_addr = ps_para->as_list[ui_idx].ui_phy_addr;
			s_dst_buf.puc_buf_vir_addr = ps_para->as_list[ui_idx].puc_virtual_addr;
			s_dst_buf.ui_width = ps_para->as_list[ui_idx].ui_w;
			s_dst_buf.ui_line_jump_pixels = ps_para->as_list[ui_idx].ui_w;
			s_dst_buf.ui_height = ps_para->as_list[ui_idx].ui_h;

			if(ui_dbg_draw_times[0] < 2){
				memset(ps_para->as_list[ui_idx].puc_virtual_addr, 0x00, ps_para->as_list[ui_idx].ui_w*ps_para->as_list[ui_idx].ui_h*2);
				
				ab_osd_draw_text(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_RED, 0x00000000, 30, "Humidity(RH):", 0, 50, AB_OSD_ALIGN_V_DOWN|AB_OSD_ALIGN_H_LEFT);
				ab_osd_draw_text(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_RED, 0x00000000, 30, "Temperature(C):", 0, 100, AB_OSD_ALIGN_V_DOWN|AB_OSD_ALIGN_H_LEFT);
				ab_osd_draw_text(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_RED, 0x00000000, 30, "Luxval(lux):", 0, 150, AB_OSD_ALIGN_V_DOWN|AB_OSD_ALIGN_H_LEFT);
				ab_osd_draw_text(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_RED, 0x00000000, 30, "ColTemp(K):", 0, 200, AB_OSD_ALIGN_V_DOWN|AB_OSD_ALIGN_H_LEFT);
				ab_osd_draw_text(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_RED, 0x00000000, 30, "PIR:", 0, 250, AB_OSD_ALIGN_V_DOWN|AB_OSD_ALIGN_H_LEFT);
				ab_osd_draw_text(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_RED, 0x00000000, 30, "PCB(C):", 0, 300, AB_OSD_ALIGN_V_DOWN|AB_OSD_ALIGN_H_LEFT);

				ui_dbg_draw_times[0]++;
			}else{
				char Humidity[30];
				char Temperature[30];
				char Luxval[30];
				char ColTmp[30];
				char PIR[30];
				char PCB[30];
				
				ab_osd_clear(&s_dst_buf, COLOR_ARGB8888_TRANSPARENT, 300, 0, s_dst_buf.ui_width-300, 50);
				ab_osd_clear(&s_dst_buf, COLOR_ARGB8888_TRANSPARENT, 320, 50, s_dst_buf.ui_width-320, 50);
				ab_osd_clear(&s_dst_buf, COLOR_ARGB8888_TRANSPARENT, 250, 100, s_dst_buf.ui_width-250, 50);
				ab_osd_clear(&s_dst_buf, COLOR_ARGB8888_TRANSPARENT, 250, 150, s_dst_buf.ui_width-250, 50);
				ab_osd_clear(&s_dst_buf, COLOR_ARGB8888_TRANSPARENT, 100, 200, s_dst_buf.ui_width-100, 50);
				ab_osd_clear(&s_dst_buf, COLOR_ARGB8888_TRANSPARENT, 150, 250, s_dst_buf.ui_width-150, 50);

				snprintf(Humidity, sizeof(Humidity), "%d%s", gs_ui.s_run_info.ui_Humidity, "%");
				snprintf(Temperature, sizeof(Temperature), "%d", gs_ui.s_run_info.ui_Temperature);
				snprintf(Luxval, sizeof(Luxval), "%d", gs_ui.s_run_info.ui_luxval);
				snprintf(ColTmp, sizeof(ColTmp), "%d", gs_ui.s_run_info.uw_color_temp);
				snprintf(PIR, sizeof(PIR), "%d", gs_ui.s_run_info.u_Pir_value);
				snprintf(PCB, sizeof(PCB), "%d", gs_ui.s_run_info.i_pcbthermal);
				
				ab_osd_draw_text(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_RED, 0x00000000, 30, Humidity, 300, 50, AB_OSD_ALIGN_V_DOWN|AB_OSD_ALIGN_H_LEFT);
				ab_osd_draw_text(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_RED, 0x00000000, 30, Temperature, 320, 100, AB_OSD_ALIGN_V_DOWN|AB_OSD_ALIGN_H_LEFT);
				ab_osd_draw_text(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_RED, 0x00000000, 30, Luxval, 250, 150, AB_OSD_ALIGN_V_DOWN|AB_OSD_ALIGN_H_LEFT);
				ab_osd_draw_text(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_RED, 0x00000000, 30, ColTmp, 250, 200, AB_OSD_ALIGN_V_DOWN|AB_OSD_ALIGN_H_LEFT);
				ab_osd_draw_text(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_RED, 0x00000000, 30, PIR, 100, 250, AB_OSD_ALIGN_V_DOWN|AB_OSD_ALIGN_H_LEFT);
				ab_osd_draw_text(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_RED, 0x00000000, 30, PCB, 150, 300, AB_OSD_ALIGN_V_DOWN|AB_OSD_ALIGN_H_LEFT);
			}

			s_setup_osd.ui_display_draw_time_intervel = hd_gettime_ms();
			ps_para->as_list[ui_idx].b_need_update = true;
		}
	}
}

void host_setup_osd_enc_draw(uint32_t ui_idx, AB_MM_VDO_REC_ENC_STAMP_CB_PARA_ST* ps_para)
{
	ps_para->as_list[ui_idx].b_need_update = false;
	ps_para->as_list[ui_idx].b_lock_buffer = true;

	if(ps_para->as_list[ui_idx].puc_virtual_addr != NULL){
		if(s_setup_osd.b_enc_need_update == TRUE){
			memcpy(ps_para->as_list[ui_idx].puc_virtual_addr, gs_osd_enc_buf.puc_buf_vir_addr, gs_osd_enc_buf.ui_width*gs_osd_enc_buf.ui_height*2);
			
			s_setup_osd.b_enc_need_update = false;
			ps_para->as_list[ui_idx].b_need_update = true;
			ps_para->as_list[ui_idx].b_lock_buffer = false;
		}else if(s_setup_osd.b_enc_need_clear == TRUE){
			memset(ps_para->as_list[ui_idx].puc_virtual_addr, 0x00, ps_para->as_list[ui_idx].ui_w*ps_para->as_list[ui_idx].ui_h*2);

			s_setup_osd.b_enc_need_clear = false;
			ps_para->as_list[ui_idx].b_need_update = true;
			ps_para->as_list[ui_idx].b_lock_buffer = false;
		}
	}
}

void host_setup_osd_enc_dbg_draw(uint32_t ui_idx, AB_MM_VDO_REC_ENC_STAMP_CB_PARA_ST* ps_para)
{
	AB_OSD_BUF_ST s_dst_buf = {0};
	AB_OSD_RECT_ST s_dst_rect = {0};
	
	ps_para->as_list[ui_idx].b_need_update = false;
	ps_para->as_list[ui_idx].b_lock_buffer = true;
	
	if(ps_para->as_list[ui_idx].puc_virtual_addr != NULL){
		if((hd_gettime_ms() - s_setup_osd.ui_enc_draw_time_intervel) > 3000){
			s_dst_buf.e_color_fmt = AB_OSD_COLOR_FORMAT_ARGB1555;
			s_dst_buf.ui_buf_phy_addr = ps_para->as_list[ui_idx].ui_phy_addr;
			s_dst_buf.puc_buf_vir_addr = ps_para->as_list[ui_idx].puc_virtual_addr;
			s_dst_buf.ui_width = ps_para->as_list[ui_idx].ui_w;
			s_dst_buf.ui_line_jump_pixels = ps_para->as_list[ui_idx].ui_w;
			s_dst_buf.ui_height = ps_para->as_list[ui_idx].ui_h;
		
			if(ui_dbg_draw_times[1] < 2){
				memset(ps_para->as_list[ui_idx].puc_virtual_addr, 0x00, ps_para->as_list[ui_idx].ui_w*ps_para->as_list[ui_idx].ui_h*2);
				
				ab_osd_draw_text(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_RED, 0x00000000, 30, "Humidity(RH):", 0, 50, AB_OSD_ALIGN_V_DOWN|AB_OSD_ALIGN_H_LEFT);
				ab_osd_draw_text(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_RED, 0x00000000, 30, "Temperature(C):", 0, 100, AB_OSD_ALIGN_V_DOWN|AB_OSD_ALIGN_H_LEFT);
				ab_osd_draw_text(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_RED, 0x00000000, 30, "Luxval(lux):", 0, 150, AB_OSD_ALIGN_V_DOWN|AB_OSD_ALIGN_H_LEFT);
				ab_osd_draw_text(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_RED, 0x00000000, 30, "ColTemp(K):", 0, 200, AB_OSD_ALIGN_V_DOWN|AB_OSD_ALIGN_H_LEFT);
				ab_osd_draw_text(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_RED, 0x00000000, 30, "PIR:", 0, 250, AB_OSD_ALIGN_V_DOWN|AB_OSD_ALIGN_H_LEFT);
				ab_osd_draw_text(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_RED, 0x00000000, 30, "PCB(C):", 0, 300, AB_OSD_ALIGN_V_DOWN|AB_OSD_ALIGN_H_LEFT);
		
				ui_dbg_draw_times[1]++;
			}else{
				char Humidity[30];
				char Temperature[30];
				char Luxval[30];
				char ColTmp[30];
				char PIR[30];
				char PCB[30];
				
				ab_osd_clear(&s_dst_buf, COLOR_ARGB8888_TRANSPARENT, 300, 0, s_dst_buf.ui_width-300, 50);
				ab_osd_clear(&s_dst_buf, COLOR_ARGB8888_TRANSPARENT, 320, 50, s_dst_buf.ui_width-320, 50);
				ab_osd_clear(&s_dst_buf, COLOR_ARGB8888_TRANSPARENT, 250, 100, s_dst_buf.ui_width-250, 50);
				ab_osd_clear(&s_dst_buf, COLOR_ARGB8888_TRANSPARENT, 250, 150, s_dst_buf.ui_width-250, 50);
				ab_osd_clear(&s_dst_buf, COLOR_ARGB8888_TRANSPARENT, 100, 200, s_dst_buf.ui_width-100, 50);
				ab_osd_clear(&s_dst_buf, COLOR_ARGB8888_TRANSPARENT, 150, 250, s_dst_buf.ui_width-150, 50);
		
				snprintf(Humidity, sizeof(Humidity), "%d%s", gs_ui.s_run_info.ui_Humidity, "%");
				snprintf(Temperature, sizeof(Temperature), "%d", gs_ui.s_run_info.ui_Temperature);
				snprintf(Luxval, sizeof(Luxval), "%d", gs_ui.s_run_info.ui_luxval);
				snprintf(ColTmp, sizeof(ColTmp), "%d", gs_ui.s_run_info.uw_color_temp);
				snprintf(PIR, sizeof(PIR), "%d", gs_ui.s_run_info.u_Pir_value);
				snprintf(PCB, sizeof(PCB), "%d", gs_ui.s_run_info.i_pcbthermal);
				
				ab_osd_draw_text(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_RED, 0x00000000, 30, Humidity, 300, 50, AB_OSD_ALIGN_V_DOWN|AB_OSD_ALIGN_H_LEFT);
				ab_osd_draw_text(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_RED, 0x00000000, 30, Temperature, 320, 100, AB_OSD_ALIGN_V_DOWN|AB_OSD_ALIGN_H_LEFT);
				ab_osd_draw_text(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_RED, 0x00000000, 30, Luxval, 250, 150, AB_OSD_ALIGN_V_DOWN|AB_OSD_ALIGN_H_LEFT);
				ab_osd_draw_text(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_RED, 0x00000000, 30, ColTmp, 250, 200, AB_OSD_ALIGN_V_DOWN|AB_OSD_ALIGN_H_LEFT);
				ab_osd_draw_text(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_RED, 0x00000000, 30, PIR, 100, 250, AB_OSD_ALIGN_V_DOWN|AB_OSD_ALIGN_H_LEFT);
				ab_osd_draw_text(&s_dst_buf, &s_dst_rect, COLOR_ARGB8888_RED, 0x00000000, 30, PCB, 150, 300, AB_OSD_ALIGN_V_DOWN|AB_OSD_ALIGN_H_LEFT);
			}

			s_setup_osd.ui_enc_draw_time_intervel = hd_gettime_ms();
			ps_para->as_list[ui_idx].b_need_update = true;
		}
	}
}

void host_setup_osd_init(void)
{
	if(s_setup_osd.b_is_init == 1)return;

	host_osd_init();

	// osd genenal info
    s_setup_osd.b_display_need_update = FALSE;
    s_setup_osd.b_display_need_clear = FALSE;
    s_setup_osd.b_enc_need_update = FALSE;
	s_setup_osd.b_enc_need_clear = FALSE;
	s_setup_osd.wIndex = 0;

	// osd dbg info
	s_setup_osd.ui_display_draw_time_intervel = 0;
	s_setup_osd.ui_enc_draw_time_intervel = 0;
	ui_dbg_draw_times[0] = 0;
	ui_dbg_draw_times[1] = 0;

	memset(s_setup_osd.ui_main_disabled, 0, SETUPMENU_TOTAL);
	memset(s_setup_osd.ui_image_disabled, 0, HOSTMI_IMAGE_TOTAL);
	memset(s_setup_osd.ui_hdmi_disabled, 0, HOSTMI_HDMI_TOTAL);
	memset(s_setup_osd.ui_hdmi_resolution_disabled, 0, HOSTMI_HDMI_RESOLUTION_TOTAL);
	memset(s_setup_osd.ui_factory_main_disabled, 0, SETUP_FACTORY_MENU_TOTAL);
	memset(s_setup_osd.ui_adimage_disabled, 0, HOSTMI_ADIMAGE_TOTAL);
	memset(s_setup_osd.ui_main_turnoff, 0, SETUPMENU_TOTAL);
	s_setup_osd.ui_osd_type = HOST_OSD_TYPE_NULL;

	// osd setting info
	host_setup_menu_osd_get_info(HOST_STREAM_ID_HDMI);
	host_setup_menu_osd_get_info(HOST_STREAM_ID_UVC);

	// osd buffer info
	memset(&gs_osd_display_buf, 0, sizeof(AB_OSD_BUF_ST));
	gs_osd_display_buf.e_color_fmt = AB_OSD_COLOR_FORMAT_ARGB1555;
	gs_osd_display_buf.puc_buf_vir_addr = (uint8_t *)malloc(s_setup_osd.ui_display_w*s_setup_osd.ui_display_h*2);
	gs_osd_display_buf.ui_buf_phy_addr = 0;
	gs_osd_display_buf.ui_width = s_setup_osd.ui_display_w;
	gs_osd_display_buf.ui_height = s_setup_osd.ui_display_h;
	gs_osd_display_buf.ui_line_jump_pixels = s_setup_osd.ui_display_w;

	memset(&gs_osd_enc_buf, 0, sizeof(AB_OSD_BUF_ST));
	gs_osd_enc_buf.e_color_fmt = AB_OSD_COLOR_FORMAT_ARGB1555;
	gs_osd_enc_buf.puc_buf_vir_addr = (uint8_t *)malloc(s_setup_osd.ui_enc_w*s_setup_osd.ui_enc_h*2);
	gs_osd_enc_buf.ui_buf_phy_addr = 0;
	gs_osd_enc_buf.ui_width = s_setup_osd.ui_enc_w;
	gs_osd_enc_buf.ui_height = s_setup_osd.ui_enc_h;
	gs_osd_enc_buf.ui_line_jump_pixels = s_setup_osd.ui_enc_w;

	s_setup_osd.b_is_init = 1;
}

void host_setup_osd_deinit(void)
{
	if(s_setup_osd.b_is_init == 0)return;
	
	//host_osd_deinit();

	// osd genenal info
	s_setup_osd.b_is_init = 0;
    s_setup_osd.b_display_need_update = FALSE;
    s_setup_osd.b_enc_need_update = FALSE;
	s_setup_osd.wIndex = 0;

	// osd dbg info
	s_setup_osd.ui_display_draw_time_intervel = 0;
	s_setup_osd.ui_enc_draw_time_intervel = 0;

	memset(s_setup_osd.ui_main_disabled, 0, SETUPMENU_TOTAL);
	memset(s_setup_osd.ui_image_disabled, 0, HOSTMI_IMAGE_TOTAL);
	memset(s_setup_osd.ui_hdmi_disabled, 0, HOSTMI_HDMI_TOTAL);
	memset(s_setup_osd.ui_hdmi_resolution_disabled, 0, HOSTMI_HDMI_RESOLUTION_TOTAL);
	memset(s_setup_osd.ui_factory_main_disabled, 0, SETUP_FACTORY_MENU_TOTAL);
	memset(s_setup_osd.ui_adimage_disabled, 0, HOSTMI_ADIMAGE_TOTAL);
	memset(s_setup_osd.ui_main_turnoff, 0, SETUPMENU_TOTAL);
	s_setup_osd.ui_osd_type = HOST_OSD_TYPE_NULL;

	// osd setting info
    s_setup_osd.ui_display_font_size = 20;
    s_setup_osd.ui_enc_font_size = 20;
	s_setup_osd.ui_display_base_w = 0;
	s_setup_osd.ui_display_base_h = 0;
	s_setup_osd.ui_display_x = 0;
	s_setup_osd.ui_display_y = 0;
	s_setup_osd.ui_display_w = 0;
	s_setup_osd.ui_display_h = 0;
	s_setup_osd.ui_enc_base_w = 0;
	s_setup_osd.ui_enc_base_h = 0;
	s_setup_osd.ui_enc_x = 0;
	s_setup_osd.ui_enc_y = 0;
	s_setup_osd.ui_enc_w = 0;
	s_setup_osd.ui_enc_h = 0;

	// osd buffer info
	free(gs_osd_display_buf.puc_buf_vir_addr);
	gs_osd_display_buf.puc_buf_vir_addr = NULL;
	gs_osd_display_buf.e_color_fmt = AB_OSD_COLOR_FORMAT_ARGB1555;
	gs_osd_display_buf.ui_buf_phy_addr = 0;
	gs_osd_display_buf.ui_width = 0;
	gs_osd_display_buf.ui_height = 0;
	gs_osd_display_buf.ui_line_jump_pixels = 0;

	free(gs_osd_enc_buf.puc_buf_vir_addr);
	gs_osd_enc_buf.puc_buf_vir_addr = NULL;
	gs_osd_enc_buf.e_color_fmt = AB_OSD_COLOR_FORMAT_ARGB1555;
	gs_osd_enc_buf.ui_buf_phy_addr = 0;
	gs_osd_enc_buf.ui_width = 0;
	gs_osd_enc_buf.ui_height = 0;
	gs_osd_enc_buf.ui_line_jump_pixels = 0;
}


