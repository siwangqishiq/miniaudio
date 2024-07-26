#include <iostream>
#include <fstream>
#include <vector>

#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

void readFileAsBinary(std::string path , std::vector<char> &dataVec){
    std::ifstream binFile(path, std::ios::binary);
    binFile.seekg(0 , std::ios::end);
    
    int length = binFile.tellg();
    binFile.seekg(0 , std::ios::beg);

    dataVec.resize(length);
    binFile.read(dataVec.data() , length);
    binFile.close();
}

/**
 * @brief 
 * 
 * 
 * ma_audio_buffer_config config = ma_audio_buffer_config_init(format, channels, sizeInFrames, pData, NULL);

ma_audio_buffer audioBuffer;
ma_audio_buffer_init(&config, NULL, &audioBuffer);

ma_sound_init_from_data_source(pEngine, &audioBuffer, ...);
 * 
 * @return int 
 */
int test2(){
    ma_engine engine;
    ma_result result;

    result = ma_engine_init(nullptr , &engine);
    if(result != MA_SUCCESS){
        std::cerr << "init engine error" << std::endl;
        return -1;
    }

    return 0;
}

int test3(){
     ma_result result;
    ma_engine engine;

    ma_sound bgm1Sound;

    result = ma_engine_init(nullptr , &engine);
    if(result != MA_SUCCESS){
        std::cerr << "init engine error" << std::endl;
        return -1;
    }
    
    result = ma_sound_init_from_file(&engine, "sound/renlong.mp3", 0, 
        nullptr, nullptr, &bgm1Sound);
    
    bgm1Sound.endCallback = [](void* pUserData, ma_sound* pSound){
        std::cout << "bgm play sound ended!" << std::endl;
        //ma_sound_seek_to_pcm_frame(pSound, 0);
        // ma_sound_start(pSound);
        ma_sound_seek_to_pcm_frame(pSound , 0);
        ma_sound_stop(pSound);
        ma_sound_start(pSound);
        std::cout << "bgm replay sound!" << std::endl;
    };

    if(result != MA_SUCCESS){
        std::cerr << "init sound error" << std::endl;
        return -1;
    }

    // ma_engine_play_sound(&engine , "sound/test.wav" , nullptr);
    char ch;
    do{
        ch = getchar();
        if(ch == 'a'){
            ma_engine_play_sound(&engine , "sound/switch1.wav" , nullptr);
        }else if(ch == 'b'){
            ma_engine_play_sound(&engine , "sound/switch2.wav" , nullptr);
        }else if(ch == 'c'){
            ma_engine_play_sound(&engine , "sound/yes.mp3" , nullptr);
        }else if(ch == 'y'){
            ma_sound_start(&bgm1Sound);
        }else if(ch == 'n'){
            ma_sound_stop(&bgm1Sound);
        }else if(ch == 'r'){
            ma_sound_seek_to_pcm_frame(&bgm1Sound , 0);
            ma_sound_stop(&bgm1Sound);
        }
    }while(ch != 'q');//
    ma_sound_uninit(&bgm1Sound);
    // ma_decoder_init_memory();

    return 0;
}

void data_callback(ma_device* pDevice, 
    void* pOutput, 
    const void* pInput, 
    ma_uint32 frameCount){

    std::cout << "readPcmFileCount = " << readPcmFileCount <<" frameCount :" << frameCount << std::endl;
    
    std::ifstream* pcmFile = (std::ifstream*)pDevice->pUserData;
    if (!pcmFile->is_open()) {
        return;
    }

    // int readPcmFileCount = frameCount * pDevice->playback.channels * ma_get_bytes_per_sample(pDevice->playback.format);
    // std::cout << "readPcmFileCount = " << readPcmFileCount <<" frameCount :" << frameCount << std::endl;
    // pcmFile->read((char *)pOutput, 
    //     frameCount * pDevice->playback.channels * ma_get_bytes_per_sample(pDevice->playback.format));
    
    // if (pcmFile->gcount() == 0) {
    //     ma_device_stop(pDevice);
    // }
}

int test_play_pcm(){
      const char *file = "sound/output_44100_2_s16le.pcm";
    std::ifstream pcmFile(file, std::ios::binary);

    if (!pcmFile.is_open()) {
        std::cerr << "open file error: " << file << std::endl;
        return -1;
    }

    // 配置播放设备
    ma_device_config deviceConfig = ma_device_config_init(ma_device_type_playback);
    deviceConfig.playback.format = ma_format_s16;   // 示例采样格式，16 位有符号
    deviceConfig.playback.channels = 2;             // 示例通道数，1为单声道，2为立体声
    deviceConfig.sampleRate = 44100;                // 示例采样率
    deviceConfig.dataCallback = data_callback;
    deviceConfig.pUserData = &pcmFile;

    ma_device device;
    if (ma_device_init(nullptr, &deviceConfig, &device) != MA_SUCCESS) {
        std::cerr << "init play device error" << std::endl;
        return -1;
    }

    if (ma_device_start(&device) != MA_SUCCESS) {
        std::cerr << "start deivce error" << std::endl;
        ma_device_uninit(&device);
        return -1;
    }

    std::cout << "Playing..." << std::endl;
    std::cin.get(); // 等待用户按下回车键以停止播放

    // 停止播放并清理
    ma_device_uninit(&device);
    pcmFile.close();

    std::cout << "Play Ended." << std::endl;

    return 0;
}

int main(){
    return 0;
}
