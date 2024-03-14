# !/usr/bin/bash
# python3 ./.pio/libdeps/nucleo_f446re/Nanopb/generator/nanopb_generator.py --proto-path=./proto/ ./proto/Common.proto
# python3 ./.pio/libdeps/nucleo_f446re/Nanopb/generator/nanopb_generator.py --proto-path=./proto/ ./proto/Bridge.proto
python3 ./.pio/libdeps/nucleo_f446re/Nanopb/generator/nanopb_generator.py --cpp-descriptors --output-dir=./lib/ --proto-path=./proto/ ./proto/*.proto