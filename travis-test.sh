#!/usr/bin/env bash

echo Travis build - detected OS is: "$TRAVIS_OS_NAME"
set -e

# if [[ "$TRAVIS_OS_NAME" == "linux" ]]; then
  # Initialize display driver
  DISPLAY=:99
  export DISPLAY
  LIBGL_ALWAYS_SOFTWARE=1
  export LIBGL_ALWAYS_SOFTWARE
  /sbin/start-stop-daemon --start --quiet --pidfile /tmp/custom_xvfb_99.pid --make-pidfile --background --exec /usr/bin/Xvfb -- :99 -ac -screen 0 1280x1024x32 +extension GLX +render
  sleep 5

  glxinfo
# fi


dune runtest
