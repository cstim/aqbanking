/***************************************************************************
 $RCSfile$
 -------------------
 cvs         : $Id$
 begin       : Mon Mar 01 2004
 copyright   : (C) 2004 by Martin Preuss
 email       : martin@libchipcard.de

 ***************************************************************************
 *          Please see toplevel file COPYING for license details           *
 ***************************************************************************/


#ifndef AQBANKING_ACCOUNT_L_H
#define AQBANKING_ACCOUNT_L_H

#include <gwenhywfar/misc.h>
#include <gwenhywfar/misc2.h>
#include <gwenhywfar/inherit.h>
#include <aqbanking/account_be.h>

GWEN_LIST_FUNCTION_DEFS(AB_ACCOUNT, AB_Account)


int AB_Account_Update(AB_ACCOUNT *a);


#endif /* AQBANKING_ACCOUNT_L_H */
