/***************************************************************************
    begin       : Mon Mar 01 2004
    copyright   : (C) 2019 by Martin Preuss
    email       : martin@libchipcard.de

 ***************************************************************************
 *          Please see toplevel file COPYING for license details           *
 ***************************************************************************/


#ifndef AQEBICS_CLIENT_R_UNSECURED_L_H
#define AQEBICS_CLIENT_R_UNSECURED_L_H

#include "aqebics/client/provider_l.h"

#include <aqbanking/backendsupport/provider.h>
#include <aqbanking/backendsupport/user.h>
#include <aqbanking/backendsupport/httpsession.h>



EB_MSG *EBC_Provider_MkUnsecuredRequest(AB_PROVIDER *pro,
                                        AB_USER *u,
                                        const char *orderType,
                                        const char *orderAttribute,
                                        const char *orderData);

EB_MSG *EBC_Provider_MkUnsecuredRequest_H004(AB_PROVIDER *pro,
                                             AB_USER *u,
                                             const char *orderType,
                                             const char *orderAttribute,
                                             const char *orderData);


#endif

