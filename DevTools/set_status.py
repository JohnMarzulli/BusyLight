import asyncio
import sys
from bleak import BleakScanner, BleakClient

NUS_RX_UUID = "6e400002-b5a3-f393-e0a9-e50e24dcca9e"

STATUSES = {
    "0": "Off",
    "1": "Available",
    "2": "Busy",
    "3": "Coffee please",
}


async def set_status(cmd: str):
    print("Scanning for BusyLight...")
    device = await BleakScanner.find_device_by_name("BusyLight", timeout=10.0)
    if not device:
        print("Error: BusyLight not found")
        sys.exit(1)

    print(f"Found: {device.address}")
    async with BleakClient(device, timeout=30.0) as client:
        await client.write_gatt_char(NUS_RX_UUID, cmd.encode(), response=False)
        print(f"Status set: {STATUSES[cmd]}")


def usage():
    print("Usage: set_status.py <command>")
    print()
    print("Commands:")
    for key, label in STATUSES.items():
        print(f"  {key}  {label}")
    sys.exit(1)


if __name__ == "__main__":
    if len(sys.argv) != 2 or sys.argv[1] not in STATUSES:
        usage()

    asyncio.run(set_status(sys.argv[1]))
