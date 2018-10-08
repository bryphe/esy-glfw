#!/usr/bin/env bash

echo Travis build - detected OS is: "$TRAVIS_OS_NAME"
set -e

if [[ "$TRAVIS_OS_NAME" == "linux" ]]; then
  # Initialize display driver
  DISPLAY=:99.0
  export DISPLAY
  LIBGL_ALWAYS_SOFTWARE=1
  export LIBGL_ALWAYS_SOFTWARE
  sh -e /etc/init.d/xvfb start
  sleep 5
  glxinfo
fi

# https://stackoverflow.com/questions/28837117/travis-ci-run-lwjgl-tests-in-non-headless-environment
# - /sbin/start-stop-daemon --start --quiet --pidfile /tmp/custom_xvfb_99.pid --make-pidfile --background --exec /usr/bin/Xvfb -- :99 -screen 0 1400x900x24 -ac +extension GLX +render

esy b dune runtest
