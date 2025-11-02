# Building Android APK Locally with Docker

This guide explains how to build the Interrogator Android APK on your local machine using Docker.

## Prerequisites

1. **Docker** - Install Docker Desktop or Docker Engine:
   - **Windows/Mac**: [Docker Desktop](https://www.docker.com/products/docker-desktop)
   - **Linux**: Docker Engine (via package manager)

2. **Disk Space** - Ensure you have at least 15GB of free disk space for:
   - Docker image (~8GB)
   - Build artifacts (~2GB)
   - Android SDK and Qt (~5GB)

## Quick Start

### 1. Make the build script executable

```bash
chmod +x build-android-local.sh
```

### 2. Run the build

```bash
./build-android-local.sh
```

The script will:
1. Build a Docker image with all required tools (Android SDK, NDK, Qt 6.5.3)
2. Compile the application inside the container
3. Package the APK
4. Copy the APK to your current directory as `Interrogator-release.apk`

## Build Process Details

### What the Docker Image Contains

The Docker image (`Dockerfile.android`) includes:
- Ubuntu 22.04 base
- Java JDK 17
- Android SDK Command-line Tools
- Android SDK Platform 34
- Android Build Tools 34.0.0
- Android NDK 21.4.7075529
- Qt 6.5.3 for Android (arm64-v8a)
- Qt 6.5.3 for desktop (gcc_64)

### Build Steps

1. **Image Building**: Creates a Docker image with all dependencies (first run takes 10-15 minutes)
2. **Configuration**: Uses Android qmake to configure the project
3. **Compilation**: Builds the native library
4. **Packaging**: Uses androiddeployqt to create the APK
5. **Output**: Copies the APK to `Interrogator-release.apk`

## Troubleshooting

### Docker Build Fails

**Issue**: Docker build fails with "no space left on device"
**Solution**: 
```bash
docker system prune -a  # Clean up unused Docker resources
```

### Build Script Permission Denied

**Issue**: `./build-android-local.sh: Permission denied`
**Solution**:
```bash
chmod +x build-android-local.sh
```

### APK Not Generated

**Issue**: Build completes but no APK found
**Solution**: Check the build output inside the Docker container:
```bash
docker run --rm -v "$(pwd):/workspace" -w /workspace interrogator-android-builder /bin/bash -c "ls -la android-build/"
```

## Advanced Usage

### Interactive Docker Shell

To debug build issues, launch an interactive shell:

```bash
docker run --rm -it -v "$(pwd):/workspace" -w /workspace interrogator-android-builder /bin/bash
```

Then manually run build commands:

```bash
export ANDROID_QT_ROOT=/opt/qt/6.5.3/android_arm64_v8a
export ANDROID_SDK_ROOT=/opt/android-sdk

# Configure
${ANDROID_QT_ROOT}/bin/qmake Interrogator.pro -spec android-clang

# Build
make -j$(nproc)

# Package
${ANDROID_QT_ROOT}/bin/androiddeployqt --input android-build/android-deployment-settings.json --output android-build
```

### Rebuild Docker Image

If you need to rebuild the Docker image (e.g., to update Qt version):

```bash
docker build --no-cache -f Dockerfile.android -t interrogator-android-builder .
```

### Different Android Architectures

To build for different architectures, modify the build script:

- **ARMv7**: Change `android_arm64_v8a` to `android_armv7` and `arm64-v8a` to `armeabi-v7a`
- **x86_64**: Change to `android_x86_64` and `x86_64`

## Output

After a successful build, you'll find:
- **Interrogator-release.apk** - The unsigned release APK (ready for testing)
- **android-build/** - Build artifacts directory

## Next Steps

### Install on Device

To install the APK on an Android device:

```bash
adb install -r Interrogator-release.apk
```

### Sign the APK (for distribution)

For publishing to Google Play or distribution, sign the APK:

```bash
# Generate keystore (one-time)
keytool -genkey -v -keystore my-release-key.keystore -alias my-key-alias -keyalg RSA -keysize 2048 -validity 10000

# Sign APK
jarsigner -verbose -sigalg SHA256withRSA -digestalg SHA-256 -keystore my-release-key.keystore Interrogator-release.apk my-key-alias

# Zipalign (optimize)
zipalign -v 4 Interrogator-release.apk Interrogator-release-signed.apk
```

## Performance Notes

- **First build**: 15-20 minutes (includes downloading ~8GB of dependencies)
- **Subsequent builds**: 2-5 minutes (Docker image is cached)
- **Incremental builds**: 1-2 minutes (if only source code changed)

## Comparison with GitHub Actions

| Aspect | Local Docker Build | GitHub Actions |
|--------|-------------------|----------------|
| Speed (first) | 15-20 min | 20-25 min |
| Speed (subsequent) | 2-5 min | 20-25 min |
| Requires | Docker, disk space | GitHub account |
| Cost | Free (local resources) | Free (public repos) |
| Debugging | Full shell access | Limited logs |

## Support

For issues or questions about local Docker builds, please open an issue on the GitHub repository.
