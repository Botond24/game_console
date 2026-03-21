ENTRY(game_entry)

MEMORY
{
  GAME_RAM (rwx) : ORIGIN = 0x20010000, LENGTH = 0x00030000
}

SECTIONS
{
  .text :
  {
    *(.text.game_entry)
    *(.text*)
    *(.rodata*)
  } > GAME_RAM

  .data :
  {
    *(.data*)
  } > GAME_RAM

  .bss (NOLOAD) :
  {
    *(.bss*)
    *(COMMON)
  } > GAME_RAM

  /DISCARD/ :
  {
    *(.comment*)
    *(.note*)
    *(.eh_frame*)
  }
}
