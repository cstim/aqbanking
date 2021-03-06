/***************************************************************************
    begin       : Mon Mar 01 2004
    copyright   : (C) 2004-2013 by Martin Preuss
    email       : martin@libchipcard.de

 ***************************************************************************
 *          Please see toplevel file COPYING for license details           *
 ***************************************************************************/


#ifndef AH_ADMINJOBS_P_H
#define AH_ADMINJOBS_P_H

#include "adminjobs_l.h"


/* __________________________________________________________________________
 * AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
 *                             AH_Job_UpdateBank
 * YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY
 */

typedef struct AH_JOB_UPDATEBANK AH_JOB_UPDATEBANK;
struct AH_JOB_UPDATEBANK {
  int scanned;
};
static void GWENHYWFAR_CB AH_Job_UpdateBank_FreeData(void *bp, void *p);

static int AH_Job_UpdateBank_Process(AH_JOB *j, AB_IMEXPORTER_CONTEXT *ctx);




/* __________________________________________________________________________
 * AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
 *                             AH_Job_TestVersion
 * YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY
 */
typedef struct AH_JOB_TESTVERSION AH_JOB_TESTVERSION;
struct AH_JOB_TESTVERSION {
  AH_JOB_TESTVERSION_RESULT versionSupported;
};
static void GWENHYWFAR_CB AH_Job_TestVersion_FreeData(void *bp, void *p);
static int AH_Job_TestVersion_Process(AH_JOB *j, AB_IMEXPORTER_CONTEXT *ctx);




/* __________________________________________________________________________
 * AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
 *                             AH_Job_GetStatus
 * YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY
 */
typedef struct AH_JOB_GETSTATUS AH_JOB_GETSTATUS;
struct AH_JOB_GETSTATUS {
  AH_RESULT_LIST *results;
  GWEN_TIME *fromDate;
  GWEN_TIME *toDate;
};
static void GWENHYWFAR_CB AH_Job_GetStatus_FreeData(void *bp, void *p);
static int AH_Job_GetStatus_Process(AH_JOB *j, AB_IMEXPORTER_CONTEXT *ctx);



/* __________________________________________________________________________
 * AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
 *                             AH_Job_GetItanModes
 * YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY
 */
#define AH_JOB_GETITANMODES_MAXMODES 20
typedef struct AH_JOB_GETITANMODES AH_JOB_GETITANMODES;
struct AH_JOB_GETITANMODES {
  int modeList[AH_JOB_GETITANMODES_MAXMODES+1];
  int modeCount;
};
static void GWENHYWFAR_CB AH_Job_GetItanModes_FreeData(void *bp, void *p);
static int AH_Job_GetItanModes_Process(AH_JOB *j, AB_IMEXPORTER_CONTEXT *ctx);




#endif /* AH_ADMINJOBS_P_H */



