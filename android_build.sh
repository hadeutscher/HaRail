adb shell rm /data/local/tmp/harail
rm Release/harail
ANDROID=1 make
adb push Release/harail /data/local/tmp/
adb shell /data/local/tmp/harail
