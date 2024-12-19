#ifndef _pin_magic_
#define _pin_magic_

// This header file serves two purposes:
//
// 1) Isolate non-portable MCU port- and pin-specific identifiers and
//    operations so the library code itself remains somewhat agnostic
//    (PORTs and pin numbers are always referenced through macros).
//
// 2) GCC doesn't always respect the "inline" keyword, so this is a
//    ham-fisted manner of forcing the issue to minimize function calls.
//    This sometimes makes the library a bit bigger than before, but fast++.
//    However, because they're macros, we need to be SUPER CAREFUL about
//    parameters -- for example, write8(x) may expand to multiple PORT
//    writes that all refer to x, so it needs to be a constant or fixed
//    variable and not something like *ptr++ (which, after macro
//    expansion, may increment the pointer repeatedly and run off into
//    la-la land).  Macros also give us fine-grained control over which
//    operations are inlined on which boards (balancing speed against
//    available program space).

// When using the TFT shield, control and data pins exist in set physical
// locations, but the ports and bitmasks corresponding to each vary among
// boards.  A separate set of pin definitions is given for each supported
// board type.
// When using the TFT breakout board, control pins are configurable but
// the data pins are still fixed -- making every data pin configurable
// would be much too slow.  The data pin layouts are not the same between
// the shield and breakout configurations -- for the latter, pins were
// chosen to keep the tutorial wiring manageable more than making optimal
// use of ports and bitmasks.  So there's a second set of pin definitions
// given for each supported board.

// Shield pin usage:
// LCD Data Bit :    7    6    5    4    3    2    1    0
// Digital pin #:    7    6   13    4   11   10    9    8
// Uno port/pin :  PD7  PD6  PB5  PD4  PB3  PB2  PB1  PB0
// Mega port/pin:  PH4  PH3  PB7  PG5  PB5  PB4  PH6  PH5
// Leo port/pin :  PE6  PD7  PC7  PD4  PB7  PB6  PB5  PB4
// Due port/pin : PC23 PC24 PB27 PC26  PD7 PC29 PC21 PC22
// Breakout pin usage:
// LCD Data Bit :   7   6   5   4   3   2   1   0
// Uno dig. pin :   7   6   5   4   3   2   9   8
// Uno port/pin : PD7 PD6 PD5 PD4 PD3 PD2 PB1 PB0
// Mega dig. pin:  29  28  27  26  25  24  23  22
// Mega port/pin: PA7 PA6 PA5 PA4 PA3 PA2 PA1 PA0 (one contiguous PORT)
// Leo dig. pin :   7   6   5   4   3   2   9   8
// Leo port/pin : PE6 PD7 PC6 PD4 PD0 PD1 PB5 PB4
// Due dig. pin :  40  39  38  37  36  35  34  33
// Due port/pin : PC8 PC7 PC6 PC5 PC4 PC3 PC2 PC1 (one contiguous PORT. -ish…)

// Pixel read operations require a minimum 400 nS delay from RD_ACTIVE
// to polling the input pins.  At 16 MHz, one machine cycle is 62.5 nS.
// This code burns 7 cycles (437.5 nS) doing nothing; the RJMPs are
// equivalent to two NOPs each, final NOP burns the 7th cycle, and the
// last line is a radioactive mutant emoticon.
#define DELAY7                                                                 \
  asm volatile("nop\n\t"                                                       \
               "nop\n\t"                                                       \
               "nop\n\t"                                                       \
               "nop\n\t"                                                       \
               "nop\n\t"                                                       \
               "nop\n\t"                                                       \
               "nop\n\t" ::);


#if defined(ARDUINO_ARCH_NRF52840)

#define write8inline(d)                                                        \
  {                                                                            \
    NRF_P1->OUTSET = ((d&0x3)<<11) | ((d&0x4)<<13) | ((d&0x18)<<10);           \
    NRF_P0->OUTSET = ((d&0x20)<<18) | ((d&0x40)<<15) | ((d&0x80)<<20);         \
    NRF_P1->OUTCLR = ((~d & 0x3) << 11) | ((~d & 0x4) << 13) | ((~d&0x18)<<10);\
    NRF_P0->OUTCLR = ((~d & 0x20) << 18)|((~d & 0x40) << 15) |((~d&0x80)<<20); \      
    WR_STROBE;                                                                 \
  }
#define read8inline(result)                                                    \
  {                                                                            \
    RD_ACTIVE;                                                                 \
    DELAY7;                                                                    \
    result = (((NRF_P1->IN & (0x3 << 11)) >> 11)                                \
      | (NRF_P1->IN & (0x4 << 13) >> 13)                                       \
      | (NRF_P1->IN & (0x18 << 10) >> 10)                                      \
      | ((NRF_P0->IN & (0x20 << 18)) >> 18)                                    \
      | ((NRF_P0->IN & (0x40 << 15)) >> 15)                                    \
      | ((NRF_P0->IN & (0x80 << 20)) >> 20));                                   \
    RD_IDLE;                                                                   \
  }
#define setWriteDirInline()                                                    \
  {                                                                            \
    NRF_P1->DIRSET = (0x3 << 11) | (0x4 << 13) | (0x18 << 10);                 \
    NRF_P0->DIRSET = (0x20 << 18) | (0x40 << 15) | (0x80 << 20);               \ 
  }
#define setReadDirInline()                                                     \
  {                                                                            \
    NRF_P1->DIRCLR = (0x3 << 11) | (0x4 << 13) | (0x18 << 10);                 \
    NRF_P0->DIRCLR = (0x20 << 18) | (0x40 << 15) | (0x80 << 20);               \
  }

#define RD_ACTIVE rdPort->OUTCLR = rdPinSet
#define RD_IDLE rdPort->OUTSET = rdPinSet    // PIO_Set(rdPort, rdPinSet)
#define WR_ACTIVE wrPort->OUTCLR = wrPinSet  // PIO_Clear(wrPort, wrPinSet)
#define WR_IDLE wrPort->OUTSET = wrPinSet    // PIO_Set(wrPort, wrPinSet)
#define CD_COMMAND cdPort->OUTCLR = cdPinSet // PIO_Clear(cdPort, cdPinSet)
#define CD_DATA cdPort->OUTSET = cdPinSet    // PIO_Set(cdPort, cdPinSet)
#define CS_ACTIVE csPort->OUTCLR = csPinSet  // PIO_Clear(csPort, csPinSet)
#define CS_IDLE csPort->OUTSET = csPinSet   

#else

#error "Board type unsupported / not recognized"

#endif

// Data write strobe, ~2 instructions and always inline
#define WR_STROBE                                                              \
  {                                                                            \
    WR_ACTIVE;                                                                 \
    WR_IDLE;                                                                   \
  }

// These higher-level operations are usually functionalized,
// except on Mega where's there's gobs and gobs of program space.

// Set value of TFT register: 8-bit address, 8-bit value
#define writeRegister8inline(a, d)                                             \
  {                                                                            \
    CD_COMMAND;                                                                \
    write8(a);                                                                 \
    CD_DATA;                                                                   \
    write8(d);                                                                 \
  }

// Set value of TFT register: 16-bit address, 16-bit value
// See notes at top about macro expansion, hence hi & lo temp vars
#define writeRegister16inline(a, d)                                            \
  {                                                                            \
    uint8_t hi, lo;                                                            \
    hi = (a) >> 8;                                                             \
    lo = (a);                                                                  \
    CD_COMMAND;                                                                \
    write8(hi);                                                                \
    write8(lo);                                                                \
    hi = (d) >> 8;                                                             \
    lo = (d);                                                                  \
    CD_DATA;                                                                   \
    write8(hi);                                                                \
    write8(lo);                                                                \
  }

// Set value of 2 TFT registers: Two 8-bit addresses (hi & lo), 16-bit value
#define writeRegisterPairInline(aH, aL, d)                                     \
  {                                                                            \
    uint8_t hi = (d) >> 8, lo = (d);                                           \
    CD_COMMAND;                                                                \
    write8(aH);                                                                \
    CD_DATA;                                                                   \
    write8(hi);                                                                \
    CD_COMMAND;                                                                \
    write8(aL);                                                                \
    CD_DATA;                                                                   \
    write8(lo);                                                                \
  }

#endif // _pin_magic_
