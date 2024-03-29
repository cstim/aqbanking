/***************************************************************************
 begin       : Sun Oct 27 2019
 copyright   : (C) 2019 by Martin Preuss
 email       : martin@libchipcard.de

 ***************************************************************************
 * This file is part of the project "AqBanking".                           *
 * Please see toplevel file COPYING of that project for license details.   *
 ***************************************************************************/

#ifndef AQFINTS_SESSION_VERIFY_H
#define AQFINTS_SESSION_VERIFY_H


#include "libaqfints/session/session.h"
#include "libaqfints/msg/message.h"



int AQFINTS_Session_VerifyMessage(AQFINTS_SESSION *sess, AQFINTS_MESSAGE *message);

#endif

