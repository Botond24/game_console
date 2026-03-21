# game_console
name tbd

## Firmware Block Diagram

```mermaid
flowchart TD
    SD[(SD Card\nFAT32)]

    subgraph Core0 ["Core 0 -- Bootloader"]
        Menu["Menu\n- Lists .bin files from SD\n- Selects & launches game"]
        Overlay["Overlay Loop\n- Monitors A+B+UP\n- Kills game, returns to menu"]
        HAL["HAL\n- draw_pixel()\n- read_input()"]
    end

    subgraph Core1 ["Core 1 -- User Game"]
        Game["Game Binary\n- Loaded into RAM @ 0x20010000\n- Owns its own loop\n- Calls HAL only"]
    end

    subgraph Hardware ["Peripherals"]
        OLED["OLED Display\n(mutex-guarded)"]
        BTN[Buttons]
    end

    SD -->|"Read once at launch"| Menu
    Menu -->|"memcpy to 0x20010000\npush addr via FIFO"| Game
    Menu -->|"starts"| Overlay
    Overlay -->|"rp2040.restartCore1()\non A+B+UP"| Game
    Game -->|"calls"| HAL
    HAL -->|"drawPixel()"| OLED
    HAL -->|"reads"| BTN
```

## Folders

- `bootloader/` - Core bootloader firmware for the RP2040 that runs on Core 0. You don't need to touch this unless you want to.
- `game/` - Template game project structure and example game firmware that runs on Core 1. This is the base template for developing new games.
- `pcb/` - Hardware design files including the KiCad PCB schematic, layout, footprints library, and production files (BOM, positions, netlist).
- `test-games/` - Test games and development utilities for validation and demonstration purposes.


## Notes 

- Everything is incredibly rudimentary and you shouldn't expect us to respond to issues. We welcome contributions though, so if you want to add features or fix bugs, fork the repo and send a PR.
- Add your game to the [discussions tab](https://github.com/CircuitReeRUG/game_console/discussions/4)! We would love to see what you make with this!
- You can tag @frogspyder on the Cover Discord server if you want to chat about anything related to this project and/or drop by in the [Discord thread](https://discord.com/channels/1073220526585159681/1484931557876826284).
