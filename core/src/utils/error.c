#include <odas2/utils/error.h>
#include <odas2/signals/signal.h>

#define XSTR(a) STR(a)
#define STR(a) #a

_Thread_local static odas2_error_t current_error = ODAS2_ERROR_NONE;

odas2_error_t odas2_error_number() {
    return current_error;
}

const char* odas2_error_message() {
    switch (current_error) {
        case ODAS2_ERROR_NONE:
            return "No error";

        case ODAS2_ERROR_MSGOUT_CONSTRUCT_FILE_CREATION:
            return "msgout_construct: cannot create file.";
        case ODAS2_ERROR_MSGOUT_WRITE:
            return "msgout_write: cannot write to file.";

        case ODAS2_ERROR_RAWOUT_CONSTRUCT_FILE_CREATION:
            return "rawout_construct: cannot create file.";
        case ODAS2_ERROR_RAWOUT_WRITE:
            return "rawout_write: cannot write to file.";

        case ODAS2_ERROR_WAVIN_CONSTRUCT_FILE_NOT_EXIST:
            return "wavin_construct: file does not exist.";
        case ODAS2_ERROR_WAVIN_CONSTRUCT_INVALID_WAV_FILE:
            return "wavin_construct: invalid WAV file format.";
        case ODAS2_ERROR_WAVIN_CONSTRUCT_INVALID_WAV_HEADER:
            return "wavin_construct: invalid WAV header.";

        case ODAS2_ERROR_WAVOUT_CONSTRUCT_FILE_CREATION:
            return "wavout_construct: cannot create file.";
        case ODAS2_ERROR_WAVOUT_CONSTRUCT_HEADER_WRITE:
            return "wavout_construct: cannot write WAV header.";
        case ODAS2_ERROR_WAVOUT_WRITE_SAMPLES:
            return "wavout_write: cannot write samples to WAV file.";

        case ODAS2_ERROR_COVS_CONSTRUCT_LABEL:
            return "covs_construct: label must be a string with less than " XSTR(SIGNAL_LABEL_SIZE) " characters.";
        case ODAS2_ERROR_COVS_CONSTRUCT_NUM_CHANNELS:
            return "covs_construct: number of channels must be at least 2.";
        case ODAS2_ERROR_COVS_CONSTRUCT_NUM_BINS:
            return "covs_construct: number of bins must be at least 1.";

        case ODAS2_ERROR_DOAS_CONSTRUCT_LABEL:
            return "doas_construct: label must be a string with less than " XSTR(SIGNAL_LABEL_SIZE) " characters.";
        case ODAS2_ERROR_DOAS_CONSTRUCT_NUM_DIRECTIONS:
            return "doas_construct: number of directions must be at least 1.";

        case ODAS2_ERROR_DSF_CONSTRUCT_LABEL:
            return "dsf_construct: label must be a string with less than " XSTR(SIGNAL_LABEL_SIZE) " characters.";

        case ODAS2_ERROR_FREQS_CONSTRUCT_LABEL:
            return "freqs_construct: label must be a string with less than " XSTR(SIGNAL_LABEL_SIZE) " characters.";
        case ODAS2_ERROR_FREQS_CONSTRUCT_NUM_CHANNELS:
            return "freqs_construct: number of channels must be at least 1.";
        case ODAS2_ERROR_FREQS_CONSTRUCT_NUM_BINS:
            return "freqs_construct: number of bins must be at least 1.";

        case ODAS2_ERROR_HOPS_CONSTRUCT_LABEL:
            return "hops_construct: label must be a string with less than " XSTR(SIGNAL_LABEL_SIZE) " characters.";
        case ODAS2_ERROR_HOPS_CONSTRUCT_NUM_CHANNELS:
            return "hops_construct: number of channels must be at least 1.";
        case ODAS2_ERROR_HOPS_CONSTRUCT_NUM_SHIFTS:
            return "hops_construct: number of shifts must be at least 1.";

        case ODAS2_ERROR_IMGS_CONSTRUCT_LABEL:
            return "imgs_construct: label must be a string with less than " XSTR(SIGNAL_LABEL_SIZE) " characters.";
        case ODAS2_ERROR_IMGS_CONSTRUCT_NUM_POINTS:
            return "imgs_construct: number of points must be at least 1.";

        case ODAS2_ERROR_MASKS_CONSTRUCT_LABEL:
            return "masks_construct: label must be a string with less than " XSTR(SIGNAL_LABEL_SIZE) " characters.";
        case ODAS2_ERROR_MASKS_CONSTRUCT_NUM_CHANNELS:
            return "masks_construct: number of channels must be at least 1.";
        case ODAS2_ERROR_MASKS_CONSTRUCT_NUM_BINS:
            return "masks_construct: number of bins must be at least 1.";

        case ODAS2_ERROR_TDOAS_CONSTRUCT_LABEL:
            return "tdoas_construct: label must be a string with less than " XSTR(SIGNAL_LABEL_SIZE) " characters.";
        case ODAS2_ERROR_TDOAS_CONSTRUCT_NUM_CHANNELS:
            return "tdoas_construct: number of channels must be at least 2.";
        case ODAS2_ERROR_TDOAS_CONSTRUCT_NUM_SOURCES:
            return "tdoas_construct: number of sources must be at least 1.";

        case ODAS2_ERROR_WEIGHTS_CONSTRUCT_LABEL:
            return "weights_construct: label must be a string with less than " XSTR(SIGNAL_LABEL_SIZE) " characters.";
        case ODAS2_ERROR_WEIGHTS_CONSTRUCT_NUM_SOURCES:
            return "weights_construct: number of sources must be at least 1.";
        case ODAS2_ERROR_WEIGHTS_CONSTRUCT_NUM_CHANNELS:
            return "weights_construct: number of channels must be at least 1.";
        case ODAS2_ERROR_WEIGHTS_CONSTRUCT_NUM_BINS:
            return "weights_construct: number of bins must be at least 1.";

        case ODAS2_ERROR_FFT_CONSTRUCT_INVALID_NUM_SAMPLES:
            return "fft_construct: number of samples must be a power of 2 and at least 2.";

        case ODAS2_ERROR_MICS_HARDWARE_UNKNOWN:
            return "mics_construct: unknown hardware type.";

        case ODAS2_ERROR_POINTS_GEOMETRY_UNKNOWN:
            return "points_construct: unknown geometry type.";

        case ODAS2_ERROR_BEAMFORMER_CONSTRUCT_NUM_SOURCES:
            return "beamformer_construct: number of sources must be at least 1.";
        case ODAS2_ERROR_BEAMFORMER_CONSTRUCT_NUM_CHANNELS:
            return "beamformer_construct: number of channels must be at least 1.";
        case ODAS2_ERROR_BEAMFORMER_CONSTRUCT_NUM_BINS:
            return "beamformer_construct: number of bins must be at least 1.";
        case ODAS2_ERROR_BEAMFORMER_PROCESS_WEIGHTS_NUM_SOURCES:
            return "beamformer_process: number of sources in weights must match the number of sources in the beamformer.";
        case ODAS2_ERROR_BEAMFORMER_PROCESS_WEIGHTS_NUM_CHANNELS:
            return "beamformer_process: number of channels in weights must match the number of channels in the beamformer.";
        case ODAS2_ERROR_BEAMFORMER_PROCESS_WEIGHTS_NUM_BINS:
            return "beamformer_process: number of bins in weights must match the number of bins in the beamformer.";
        case ODAS2_ERROR_BEAMFORMER_PROCESS_IN_NUM_CHANNELS:
            return "beamformer_process: number of channels in input must match the number of channels in the beamformer.";
        case ODAS2_ERROR_BEAMFORMER_PROCESS_IN_NUM_BINS:
            return "beamformer_process: number of bins in input must match the number of bins in the beamformer.";
        case ODAS2_ERROR_BEAMFORMER_PROCESS_OUT_NUM_CHANNELS:
            return "beamformer_process: number of channels in output must match the number of sources in the beamformer.";
        case ODAS2_ERROR_BEAMFORMER_PROCESS_OUT_NUM_BINS:
            return "beamformer_process: number of bins in output must match the number of bins in the beamformer.";

        case ODAS2_ERROR_DELAYSUM_CONSTRUCT_NUM_SOURCES:
            return "delaysum_construct: number of sources must be at least 1.";
        case ODAS2_ERROR_DELAYSUM_CONSTRUCT_NUM_CHANNELS:
            return "delaysum_construct: number of channels must be at least 2.";
        case ODAS2_ERROR_DELAYSUM_CONSTRUCT_NUM_BINS:
            return "delaysum_construct: number of bins must be at least 1.";
        case ODAS2_ERROR_DELAYSUM_PROCESS_TDOAS_NUM_SOURCES:
            return "delaysum_process: number of sources in TDOAs must match the number of sources in the delaysum.";
        case ODAS2_ERROR_DELAYSUM_PROCESS_TDOAS_NUM_CHANNELS:
            return "delaysum_process: number of channels in TDOAs must match the number of channels in the delaysum.";
        case ODAS2_ERROR_DELAYSUM_PROCESS_WEIGHTS_NUM_SOURCES:
            return "delaysum_process: number of sources in weights must match the number of sources in the delaysum.";
        case ODAS2_ERROR_DELAYSUM_PROCESS_WEIGHTS_NUM_CHANNELS:
            return "delaysum_process: number of channels in weights must match the number of channels in the delaysum.";
        case ODAS2_ERROR_DELAYSUM_PROCESS_WEIGHTS_NUM_BINS:
            return "delaysum_process: number of bins in weights must match the number of bins in the delaysum.";

        case ODAS2_ERROR_ENHANCEMENT_INVALID_NUM_BINS:
            return "enhancement_construct: number of bins must match the number of bins of the neural network.";

        case ODAS2_ERROR_GCC_CONSTRUCT_NUM_SOURCES:
            return "gcc_construct: number of sources must be at least 1.";
        case ODAS2_ERROR_GCC_CONSTRUCT_NUM_CHANNELS:
            return "gcc_construct: number of channels must be at least 2.";
        case ODAS2_ERROR_GCC_CONSTRUCT_NUM_BINS:
            return "gcc_construct: number of bins converted to number of samples must be at least 2 and a power of 2.";
        case ODAS2_ERROR_GCC_PROCESS_TDOAS_NUM_SOURCES:
            return "gcc_process: number of sources in TDOAs must match the number of sources in the GCC.";
        case ODAS2_ERROR_GCC_PROCESS_TDOAS_NUM_CHANNELS:
            return "gcc_process: number of channels in TDOAs must match the number of channels in the GCC.";
        case ODAS2_ERROR_GCC_PROCESS_COVS_NUM_CHANNELS:
            return "gcc_process: number of channels in covs must match the number of channels in the GCC.";
        case ODAS2_ERROR_GCC_PROCESS_COVS_NUM_BINS:
            return "gcc_process: number of bins in covs must match the number of bins in the GCC.";

        case ODAS2_ERROR_MIXER_CONSTRUCT_NUM_CHANNELS:
            return "mixer_construct: number of channels must be at least 1.";
        case ODAS2_ERROR_MIXER_PROCESS_MAX_MAP_INDEX:
            return "mixer_process: input hops does not have enough channels.";
        case ODAS2_ERROR_MIXER_PROCESS_NUM_CHANNELS:
            return "mixer_process: number of channels in output hops must match the number of channels in the mixer.";

        case ODAS2_ERROR_MVDR_CONSTRUCT_NUM_CHANNELS:
            return "mvdr_construct: number of channels must be at least 2.";
        case ODAS2_ERROR_MVDR_CONSTRUCT_NUM_BINS:
            return "mvdr_construct: number of bins must be at least 1.";
        case ODAS2_ERROR_MVDR_PROCESS_COVS_NUM_CHANNELS:
            return "mvdr_process: number of channels in covs must match the number of channels in the MVDR.";
        case ODAS2_ERROR_MVDR_PROCESS_COVS_NUM_BINS:
            return "mvdr_process: number of bins in covs must match the number of bins in the MVDR.";
        case ODAS2_ERROR_MVDR_PROCESS_WEIGHTS_NUM_SOURCES:
            return "mvdr_process: number of sources in weights must match the number of sources in the MVDR (1).";
        case ODAS2_ERROR_MVDR_PROCESS_WEIGHTS_NUM_CHANNELS:
            return "mvdr_process: number of channels in weights must match the number of channels in the MVDR.";
        case ODAS2_ERROR_MVDR_PROCESS_WEIGHTS_NUM_BINS:
            return "mvdr_process: number of bins in weights must match the number of bins in the MVDR.";

        case ODAS2_ERROR_PHAT_CONSTRUCT_NUM_CHANNELS:
            return "phat_construct: number of channels must be at least 2.";
        case ODAS2_ERROR_PHAT_CONSTRUCT_NUM_BINS:
            return "phat_construct: number of bins must be at least 1.";
        case ODAS2_ERROR_PHAT_PROCESS_COVS_IN_NUM_CHANNELS:
            return "phat_process: number of channels in input covs must match the number of channels in the phat.";
        case ODAS2_ERROR_PHAT_PROCESS_COVS_IN_NUM_BINS:
            return "phat_process: number of bins in input covs must match the number of bins in the phat.";
        case ODAS2_ERROR_PHAT_PROCESS_COVS_OUT_NUM_CHANNELS:
            return "phat_process: number of channels in output covs must match the number of channels in the phat.";
        case ODAS2_ERROR_PHAT_PROCESS_COVS_OUT_NUM_BINS:
            return "phat_process: number of bins in output covs must match the number of bins in the phat.";

        case ODAS2_ERROR_POSTFILTER_CONSTRUCT_NUM_CHANNELS:
            return "postfilter_construct: number of channels must be at least 1.";
        case ODAS2_ERROR_POSTFILTER_CONSTRUCT_NUM_BINS:
            return "postfilter_construct: number of bins must be at least 1.";
        case ODAS2_ERROR_POSTFILTER_PROCESS_FREQS_IN_NUM_CHANNELS:
            return "postfilter_process: number of channels in input freqs must match the number of channels in the postfilter.";
        case ODAS2_ERROR_POSTFILTER_PROCESS_FREQS_IN_NUM_BINS:
            return "postfilter_process: number of bins in input freqs must match the number of bins in the postfilter.";
        case ODAS2_ERROR_POSTFILTER_PROCESS_MASKS_NUM_CHANNELS:
            return "postfilter_process: number of channels in masks must match the number of channels in the postfilter.";
        case ODAS2_ERROR_POSTFILTER_PROCESS_MASKS_NUM_BINS:
            return "postfilter_process: number of bins in masks must match the number of bins in the postfilter.";
        case ODAS2_ERROR_POSTFILTER_PROCESS_FREQS_OUT_NUM_CHANNELS:
            return "postfilter_process: number of channels in output freqs must match the number of channels in the postfilter.";
        case ODAS2_ERROR_POSTFILTER_PROCESS_FREQS_OUT_NUM_BINS:
            return "postfilter_process: number of bins in output freqs must match the number of bins in the postfilter.";

        case ODAS2_ERROR_SCM_CONSTRUCT_NUM_CHANNELS:
            return "scm_construct: number of channels must be at least 2.";
        case ODAS2_ERROR_SCM_CONSTRUCT_NUM_BINS:
            return "scm_construct: number of bins must be at least 1.";
        case ODAS2_ERROR_SCM_CONSTRUCT_ALPHA:
            return "scm_construct: alpha must be between 0 and 1.";
        case ODAS2_ERROR_SCM_PROCESS_FREQS_NUM_CHANNELS:
            return "scm_process: number of channels in input freqs must match the number of channels in the scm.";
        case ODAS2_ERROR_SCM_PROCESS_FREQS_NUM_BINS:
            return "scm_process: number of bins in input freqs must match the number of bins in the scm.";
        case ODAS2_ERROR_SCM_PROCESS_MASKS_NUM_CHANNELS:
            return "scm_process: number of channels in masks must match the number of channels in the scm.";
        case ODAS2_ERROR_SCM_PROCESS_MASKS_NUM_BINS:
            return "scm_process: number of bins in masks must match the number of bins in the scm.";
        case ODAS2_ERROR_SCM_PROCESS_COVS_NUM_CHANNELS:
            return "scm_process: number of channels in covs must match the number of channels in the scm.";
        case ODAS2_ERROR_SCM_PROCESS_COVS_NUM_BINS:
            return "scm_process: number of bins in covs must match the number of bins in the scm.";

        case ODAS2_ERROR_SSL_CONSTRUCT_NUM_MICS:
            return "ssl_construct: number of microphones must be at least 2.";
        case ODAS2_ERROR_SSL_CONSTRUCT_NUM_POINTS:
            return "ssl_construct: number of points must be at least 1.";
        case ODAS2_ERROR_SSL_CONSTRUCT_SAMPLE_RATE:
            return "ssl_construct: sample rate must be greater than 0.";
        case ODAS2_ERROR_SSL_CONSTRUCT_SOUND_SPEED:
            return "ssl_construct: sound speed must be greater than 0.";
        case ODAS2_ERROR_SSL_CONSTRUCT_NUM_SOURCES:
            return "ssl_construct: number of sources must be at least 1.";
        case ODAS2_ERROR_SSL_CONSTRUCT_NUM_DIRECTIONS:
            return "ssl_construct: number of directions must be at least 1.";
        case ODAS2_ERROR_SSL_PROCESS_TDOAS_NUM_SOURCES:
            return "ssl_process: number of sources in TDOAs must match the number of sources in the ssl.";
        case ODAS2_ERROR_SSL_PROCESS_TDOAS_NUM_CHANNELS:
            return "ssl_process: number of channels in TDOAs must match the number of channels in the ssl.";
        case ODAS2_ERROR_SSL_PROCESS_DOAS_NUM_DIRECTIONS:
            return "ssl_process: number of directions in DOAs must match the number of directions in the ssl.";

        case ODAS2_ERROR_SST_CONSTRUCT_NUM_TRACKS:
            return "sst_construct: number of tracks must be at least 1.";
        case ODAS2_ERROR_SST_CONSTRUCT_NUM_DIRECTIONS:
            return "sst_construct: number of directions must be at least 1.";
        case ODAS2_ERROR_SST_CONSTRUCT_NUM_PASTS:
            return "sst_construct: number of pasts must be at least 1.";
        case ODAS2_ERROR_SST_PROCESS_NUM_DIRECTIONS:
            return "sst_process: number of directions in input DOAs must match the number of directions in the sst.";
        case ODAS2_ERROR_SST_PROCESS_NUM_TRACKS:
            return "sst_process: number of directions in output DOAs must match the number of tracks in the sst.";

        case ODAS2_ERROR_STEERING_CONSTRUCT_NUM_MICS:
            return "steering_construct: number of microphones must be at least 2.";
        case ODAS2_ERROR_STEERING_CONSTRUCT_SAMPLE_RATE:
            return "steering_construct: sample rate must be greater than 0.";
        case ODAS2_ERROR_STEERING_CONSTRUCT_SOUND_SPEED:
            return "steering_construct: sound speed must be greater than 0.";
        case ODAS2_ERROR_STEERING_CONSTRUCT_NUM_SOURCES:
            return "steering_construct: number of sources must be at least 1.";
        case ODAS2_ERROR_STEERING_PROCESS_DOAS_NUM_DIRECTIONS:
            return "steering_process: number of directions in input DOAs must match the number of sources in the steering.";
        case ODAS2_ERROR_STEERING_PROCESS_TDOAS_NUM_SOURCES:
            return "steering_process: number of sources in TDOAs must match the number of sources in the steering.";
        case ODAS2_ERROR_STEERING_PROCESS_TDOAS_NUM_CHANNELS:
            return "steering_process: number of channels in TDOAs must match the number of channels in the steering.";

        case ODAS2_ERROR_STFT_CONSTRUCT_NUM_CHANNELS:
            return "stft_construct: number of channels must be at least 1.";
        case ODAS2_ERROR_STFT_CONSTRUCT_NUM_SAMPLES:
            return "stft_construct: number of samples must be at least 2 and a power of 2.";
        case ODAS2_ERROR_STFT_CONSTRUCT_NUM_SHIFTS:
            return "stft_construct: number of shifts must be at least 1 and at most half the number of samples.";
        case ODAS2_ERROR_STFT_CONSTRUCT_INVALID_WINDOW:
            return "stft_construct: invalid window type.";
        case ODAS2_ERROR_STFT_PROCESS_HOPS_NUM_CHANNELS:
            return "stft_process: number of channels in input hops must match the number of channels in the stft.";
        case ODAS2_ERROR_STFT_PROCESS_HOPS_NUM_SHIFTS:
            return "stft_process: number of shifts in input hops must match the number of shifts in the stft.";
        case ODAS2_ERROR_STFT_PROCESS_FREQS_NUM_CHANNELS:
            return "stft_process: number of channels in output freqs must match the number of channels in the stft.";
        case ODAS2_ERROR_STFT_PROCESS_FREQS_NUM_BINS:
            return "stft_process: number of bins in output freqs must match the number of bins in the stft.";

        case ODAS2_ERROR_ISTFT_CONSTRUCT_NUM_CHANNELS:
            return "istft_construct: number of channels must be at least 1.";
        case ODAS2_ERROR_ISTFT_CONSTRUCT_NUM_SAMPLES:
            return "istft_construct: number of samples must be at least 2 and a power of 2.";
        case ODAS2_ERROR_ISTFT_CONSTRUCT_NUM_SHIFTS:
            return "istft_construct: number of shifts must be at least 1 and at most half the number of samples.";
        case ODAS2_ERROR_ISTFT_CONSTRUCT_INVALID_WINDOW:
            return "istft_construct: invalid window type.";
        case ODAS2_ERROR_ISTFT_PROCESS_HOPS_NUM_CHANNELS:
            return "istft_process: number of channels in output hops must match the number of channels in the istft.";
        case ODAS2_ERROR_ISTFT_PROCESS_HOPS_NUM_SHIFTS:
            return "istft_process: number of shifts in output hops must match the number of shifts in the istft.";
        case ODAS2_ERROR_ISTFT_PROCESS_FREQS_NUM_CHANNELS:
            return "istft_process: number of channels in input freqs must match the number of channels in the istft.";
        case ODAS2_ERROR_ISTFT_PROCESS_FREQS_NUM_BINS:
            return "istft_process: number of bins in input freqs must match the number of bins in the istft.";

        default:
            return "Unknown error";
    }
}

void odas2_set_error_number(odas2_error_t err) {
    current_error = err;
}
