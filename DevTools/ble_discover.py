import asyncio
from bleak import BleakScanner, BleakClient


async def main():
    print("Scanning for BusyLight...")
    device = await BleakScanner.find_device_by_name("BusyLight", timeout=10.0)
    if not device:
        print("Device not found")
        return
    print(f"Found: {device.address}")

    print("Connecting...")
    async with BleakClient(device, timeout=30.0) as client:
        print("Connected!")
        for service in client.services:
            print(f"Service: {service.uuid}")
            for char in service.characteristics:
                print(f"  Char: {char.uuid} | Props: {char.properties}")

asyncio.run(main())
