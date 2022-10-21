# Interrogator

[![CI for Interrogator](https://github.com/bycym/Interrogator/actions/workflows/main.yml/badge.svg)](https://github.com/bycym/Interrogator/actions/workflows/main.yml) [![Mobile build CI for Interrogator](https://github.com/bycym/Interrogator/actions/workflows/mobile.yml/badge.svg)](https://github.com/bycym/Interrogator/actions/workflows/mobile.yml)

Ask (e.g.) English words from (e.g.) hungarian. Made in Qt

The "word" .txt struct e.g.:

word, word, word[tab]answer, answer

Feature:
- open .txt files
- ask wrongs answers only
- change color of the text if its wrong (red) or correct (green)
- show the right answer after the typed answer
- show asking words
- count asked words
- count good/bad answers
- multiple answer
- can switch between language

Known bugs:
- after the last answer still count good bad answers
- there is some spaces in the right answers list

[Download](https://arcsibo.gumroad.com/l/ZvJvu)

![alt tag](https://github.com/arcsibo/Interrogator/blob/master/screenshot.png)


## Build mac:
qmake CONFIG+=release && make
