import os
import sys

Import("env")

StringSecrets = {
    "WIFI_SSID": "CHROMANCE_WIFI_SSID",
    "WIFI_PASSWORD": "CHROMANCE_WIFI_PASSWORD",
    "OTA_PASSWORD": "CHROMANCE_OTA_PASSWORD",
    "MQTT_BROKER": "CHROMANCE_MQTT_BROKER",
    "MQTT_USERNAME": "CHROMANCE_MQTT_USERNAME",
    "MQTT_PASSWORD": "CHROMANCE_MQTT_PASSWORD",
}


def ToCString(value):
    escaped = "".join(
        chr(byte) if 0x20 <= byte < 0x7F and chr(byte) not in '"\\?' else "\\%03o" % byte
        for byte in value.encode("utf-8")
    )
    return '"%s"' % escaped


missing = [name for name in list(StringSecrets) + ["MQTT_PORT"] if not os.environ.get(name)]
mqttPort = os.environ.get("MQTT_PORT", "")

# IDE indexing and clean runs don't compile anything, so they don't need the secrets
if not env.IsIntegrationDump() and not env.IsCleanTarget():
    if missing:
        sys.stderr.write("Missing environment variables: %s (see README.md)\n" % ", ".join(missing))
        env.Exit(1)
    if not mqttPort.isdigit():
        sys.stderr.write("MQTT_PORT must be a number\n")
        env.Exit(1)

# A generated header, not -D flags, so secrets never pass through the shell or show up in compile commands
lines = ["#define %s %s" % (define, ToCString(os.environ.get(name, ""))) for name, define in StringSecrets.items()]
lines.append("#define CHROMANCE_MQTT_PORT %s" % (mqttPort if mqttPort.isdigit() else "0"))
content = "#pragma once\n" + "\n".join(lines) + "\n"

includeDir = os.path.join(env.subst("$BUILD_DIR"), "secrets")
headerPath = os.path.join(includeDir, "secretsEnv.h")
os.makedirs(includeDir, exist_ok=True)
if not os.path.exists(headerPath) or open(headerPath).read() != content:
    with open(headerPath, "w") as header:
        header.write(content)

env.Append(CPPPATH=[includeDir])
