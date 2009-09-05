/***************************************************************************
 begin       : Thu Jun 18 2009
 copyright   : (C) 2009 by Martin Preuss
 email       : martin@libchipcard.de

 ***************************************************************************
 * This file is part of the project "AqBanking".                           *
 * Please see toplevel file COPYING of that project for license details.   *
 ***************************************************************************/


#ifndef AQBANKING_GUI_P_H
#define AQBANKING_GUI_P_H


#include "abgui.h"


typedef struct AB_GUI AB_GUI;
struct AB_GUI {
  AB_BANKING *banking;
  GWEN_GUI_CHECKCERT_FN checkCertFn;
};

static void AB_Gui_FreeData(void *bp, void *p);
static int AB_Gui__HashPair(const char *token,
			    const char *pin,
			    GWEN_BUFFER *buf);
static int AB_Gui_CheckCert(GWEN_GUI *gui,
			    const GWEN_SSLCERTDESCR *cd,
			    GWEN_IO_LAYER *io, uint32_t guiid);



#endif

