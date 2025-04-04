#include <driver/i2s.h>

// Sample buffer and rate configuration
#define SAMPLE_BUFFER_SIZE 512
#define SAMPLE_RATE 16000 // 16kHz sample rate

// Microphone channel configuration
// Using I2S_CHANNEL_FMT_ONLY_RIGHT to specifically capture only the right channel
// This is the correct way to capture only right channel data with ESP32 I2S
#define I2S_MIC_CHANNEL I2S_CHANNEL_FMT_ONLY_RIGHT

// Pin definitions
#define I2S_MIC_SERIAL_CLOCK GPIO_NUM_16    // BCLK
#define I2S_MIC_LEFT_RIGHT_CLOCK GPIO_NUM_5 // WS
#define I2S_MIC_SERIAL_DATA GPIO_NUM_17     // SD

// I2S configuration structure
i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = SAMPLE_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
    .channel_format = I2S_MIC_CHANNEL,
    .communication_format = I2S_COMM_FORMAT_STAND_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 8,
    .dma_buf_len = 1024,
    .use_apll = true,
    .tx_desc_auto_clear = false,
    .fixed_mclk = 0
};

// I2S pin configuration structure
i2s_pin_config_t i2s_mic_pins = {
    .bck_io_num = I2S_MIC_SERIAL_CLOCK,
    .ws_io_num = I2S_MIC_LEFT_RIGHT_CLOCK,
    .data_out_num = I2S_PIN_NO_CHANGE,
    .data_in_num = I2S_MIC_SERIAL_DATA
};

void setup()
{
    // Initialize serial communication for debugging and plotting
    Serial.begin(115200);
    Serial.println("ESP32 I2S Microphone Test - Right Channel Only");
    
    // Install and configure the I2S driver
    esp_err_t err;
    
    // Delete the I2S driver if it exists
    i2s_driver_uninstall(I2S_NUM_0);
    
    // Install I2S driver
    err = i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
    if (err != ESP_OK) {
        Serial.printf("Failed installing I2S driver: %d\n", err);
        while (1);
    }
    
    // Set I2S pins
    err = i2s_set_pin(I2S_NUM_0, &i2s_mic_pins);
    if (err != ESP_OK) {
        Serial.printf("Failed setting I2S pins: %d\n", err);
        while (1);
    }
    
    // Clear I2S buffer
    i2s_zero_dma_buffer(I2S_NUM_0);
    Serial.println("I2S driver installed successfully");
    delay(1000); // Give some time to stabilize
}

// Buffer for raw audio samples
int32_t raw_samples[SAMPLE_BUFFER_SIZE];

void loop()
{
    // Read audio data from the I2S device
    size_t bytes_read = 0;
    esp_err_t err = i2s_read(I2S_NUM_0, raw_samples, sizeof(int32_t) * SAMPLE_BUFFER_SIZE, &bytes_read, portMAX_DELAY);
    
    if (err != ESP_OK) {
        Serial.printf("Failed reading I2S data: %d\n", err);
        delay(1000);
        return;
    }
    
    int samples_read = bytes_read / sizeof(int32_t);
    Serial.printf("Read %d samples\n", samples_read);
    
    // Check for non-zero data
    bool all_zeros = true;
    int32_t max_val = 0, min_val = 0;
    
    // Output samples to serial port for visualization
    for (int i = 0; i < samples_read; i++) {
        // Shift the raw data to get the actual value
        int32_t sample = raw_samples[i] >> 14; // Bit shift to get proper range
        
        if (sample != 0) all_zeros = false;
        if (sample > max_val) max_val = sample;
        if (sample < min_val) min_val = sample;
        
        Serial.printf("%ld\n", sample);
    }
    
    // Provide diagnostic information
    if (all_zeros) {
        Serial.println("WARNING: All samples are zero - check microphone connections");
    } else {
        Serial.printf("Sample range: Min=%ld, Max=%ld\n", min_val, max_val);
    }
    
    delay(100); // Small delay between sample batches
}