/*-----------------------------------------------------------------------------+
|                                                                              |
| filename: main.c                                                             |
| project:  ZX Spectrum Next - P(rint)W(orking)D(irectory)                     |
| author:   Stefan Zell                                                        |
| date:     10/18/2025                                                         |
|                                                                              |
+------------------------------------------------------------------------------+
|                                                                              |
| description:                                                                 |
|                                                                              |
| Application to print current working directory in ZX Spectrum Next           |
|                                                                              |
+------------------------------------------------------------------------------+
|                                                                              |
| Copyright (c) 10/18/2025 STZ Engineering                                     |
|                                                                              |
| This software is provided  "as is",  without warranty of any kind, express   |
| or implied. In no event shall STZ or its contributors be held liable for any |
| direct, indirect, incidental, special or consequential damages arising out   |
| of the use of or inability to use this software.                             |
|                                                                              |
| Permission is granted to anyone  to use this  software for any purpose,      |
| including commercial applications,  and to alter it and redistribute it      |
| freely, subject to the following restrictions:                               |
|                                                                              |
| 1. Redistributions of source code must retain the above copyright            |
|    notice, definition, disclaimer, and this list of conditions.              |
|                                                                              |
| 2. Redistributions in binary form must reproduce the above copyright         |
|    notice, definition, disclaimer, and this list of conditions in            |
|    documentation and/or other materials provided with the distribution.      |
|                                                                          ;-) |
+-----------------------------------------------------------------------------*/

/*============================================================================*/
/*                               Includes                                     */
/*============================================================================*/
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#include <arch/zxn.h>
#include <arch/zxn/esxdos.h>

#include "version.h"
#include "pwd.h"

/*============================================================================*/
/*                               Defines                                      */
/*============================================================================*/
#define END_OF_LIST (0x7FFF)

/*============================================================================*/
/*                               Namespaces                                   */
/*============================================================================*/

/*============================================================================*/
/*                               Strukturen                                   */
/*============================================================================*/

/*============================================================================*/
/*                               Typ-Definitionen                             */
/*============================================================================*/
/*!
Structure of an entry of the global error code and text table
*/
typedef struct _errentry
{
  /*!
  Error code
  */
  int iCode;

  /*!
  Pointer to a textual description
  */
  const unsigned char* acText;
} errentry_t;

/*============================================================================*/
/*                               Konstanten                                   */
/*============================================================================*/

/*============================================================================*/
/*                               Variablen                                    */
/*============================================================================*/

/*!
In dieser Struktur werden alle globalen Daten der Anwendung gespeichert.
*/
static struct _state
{
  /*!
  If this flag is set, then this structure is initialized
  */
  bool bInitialized;

  /*!
  Action to execute (help, version, sreenshot, ...)
  */
  action_t eAction;

  /*!
  Backup: Current speed of Z80
  */
  uint8_t uiCpuSpeed;

  /*!
  */
  unsigned char acPathName[ESX_PATHNAME_MAX];

  /*!
  Exitcode of the application, that is handovered to BASIC
  */
  int iExitCode;

} g_tState;

/*============================================================================*/
/*                               Prototypen                                   */
/*============================================================================*/

/*!
Diese Funktion wird einmalig beim Start der Anwendung zur Initialisierung aller
benoetigten Ressourcen aufgerufen.
*/
void _construct(void);

/*!
Diese Funktion wird einmalig beim Beenden der Anwendung zur Freigabe aller
belegten Ressourcen aufgerufen.
*/
void _destruct(void);

/*!
Diese Funktion interpretiert alle Argumente, die der Anwendung uebergeben
wurden.
*/
int parseArguments(int argc, char* argv[]);

/*!
Ausgabe der Hilfe dieser Anwendung.
*/
int showHelp(void);

/*!
Ausgabe der Versionsinformation dieser Anwendung.
*/
int showInfo(void);

/*!
This function prints the current working directory
*/
int printCwd(void);

/*!
This function returns a pointer to a textual error message to given error code.
@param iCode Error code
@return Pointer to a human readable error message
*/
const unsigned char* zxn_strerror(int iCode);

/*!
In this function cleans up the given path ('\\' => '/', remove trailing '/' )
@param acPath Path to clean up
@return "0" = no error
*/
int normalizepath(unsigned char* acPath);

/*============================================================================*/
/*                               Klassen                                      */
/*============================================================================*/

/*============================================================================*/
/*                               Implementierung                              */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/* _construct()                                                               */
/*----------------------------------------------------------------------------*/
void _construct(void)
{
  g_tState.eAction       = ACTION_NONE;
  g_tState.iExitCode     = EOK;
  g_tState.acPathName[0] = '\0';
  g_tState.uiCpuSpeed    = ZXN_READ_REG(REG_TURBO_MODE) & 0x03;

  ZXN_NEXTREG(REG_TURBO_MODE, RTM_28MHZ);

  g_tState.bInitialized  = true;
}


/*----------------------------------------------------------------------------*/
/* _destruct()                                                                */
/*----------------------------------------------------------------------------*/
void _destruct(void)
{
  if (g_tState.bInitialized)
  {
    ZXN_WRITE_REG(REG_TURBO_MODE, g_tState.uiCpuSpeed);
  }
}


/*----------------------------------------------------------------------------*/
/* main()                                                                     */
/*----------------------------------------------------------------------------*/
int main(int argc, char* argv[])
{
  _construct();
  atexit(_destruct);

  if (EOK == (g_tState.iExitCode = parseArguments(argc, argv)))
  {
    switch (g_tState.eAction)
    {
      case ACTION_NONE:
        g_tState.iExitCode = EOK;
        break;

      case ACTION_INFO:
        g_tState.iExitCode = showInfo();
        break;

      case ACTION_HELP:
        g_tState.iExitCode = showHelp();
        break;

      case ACTION_PRINT:
        g_tState.iExitCode = printCwd();
        break;
    }
  }

  return (int) (EOK == g_tState.iExitCode ? 0 : zxn_strerror(g_tState.iExitCode));
}


/*----------------------------------------------------------------------------*/
/* parseArguments()                                                           */
/*----------------------------------------------------------------------------*/
int parseArguments(int argc, char* argv[])
{
  int iReturn = EOK;

  g_tState.eAction = ACTION_NONE;

  int i = 1;

  while (i < argc)
  {
    const char* acArg = argv[i];

    if ('-' == acArg[0])
    {
      if ((0 == stricmp(acArg, "-h")) || (0 == stricmp(acArg, "--help")))
      {
        g_tState.eAction = ACTION_HELP;
      }
      else if ((0 == strcmp(acArg, "-v")) || (0 == stricmp(acArg, "--version")))
      {
        g_tState.eAction = ACTION_INFO;
      }
      else
      {
        fprintf(stderr, "unknown option: %s\n", acArg);
        iReturn = EINVAL;
        break;
      }
    }
    else
    {
      fprintf(stderr, "unexpected argument: %s\n", acArg);
      iReturn = EINVAL;
      break;
    }

    ++i;
  }

  if (ACTION_NONE == g_tState.eAction)
  {
    g_tState.eAction = ACTION_PRINT;
  }

  return iReturn;
}


/*----------------------------------------------------------------------------*/
/* showHelp()                                                                 */
/*----------------------------------------------------------------------------*/
int showHelp(void)
{
  unsigned char acAppName[0x10];
  strncpy(acAppName, VER_INTERNALNAME_STR, sizeof(acAppName));
  strupr(acAppName);

  printf("%s\n\n", VER_FILEDESCRIPTION_STR);

  printf("%s\n\n", "For anyone (like me) who keeps forgetting that \"CD\" " \
         "does exactly the same thing ...");

  printf("%s [-h][-v]\n\n", acAppName);
  //      0.........1.........2.........3.
  printf(" -h[elp]     print this help\n");
  printf(" -v[ersion]  print version info\n");

  return EOK;
}


/*----------------------------------------------------------------------------*/
/* showInfo()                                                                 */
/*----------------------------------------------------------------------------*/
int showInfo(void)
{
  uint16_t uiOsVersion = esx_m_dosversion();

  unsigned char acAppName[0x10];
  strncpy(acAppName, VER_INTERNALNAME_STR, sizeof(acAppName));
  strupr(acAppName);

  printf("%s " VER_LEGALCOPYRIGHT_STR "\n", acAppName);
  //      0.........1.........2.........3.
  printf(" Version %s (NextOS %d.%02d)\n",
         VER_FILEVERSION_STR,
         ESX_DOSVERSION_NEXTOS_MAJOR(uiOsVersion),
         ESX_DOSVERSION_NEXTOS_MINOR(uiOsVersion));
  printf(" Stefan Zell (info@diezells.de)\n");

  return EOK;
}


/*----------------------------------------------------------------------------*/
/* printCwd()                                                                 */
/*----------------------------------------------------------------------------*/
int printCwd(void)
{
  int iReturn = EOK;

  if (0 == (iReturn = esx_f_getcwd(g_tState.acPathName)))
  {
    normalizepath(g_tState.acPathName);
    fprintf(stdout, "%s\n", g_tState.acPathName);
  }
  else
  {
    fprintf(stderr, "reading cwd failed: %d\n", iReturn);
    iReturn = EBADF;
  }

  return iReturn;
}


/*----------------------------------------------------------------------------*/
/* normalizepath()                                                            */
/*----------------------------------------------------------------------------*/
int normalizepath(unsigned char* acPath)
{
  /*
  ZX Spectrum Next Pfad-Normalisierung (in-place, joinbar)
  - '\\' => '/'
  - doppelte '/' zu einem '/'
  - trailing '/' entfernen (ausser bei "/" => "/.")
  - "/" wird zu "/."   (joinbar, aber bleibt im Root)
  - "X:/" wird zu "X:"
  Rueckgabe: EOK oder EINVAL bei Fehler.
  */

  if (NULL == acPath)
  {
    return EINVAL;
  }

  /* 1) '\' => '/' und doppelte '/' entfernen */
  size_t r = 0, w = 0;
  while ('\0' != acPath[r])
  {
    char c = acPath[r++];

    if ('\\' == c)
    {
      c = '/';
    }

    if ('/' == c)
    {
      if ((0 < w) && ('/' == acPath[w - 1]))
      {
        continue;
      }
    }

    acPath[w++] = c;
  }

  acPath[w] = '\0';

  /* 2) Spezialfaelle fuer "joinbare" Basen */
  if ((1 == w) && ('/' == acPath[0]))
  {
    /* "/" => "/." */
    acPath[1] = '.';
    acPath[2] = '\0';
    return EOK;
  }

  if ((3 == w) && isalpha((unsigned char) acPath[0]) && (':' == acPath[1]) && ('/' == acPath[2]))
  {
    /* "X:/" => "X:" */
    acPath[2] = '\0';
    return EOK;
  }

  /* 3) Allgemein: trailing '/' entfernen */
  while ((0 < w) && ('/' == acPath[w - 1]))
  {
    acPath[--w] = '\0';
  }

  return EOK;
}


/*----------------------------------------------------------------------------*/
/* zxn_strerror()                                                             */
/*----------------------------------------------------------------------------*/
const unsigned char* zxn_strerror(int iCode)
{
  /*!
  Table to define textual error messages that are returned to NextOS/BASIC.
  Each text-entry needs to be terminated with BIT7 set in the last character of
  the string. 
  */
  static const errentry_t g_tErrTable[] =
  {
    {EOK,         "no erro"                   "\xF2"}, /* 'r' | 0x80 */
    {EACCES,      "access denie"              "\xE4"}, /* 'd' | 0x80 */
    {EBADF,       "bad fil"                   "\xE5"}, /* 'e' | 0x80 */
    {EBDFD,       "bad file descripto"        "\xF2"}, /* 'r' | 0x80 */
    {EDOM,        "out of domain of functio"  "\xEE"}, /* 'n' | 0x80 */
    {EFBIG,       "file too larg"             "\xE5"}, /* 'e' | 0x80 */
    {EINVAL,      "invalid valu"              "\xE5"}, /* 'e' | 0x80 */
    {EMFILE,      "too many open file"        "\xE5"}, /* 'e' | 0x80 */
    {ENFILE,      "too many open files in syste\xED"}, /* 'm' | 0x80 */
    {ENOLCK,      "no locks availabl"         "\xE5"}, /* 'e' | 0x80 */
    {ENOMEM,      "out of me"                 "\xED"}, /* 'm' | 0x80 */
    {ENOTSUP,     "not supporte"              "\xE4"}, /* 'd' | 0x80 */
    {EOVERFLOW,   "overflo"                   "\xEF"}, /* 'w' | 0x80 */
    {ERANGE,      "out of rang"               "\xE5"}, /* 'e' | 0x80 */
    {ESTAT,       "bad stat"                  "\xF4"}, /* 't' | 0x80 */
    {EAGAIN,      "resource temp. unavailabl" "\xE5"}, /* 'e' | 0x80 */
    {EWOULDBLOCK, "operation would bloc"      "\xEB"}, /* 'k' | 0x80 */
    /* ---------------- APPLICATION SPECIFIC ----------------------- */
    {EBREAK,      "D BREAK - no repea"        "\xF4"}, /* 't' | 0x80 */
    {ETIMEOUT,    "timeout erro"              "\xF2"}, /* 'r' | 0x80 */
    /* ---------------- END-OF-LIST -------------------------------- */
    {END_OF_LIST, "unknown erro"              "\xF2"}  /* 'r' | 0x80 */
  };

  const errentry_t* pIndex = g_tErrTable;

  while (END_OF_LIST != pIndex->iCode)
  {
    if (iCode == pIndex->iCode)
    {
      break;
    }

    ++pIndex;
  }

  return pIndex->acText;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
