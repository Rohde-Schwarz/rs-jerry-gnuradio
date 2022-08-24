# rs-jerry-gnuradio

This repository is meant to be an example on how to use the rs-jerry-driver generally in a more elaborated way as well as offering a ready-to-use gnuradio block for people who want to further use the IQ samples provided by the MSR4 in gnuradio.

Hence this repository requires the rs-jerry-driver to be installed on the system globally and to work properly.

This repository is based on [bkarl's gnuradio-dpdk repo](https://github.com/bkarl/gnuradio-dpdk).

## Required Software
- at least gnuRadio v3.10
- rs-jerry-driver v1.0
- everything in the Software section of the rs-jerry-driver repo

## Installation
```
cd gr-msr4
mkdir build
cd build
cmake ..
make
sudo make install
```

## Usage
- start gnuRadio as root
- navigate to the `msr4`-category in the sidebar
- drag&drop the `rs jerry`-block from the sidebar into the flowgraph
- configure the `rs jerry`-block. generally the tabs `General` and `Streaming Settings` are required to be modified
- drag the `out`-flowcontrol of the `rs jerry`-block into desired input flowcontrol
- done

## Examples
A simple example displaying the recieved IQ samples in a QT GUI Time Sink can be found under `gr-msr4/examples`.

## Sidnote: rs-jerry-setup loopback
The generated IQ samples of the loopback of the rs-jerry-setup repo can be further processed or displayed in the `simple_jerry_example.grc` aswell.

Simply select the **receiving** VF port id of the loopback in the `DPDK Settings` tab and start the TRex generator beforehand.