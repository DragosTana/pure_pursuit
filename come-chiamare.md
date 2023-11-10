# ho fatto script per tutto
perchè sono un coglione e mi fa fatica riscrivere sta roba ogni volta

avrei dovuto fare un makefile, ma non so fare makefile

## come chiamare gli script
gli script sono fatti per essere chiamati dalla root del progetto (quindi da `pure_pursuit`), tutte le path negli script sono relative alla root del progetto, se li chiami da un'altra directory ti romperanno il cazzo sul fatto che non esistono file

visto che le path sono relative non credo che si romperà niente da te, ma sul tuo computer non ho testato niente

sono tutti normalissimi script di bash (non so un cazzo di bash)

## compilare
``` bash
./scripts/compile
```

## runnare
per runnare con la visualizzazione fai
``` bash
./scripts/run
```

per runnare senza la visualizzazione (visto matplotlib odia tutti i desktop che uso (anche quelli normali, mi è partito anche su cinnamon))
``` bash
./scripts/nohead
```

per runnare coi waypoint vecchi fai
``` bash
./scripts/oldpoint_run
```

## test
per i test ho messo compilazione e run nello stesso script, perchè sì
``` bash
./scripts/test
```
