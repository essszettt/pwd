/*-----------------------------------------------------------------------------+
|                                                                              |
| filename: pwd.h                                                              |
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

#if !defined(__PWD_H__)
  #define __PWD_H__

/*============================================================================*/
/*                               Includes                                     */
/*============================================================================*/

/*============================================================================*/
/*                               Defines                                      */
/*============================================================================*/
/*!
Beginning of project specific error codes.
*/
#define ERROR_SPECIFIC (0x0200)

/*!
Error code: BREAK was pressed; abort execution
*/
#define EBREAK (ERROR_SPECIFIC + 0x00)

/*!
Error code: Timeout in operation
*/
#define ETIMEOUT (ERROR_SPECIFIC + 0x01)

#ifndef ERANGE
  /*!
  (Re)definition of error "ERANGE". Maybe just a type in <errno.h> that needs to
  be corrected ...
  */
  #define ERANGE __ERANGE
  #warning "ERANGE not defined in <errno.h> (typo ?)"
#endif

#ifndef RTM_28MHZ
  /*!
  Missing constant for 28-Mhz-speed of ZXN (missing in <zxn.h>)
  */
  #define RTM_28MHZ 0x03
  #warning "RTM_28MHZ not defined in <zxn.h>"
#endif

/*!
This macro can be used like "printf" but it's implementation is only compiled
and linked in debug-builds ("#define __DEBUG__").
*/
#ifdef __DEBUG__
  #define DBGPRINTF(...) do { (void) printf(__VA_ARGS__); } while (0)
#else
  #define DBGPRINTF(...) do { } while (0)
#endif

/*============================================================================*/
/*                               Namespaces                                   */
/*============================================================================*/

/*============================================================================*/
/*                               Konstanten                                   */
/*============================================================================*/

/*============================================================================*/
/*                               Variablen                                    */
/*============================================================================*/

/*============================================================================*/
/*                               Strukturen                                   */
/*============================================================================*/

/*============================================================================*/
/*                               Typ-Definitionen                             */
/*============================================================================*/
/*!
Enumeration/list of all actions the application can execute
*/
typedef enum _action
{
  ACTION_NONE = 0,
  ACTION_HELP,
  ACTION_INFO,
  ACTION_PRINT
} action_t;

/*============================================================================*/
/*                               Prototypen                                   */
/*============================================================================*/

/*============================================================================*/
/*                               Klassen                                      */
/*============================================================================*/

/*============================================================================*/
/*                               Implementierung                              */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

#endif /* __PWD_H__ */
