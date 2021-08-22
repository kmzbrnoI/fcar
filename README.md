# fcar
KMZ's Sketchs for Faller Car System

## Install on Linux (Fedora)

    # Arduino IDE
    sudo dnf install -y arduino

    # install boards
    arduino --install-boards arduino:avr

### Install arduino-cli

    mkdir -p ~/.local/bin
    # check that created directory is in your PATH
    $ grep '~/.local/bin' ~/.bashrc
    export PATH=~/.local/bin:$PATH

    sudo dnf install -y autoconf automake gcc clang-tools-extra
    sudo curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | BINDIR=~/.local/bin sh

### Install required libraries

See subdirectories and search which libraries are required.

    arduino-cli lib install Bounce2 Servo
