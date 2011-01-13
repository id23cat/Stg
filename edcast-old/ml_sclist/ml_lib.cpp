/*
** Copyright (C) 2003 Nullsoft, Inc.
**
** This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held 
** liable for any damages arising from the use of this software. 
**
** Permission is granted to anyone to use this software for any purpose, including commercial applications, and to 
** alter it and redistribute it freely, subject to the following restrictions:
**
**   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. 
**      If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
**
**   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
**
**   3. This notice may not be removed or altered from any source distribution.
**
*/

#include <windows.h>
#include "ml.h"

void freeRecord(itemRecord *p) {
  free(p->title);
  free(p->artist);
  free(p->comment);
  free(p->album);
  free(p->genre);
  free(p->filename);
}

void freeRecordList(itemRecordList *obj)
{
  emptyRecordList(obj);
  free(obj->Items);
  obj->Items=0;
  obj->Alloc=obj->Size=0;
}

void emptyRecordList(itemRecordList *obj)
{
  itemRecord *p=obj->Items;
  while (obj->Size-->0)
  {
    freeRecord(p);
    p++;
  }
  obj->Size=0;
}

void allocRecordList(itemRecordList *obj, int newsize, int granularity)
{
  if (newsize < obj->Alloc || newsize < obj->Size) return;

  obj->Alloc=newsize+granularity;
  obj->Items=(itemRecord*)realloc(obj->Items,sizeof(itemRecord)*obj->Alloc);
  if (!obj->Items) obj->Alloc=0;
}
