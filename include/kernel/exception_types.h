#ifndef EXCEPTION_TYPES_H
#define EXCEPTION_TYPES_H

#define STORED_FRAME_SIZE 272 // all the registers

// Exception Types
#define SYNC_EL1T          0x01
#define IRQ_EL1T           0x02
#define FIQ_EL1T           0x03
#define SERROR_EL1T        0x04

#define SYNC_EL1H          0x05
#define IRQ_EL1H           0x06
#define FIQ_EL1H           0x07
#define SERROR_EL1H        0x08

#define SYNC_EL0_64        0x09
#define IRQ_EL0_64         0x0A
#define FIQ_EL0_64         0x0B 
#define SERROR_EL0_64      0x0C

#define SYNC_EL0_32        0x0D
#define IRQ_EL0_32         0x0E 
#define FIQ_EL0_32         0x0F
#define SERROR_EL0_32      0x10



#endif
