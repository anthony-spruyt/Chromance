import os
import sys

from SCons.Script import DefaultEnvironment

env = DefaultEnvironment()

STRING_SECRETS = {
    "WIFI_SSID": "CHROMANCE_WIFI_SSID",
    "WIFI_PASSWORD": "CHROMANCE_WIFI_PASSWORD",
    "OTA_PASSWORD": "CHROMANCE_OTA_PASSWORD",
    "MQTT_BROKER": "CHROMANCE_MQTT_BROKER",
    "MQTT_USERNAME": "CHROMANCE_MQTT_USERNAME",
    "MQTT_PASSWORD": "CHROMANCE_MQTT_PASSWORD",
}


def to_c_string(value):
    escaped = "".join(
        chr(byte) if 0x20 <= byte < 0x7F and chr(byte) not in '"\\?' else f"\\{byte:03o}"
        for byte in value.encode("utf-8")
    )
    return f'"{escaped}"'


missing = [name for name in [*STRING_SECRETS, "MQTT_PORT"] if not os.environ.get(name)]
mqtt_port = os.environ.get("MQTT_PORT", "")

# IDE indexing and clean runs don't compile anything, so they don't need the secrets
if not env.IsIntegrationDump() and not env.IsCleanTarget():
    if missing:
        sys.stderr.write(f"Missing environment variables: {', '.join(missing)} (see README.md)\n")
        env.Exit(1)
    if not mqtt_port.isdigit():
        sys.stderr.write("MQTT_PORT must be a number\n")
        env.Exit(1)

# A generated header, not -D flags, so secrets never pass through the shell or show up in compile commands
lines = [f"#define {define} {to_c_string(os.environ.get(name, ''))}" for name, define in STRING_SECRETS.items()]
lines.append(f"#define CHROMANCE_MQTT_PORT {mqtt_port if mqtt_port.isdigit() else '0'}")
content = "#pragma once\n" + "\n".join(lines) + "\n"

include_dir = os.path.join(env.subst("$BUILD_DIR"), "secrets")
header_path = os.path.join(include_dir, "secretsEnv.h")
os.makedirs(include_dir, exist_ok=True)
existing = None
if os.path.exists(header_path):
    with open(header_path) as header:
        existing = header.read()
if existing != content:
    with open(header_path, "w") as header:
        header.write(content)

env.Append(CPPPATH=[include_dir])
