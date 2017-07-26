/* Linker script to configure memory regions. */

SEARCH_DIR(.)
GROUP(-lgcc -lc -lnosys)

MEMORY
{
  /* FLASH (rx):    ORIGIN = 0x1b000, LENGTH = 0x25000   */ /* QFAC */
  /* RAM (rwx) :    ORIGIN = 0x20001870, LENGTH = 0x6790 */ /* QFAC */
   FLASH (rx)  :    ORIGIN = 0x1B000,      LENGTH = 0x25000
   RAM (rwx)   :    ORIGIN = 0x20002450,   LENGTH = 0x1BB0
   
}

SECTIONS
{
  .fs_data :
  {
    PROVIDE(__start_fs_data = .);
    KEEP(*(.fs_data))
    PROVIDE(__stop_fs_data = .);
  } > RAM
} INSERT AFTER .data;

INCLUDE "nrf5x_common.ld"