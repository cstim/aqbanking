/***************************************************************************
 $RCSfile$
 -------------------
 cvs         : $Id$
 begin       : Thu Apr 29 2004
 copyright   : (C) 2004 by Martin Preuss
 email       : martin@libchipcard.de

 ***************************************************************************
 *          Please see toplevel file COPYING for license details           *
 ***************************************************************************/


#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include "dtaus_p.h"
#include "dtaus-export_p.h"
#include <aqhbci/aqhbci.h>
#include <gwenhywfar/text.h>
#include <gwenhywfar/debug.h>
#include <gwenhywfar/gwentime.h>

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#ifdef HAVE_LOCALE_H
# include <locale.h>
#endif


int AHB_DTAUS__ToDTA(int c) {
  if (isdigit(c))
    return c;
  if (c>='A' && c<='Z')
    return c;
  if (c>='a' && c<='z')
    return toupper(c);
  if (strchr(" .,&-+*%/$", c))
    return c;
  switch(c) {
  case '�': return 0x5b;
  case '�': return 0x5c;
  case '�': return 0x5d;
  case '�': return 0x7e;
  default:
    break;
  } /* switch */
  return 0;
}



int AHB_DTAUS__AddWord(GWEN_BUFFER *dst,
                       unsigned int size,
                       const char *s) {
  unsigned int i;
  unsigned int ssize;

  assert(dst);
  assert(size);
  assert(s);

  DBG_DEBUG(AQHBCI_LOGDOMAIN, "Adding word: %s", s);

  ssize=strlen(s);
  if (ssize>size) {
    DBG_WARN(AQHBCI_LOGDOMAIN, "Word too long, chopping it \"%s\" (%d>%d)",
             s, ssize, size);
  }

  for (i=0; i<size; i++) {
    char c;

    if (i>=ssize)
      c=0;
    else
      c=AHB_DTAUS__ToDTA(s[i]);

    if (c)
      GWEN_Buffer_AppendByte(dst, c);
    else
      GWEN_Buffer_AppendByte(dst, ' ');
  } /* for */
  return 0;
}



int AHB_DTAUS__AddNum(GWEN_BUFFER *dst,
                      unsigned int size,
                      const char *s) {
  unsigned int i, j;

  assert(dst);
  assert(s);

  DBG_DEBUG(AQHBCI_LOGDOMAIN, "Adding num : %s", s);

  i=strlen(s);
  if (i>size) {
    DBG_WARN(AQHBCI_LOGDOMAIN, "Word too long, chopping it");
    i=size;
  }
  j=size-i;
  if (j) {
    unsigned int k;

    for (k=0; k<j; k++)
      GWEN_Buffer_AppendByte(dst, '0');
  }
  GWEN_Buffer_AppendString(dst, s);
  return 0;
}



int AHB_DTAUS__CreateSetA(GWEN_BUFFER *dst,
                          GWEN_DB_NODE *cfg) {
  unsigned int i;
  GWEN_TIME *gt;
  char buffer[16];
  int day, month, year;
  const char *p;
  GWEN_DB_NODE *dbT;

  DBG_DEBUG(AQHBCI_LOGDOMAIN, "Creating A set");
  /* field 1, 2: record header */
  GWEN_Buffer_AppendString(dst, "0128A");

  /* field 3: type */
  p=GWEN_DB_GetCharValue(cfg, "type", 0, "transfer");
  if (strcasecmp(p, "transfer")==0 ||
      strcasecmp(p, "transaction")==0)
    GWEN_Buffer_AppendString(dst, "GK");
  else if (strcasecmp(p, "debitnote")==0)
    GWEN_Buffer_AppendString(dst, "LK");
  else {
    DBG_ERROR(AQHBCI_LOGDOMAIN, "Unknown group \"%s\"", GWEN_DB_GroupName(cfg));
    return -1;
  }

  /* field 4: bank code */
  if (AHB_DTAUS__AddNum(dst, 8,
                          GWEN_DB_GetCharValue(cfg,
                                               "bankCode",
                                               0, "0"))) {
    DBG_ERROR(AQHBCI_LOGDOMAIN, "Error writing to buffer");
    return -1;
  }

  /* field 5: 0s */
  for (i=0; i<8; i++) GWEN_Buffer_AppendByte(dst, '0');

  /* field 6: sender name */
  if (AHB_DTAUS__AddWord(dst, 27,
                         GWEN_DB_GetCharValue(cfg,
                                              "name",
                                              0, "0"))) {
    DBG_ERROR(AQHBCI_LOGDOMAIN, "Error writing to buffer");
    return -1;
  }

  /* field 7: date */
  gt=GWEN_CurrentTime();
  if (GWEN_Time_GetBrokenDownDate(gt, &day, &month, &year)) {
    DBG_ERROR(AQHBCI_LOGDOMAIN, "Unable to break down date");
    GWEN_Time_free(gt);
    return -1;
  }
  GWEN_Time_free(gt);
  snprintf(buffer, sizeof(buffer), "%02d%02d%02d", day, month+1, year%100);
  if (AHB_DTAUS__AddWord(dst, 6, buffer)) {
    DBG_ERROR(AQHBCI_LOGDOMAIN, "Error writing to buffer");
    return -1;
  }

  /* field 8: blanks */
  for (i=0; i<4; i++) GWEN_Buffer_AppendByte(dst, ' ');

  /* field 9: account id */
  if (AHB_DTAUS__AddNum(dst, 10,
                        GWEN_DB_GetCharValue(cfg,
                                             "accountid",
                                             0, ""))) {
    DBG_ERROR(AQHBCI_LOGDOMAIN, "Error writing to buffer");
    return -1;
  }

  /* field 10: customer reference */
  if (AHB_DTAUS__AddNum(dst, 10,
                        GWEN_DB_GetCharValue(cfg,
                                             "custref",
                                             0, "0"))) {
    DBG_ERROR(AQHBCI_LOGDOMAIN, "Error writing to buffer");
    return -1;
  }

  /* field 11a: blanks */
  for (i=0; i<15; i++) GWEN_Buffer_AppendByte(dst, ' ');

  /* field 11b: date of execution */
  dbT=GWEN_DB_GetGroup(cfg, GWEN_PATH_FLAGS_NAMEMUSTEXIST, "execdate");
  if (dbT) {
    GWEN_TIME *ti;

    ti=GWEN_Time_fromDb(dbT);
    if (!ti) {
      DBG_ERROR(AQHBCI_LOGDOMAIN, "Bad execution date");
      return -1;
    }

    if (GWEN_Time_GetBrokenDownDate(ti, &day, &month, &year)) {
      DBG_ERROR(AQHBCI_LOGDOMAIN, "Bad execution date");
      GWEN_Time_free(ti);
      return -1;
    }
    snprintf(buffer, sizeof(buffer), "%02d%02d%04d", day, month+1, year);

    if (AHB_DTAUS__AddWord(dst, 8, buffer)) {
      DBG_ERROR(AQHBCI_LOGDOMAIN, "Error writing to buffer");
      GWEN_Time_free(ti);
      return -1;
    }
    GWEN_Time_free(ti);
  }
  else {
    if (AHB_DTAUS__AddWord(dst, 8, "")) {
      DBG_ERROR(AQHBCI_LOGDOMAIN, "Error writing to buffer");
      return -1;
    }
  }

  /* field 11c: blanks */
  for (i=0; i<24; i++) GWEN_Buffer_AppendByte(dst, ' ');

  /* field 12: currency */
  p=GWEN_DB_GetCharValue(cfg, "currency", 0, "EUR");
  if (strcasecmp(p, "EUR")==0)
    GWEN_Buffer_AppendByte(dst, '1');
  else if (strcasecmp(p, "DEM")==0)
    GWEN_Buffer_AppendByte(dst, ' ');
  else {
    DBG_ERROR(AQHBCI_LOGDOMAIN, "Unknown currency \"%s\"", p);
    return -1;
  }

  return 0;
}



double AHB_DTAUS__string2double(const char *s){
  assert(s);

  while(*s && isspace(*s)) s++;
  if (!*s) {
    DBG_INFO(AQHBCI_LOGDOMAIN, "Empty value");
    return 0.0;
  }
  else {
    unsigned int i;
    double d;
    char c;
    int rv;
    char numbuf[128];
#ifdef HAVE_SETLOCALE
    const char *currentLocale;
#endif

    /* get floating point */
    i=0;
    c=0;
    while(*s) {
      c=*s;
      if (c==',')
        c='.';
      else if (!isdigit(c)) {
        DBG_ERROR(AQHBCI_LOGDOMAIN, "Non-digit character in value");
        return 0;
      }
      assert(i<sizeof(numbuf)-1);
      numbuf[i++]=c;
      s++;
    }
    if (c=='.') {
      /* last was comma, make sure at least one digit follows */
      assert(i<sizeof(numbuf)-1);
      numbuf[i++]='0';
    }
    numbuf[i]=0;
#ifdef HAVE_SETLOCALE
    currentLocale=setlocale(LC_NUMERIC, 0);
    setlocale(LC_NUMERIC, "C");
#endif
    rv=sscanf(numbuf, "%lf", &d);
#ifdef HAVE_SETLOCALE
    setlocale(LC_NUMERIC, currentLocale);
#endif
    if (rv!=1) {
      DBG_ERROR(AQHBCI_LOGDOMAIN, "Could not read floating point value");
      return 0;
    }

    return d;
  }
}



int AHB_DTAUS__CreateSetC(GWEN_BUFFER *dst,
                          GWEN_DB_NODE *cfg,
                          GWEN_DB_NODE *xa,
                          double *sumEUR,
                          double *sumDEM,
                          double *sumBankCodes,
                          double *sumAccountIds){
  unsigned int i;
  const char *p;
  char buffer[32];
  int isDebitNote;
  int isEuro;
  unsigned int extSets;
  unsigned int startPos;
  double dd;

  DBG_DEBUG(AQHBCI_LOGDOMAIN, "Creating C set");

  /* ______________________________________________________________________
   * preparations
   */
  startPos=GWEN_Buffer_GetPos(dst);
  GWEN_Buffer_AllocRoom(dst, 256);

  isDebitNote=(strcasecmp(GWEN_DB_GetCharValue(cfg, "type", 0, "transfer"),
                          "debitnote")==0);
  isEuro=(strcasecmp(GWEN_DB_GetCharValue(cfg, "currency", 0, "EUR"),
                     "EUR")==0);

  /* compute number of extension sets */
  extSets=0;
  /* add purpose */
  for (i=1; ; i++) {
    if (GWEN_DB_GetCharValue(xa, "purpose", i, 0)==0)
      break;
    if (i>14) {
      DBG_ERROR(AQHBCI_LOGDOMAIN, "Too many purpose lines (maxmimum is 14)");
      return -1;
    }
    extSets++;
  } /* for */

  /* add name */
  for (i=1; ; i++) {
    if (GWEN_DB_GetCharValue(xa, "localName", i, 0)==0)
      break;
    if (i>1) {
      DBG_ERROR(AQHBCI_LOGDOMAIN, "Too many name lines (maxmimum is 2)");
      return -1;
    }
    extSets++;
  } /* for */

  /* add other name */
  for (i=1; ; i++) {
    if (GWEN_DB_GetCharValue(xa, "remoteName", i, 0)==0)
      break;
    if (i>1) {
      DBG_ERROR(AQHBCI_LOGDOMAIN, "Too many peer name lines (maxmimum is 2)");
      return -1;
    }
    extSets++;
  } /* for */

  /* check number of extension sets */
  if (extSets>15) {
    DBG_ERROR(AQHBCI_LOGDOMAIN, "Too many extension sets (%d)", extSets);
    return -1;
  }

  /* ______________________________________________________________________
   * actually write C set
   */

  /* field 1, 2: record header */
  snprintf(buffer, sizeof(buffer), "%04d", 187+(extSets*29));
  GWEN_Buffer_AppendString(dst, buffer);
  GWEN_Buffer_AppendByte(dst, 'C');

  /* field 3: acting bank code */
  if (AHB_DTAUS__AddNum(dst, 8,
                        GWEN_DB_GetCharValue(cfg,
                                             "bankCode",
                                             0, ""))) {
    DBG_ERROR(AQHBCI_LOGDOMAIN, "Error writing to buffer");
    return -1;
  }

  /* field 4: destination bank code */
  p=GWEN_DB_GetCharValue(xa,
                         "remoteAccountNumber",
                         0, 0);
  if (p) {
    if (1!=sscanf(p, "%lf", &dd)) {
      DBG_ERROR(AQHBCI_LOGDOMAIN, "Bad bank code");
      return -1;
    }
    *sumBankCodes+=dd;
    if (AHB_DTAUS__AddNum(dst, 8, p)) {
      DBG_ERROR(AQHBCI_LOGDOMAIN, "Error writing to buffer");
      return -1;
    }
  }
  else {
    DBG_ERROR(AQHBCI_LOGDOMAIN, "Peer bank code missing");
    return -1;
  }

  /* field 5: destination account id */
  p=GWEN_DB_GetCharValue(xa,
                         "remoteAccountNumber",
                         0, 0);
  if (p) {
    if (1!=sscanf(p, "%lf", &dd)) {
      DBG_ERROR(AQHBCI_LOGDOMAIN, "Bad account id");
      return -1;
    }
    *sumAccountIds+=dd;
    if (AHB_DTAUS__AddNum(dst, 10, p)) {
      DBG_ERROR(AQHBCI_LOGDOMAIN, "Error writing to buffer");
      return -1;
    }
  }
  else {
    DBG_ERROR(AQHBCI_LOGDOMAIN, "Peer account id missing");
    return -1;
  }

  /* field 6: inernal customer number (0s for now) */
  for (i=0; i<13; i++) GWEN_Buffer_AppendByte(dst, '0');

  /* field 7a: text key */
  snprintf(buffer, sizeof(buffer), "%02d",
           GWEN_DB_GetIntValue(xa, "textkey", 0, isDebitNote?5:51));
  if (AHB_DTAUS__AddNum(dst, 2, buffer)) {
    DBG_ERROR(AQHBCI_LOGDOMAIN, "Error writing to buffer");
    return -1;
  }

  /* field 7b: text key extension */
  snprintf(buffer, sizeof(buffer), "%03d",
           GWEN_DB_GetIntValue(xa, "textkeyext", 0, 0));
  if (AHB_DTAUS__AddNum(dst, 3, buffer)) {
    DBG_ERROR(AQHBCI_LOGDOMAIN, "Error writing to buffer");
    return -1;
  }

  /* field 8: bank internal field */
  GWEN_Buffer_AppendByte(dst, ' ');

  /* field 9: value in DEM */
  if (!isEuro) {
    dd=AHB_DTAUS__string2double(GWEN_DB_GetCharValue(xa,
                                                     "value/value",
                                                     0, "0,"));
    *sumDEM+=dd;
    snprintf(buffer, sizeof(buffer), "%011.0lf", dd*100.0);
    if (AHB_DTAUS__AddNum(dst, 11, buffer)) {
      DBG_ERROR(AQHBCI_LOGDOMAIN, "Error writing to buffer");
      return -1;
    }
  }
  else {
    if (AHB_DTAUS__AddNum(dst, 11, "0")) {
      DBG_ERROR(AQHBCI_LOGDOMAIN, "Error writing to buffer");
      return -1;
    }
  }

  /* field 10: first bank */
  if (AHB_DTAUS__AddNum(dst, 8,
                        GWEN_DB_GetCharValue(cfg,
                                             "bankCode",
                                             0, ""))) {
    DBG_ERROR(AQHBCI_LOGDOMAIN, "Error writing to buffer");
    return -1;
  }

  /* field 11: destination account id */
  if (AHB_DTAUS__AddNum(dst, 10,
                          GWEN_DB_GetCharValue(xa,
                                               "remoteAccountNumber",
                                               0, ""))) {
    DBG_ERROR(AQHBCI_LOGDOMAIN, "Error writing to buffer");
    return -1;
  }

  /* field 12: value in EUR */
  if (isEuro) {
    dd=AHB_DTAUS__string2double(GWEN_DB_GetCharValue(xa,
                                                     "value/value",
                                                     0, "0,"));
    *sumEUR+=dd;
    snprintf(buffer, sizeof(buffer), "%011.0lf", dd*100.0);
    if (AHB_DTAUS__AddNum(dst, 11, buffer)) {
      DBG_ERROR(AQHBCI_LOGDOMAIN, "Error writing to buffer");
      return -1;
    }
  }
  else {
    if (AHB_DTAUS__AddNum(dst, 11, "0")) {
      DBG_ERROR(AQHBCI_LOGDOMAIN, "Error writing to buffer");
      return -1;
    }
  }

  /* field 13: blanks */
  for (i=0; i<3; i++) GWEN_Buffer_AppendByte(dst, ' ');

  /* field 14a: peer name */
  if (AHB_DTAUS__AddWord(dst, 27,
                           GWEN_DB_GetCharValue(xa,
                                                "remoteName",
                                                0, ""))) {
    DBG_ERROR(AQHBCI_LOGDOMAIN, "Error writing to buffer");
    return -1;
  }

  /* field 14b: blanks */
  for (i=0; i<8; i++) GWEN_Buffer_AppendByte(dst, ' ');

  /* field 15: name */
  if (AHB_DTAUS__AddWord(dst, 27,
                         GWEN_DB_GetCharValue(xa,
                                              "localname",
                                              0, ""))) {
    DBG_ERROR(AQHBCI_LOGDOMAIN, "Error writing to buffer");
    return -1;
  }

  /* field 16: purpose */
  if (AHB_DTAUS__AddWord(dst, 27,
                         GWEN_DB_GetCharValue(xa,
                                              "purpose",
                                              0, ""))) {
    DBG_ERROR(AQHBCI_LOGDOMAIN, "Error writing to buffer");
    return -1;
  }

  /* field 17a: currency */
  if (isEuro)
    GWEN_Buffer_AppendByte(dst, '1');
  else
    GWEN_Buffer_AppendByte(dst, ' ');

  /* field 17b: blanks */
  for (i=0; i<2; i++) GWEN_Buffer_AppendByte(dst, ' ');

  /* field 18: number of extension sets */
  snprintf(buffer, sizeof(buffer), "%02d", extSets);
  if (AHB_DTAUS__AddNum(dst, 2, buffer)) {
    DBG_ERROR(AQHBCI_LOGDOMAIN, "Error writing to buffer");
    return -1;
  }

  /* really append extension sets */
  extSets=0;

  /* add peer name lines */
  for (i=1; ; i++) {
    unsigned int j;

    p=GWEN_DB_GetCharValue(xa, "remoteName", i, 0);
    if (!p)
      break;

    /* append extension set */
    GWEN_Buffer_AppendString(dst, "01");
    if (AHB_DTAUS__AddWord(dst, 27, p)) {
      DBG_ERROR(AQHBCI_LOGDOMAIN, "Error writing to buffer");
      return -1;
    }

    extSets++;
    if (extSets==2) {
      for (j=0; j<11; j++) GWEN_Buffer_AppendByte(dst, ' ');
    }
    else if (extSets>2) {
      if (((extSets-2)%4)==0) {
        for (j=0; j<12; j++) GWEN_Buffer_AppendByte(dst, ' ');
      }
    }
  } /* for */

  /* add purpose lines */
  for (i=1; ; i++) {
    unsigned int j;

    p=GWEN_DB_GetCharValue(xa, "purpose", i, 0);
    if (!p)
      break;

    /* append extension set */
    GWEN_Buffer_AppendString(dst, "02");
    if (AHB_DTAUS__AddWord(dst, 27, p)) {
      DBG_ERROR(AQHBCI_LOGDOMAIN, "Error writing to buffer");
      return -1;
    }

    extSets++;
    if (extSets==2) {
      for (j=0; j<11; j++) GWEN_Buffer_AppendByte(dst, ' ');
    }
    else if (extSets>2) {
      if (((extSets-2)%4)==0) {
        for (j=0; j<12; j++) GWEN_Buffer_AppendByte(dst, ' ');
      }
    }
  } /* for */

  /* add name lines */
  for (i=1; ; i++) {
    unsigned int j;

    p=GWEN_DB_GetCharValue(xa, "localname", i, 0);
    if (!p)
      break;

    /* append extension set */
    GWEN_Buffer_AppendString(dst, "03");
    if (AHB_DTAUS__AddWord(dst, 27, p)) {
      DBG_ERROR(AQHBCI_LOGDOMAIN, "Error writing to buffer");
      return -1;
    }

    extSets++;
    if (extSets==2) {
      for (j=0; j<11; j++) GWEN_Buffer_AppendByte(dst, ' ');
    }
    else if (extSets>2) {
      if (((extSets-2)%4)==0) {
        for (j=0; j<12; j++) GWEN_Buffer_AppendByte(dst, ' ');
      }
    }
  } /* for */


  if (extSets<=2) {
    unsigned int size;
    unsigned int j;

    /* align buffer to 256 */
    size=(GWEN_Buffer_GetPos(dst)-startPos);
    assert(size<256);
    j=256-size;
    for (i=0; i<j; i++) GWEN_Buffer_AppendByte(dst, ' ');
  }
  else {
    /* align buffer to 128 */
    unsigned int size;
    unsigned int j;

    size=(GWEN_Buffer_GetPos(dst)-startPos)-256;
    j=((size+127)&~127)-size;
    if (j)
      for (i=0; i<j; i++) GWEN_Buffer_AppendByte(dst, ' ');
  }

  return 0;
}



int AHB_DTAUS__CreateSetE(GWEN_BUFFER *dst,
                          GWEN_DB_NODE *cfg,
                          int csets,
                          double sumEUR,
                          double sumDEM,
                          double sumBankCodes,
                          double sumAccountIds){
  unsigned int i;
  char buffer[32];

  DBG_DEBUG(AQHBCI_LOGDOMAIN, "Creating E set");

  /* field 1, 2: record header */
  GWEN_Buffer_AppendString(dst, "0128E");

  /* field 3: reserved */
  for (i=0; i<5; i++) GWEN_Buffer_AppendByte(dst, ' ');

  /* field 4: number of C sets */
  snprintf(buffer, sizeof(buffer), "%07d", csets);
  if (AHB_DTAUS__AddNum(dst, 7, buffer)) {
    DBG_ERROR(AQHBCI_LOGDOMAIN, "Error writing to buffer");
    return -1;
  }

  /* field 5: sum of DEM values */
  snprintf(buffer, sizeof(buffer), "%013.0lf", sumDEM*100.0);
  if (AHB_DTAUS__AddNum(dst, 13, buffer)) {
    DBG_ERROR(AQHBCI_LOGDOMAIN, "Error writing to buffer");
    return -1;
  }

  /* field 6: sum of peer account ids */
  snprintf(buffer, sizeof(buffer), "%017.0lf", sumAccountIds);
  if (AHB_DTAUS__AddNum(dst, 17, buffer)) {
    DBG_ERROR(AQHBCI_LOGDOMAIN, "Error writing to buffer");
    return -1;
  }

  /* field 7: sum of peer bank codes */
  snprintf(buffer, sizeof(buffer), "%017.0lf", sumBankCodes);
  if (AHB_DTAUS__AddNum(dst, 17, buffer)) {
    DBG_ERROR(AQHBCI_LOGDOMAIN, "Error writing to buffer");
    return -1;
  }

  /* field 8: sum of EUR values */
  snprintf(buffer, sizeof(buffer), "%013.0lf", sumEUR*100.0);
  if (AHB_DTAUS__AddNum(dst, 13, buffer)) {
    DBG_ERROR(AQHBCI_LOGDOMAIN, "Error writing to buffer");
    return -1;
  }

  /* field 9: reserved */
  for (i=0; i<51; i++) GWEN_Buffer_AppendByte(dst, ' ');

  return 0;
}



int AHB_DTAUS__Export(GWEN_DBIO *dbio,
                      GWEN_BUFFEREDIO *bio,
                      GWEN_TYPE_UINT32 flags,
                      GWEN_DB_NODE *data,
                      GWEN_DB_NODE *cfg){
  double sumEUR;
  double sumDEM;
  double sumBankCodes;
  double sumAccountIds;
  unsigned int cSets;
  GWEN_BUFFER *dst;
  GWEN_DB_NODE *gr;
  int isDebitNote;
  int isEuro;
  const char *p;
  GWEN_TYPE_UINT32 size;
  GWEN_TYPE_UINT32 bytesWritten;

  isDebitNote=(strcasecmp(GWEN_DB_GetCharValue(cfg, "type", 0, "transfer"),
                          "debitnote")==0);
  isEuro=(strcasecmp(GWEN_DB_GetCharValue(cfg, "currency", 0, "EUR"),
                     "EUR")==0);
  cSets=0;
  sumEUR=0;
  sumDEM=0;
  sumBankCodes=0;
  sumAccountIds=0;

  dst=GWEN_Buffer_new(0, 1024, 0, 1);
  GWEN_Buffer_SetHardLimit(dst, 64*1024);

  /* create A set */
  if (AHB_DTAUS__CreateSetA(dst, cfg)) {
    DBG_INFO(AQHBCI_LOGDOMAIN, "Error creating A set");
    GWEN_Buffer_free(dst);
    return -1;
  }

  /* create C sets */
  gr=GWEN_DB_GetFirstGroup(data);
  while(gr) {
    if ((isDebitNote && strcasecmp(GWEN_DB_GroupName(gr), "debitnote")==0) ||
        (!isDebitNote && strcasecmp(GWEN_DB_GroupName(gr), "transfer")==0)){
      if (AHB_DTAUS__CreateSetC(dst, cfg, gr,
                                &sumEUR, &sumDEM,
                                &sumBankCodes, &sumAccountIds)) {
        DBG_ERROR(AQHBCI_LOGDOMAIN, "Error creating C set from this data:");
        GWEN_DB_Dump(gr, stderr, 2);
        GWEN_Buffer_free(dst);
        return -1;
      }
      cSets++;
    } /* if group matches */
    gr=GWEN_DB_GetNextGroup(gr);
  } /* while */

  /* create E set */
  if (AHB_DTAUS__CreateSetE(dst, cfg, cSets,
                            sumEUR, sumDEM,
                            sumBankCodes, sumAccountIds)) {
    DBG_INFO(AQHBCI_LOGDOMAIN, "Error creating E set");
    GWEN_Buffer_free(dst);
    return -1;
  }

  /* DTAUS finished, write it */
  p=GWEN_Buffer_GetStart(dst);
  size=GWEN_Buffer_GetUsedBytes(dst);
  bytesWritten=0;
  while(bytesWritten<size) {
    GWEN_ERRORCODE err;
    unsigned int bsize;

    bsize=size-bytesWritten;
    err=GWEN_BufferedIO_WriteRaw(bio, p, &bsize);
    if (!GWEN_Error_IsOk(err)) {
      DBG_ERROR_ERR(AQHBCI_LOGDOMAIN, err);
      GWEN_Buffer_free(dst);
      return -1;
    }
    if (bsize==0) {
      DBG_ERROR(AQHBCI_LOGDOMAIN, "Broken pipe");
      GWEN_Buffer_free(dst);
      return -1;
    }
    p+=bsize;
    bytesWritten+=bsize;
  } /* while */

  GWEN_Buffer_free(dst);
  return 0;
}










