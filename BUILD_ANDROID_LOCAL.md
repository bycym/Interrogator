# Local Android APK Build with Docker

This guide explains how to build the Android APK for the Interrogator application locally using Docker.

## Prerequisites

1. **Docker**: Install Docker Desktop or Docker Engine
   - Windows/Mac: [Docker Desktop](https://www.docker.com/products/docker-desktop/)
   - Linux: [Docker Engine](https://docs.docker.com/engine/install/)

2. **Docker Compose**: Usually included with Docker Desktop
   - Linux: May need to install separately

3. **Disk Space**: Ensure you have at least 15GB of free disk space
   - Qt 6.5.3: ~3GB
   - Android SDK/NDK: ~5GB
   - Build artifacts: ~2GB

## Quick Start

### Method 1: Using Docker Compose (Recommended)

1. **Build the Docker image** (first time only):
   ```bash
   docker-compose build
   ```

2. **Run the build**:
   ```bash
   docker-compose up
   ```

3. **Find your APK**:
   The APK will be in the project root directory as `Interrogator-debug.apk`

### Method 2: Using Docker Directly

1. **Build the Docker image**:
   ```bash
   docker build -f Dockerfile.android -t interrogator-android-builder .
   ```

2. **Run the build container**:
   ```bash
   docker run --rm -v $(pwd):/workspace interrogator-android-builder /workspace/scripts/build-android-local.sh
   ```

3. **Find your APK**:
   The APK will be in the project root directory as `Interrogator-debug.apk`

## Interactive Build (for debugging)

If you want to enter the container and build manually:

```bash
# Start an interactive shell in the container
docker run -it --rm -v $(pwd):/workspace interrogator-android-builder /bin/bash

# Inside the container, run the build script
/workspace/scripts/build-android-local.sh

# Or build step by step manually:
mkdir -p android-build && cd android-build
/opt/qt/6.5.3/android_arm64_v8a/bin/qmake .. ANDROID_ABIS=arm64-v8a
make -j$(nproc)
/opt/qt/6.5.3/android_arm64_v8a/bin/androiddeployqt --input android-Interrogator-deployment-settings.json --output android-build --android-platform android-34 --gradle
```

## Build Configuration

### Environment Variables

You can customize the build by setting environment variables:

```bash
# Using docker-compose
QT_VERSION=6.5.3 docker-compose up

# Using docker directly
docker run --rm \
  -e QT_VERSION=6.5.3 \
  -v $(pwd):/workspace \
  interrogator-android-builder \
  /workspace/scripts/build-android-local.sh
```

Available variables:
- `QT_VERSION`: Qt version to use (default: 6.5.3)
- `ANDROID_SDK_ROOT`: Android SDK location (default: /opt/android-sdk)
- `QT_ROOT`: Qt installation directory (default: /opt/qt)

### Build Options

The Docker image includes:
- **Qt 6.5.3** (Android arm64-v8a and Desktop gcc_64)
- **Android SDK API 34** (Android 14)
- **Android Build Tools 34.0.0**
- **NDK 21.4.7075529**
- **Java JDK 17**

## Troubleshooting

### Build Fails with "qmake not found"

Ensure the Qt installation completed successfully:
```bash
docker run -it interrogator-android-builder ls -la /opt/qt/6.5.3/
```

### Build Fails with "androiddeployqt not found"

Check if androiddeployqt exists:
```bash
docker run -it interrogator-android-builder ls -la /opt/qt/6.5.3/android_arm64_v8a/bin/
```

### APK Not Generated

1. Check the build logs for errors
2. Verify Gradle build succeeded
3. Check android-build directory:
   ```bash
   docker run -it -v $(pwd):/workspace interrogator-android-builder find /workspace/android-build -name "*.apk"
   ```

### Out of Disk Space

If you encounter disk space issues:

1. **Clean Docker cache**:
   ```bash
   docker system prune -a --volumes
   ```

2. **Remove unused images**:
   ```bash
   docker image prune -a
   ```

### Rebuilding the Docker Image

If you need to rebuild the image from scratch:

```bash
# Using docker-compose
docker-compose build --no-cache

# Using docker directly
docker build --no-cache -f Dockerfile.android -t interrogator-android-builder .
```

## Performance Tips

### Use Build Caches

The docker-compose.yml includes volume caches for Qt and Android SDK:

```yaml
volumes:
  qt-cache:/opt/qt
  android-sdk-cache:/opt/android-sdk
```

This speeds up subsequent builds significantly.

### Multi-core Builds

The build script uses `make -j$(nproc)` to utilize all available CPU cores. For very slow machines, you can limit this:

```bash
# Edit scripts/build-android-local.sh and change:
make -j2  # Use only 2 cores
```

## Clean Build

To perform a completely clean build:

```bash
# Remove build artifacts
rm -rf android-build Interrogator-debug.apk

# Rebuild with docker-compose
docker-compose down -v  # Remove volumes
docker-compose build --no-cache
docker-compose up
```

## Building Different Architectures

The default build targets arm64-v8a. To build for other architectures:

1. Modify `Dockerfile.android` to install the desired Qt architecture:
   ```dockerfile
   # For armv7:
   RUN aqt install-qt linux android ${QT_VERSION} android_armv7 -O ${QT_ROOT}
   
   # For x86_64:
   RUN aqt install-qt linux android ${QT_VERSION} android_x86_64 -O ${QT_ROOT}
   ```

2. Update `scripts/build-android-local.sh` to use the correct architecture in qmake:
   ```bash
   ${ANDROID_QMAKE} .. ANDROID_ABIS=armeabi-v7a  # for armv7
   # or
   ${ANDROID_QMAKE} .. ANDROID_ABIS=x86_64  # for x86_64
   ```

## CI/CD Integration

You can use this Docker setup in CI/CD pipelines:

```yaml
# Example GitHub Actions workflow
- name: Build Android APK with Docker
  run: |
    docker-compose build
    docker-compose up
    
- name: Upload APK
  uses: actions/upload-artifact@v3
  with:
    name: android-apk
    path: Interrogator-debug.apk
```

## Additional Resources

- [Qt for Android Documentation](https://doc.qt.io/qt-6/android.html)
- [Android SDK Documentation](https://developer.android.com/studio/command-line)
- [Docker Documentation](https://docs.docker.com/)

## Support

If you encounter issues not covered in this guide, please:

1. Check the build logs carefully
2. Verify all prerequisites are met
3. Try a clean rebuild
4. Open an issue in the repository with full error logs
