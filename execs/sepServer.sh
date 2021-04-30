#!/bin/bash

./ble_scan
wait
parallel ::: ./mqtt_demo_mutual_auth ./read_write
