#  Game of Life
## Projekt do předmětu PC2M


Implementace Conwayova celulárního automatu. \
Podrobná dokumentace je v souboru docs/dokumentace.pdf \
Zkompilované spustitelné soubory jsou ve adresáři bin/ \
Ukázkové vstupní konfigurace jsou v adresáři input/

### Činnost programu
Program načte ze vstupního csv počáteční konfiguraci, nechá proběhnout
vývoj herního pole po stanovený počet generací a výsledné herní pole uloží do csv souboru
ve stejném formátu, jako je vstup. Průběžný vývoj herního pole je vypisován do konzole
jedním ze dvou způsobů.

### Ovládání
Veškeré parametry jsou programu předávány při spuštění pomocí argumentů příkazového
řádku. Syntaxe je následovná: \
<samp>název_programu [INPUT FILE] [OUTPUT FILE] [GENERATION COUNT] [PRINTING TYPE]<samp> 
