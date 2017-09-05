// File: generic-list.c
//
// Description: Generic List Functionality
//
// Copyright 2012 by Ember Corporation. All rights reserved.                *80*

#include PLATFORM_HEADER
#include "stack/include/ember-types.h"
#include "hal/hal.h"
#ifdef UNIX_HOST
#include "host-ncp/host-buffer-management.h"
#else
#include "stack/framework/buffer-management.h"
#endif
#include "generic-list.h"

ListElement *emListGet(const ListType *listType,
                       Buffer list,
                       const ListElement *element,
                       uint8_t *optionalReportedIndex)
{
  ListElement *gotten = NULL;
  Buffer this;
  uint8_t i = 0;

  if (optionalReportedIndex != NULL) {
    *optionalReportedIndex = 0xFF;
  }

  for (this = list; this != NULL_BUFFER; this = emGetQueueLink(this)) {
    ListElement *thisElement = (ListElement*)emGetBufferPointer(this);

    //
    // first check the compare function
    // if it's NULL, check the sort function
    //
    if (listType->compareFunction != NULL) {
      if (listType->compareFunction(thisElement, element)) {
        gotten = thisElement;
        break;
      }
    } else if (listType->sortFunction != NULL
               && listType->sortFunction(thisElement, element) == 0) {
      gotten = thisElement;
      break;
    }

    i++;
  }

  if (gotten != NULL
      && optionalReportedIndex != NULL) {
    *optionalReportedIndex = i;
  }

  return gotten;
}

ListElement *emListGetFromIndex(Buffer list, uint8_t index)
{
  ListElement *gotten = NULL;
  uint8_t i = 0;
  Buffer this;

  for (this = list; this != NULL_BUFFER; this = emGetQueueLink(this), i++) {
    if (i == index) {
      gotten = (ListElement*)emGetBufferPointer(this);
      break;
    }
  }

  return gotten;
}

uint8_t emListGetSize(Buffer list)
{
  Buffer this = list;
  uint8_t size = 0;

  while (this != NULL_BUFFER) {
    size++;
    this = emGetQueueLink(this);
  }

  return size;
}

static void popTail(Buffer list)
{
  Buffer this = list;
  Buffer previous = this;

  while (this != NULL_BUFFER && emGetQueueLink(this) != NULL_BUFFER) {
    previous = this;
    this = emGetQueueLink(this);
  }

  emSetQueueLink(previous, NULL_BUFFER);
}

static void popHead(Buffer *list)
{
  *list = emGetQueueLink(*list);
}

static void deleteFromBuffer(Buffer *list,
                             Buffer this,
                             Buffer previous)
{
  if (this == *list) {
    *list = emGetQueueLink(*list);
  } else {
    emSetQueueLink(previous, emGetQueueLink(this));
  }
}

bool emListDeleteFromIndex(const ListType *listType,
                           Buffer *list,
                           uint8_t index)
{
  Buffer this;
  Buffer previous;
  bool deleted = false;
  uint8_t i = 0;

  for (this = *list, previous = this;
       this != NULL_BUFFER;
       previous = this, this = emGetQueueLink(this), i++) {
    if (i == index) {
      deleteFromBuffer(list, this, previous);
      deleted = true;
      break;
    }
  }

  return deleted;
}

bool emListDelete(const ListType *listType,
                  Buffer *list,
                  const ListElement *element)
{
  Buffer this;
  Buffer previous;
  bool deleted = false;

  for (this = *list, previous = this;
       this != NULL_BUFFER;
       previous = this, this = emGetQueueLink(this)) {
    const ListElement *thisElement =
      (const ListElement*)emGetBufferPointer(this);

    if (listType->compareFunction(thisElement, element)) {
      // delete it now!
      deleteFromBuffer(list, this, previous);
      deleted = true;
      break;
    }
  }

  return deleted;
}

ListElement *emListAdd(const ListType *listType,
                       Buffer *list,
                       const ListElement *elementToAdd,
                       uint8_t *optionalReportedIndex)
{
  return emListReallyAdd(listType, list, elementToAdd, optionalReportedIndex, false);
}

ListElement *emListReallyAdd(const ListType *listType,
                             Buffer *list,
                             const ListElement *elementToAdd,
                             uint8_t *optionalReportedIndex,
                             bool doPush)
{
  // does it already exist?
  ListElement *gotten =
    emListGet(listType, *list, elementToAdd, optionalReportedIndex);
  ListElement *result = NULL;

  assert(list != NULL);

  if (gotten != NULL) {
    // it already exists, update
    MEMMOVE(gotten, elementToAdd, listType->sizeofElement);
    return gotten;
  }

  uint8_t i;
  Buffer this;
  Buffer previous;
  uint8_t size = emListGetSize(*list);

  if (size + 1 > listType->maxElementCount) {
    // the list is too big, remove an element!
    if ((listType->flags & LIST_REMOVE_TAIL_WHEN_FULL) != 0) {
      popTail(*list);
      assert(emListGetSize(*list) == size - 1);
    } else if ((listType->flags & LIST_REMOVE_HEAD_WHEN_FULL) != 0) {
      popHead(list);
      assert(emListGetSize(*list) == size - 1);
    } else if ((listType->flags & LIST_REMOVE_NONE_WHEN_FULL) != 0) {
      // remove nothing, flee
      return NULL;
    }
  }

  //
  // find where to place the new element
  //
  for (this = *list, previous = this, i = 0;
       this != NULL_BUFFER;
       previous = this, this = emGetQueueLink(this), i++) {
    ListElement *thisElement = (ListElement*)emGetBufferPointer(this);

    // if the list is not ordered,
    // sortFunction will be NULL
    if (!doPush
        && (listType->sortFunction == NULL
            || (listType->sortFunction(elementToAdd, thisElement)
                < 0))) {
      // found it!
      break;
    }
  }

  //
  // add the element
  //
  Buffer newElement = emAllocateBuffer(listType->sizeofElement);
  uint8_t *newElementBytes = NULL;

  if (newElement != NULL_BUFFER) {
    if (optionalReportedIndex != NULL) {
      *optionalReportedIndex = i;
    }

    newElementBytes = emGetBufferPointer(newElement);
    MEMCOPY(newElementBytes, elementToAdd, listType->sizeofElement);

    if (*list == NULL_BUFFER) {
      *list = newElement;
    } else {
      if (this != NULL_BUFFER) {
        emSetQueueLink(newElement, this);
      }

      if (previous != this) {
        emSetQueueLink(previous, newElement);
      }

      if (this == *list) {
        *list = newElement;
      }
    }

    result = (ListElement*)newElementBytes;
  }

  return result;
}

void emListMark(const ListType *listType,
                Buffer *list,
                Marker mark)
{
  Buffer this = *list;

  if (listType->marker != NULL) {
    while (this != NULL_BUFFER) {
      listType->marker((ListElement *)emGetBufferPointer(this), mark);
      this = emGetQueueLink(this);
    }
  }

  mark(list);
}
