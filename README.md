# ODAS

## Building on Unix

### Prerequisites:

- gcc
- cmake

### Install using apt:

```
$ sudo apt install git
$ sudo apt install cmake
$ sudo apt install gcc
```

### Build instructions:

```
$ git clone https://github.com/francoisgrondin/odas2.git
$ cd odas2/core
$ mkdir build
$ cd build
$ cmake ..
$ make
```

## Demos

- Visualize the waveforms from an audio file:

```
$ cat audio/speeches.wav | core/bin/demo_wave | python3 interface/plot.py
```

- Visualize the waveforms from 5 sec of recording of an audio file (assuming there is a device accessible at hw:1,0, with 4 channels, at 16000 samples/sec and 16 bits signed samples). Note that the buffer tool is needed in the pipeline here to avoid buffer overflow as the arecord feeds audio in a non-blocking manner.

```
$ arecord -f S16_LE -r 16000 -c 4 --device "hw:1,0" -d 5| core/bin/buffer | core/bin/demo_wave | python3 interface/plot.py
```

- Visualize the spectrograms from an audio file:

```
$ cat audio/speeches.wav | core/bin/demo_spex | python3 interface/plot.py
```

- Generate audio to perform delay and sum beamforming (similar to BeamformIt):

```
$ cat audio/mix.wav | core/bin/demo_delaysum > audio/bf.wav
```

- Generate audio to perform MVDR beamforming (tries to enhance all coherent sources):

```
$ cat audio/mix.wav | core/bin/demo_mvdr > audio/bf.wav
```

- Visualize the directions of arrival of sound (without filtering):

```
$ cat audio/mix.wav | core/bin/demo_doas | python3 interface/plot.py
```

## Unit tests

Use the following command to run all unit tests:

```
core/bin/tests
```