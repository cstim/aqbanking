/***************************************************************************
    begin       : Mon Mar 01 2004
    copyright   : (C) 2019 by Martin Preuss
    email       : martin@libchipcard.de

 ***************************************************************************
 *          Please see toplevel file COPYING for license details           *
 ***************************************************************************/

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif


#include "r_download_l.h"

#include "aqebics/aqebics_l.h"
#include "aqebics/msg/msg.h"
#include "aqebics/msg/keys.h"
#include "aqebics/msg/zip.h"
#include "aqebics/msg/xml.h"
#include "aqebics/client/user_l.h"

#include <gwenhywfar/base64.h>
#include <gwenhywfar/gui.h>
#include <gwenhywfar/httpsession.h>




int EBC_Provider_XchgDownloadRequest(AB_PROVIDER *pro,
                                     GWEN_HTTP_SESSION *sess,
                                     AB_USER *u,
                                     const char *requestType,
                                     GWEN_BUFFER *targetBuffer,
                                     int withReceipt,
                                     const GWEN_DATE *fromDate,
                                     const GWEN_DATE *toDate)
{
  const char *s;

  s=EBC_User_GetProtoVersion(u);
  if (!(s && *s))
    s="H002";
  if (strcasecmp(s, "H002")==0)
    return EBC_Provider_XchgDownloadRequest_H002(pro, sess, u, requestType, targetBuffer,
                                                 withReceipt,
                                                 fromDate, toDate);
  else if (strcasecmp(s, "H003")==0)
    return EBC_Provider_XchgDownloadRequest_H003(pro, sess, u, requestType, targetBuffer,
                                                 withReceipt,
                                                 fromDate, toDate);
  else {
    DBG_ERROR(AQEBICS_LOGDOMAIN, "Proto version [%s] not supported", s);
    return GWEN_ERROR_INTERNAL;
  }
}






