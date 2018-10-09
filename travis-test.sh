#!/usr/bin/env bash

OS_NAME=$(uname -s)
echo Travis build - detected OS is: "$OS_NAME"

set -e

if [[ "$OS_NAME" == "Linux" ]]; then
  # Initialize display driver
  DISPLAY=:99.0
  export DISPLAY
  LIBGL_ALWAYS_SOFTWARE=1
  export LIBGL_ALWAYS_SOFTWARE
  echo Starting daemon...

  # /sbin/start-stop-daemon --start --quiet --pidfile /tmp/custom_xvfb_99.pid --make-pidfile --background --exec /usr/bin/Xvfb -- :99 -ac -screen 0 1280x1024x32 +extension GLX +render
  sh -e /etc/init.d/xvfb start
  sleep 5
  echo xfb started

  echo running glxinfo...
  glxinfo

  # Only run tests on Linux, for now-
  dune runtest
fi
