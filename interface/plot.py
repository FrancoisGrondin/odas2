import argparse
import json

import matplotlib.pyplot as plt
import numpy as np


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

            if msg['type'] == "doas":

                if msg['label'] not in cache:
                    cache[msg['label']] = {}
                    cache[msg['label']]['type'] = "doas"
                    cache[msg['label']]['data'] = []

                data = np.zeros((len(msg['dirs']), 5), dtype=np.float32)
                
                for src_index in range(len(msg['dirs'])):
                    data[src_index, 0] = msg['dirs'][src_index]['type']
                    data[src_index, 1] = msg['dirs'][src_index]['x']
                    data[src_index, 2] = msg['dirs'][src_index]['y']
                    data[src_index, 3] = msg['dirs'][src_index]['z']
                    data[src_index, 4] = msg['dirs'][src_index]['energy']

                cache[msg['label']]['data'].append(data)

            if msg['type'] == "tdoas":

                if msg['label'] not in cache:
                    cache[msg['label']] = {}
                    cache[msg['label']]['type'] = "tdoas"
                    cache[msg['label']]['data'] = []

                data = np.zeros((len(msg['taus']), len(msg['taus'][0]), 2), dtype=np.float32)

                for src_index in range(len(msg['taus'])):
                    for pair_index in range(len(msg['taus'][src_index])):
                        data[src_index, pair_index, 0] = msg['taus'][src_index][pair_index]['delay']
                        data[src_index, pair_index, 1] = msg['taus'][src_index][pair_index]['amplitude']

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

        if cache[key]['type'] == "doas":

            doas = np.stack(cache[key]['data'], axis=0)
            num_srcs = doas.shape[1]
            num_frames = doas.shape[0]

            fig, axs = plt.subplots(num_srcs)
            if num_srcs == 1:
                axs = [axs]

            for index_src in range(num_srcs):

                indexes = np.arange(num_frames)
                mask = doas[:, index_src, 0] != 0

                axs[index_src].scatter(indexes[mask], doas[mask, index_src, 1])
                axs[index_src].scatter(indexes[mask], doas[mask, index_src, 2])
                axs[index_src].scatter(indexes[mask], doas[mask, index_src, 3])

                axs[index_src].set_xlim([0, num_frames])
                axs[index_src].set_ylim([-1, +1])

        if cache[key]['type'] == "tdoas":

            tdoas = np.stack(cache[key]['data'], axis=0)
            num_srcs = tdoas.shape[1]
            num_pairs = tdoas.shape[2]
            num_frames = tdoas.shape[0]
            min_tau = np.min(tdoas)
            max_tau = np.max(tdoas)

            fig, axs = plt.subplots(num_srcs)
            if num_srcs == 1:
                axs = [axs]

            for index_src in range(num_srcs):

                indexes = np.arange(num_frames)

                for index_pair in range(num_pairs):

                    axs[index_src].scatter(indexes, tdoas[:, index_src, index_pair, 0])

                axs[index_src].set_xlim([0, num_frames])
                axs[index_src].set_ylim([min_tau, max_tau])



    plt.show()

if __name__ == '__main__':
    main()    