/* rlm_easpanel.c
 *
 *   (C) Copyright 2018 Fred Gleason <fredg@paravelsystems.com>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License version 2
 *   as published by the Free Software Foundation.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public
 *   License along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * This is a Rivendell Loadable Module.  It sends Now&Next PAD data via
 * UDP packets to the configured EASPanel system(s).
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <rlm/rlm.h>

int rlm_easpanel_devs;
char *rlm_easpanel_addresses;
uint16_t *rlm_easpanel_ports;

void rlm_easpanel_RLMStart(void *ptr,const char *arg)
{
  char address[17];
  char section[256];
  char errtext[256];
  int i=1;

  rlm_easpanel_devs=0;
  rlm_easpanel_addresses=NULL;
  rlm_easpanel_ports=NULL;

  sprintf(section,"Easpanel%d",i++);
  strncpy(address,RLMGetStringValue(ptr,arg,section,"IpAddress",""),15);
  if(strlen(address)==0) {
    RLMLog(ptr,LOG_WARNING,"rlm_easpanel: no easpanel destinations specified");
    return;
  }
  while(strlen(address)>0) {
    rlm_easpanel_addresses=
      realloc(rlm_easpanel_addresses,(rlm_easpanel_devs+1)*(rlm_easpanel_devs+1)*16);
    strcpy(rlm_easpanel_addresses+16*rlm_easpanel_devs,address);
    rlm_easpanel_ports=realloc(rlm_easpanel_ports,(rlm_easpanel_devs+1)*sizeof(uint16_t));
    rlm_easpanel_ports[rlm_easpanel_devs]=
      RLMGetIntegerValue(ptr,arg,section,"UdpPort",0);

    sprintf(errtext,"rlm_easpanel: configured destination \"%s:%d\"",address,
	    rlm_easpanel_ports[rlm_easpanel_devs]);
    rlm_easpanel_devs++;
    RLMLog(ptr,LOG_INFO,errtext);
    sprintf(section,"Easpanel%d",i++);
    strncpy(address,RLMGetStringValue(ptr,arg,section,"IpAddress",""),15);
  }
}


void rlm_easpanel_RLMFree(void *ptr)
{
  free(rlm_easpanel_addresses);
  free(rlm_easpanel_ports);
}


void rlm_easpanel_RLMPadDataSent(void *ptr,const struct rlm_svc *svc,
			    const struct rlm_log *log,
			    const struct rlm_pad *now,
			    const struct rlm_pad *next)
{
  int i;
  char str[1500];
  char msg[1500];

  for(i=0;i<rlm_easpanel_devs;i++) {
    snprintf(str,1500,"%s\t%u",now->rlm_group,now->rlm_cartnum);
    RLMSendUdp(ptr,rlm_easpanel_addresses+i*16,rlm_easpanel_ports[i],str,strlen(str));
    snprintf(msg,1500,"rlm_easpanel: sending pad update: \"%s\"",str);
    RLMLog(ptr,LOG_INFO,msg);
  }
}
