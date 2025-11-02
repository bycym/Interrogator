#!/bin/bash
set -e

echo "=================================="
echo "Android APK Build Script (Docker)"
echo "=================================="

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Verify Qt installation
echo -e "${YELLOW}Verifying Qt installation...${NC}"
if [ ! -f "${QT_ANDROID}/bin/qmake" ]; then
    echo -e "${RED}ERROR: Qt Android qmake not found at ${QT_ANDROID}/bin/qmake${NC}"
    exit 1
fi
echo -e "${GREEN}✓ Qt Android found at ${QT_ANDROID}${NC}"

# Verify androiddeployqt
if [ ! -f "${QT_ANDROID}/bin/androiddeployqt" ]; then
    echo -e "${RED}ERROR: androiddeployqt not found at ${QT_ANDROID}/bin/androiddeployqt${NC}"
    exit 1
fi
echo -e "${GREEN}✓ androiddeployqt found${NC}"

# Clean previous builds
echo -e "${YELLOW}Cleaning previous builds...${NC}"
rm -rf /workspace/build-android
mkdir -p /workspace/build-android
cd /workspace/build-android

# Configure with qmake
echo -e "${YELLOW}Configuring project with qmake...${NC}"
${QT_ANDROID}/bin/qmake /workspace/Interrogator.pro \
    -spec android-clang \
    CONFIG+=release \
    ANDROID_ABIS="arm64-v8a"

if [ $? -ne 0 ]; then
    echo -e "${RED}ERROR: qmake configuration failed${NC}"
    exit 1
fi
echo -e "${GREEN}✓ Project configured${NC}"

# Build the project
echo -e "${YELLOW}Building project...${NC}"
make -j$(nproc)

if [ $? -ne 0 ]; then
    echo -e "${RED}ERROR: Build failed${NC}"
    exit 1
fi
echo -e "${GREEN}✓ Build completed${NC}"

# Generate android deployment settings
echo -e "${YELLOW}Generating Android deployment configuration...${NC}"
make install INSTALL_ROOT=/workspace/build-android/android-build

# Create deployment JSON
cat > /workspace/build-android/android-deployment-settings.json <<EOF
{
   "description": "Interrogator Android deployment configuration",
   "qt": "${QT_ANDROID}",
   "sdk": "${ANDROID_SDK_ROOT}",
   "sdkBuildToolsRevision": "34.0.0",
   "ndk": "${ANDROID_NDK_ROOT}",
   "toolchain-prefix": "llvm",
   "tool-prefix": "llvm",
   "ndk-host": "linux-x86_64",
   "target-architecture": "arm64-v8a",
   "application-binary": "/workspace/build-android/libInterrogator_arm64-v8a.so",
   "android-package-source-directory": "/workspace/android",
   "android-min-sdk-version": "23",
   "android-target-sdk-version": "34"
}
EOF

# Package APK
echo -e "${YELLOW}Packaging APK with androiddeployqt...${NC}"
${QT_ANDROID}/bin/androiddeployqt \
    --input /workspace/build-android/android-deployment-settings.json \
    --output /workspace/build-android/android-build \
    --android-platform android-34 \
    --jdk ${JAVA_HOME} \
    --gradle

if [ $? -ne 0 ]; then
    echo -e "${RED}ERROR: APK packaging failed${NC}"
    exit 1
fi

# Find and copy APK to output directory
echo -e "${YELLOW}Locating generated APK...${NC}"
APK_FILE=$(find /workspace/build-android/android-build -name "*.apk" | head -n 1)

if [ -z "$APK_FILE" ]; then
    echo -e "${RED}ERROR: No APK file generated${NC}"
    exit 1
fi

# Copy to output directory
mkdir -p /build
cp "$APK_FILE" /build/Interrogator.apk

echo -e "${GREEN}=================================="
echo -e "✓ BUILD SUCCESSFUL!"
echo -e "==================================${NC}"
echo -e "APK location: /build/Interrogator.apk"
echo -e "On host system: $(pwd)/build/Interrogator.apk"

# Show APK info
if [ -f /build/Interrogator.apk ]; then
    APK_SIZE=$(du -h /build/Interrogator.apk | cut -f1)
    echo -e "APK size: ${APK_SIZE}"
fi
