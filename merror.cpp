/*
 *      merror.cpp -- Kapture
 *
 *      Copyright (C) 2006-2009
 *          Detlev Casanova (detlev.casanova@gmail.com)
 *
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2 of the License, or
 *      (at your option) any later version.
 *
 */

#include <stdio.h>
#include "merror.h"

#ifdef DEBUG
void KError(const char* error, int errorno)
{
	printf(" E %s (Error %d)\n", error, errorno);
#else
void KError(const char*, int)
{
#endif
}

#ifdef DEBUG
void KError(const QString& error, int errorno)
{
	printf(" E %s (Error %d)\n", error.toLatin1().constData(), errorno);
#else
void KError(const QString&, int)
{
#endif
}
