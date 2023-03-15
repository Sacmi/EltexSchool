#!/bin/bash

SYSTEMD_PID=$(pgrep systemd)

ps aux --ppid $SYSTEMD_PID | grep -v "ps aux --ppid $SYSTEMD_PID"
