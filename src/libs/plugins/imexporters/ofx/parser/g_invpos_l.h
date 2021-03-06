/***************************************************************************
 begin       : Mon Jan 07 2008
 copyright   : (C) 2008 by Martin Preuss
 email       : martin@libchipcard.de

 ***************************************************************************
 *          Please see toplevel file COPYING for license details           *
 ***************************************************************************/


#ifndef AIO_OFX_G_INVPOS_L_H
#define AIO_OFX_G_INVPOS_L_H


#include "ofxgroup_l.h"
#include <aqbanking/types/security.h>



AIO_OFX_GROUP *AIO_OfxGroup_INVPOS_new(const char *groupName,
                                       AIO_OFX_GROUP *parent,
                                       GWEN_XML_CONTEXT *ctx);

AB_SECURITY *AIO_OfxGroup_INVPOS_TakeSecurity(const AIO_OFX_GROUP *g);


#endif
