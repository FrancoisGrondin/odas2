import argparse
import json
import numpy as np
import matplotlib.pyplot as plt

def main():
    
    # Parameters
    parser = argparse.ArgumentParser()
    parser.add_argument('--input', type=str, default='/dev/stdin', help='Input file')
    args = parser.parse_args()

    # Empty cache initially
    cache = {}

    # Load line by line
    with open (args.input) as fin:
        
        for line in fin:
        
            msg = json.loads(line)
    
            if msg['type'] == "hops":

                if msg['label'] not in cache:
                    cache[msg['label']] = {}
                    cache[msg['label']]['type'] = "hops"
                    cache[msg['label']]['data'] = []

                cache[msg['label']]['data'].append(np.asarray(msg['samples'], dtype=np.float32))

            if msg['type'] == "freqs":

                if msg['label'] not in cache:
                    cache[msg['label']] = {}
                    cache[msg['label']]['type'] = "freqs"
                    cache[msg['label']]['data'] = []

                data = np.asarray(msg['bins'], dtype=np.float32)
                data = data[:, :, 0] + 1j * data[:, :, 1]

                cache[msg['label']]['data'].append(data)

    # Plot item by item
    for key in cache:

        if cache[key]['type'] == "hops":

            hops = np.stack(cache[key]['data'], axis=1)
            num_channels = hops.shape[0]
            num_frames = hops.shape[1]
            hops_size = hops.shape[2]
            waveforms = np.reshape(hops, (num_channels, num_frames * hops_size))

            fig, axs = plt.subplots(num_channels)

            for index_channel in range(num_channels):

                axs[index_channel].plot(waveforms[index_channel, :])

        if cache[key]['type'] == "freqs":

            spex = np.stack(cache[key]['data'], axis=1)
            num_channels = spex.shape[0]
            
            fig, axs = plt.subplots(num_channels)

            for index_channel in range(num_channels):

                axs[index_channel].imshow(np.transpose(np.log(np.abs(spex[index_channel, :, :])+1e-10)), aspect='auto', origin='lower')

    plt.show()

if __name__ == '__main__':
    main()    