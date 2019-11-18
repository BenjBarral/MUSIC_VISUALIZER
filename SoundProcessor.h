//
//  SoundProcessor.h
//  audioVisualizer
//
//  Created by Benjamin Barral on 11/11/2019.
//

#ifndef SoundProcessor_h
#define SoundProcessor_h


#include <SFML/Audio.hpp>

static int max_samples_for_amplitude = 200;

class SoundProcessor {
private:
    int m_num_frequencies;
    sf::Int16* m_samples;
    sf::Uint64 count;
    int m_num_computed_samples;
    std::shared_ptr<Aquila::Fft> m_fft_calculator;
    
public:
    SoundProcessor(const int& num_frequencies){
        m_num_frequencies = num_frequencies;
        m_samples = new sf::Int16[num_frequencies];
        m_fft_calculator = Aquila::FftFactory::getFft(num_frequencies);
    }
    
    void ComputeSpectrum(sf::InputSoundFile& f, double* spectrum_band_values, double& global_max_intensity){
        m_num_computed_samples += 1;
        count = f.read(m_samples, m_num_frequencies);
        
        /// SOUND : Fft on the samples
        double fftSamples[m_num_frequencies];
        for (int i = 0; i < m_num_frequencies; i++)
            fftSamples[i] = double(m_samples[i]);
        const double* fftptr = fftSamples;
        Aquila::SpectrumType spectrum = m_fft_calculator->fft(fftptr);
        for (int i = 0; i < m_num_frequencies; i++)
            spectrum_band_values[i] = abs(spectrum[i]);
        
        double max_intensity = *(std::max_element(spectrum_band_values, spectrum_band_values + m_num_frequencies));
        if (max_intensity > global_max_intensity & m_num_computed_samples < max_samples_for_amplitude) global_max_intensity = max_intensity;
    }
};

#endif /* SoundProcessor_h */
