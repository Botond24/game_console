# game_console
name tbd

## Firmware Block Diagram

```mermaid
flowchart TD
    SD[(SD Card)]

    subgraph System ["Core System"]
        Loader["Loader\n(@saraanabota, @KrisPuusepp)\n- Polls SD card periodically\n- loads new game images"]
        PeriphAPI["Periph API\n(@confestim, @MrDragonMister)\n- End-user interface\n- Manages OLED & Buttons"]
    end

    subgraph UserSpace ["User Programs"]
        Snake["Snake game (Example)\n(@justtryingthingsout, @PatrickGM1)"]
        OtherGames["User game"]
    end

    subgraph Hardware ["Peripherals"]
        OLED[OLED Display]
        BTN[Buttons]
    end

    SD -.->|Polled every n seconds| Loader
    Loader -->|Loads & Executes| Snake
    Loader -->|Loads & Executes| OtherGames
    
    Snake -->|Calls| PeriphAPI
    OtherGames -->|Calls| PeriphAPI
    
    Loader -->|Uses| PeriphAPI
    
    PeriphAPI -->|Draws to| OLED
    PeriphAPI -->|Reads state| BTN
```