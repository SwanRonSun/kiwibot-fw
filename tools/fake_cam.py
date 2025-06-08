#!/usr/bin/env python3
"""
fake_cam.py  –  stream 4-byte <int16 angle, int16 range> packets
Usage examples
  • Print mock packets:            python fake_cam.py --angle 30 --range 200
  • Spam a Nano on COM6 @115200:   python fake_cam.py -p COM6 -b 115200 -Hz 50
"""

import argparse, struct, sys, time
try:
    import serial
except ImportError:
    serial = None   # printing mode doesn’t need pyserial

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("-p", "--port",    help="COM port (omit to just print)", default=None)
    ap.add_argument("-b", "--baud",    help="baud rate", type=int, default=115200)
    ap.add_argument("--angle",         help="deg (-180…+180)",  type=int, default=45)
    ap.add_argument("--range",         help="mm  (0…32767)",    type=int, default=180)
    ap.add_argument("-Hz",             help="packets per second", type=float, default=10)
    args = ap.parse_args()

    payload = struct.pack("<hh", args.angle, args.range)   # little-endian int16,int16

    if args.port:
        if serial is None:
            sys.exit("PySerial missing; run  'python -m pip install pyserial'")
        ser = serial.Serial(args.port, args.baud, timeout=0)
        print(f"Streaming to {args.port} @ {args.baud} baud  ({args.Hz} Hz)… Ctrl-C to stop.")
        try:
            while True:
                ser.write(payload)
                time.sleep(1/args.Hz)
        except KeyboardInterrupt:
            print("\nDone.")
        finally:
            ser.close()
    else:
        print("Printing packets (Ctrl-C to stop)…")
        try:
            while True:
                sys.stdout.buffer.write(payload)
                sys.stdout.flush()
                time.sleep(1/args.Hz)
        except KeyboardInterrupt:
            pass

if __name__ == "__main__":
    main()
