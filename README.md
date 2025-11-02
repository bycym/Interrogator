# Interrogator
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

## Build Instructions

### Build for Mac:
```bash
qmake CONFIG+=release && make
```

### Build for Android:
The project includes an automated GitHub Actions workflow for building Android APKs. The workflow (`/.github/workflows/mobile.yml`) automatically:

1. Sets up Android SDK and NDK
2. Installs Qt for Android development
3. Configures the project for Android builds
4. Generates the APK file
5. Uploads the APK as a build artifact

To build manually for Android, you need:
- Qt 5.15.2 or later with Android support
- Android SDK (API level 30)
- Android NDK (version 21.4.7075529)
- Java JDK 11

Build commands:
```bash
qmake -spec android-clang CONFIG+=release ANDROID_ABIS=arm64-v8a
make
androiddeployqt --input android-Interrogator-deployment-settings.json --output android-build --android-platform android-30 --jdk $JAVA_HOME --gradle --release
```

The Android build will generate an APK in the `android-build/build/outputs/apk/release/` directory.
