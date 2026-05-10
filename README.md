# Pokemon RPG — Raylib C++

2D top-down RPG engine i Raylib. Grund för att bygga vidare på.

## Beroenden (Zorin OS / Ubuntu)

```bash
sudo apt update
sudo apt install cmake build-essential libraylib-dev
```

> Om `libraylib-dev` inte finns, bygg raylib från källkod:
> ```bash
> sudo apt install libgl1-mesa-dev libglu1-mesa-dev libx11-dev libxrandr-dev libxi-dev
> git clone --depth 1 --branch 5.0 https://github.com/raysan5/raylib.git
> cd raylib/src && make PLATFORM=PLATFORM_DESKTOP
> sudo make install
> ```

## Bygga

```bash
mkdir build && cd build
cmake ..
make -j$(nproc)
./PokemonRPG
```

## Kontroller

| Tangent       | Handling         |
|---------------|-----------------|
| WASD / Pilar  | Rörelse          |
| ESC           | Stäng fönster    |

## Projektstruktur

```
src/
  main.cpp        — Entry point
  Constants.hpp   — Globala konstanter
  Game.hpp/cpp    — Huvudloop, kamera
  TileMap.hpp/cpp — Tilemap + kollision
  Player.hpp/cpp  — Spelare, rörelse, animation
assets/
  tiles/          — Tile-sprites (lägg till senare)
  sprites/        — Karaktärssprites
  maps/           — Kartor (JSON/CSV-format vid behov)
```

## Nästa steg att lägga till

- [ ] NPC-klass med enkel patrulleringslogik
- [ ] Dialogsystem (textruta + inmatning)
- [ ] Sprite-sheets istället för programritad grafik
- [ ] Encounter-system (slumpmässiga strider i högt gräs)
- [ ] Enkelt inventarie / UI
- [ ] Fler kartor + dörröppningar mellan dem
