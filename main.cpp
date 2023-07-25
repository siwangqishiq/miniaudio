#include <iostream>

#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

int main(){
    ma_result result;
    ma_engine engine;

    ma_sound bgm1Sound;
    
    result = ma_engine_init(nullptr , &engine);
    if(result != MA_SUCCESS){
        std::cerr << "init engine error" << std::endl;
        return -1;
    }

    result = ma_sound_init_from_file(&engine, "sound/yes.mp3", 0, 
        nullptr, nullptr, &bgm1Sound);
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
    ma_decoder_init_memory();
    return 0;
}
