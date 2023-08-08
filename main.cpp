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

int test1(){
    ma_result result;
    ma_engine engine;

    ma_sound bgm1Sound;

    result = ma_engine_init(nullptr , &engine);
    if(result != MA_SUCCESS){
        std::cerr << "init engine error" << std::endl;
        return -1;
    }
    
    result = ma_sound_init_from_file(&engine, "sound/buttfly.mp3", 0, 
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

// static ma_engine gEngine;

static bool isPlaying = true;
static bool doSeek = false;

void data_callback(ma_device* pDevice, 
                void* pOutput, 
                const void* pInput, ma_uint32 frameCount){
    std::cout << "playback_data_callback frameCount : " << frameCount << std::endl;
    // ma_engine_read_pcm_frames(&gEngine, pOutput, frameCount, NULL);
    ma_decoder* pDecoder = (ma_decoder*)pDevice->pUserData;
    if (pDecoder == nullptr) {
        std::cout << "decode is nullptr" << std::endl;
        return;
    }

    if(!isPlaying){
        return;
    }
    
    if(doSeek){
        doSeek = false;
        ma_decoder_seek_to_pcm_frame(pDecoder ,0);
        return;
    }

    // ma_decoder_read_pcm_frames(pDecoder, pOutput, frameCount, nullptr);
    ma_data_source_read_pcm_frames(pDecoder, pOutput, frameCount, nullptr);
}

void play_end_callback(ma_device* pDevice){
    std::cout << "play_end_callback " << std::endl;
}


int main(){
    // return test1();
    std::vector<char> dataVec;
    readFileAsBinary("sound/wuyue.mp3" , dataVec);
    std::cout << "file size: " << dataVec.size() << std::endl;
    char *pData = dataVec.data();

    ma_result result1;

    // result1 = ma_engine_init(nullptr , &gEngine);
    // if(result1 != MA_SUCCESS){
    //     std::cerr << "init engine error" << std::endl;
    //     return -1;
    // }
    
    ma_decoder decoder;
    ma_result result;

    result = ma_decoder_init_memory(pData , dataVec.size() ,nullptr , &decoder);
    if(result != MA_SUCCESS){
        std::cerr << "decoder_init_memory error" << std::endl;
        return -1;
    }

    ma_data_source_set_looping(&decoder , MA_TRUE);

    std::cout << "format:" << decoder.outputFormat << std::endl;
    std::cout << "channels :" << decoder.outputChannels << std::endl;
    std::cout << "sampleRate  :" << decoder.outputSampleRate << std::endl;

    ma_device_config config = ma_device_config_init(ma_device_type_playback);
    config.playback.format   = decoder.outputFormat;   
    config.playback.channels = decoder.outputChannels;
    config.sampleRate        = decoder.outputSampleRate;
    config.dataCallback      = data_callback;
    config.pUserData         =  &decoder;
    config.stopCallback = play_end_callback;
    // config.notificationCallback = device_notification;
    

    ma_device device;
    if (ma_device_init(nullptr, &config, &device) != MA_SUCCESS) {
        return -1;  // Failed to initialize the device.
    }

    if (ma_device_start(&device) != MA_SUCCESS){
        std::cerr << "ma_device_start error!" << std::endl;
        return -1;
    }

    char ch;
    do{
        //
        ch = getchar();

        if(ch == 'p'){ // pause or play
            if(isPlaying){
                ma_device_stop(&device);
            }else{
                ma_device_start(&device);
            }
            isPlaying = !isPlaying;
        }else if(ch == 's'){ // seek to position 0
            doSeek = true;
        }
    }while(ch != 'q');//

    ma_device_uninit(&device);
    ma_decoder_uninit(&decoder);
    return 0;
}
