# Building Android APK with Docker

This guide explains how to build the Interrogator Android APK locally using Docker.

## Prerequisites

- Docker installed on your system
- At least 10GB of free disk space
- Git (to clone the repository)

## Quick Start

### 1. Build the Docker Image

From the project root directory, run:

```bash
docker build -f Dockerfile.android -t interrogator-android .
```

This will create a Docker image with all necessary dependencies:
- Android SDK and NDK
- Qt 6.5.3 for Android
- Build tools and Java JDK 17

**Note:** First build will take 15-30 minutes as it downloads all dependencies (~8GB).

### 2. Build the APK

Run the container to build the APK:

```bash
docker run --rm -v $(pwd):/workspace -v $(pwd)/build:/build interrogator-android
```

The APK will be generated at: `./build/Interrogator.apk`

## Detailed Usage

### Building with Custom Options

You can override the build script or run custom commands:

```bash
# Interactive shell inside container
docker run --rm -it -v $(pwd):/workspace interrogator-android /bin/bash

# Run specific build commands
docker run --rm -v $(pwd):/workspace interrogator-android \
    bash -c "cd /workspace && qmake && make"
```

### Build Output

The build process creates:
- `build-android/` - Build artifacts and intermediate files
- `build/Interrogator.apk` - Final APK file (copied to host)

### Cleaning Build Artifacts

To clean previous builds:

```bash
# Remove build directory on host
rm -rf build-android build

# Or clean inside container
docker run --rm -v $(pwd):/workspace interrogator-android \
    bash -c "rm -rf /workspace/build-android /workspace/build"
```

## Architecture Support

The default build targets **arm64-v8a** (64-bit ARM), which works on most modern Android devices.

To build for other architectures, modify the `ANDROID_ABIS` in `docker-build.sh`:

```bash
# For 32-bit ARM
ANDROID_ABIS="armeabi-v7a"

# For multiple architectures
ANDROID_ABIS="arm64-v8a armeabi-v7a"
```

## Troubleshooting

### Build fails with "androiddeployqt not found"

Ensure the Qt Android installation completed successfully:

```bash
docker run --rm interrogator-android ls -la /opt/qt/6.5.3/android_arm64_v8a/bin/
```

### APK not generated

Check the build logs for errors:

```bash
docker run --rm -v $(pwd):/workspace interrogator-android
```

Look for compilation errors or missing dependencies in the output.

### Out of disk space

Docker images and builds can be large. Free up space:

```bash
# Remove unused Docker images
docker system prune -a

# Check Docker disk usage
docker system df
```

## Environment Variables

You can customize the build environment:

```bash
docker run --rm \
    -e ANDROID_ABIS="arm64-v8a" \
    -e BUILD_TYPE="release" \
    -v $(pwd):/workspace \
    -v $(pwd)/build:/build \
    interrogator-android
```

Available variables:
- `ANDROID_HOME` - Android SDK location
- `QT_ANDROID` - Qt Android installation path
- `ANDROID_NDK_ROOT` - NDK location
- `BUILD_TYPE` - Build type (release/debug)

## Advanced Usage

### Using a Different Qt Version

Edit `Dockerfile.android` and change the Qt version:

```dockerfile
RUN aqt install-qt linux desktop 6.6.0 gcc_64 -O ${QT_ROOT} && \
    aqt install-qt linux android 6.6.0 android_arm64_v8a -O ${QT_ROOT}
```

Then rebuild the Docker image.

### Caching Qt Installation

To speed up rebuilds, use Docker volumes for Qt:

```bash
# Create named volume for Qt
docker volume create qt-cache

# Build with volume
docker run --rm \
    -v qt-cache:/opt/qt \
    -v $(pwd):/workspace \
    -v $(pwd)/build:/build \
    interrogator-android
```

### CI/CD Integration

You can use this Docker image in CI/CD pipelines:

```yaml
# Example for GitHub Actions
jobs:
  build-android:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v4
      - name: Build Docker image
        run: docker build -f Dockerfile.android -t interrogator-android .
      - name: Build APK
        run: docker run --rm -v $(pwd):/workspace -v $(pwd)/build:/build interrogator-android
      - name: Upload APK
        uses: actions/upload-artifact@v3
        with:
          name: android-apk
          path: build/Interrogator.apk
```

## File Structure

```
.
├── Dockerfile.android           # Docker image definition
├── docker-build.sh             # Build script (runs inside container)
├── BUILD_ANDROID_DOCKER.md     # This documentation
├── Interrogator.pro            # Qt project file
├── android/                    # Android-specific files
│   ├── AndroidManifest.xml
│   ├── build.gradle
│   └── res/
└── build/                      # Output directory (host)
    └── Interrogator.apk        # Generated APK
```

## Benefits of Docker Build

1. **Reproducible**: Same environment every time
2. **Isolated**: Doesn't affect your host system
3. **Portable**: Works on Linux, macOS, and Windows
4. **Clean**: Easy to reset and rebuild
5. **Version Controlled**: Dockerfile tracks exact dependencies

## Comparison: Docker vs GitHub Actions

| Aspect | Docker (Local) | GitHub Actions (CI) |
|--------|---------------|---------------------|
| Setup Time | 15-30 min (first time) | 0 min (pre-configured) |
| Build Time | 5-10 min | 5-10 min |
| Cost | Free (uses your hardware) | Free (public repos) |
| Debugging | Easy (interactive shell) | Harder (log-based) |
| Consistency | High | Very High |
| Portability | Very High | Tied to GitHub |

## Support

For issues with Docker builds, check:
1. Docker daemon is running: `docker ps`
2. Sufficient disk space: `df -h`
3. Docker version: `docker --version` (requires v20.10+)

For Qt or Android-specific issues, refer to:
- [Qt Documentation](https://doc.qt.io/qt-6/)
- [Android NDK Guide](https://developer.android.com/ndk/guides)
