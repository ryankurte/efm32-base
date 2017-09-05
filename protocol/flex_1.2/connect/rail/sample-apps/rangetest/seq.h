/***************************************************************************//**
 * @file seq.h
 * @brief Program sequences for the Range Test Software Example.
 * @copyright Copyright 2015 Silicon Laboratories, Inc. http://www.silabs.com
 ******************************************************************************/

#ifndef SEQ_H_
#define SEQ_H_

// ----------------------------------------------------------------------------
// Type definitions

// Function pointer types
typedef bool (*seqInputHandler_t)(pbState_t *);
typedef void (*seqDisplayHandler_t)(GLIB_Context_t *);
typedef bool (*seqRunner_t)();

// Kinds of Range Test program states
enum {
  SEQ_INIT,
  SEQ_MENU,
  SEQ_TRX,
};

// Sequences each Range Test Mode has in terms of what kinds of functions
// have to be called in each Mode
typedef struct {
  seqInputHandler_t input;
  seqDisplayHandler_t display;
  seqRunner_t runner;
} seqHandler_t;

// Sequence handlers arranged in a state machine-like variable.
extern seqHandler_t seqHandler;

/**************************************************************************//**
 * Initializes the sequence of the Range Test program execution
 *****************************************************************************/
void seqInit(void);

/**************************************************************************//**
 * Sequence execution of the Range Test based on the sequence handler state
 * machine.
 *****************************************************************************/
void seqSet(uint8_t newIdx);

/**************************************************************************//**
 * Changes the mode of operation into to another one.
 *****************************************************************************/
void seqRun();

#endif /* SEQ_H_ */
