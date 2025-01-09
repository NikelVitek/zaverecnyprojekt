# Závěrečný projekt - Hry na Arduino

Tento repozitář obsahuje několik jednoduchých her napsaných pro platformu Arduino. Hry jsou ovládány pomocí tlačítek a zobrazují se na TFT displeji. Cílem je vytvořit interaktivní hry, které využívají základní prvky programování, jako je zpracování vstupů, grafika a detekce kolizí.

## Obsah

- **Snake Game**  
  Klasická hra, kde hráč ovládá hada, který roste po sežrání jídla. Cílem je vyhnout se kolizi s vlastním tělem a stěnami displeje. Hra obsahuje následující funkce:
  - Generování jídla na náhodných pozicích.
  - Ovládání směru pohybu hada pomocí tlačítek.
  - Zvýšení obtížnosti díky rostoucí délce hada.
  - Detekce kolize se stěnami nebo vlastním tělem, která ukončuje hru.

- **Pong Game**  
  Hra pro dva hráče, která simuluje stolní tenis. Každý hráč ovládá svoji pálku a snaží se odrazit míček. Hra zahrnuje:
  - Pohyb pálky hráče nahoru a dolů pomocí tlačítek.
  - Odraz míčku od pálek a okrajů displeje.
  - Detekci skóre při zmeškání míčku.

## Hardware

- Arduino kompatibilní deska.
- TFT displej s rozlišením 320x240.
- Čtyři tlačítka pro ovládání.

## Instalace a spuštění

1. Nainstalujte knihovny potřebné pro TFT displej:
   - [Adafruit GFX](https://github.com/adafruit/Adafruit-GFX-Library)
   - [Adafruit ILI9341](https://github.com/adafruit/Adafruit_ILI9341)

2. Připojte hardware podle následujícího schématu:
   - **TFT displej**: 
     - DC: Pin 48  
     - CS: Pin 50  
     - MOSI: Pin 51  
     - CLK: Pin 46  
     - RST: Pin 49  
     - MISO: Pin 47
   - **Tlačítka**:
     - Tlačítko 1: Pin 18
     - Tlačítko 2: Pin 19

3. Nahrajte kód do Arduino desky.

4. Po spuštění se na displeji zobrazí menu, ve kterém lze zvolit hru pomocí tlačítek.

## Ovládání

- **Snake Game**:  
  - Tlačítko 1: Změna směru doprava.  
  - Tlačítko 2: Změna směru doleva.

- **Pong Game**:  
  - Tlačítko 1: Pohyb levé pálky nahoru/dolů.  
  - Tlačítko 2: Pohyb pravé pálky nahoru/dolů.

## Autor

Projekt byl vytvořen jako součást závěrečného projektu na téma interaktivní hry pro Arduino platformu.
