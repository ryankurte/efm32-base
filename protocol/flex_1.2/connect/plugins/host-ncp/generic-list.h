// File: generic-list.h
//
// Description: A generic List structure
//
// Copyright 2012 by Ember Corporation. All rights reserved.                *80*

#ifndef __EMBER_LIST_H__
#define __EMBER_LIST_H__

struct ListElement_t {
  uint8_t status;
};

typedef struct ListElement_t ListElement;

//
// return values for a sort function:
// -1 = less than
//  0 = equal
//  1 = greater than
//
typedef int8_t (*ListElementSortFunction)(const ListElement *first,
                                          const ListElement *second);

//
// an option compare function for when comparing
// is different from sorting
//
typedef bool (*ListElementCompareFunction)(const ListElement *first,
                                           const ListElement *second);

typedef void (*ListElementMarkFunction)(ListElement *element, Marker mark);

enum ListFlags_e {
  LIST_REMOVE_TAIL_WHEN_FULL = 1,
  LIST_REMOVE_HEAD_WHEN_FULL = 2,
  LIST_REMOVE_NONE_WHEN_FULL = 4
};

struct ListType_t {
  uint16_t sizeofElement;
  uint16_t maxElementCount;
  uint8_t flags;
  ListElementSortFunction sortFunction;
  ListElementCompareFunction compareFunction;
  ListElementMarkFunction marker;
};

typedef struct ListType_t ListType;

//
// Query the list
//
#define emListCanAddMore(list, listType) \
  (listType.maxElementCount > 0          \
   && emListGetSize(list) < listType.maxElementCount - 1)

//
// Get a ListElement from a List
//
ListElement *emListGet(const ListType *listType,
                       Buffer list,
                       const ListElement *element,
                       uint8_t *optionalReportedIndex);

ListElement *emListGetFromIndex(Buffer list, uint8_t index);
#define emListGetHead(list) emListGetFromIndex(list, 0)

//
// Get the size
//
uint8_t emListGetSize(Buffer list);

//
// Delete an element from the list that equates to the given ListElement
//
bool emListDelete(const ListType *listType,
                  Buffer *list,
                  const ListElement *element);

bool emListDeleteFromIndex(const ListType *listType,
                           Buffer *list,
                           uint8_t index);

#define emListPop(listType, list) \
  emListDeleteFromIndex(listType, list, 0)

//
// Add an element to the list that equates to the given ListElement
// The result will differ pointer-wise from the given ListElement
//
ListElement *emListAdd(const ListType *listType,
                       Buffer *list,
                       const ListElement *elementToAdd,
                       uint8_t *optionalReportedIndex);

ListElement *emListReallyAdd(const ListType *listType,
                             Buffer *list,
                             const ListElement *elementToAdd,
                             uint8_t *optionalReportedIndex,
                             bool doPush);

#define emListPush(listType, list, element) \
  emListReallyAdd(listType, list, element, NULL, true)

//
// Mark the list
//
void emListMark(const ListType *listType, Buffer *list, Marker mark);

#endif
