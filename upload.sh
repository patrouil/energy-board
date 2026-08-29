#!/bin/sh -x


clean () {
  pio run -t clean
  pio system prune -f
}
build () {
  pio run

}

run() {
  pio run --target upload
}

# main

# clean only a few times it does recompile external libs
#clean
#build
run