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
