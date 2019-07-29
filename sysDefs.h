/***************************************************************************************************
*    System:   TOP Framework
*    Module:   System level definitions
* $Workfile:   sysDefs.h $
*     $Date:   2017-12-13 16:50:46 GMT $
* $Revision:   \main\FW_TOP_Integ\9 $
****************************************************************************************************
* CONFIDENTIAL. All rights reserved. Thales Canada Transportation Solutions. This computer program
* is protected under Copyright. Recipient is to retain the program in confidence, and is not
* permitted to copy, use, distribute, modify or translate the program without authorization.
***************************************************************************************************/

/**
\file
****************************************************************************************************
\brief System-wide defines and typedefs.
***************************************************************************************************/

#ifndef SYSDEFS_H
#define SYSDEFS_H

/***************************************************************************************************
* Include Files
***************************************************************************************************/

/* external files */
#include <stdio.h>
#include <sys/types.h>                                    /* !W1625 Path required by TAS Platform */
#include <endian.h>


/***************************************************************************************************
* Common Constant Declarations for all supported operating systems.
***************************************************************************************************/

#ifndef NULL /* May be already defined indirectly by the tool chain */
/**
****************************************************************************************************
* NULL constant definitions. Use for pointers. This is a default definition in case it is not
* already defined by the TAS-OS.
***************************************************************************************************/
#define NULL ((void*)0)
#endif

/***************************************************************************************************
* Byte order
***************************************************************************************************/
#if __BYTE_ORDER == __LITTLE_ENDIAN
# define LOW_ENDIAN
#else
# if defined(LOW_ENDIAN)
#  error "LOW_ENDIAN defined incorrectly"
# endif
#endif

/**
****************************************************************************************************
* The constant shall be used to check return values for Unix functions which are normally:
* success:  0
* failure: -1 (use #RETURN_FAILURE for failure checking)
***************************************************************************************************/
#define RETURN_SUCCESS (0)

/**
****************************************************************************************************
* The constant shall be used to check return values for Unix functions which are normally:
* success:  0  (use #RETURN_SUCCESS for success checking)
* failure: -1, and errno set to indicate error reason
***************************************************************************************************/
#define RETURN_FAILURE (-1)


#ifndef STATIC
/**
****************************************************************************************************
* A default definition of STATIC is the C language "static" keyword. This definition allows to
* overwrite the default value \c static with "nothing", thus making the static item globally
* accessible for unit testing. To take advantage of this feature all static variables and functions
* should be declared using the STATIC definition not the static keyword.
***************************************************************************************************/
# define STATIC static
#endif


/**
****************************************************************************************************
* TAS Platform provides the function cs_sched_yield to force a TAS pre-emption point.
* See the TAS Platform Programmer's Guide (3CR 00055 0109 PCZZA) for more information.
* The following equate represents the value returned by cs_sched_yield if the call is successful.
***************************************************************************************************/
#define CS_SCHED_YIELD_SUCCESS      (RETURN_SUCCESS)


#define MIN_REDUNDANCY_LEVEL  (2)     /**< Minimum level of redundancy required by TAS Platform */
#define REDUNDANCY_LEVEL_2    (2)     /**< Redundancy level for configuration 2oo2 */
#define REDUNDANCY_LEVEL_3    (3)     /**< Redundancy level for configuration 2oo3 */


#ifndef REDUNDANCY_LEVEL
/**
****************************************************************************************************
* Hardware redundancy level used in the application.
***************************************************************************************************/
# error "REDUNDANCY_LEVEL must be defined"
#endif

#define MP_HW_TYPE_UNDEFINED  (0)       /**< MP board type undefined, To prevent using an undefined hardware type */
#define MP_HW_TYPE_EPCM       (1)       /**< EPCM board used as main processor hardware */
#define MP_HW_TYPE_CIQ3       (2)       /**< PowerPC CIQ3 board used as main processor hardware */

#ifndef MP_HARDWARE_TYPE
/**
****************************************************************************************************
* Definition of the default hardware type (Embedded Pentium).  The external configuration header
* file included in compilation or command line options can override this default setting.
***************************************************************************************************/
#define MP_HARDWARE_TYPE MP_HW_TYPE_EPCM
#endif



/*
****************************************************************************************************
* Definition of bit masks.
***************************************************************************************************/
#define BIT0   (0x00000001L)
#define BIT1   (0x00000002L)
#define BIT2   (0x00000004L)
#define BIT3   (0x00000008L)
#define BIT4   (0x00000010L)
#define BIT5   (0x00000020L)
#define BIT6   (0x00000040L)
#define BIT7   (0x00000080L)
#define BIT8   (0x00000100L)
#define BIT9   (0x00000200L)
#define BIT10  (0x00000400L)
#define BIT11  (0x00000800L)
#define BIT12  (0x00001000L)
#define BIT13  (0x00002000L)
#define BIT14  (0x00004000L)
#define BIT15  (0x00008000L)

#define BIT16  (0x00010000L)
#define BIT17  (0x00020000L)
#define BIT18  (0x00040000L)
#define BIT19  (0x00080000L)
#define BIT20  (0x00100000L)
#define BIT21  (0x00200000L)
#define BIT22  (0x00400000L)
#define BIT23  (0x00800000L)
#define BIT24  (0x01000000L)
#define BIT25  (0x02000000L)
#define BIT26  (0x04000000L)
#define BIT27  (0x08000000L)
#define BIT28  (0x10000000L)
#define BIT29  (0x20000000L)
#define BIT30  (0x40000000L)
#define BIT31  (0x80000000L)

/**
****************************************************************************************************
* Defines the replica ID type.
* The valid values are 1 and 2 for 2oo2 configuration and 1, 2 and 3 for 2oo3 configuration. The
* enumeration also defines the invalid value and minimum/maximum values for the replica ID. Note
* that the maximum values depend on the redundancy level of the system.
***************************************************************************************************/
typedef enum ReplicaId
{
   REPLICA_INVALID_MIN,       /**< Any replica ID matching this value or lower is invalid */
   REPLICA_ALL = 0,           /**< Used to request a broadcast to all replicas in the node */
   REPLICA_MIN = 1,           /**< Used for range checking of replica ID */
   REPLICA_1 = REPLICA_MIN,   /**< The lowest replica ID is 1. Any ID below this is invalid */
   REPLICA_2,                 /**< The highest replica ID in 2oo2 configuration is 2 */
#if REDUNDANCY_LEVEL == REDUNDANCY_LEVEL_2
   REPLICA_MAX = REPLICA_2,   /**< Maximum replica ID in 2oo2 configuration is 2 */
#elif REDUNDANCY_LEVEL == REDUNDANCY_LEVEL_3
   REPLICA_3,                 /**< The highest replica ID in 2oo3 configuration is 3 */
   REPLICA_MAX = REPLICA_3,   /**< Maximum replica ID in 2oo3 configuration is 3 */
#endif
   REPLICA_INVALID_MAX        /**< Any replica ID matching this value or higher is invalid */
} ReplicaId;


/**
****************************************************************************************************
* This enumeration defines the CAN BUS Type.
***************************************************************************************************/
typedef enum CanBusType
{
   CANBUS_NONE = 0,         /**< No CAN Bus */
   CAN_1 = 1,               /**< CAN 1 */
   CAN_2 = 2,               /**< CAN 2 */
   CAN1_AND_CAN2 = 3        /**< CAN 1 and CAN 2 */
} CanBusType;


/**
****************************************************************************************************
* The actual number of used replicas is the same as the configuration redundancy level.
***************************************************************************************************/
#define NUMBER_OF_REPLICAS       (REDUNDANCY_LEVEL)
#define MAX_NUMBER_OF_REPLICAS   (REDUNDANCY_LEVEL_3)  /* Define this for EXIF_T_RP_SERIAL_1_CFG message */


/**
****************************************************************************************************
* Define the minimum (2oo2) number of hardware replicas required by the voting system.
***************************************************************************************************/
#define MIN_NUMBER_OF_HARDWARE_REPLICAS   (MIN_REDUNDANCY_LEVEL)

/**
****************************************************************************************************
* Define the maximum (2oo3) number of hardware replicas required by the voting system.
***************************************************************************************************/
#define MAX_NUMBER_OF_HARDWARE_REPLICAS   (REDUNDANCY_LEVEL_3)


/***************************************************************************************************
* Macro Declarations
***************************************************************************************************/

/**
****************************************************************************************************
* Suppress warning message for unused variables.
***************************************************************************************************/
#define UNUSED_VAR(x)  ((void)(x))

/**
****************************************************************************************************
* This is a default value for DEBUG() macro (i.e., nothing).
* The macro is used to encapsulate the code used during debugging of the code.
* This default definition removes all debug code embedded in this macro from the source code.\n
* To enable the debugging code, see description in the debugMe.h file.
***************************************************************************************************/
#ifndef DEBUG
# define DEBUG(x)
#endif

/**
****************************************************************************************************
* The macro calculates number of elements in an array.  The "array" is the actual array name not a
* type definition. The LENGTHOF macro name is defined to resemble the sizeof() macro/operator of the
* C compiler.
***************************************************************************************************/
#define LENGTHOF(arrayName) (sizeof(arrayName)/sizeof(arrayName[0]))

/**
****************************************************************************************************
* Constant used to multiply the original value in preparation for rounding up an expression using
* the ROUND_UP_UNSIGNED macro.
***************************************************************************************************/
#define ROUNDING_CORRECTION  (10)

/**
****************************************************************************************************
* The ROUND_UP_UNSIGNED() macro is used to round unsigned integer expressions. The expressions must
* be given in the units that are 10x smaller than the expected result.  The macro adds another 5
* units to the number and divides the sum by 10. This operation results in the proper rounding of
* the expression x.
***************************************************************************************************/
#define ROUND_UP_UNSIGNED(x) (((x) + 5) / 10)

/**
****************************************************************************************************
* The ROUND_UP_SIGNED() macro is used to round signed integer expressions. The expression must be
* given in the units that are 10X smaller than the expected result.  The macro adds or subtracts 5
* units to the number and divides the sum by 10 depending on the sign of the expression.  This
* operation results in the proper rounding of the expression x which is a signed value.
* NOTE: Due to possible side effects, the parameter of the macro must not contain any increments,
* decrements, function calls or any expressions that if executed multiple times cause incorrect
* results.
***************************************************************************************************/
#define ROUND_UP_SIGNED(x) ((x) >= 0 ? ((x) + 5) / 10 : ((x) - 5) / 10)


/**
****************************************************************************************************
* The MAX() macro returns the value of the larger parameter.
***************************************************************************************************/
#define MAX(a, b)   (((a) > (b)) ? (a) : (b))


/**
****************************************************************************************************
* The MIN() macro returns the value of the smaller parameter.
***************************************************************************************************/
#define MIN(a, b)   (((a) < (b)) ? (a) : (b))


/**
****************************************************************************************************
* The ABS_DIFF() macro returns the absolute value of the difference between two values.
***************************************************************************************************/

#define ABS_DIFF(a, b) (((a) > (b)) ? ((a) - (b)) : ((b) - (a))) /* !W5040 Multiple parameter use required */


/***************************************************************************************************
* Type Definitions
***************************************************************************************************/

/* 8 bits types */
typedef unsigned char      UTiny;         /**< Unsigned single byte (8-bit) variable */
typedef signed char        STiny;         /**< Signed single byte (8-bit) variable */
typedef unsigned char      TBits;         /**< Tiny bitwise 8-bit variable (e.g. I/O port) */
#define MAX_UNSIGNED_TINY  (0xFF)         /**< Maximum value for unsigned tiny */
#define MAX_SIGNED_TINY    (0x7F)         /**< Maximum value for signed tiny */
#define MIN_SIGNED_TINY    (0x80)         /**< Minimum value for signed tiny */
#define BITS_IN_TINY       (8)            /**< Number of bits in a tiny */
#define BYTES_IN_BYTE      (1)            /**< Number of bytes in a byte */

/* The following type is discontinued; will be removed */
typedef unsigned char Byte;               /**< Unsigned 8-bit byte variable */
#define MAX_UNSIGNED_CHAR  (0xFF)         /**< Maximum value for unsigned character */
#define MAX_SIGNED_CHAR    (0x7F)         /**< Maximum value for signed character */
#define MIN_SIGNED_CHAR    (0x80)         /**< Minimum value for signed character */
#define BITS_IN_BYTE       (8)            /**< Number of bits in a byte */

/* 16 bits types */
typedef unsigned short     UShort;        /**< Unsigned 16-bit short integer variable */
typedef signed short       SShort;        /**< Signed 16-bit short integer variable */
typedef unsigned short     SBits;         /**< Unsigned 16-bit short bitwise variable (e.g. IO port)
                                               Note: The prefix S means "short" not "signed". */
#define MAX_UNSIGNED_SHORT (0xffff)       /**< Maximum value for unsigned short */
#define MAX_SIGNED_SHORT   (0x7fff)       /**< Maximum value for signed short */
#define MIN_SIGNED_SHORT   (0x8000)       /**< Minimum value for signed short */
#define BITS_IN_WORD       (16)           /**< Number of bits in a word */
#define BYTES_IN_WORD      (2)            /**< Number of bytes in a word */

/* 32 bits types */
typedef unsigned long      ULong;         /**< Unsigned 32-bit long variable */
typedef signed long        SLong;         /**< Signed 32-bit long variable */
typedef unsigned long      LBits;         /**< Unsigned 32-bit long bitwise variable */
#define MAX_UNSIGNED_LONG  (0xFFFFFFFF)   /**< Maximum value for unsigned long */
#define MAX_SIGNED_LONG    (0x7FFFFFFF)   /**< Maximum value for signed long */
#define MIN_SIGNED_LONG    (0x80000000)   /**< Minimum value for signed long */
#define BITS_IN_ULONG      (32)           /**< Number of bits in unsigned long */
#define BYTES_IN_LONG      (4)            /**< Number of bytes in unsigned long */

/* 64 bits types */
#ifdef ENABLE_DEBUGGER_CONFIGURATION
typedef unsigned long long ULong64;       /**< Unsigned 64-bit long long variable */
#endif
/* SLong64 Currently defined in vobcDefs.h, but should be moved here */
#define BITS_IN_LONG_LONG  (64)           /**< Number of bits in long long */
#define BYTES_IN_LONG_LONG (4)            /**< Number of bytes in long long */


/**
****************************************************************************************************
** Boolean type definition - enumerated TRUE/FALSE type
***************************************************************************************************/
typedef enum SysBool
{
   FALSE = 0,           /**< Boolean false value */
   TRUE  = 1            /**< Boolean true value */
} SysBool;


/**
****************************************************************************************************
* The definition allows byte swapping according to their location in the unsigned short integer. It
* allows access to individual bytes of the unsigned short.
***************************************************************************************************/
typedef struct Word2BytesType
{
#ifdef LOW_ENDIAN       /* Using LOW instead of LITTLE to avoid clash with TAS-OS definition */
      UTiny low;        /**< Intel; Low bytes stored first */
      UTiny high;       /**< Intel; High bytes stored last */
#else
      UTiny high;       /**< Motorola, PowerPC, Network; High bytes stored first */
      UTiny low;        /**< Motorola, PowerPC, Network; Low bytes stored last */
#endif /* LOW_ENDIAN */
} Word2BytesType;       /**< Byte-wise representation of a word */

typedef union Word2Bytes
{
   UShort word;         /**< Word representation of a variable */
   Word2BytesType byte; /**< Byte-wise representation of an unsigned short variable */
} Word2Bytes;                                                           /* !W1660 Data is related */


/**
****************************************************************************************************
* The definition allows byte swapping according to their location in the unsigned long integer. It
* allows access to individual bytes of the unsigned long.
***************************************************************************************************/
typedef struct Long2BytesType
{
#ifdef LOW_ENDIAN       /* Using LOW instead of LITTLE to avoid clash with TAS-OS definition */
      UTiny b0;         /**< Intel; Low bytes stored first */
      UTiny b1;         /**< Intel; byte storage order */
      UTiny b2;         /**< Intel; byte storage order */
      UTiny b3;         /**< Intel; High bytes stored last */
#else
      UTiny b3;         /**< Motorola, PowerPC, Network; High bytes stored first */
      UTiny b2;         /**< Motorola, PowerPC, Network; byte storage order */
      UTiny b1;         /**< Motorola, PowerPC, Network; byte storage order */
      UTiny b0;         /**< Motorola, PowerPC, Network; Low bytes stored last */
#endif /* LOW_ENDIAN */
} Long2BytesType;       /**< Byte-wise representation of an unsigned long variable */

typedef union Long2Bytes
{
   ULong longWord;      /**< Unsigned long representation of a variable */
   Long2BytesType byte; /**< Byte-wise representation of an unsigned long variable */
} Long2Bytes;                                                           /* !W1660 Data is related */


/* Fixed conversion definitions */

/* Distance */
#define METER_IN_KM           (1000)                       /**< Meters per kilometer */
#define MM_IN_METER           (1000)                       /**< Millimeters per meter */
#define MM_IN_KM              (MM_IN_METER * METER_IN_KM)  /**< Millimeters per kilometer */
#define CM_IN_METER           (100)                        /**< Centimeters per meter */
#define MM_IN_CM              (10)                         /**< Millimeters per centimeter */
#define CM_IN_KM              (METER_IN_KM * CM_IN_METER)  /**< Centimeters per kilometer */
#define M_IN_MILE             (1609)                       /**< Meters per mile rounded down */
#define MM_IN_MILE            (1609344)                    /**< Millimeters per mile */

/* Timing */
#define MICROSECONDS_IN_MILLISECONDS (1000)    /**< Microseconds per millisecond */

/* Other */
#define PERCENT               (100)            /**< Number of 1% units in 1.
                                                * Example: To calculate 6% of X:
                                                * \code
                                                * (X * 6 / PERCENT)
                                                * \endcode
                                                */
#define PERMILLE              (1000)           /**< Number of 0.1% units in 1.
                                                * Example: To calculate 7 per 1000 of X:
                                                * \code
                                                * (X * 7 / PERMILLE)
                                                * \endcode
                                                */

#define PER10K                (10000)          /**< Number of 0.01% units in 1.
                                                * Example: To calculate 7 units of X:
                                                * \code
                                                * (X * 7 / PER10K)
                                                * \endcode
                                                */

#endif /* SYSDEFS_H */
