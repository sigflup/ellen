#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <stdarg.h>
#include "SDL.h"

#include "link.h"

#include <mad.h>

#include "fifo.h"

#define CHUNK	16384
#define ISO_DELAY	1688


char_fifo_t decode_fifo = { (char *)0, 0,0 };

extern int song_len;
int mad_error;

typedef struct {
 int rate;
 int channels;
} pcm_t;

pcm_t output_pcm;

struct buffer {
 unsigned char *start;
 unsigned long length;
};

static enum mad_flow input(void *data, struct mad_stream *stream) {
 struct buffer *buffer = data;
 if(!buffer->length)
  return MAD_FLOW_STOP;

 mad_stream_buffer(stream, buffer->start, buffer->length);

 buffer->length = 0;
 return MAD_FLOW_CONTINUE;
}

static inline signed int scale(mad_fixed_t sample) {
 sample+=(1L<<(MAD_F_FRACBITS-16));
 if(sample>=MAD_F_ONE)
  sample = MAD_F_ONE - 1;
 else if(sample < -MAD_F_ONE)
  sample = -MAD_F_ONE;

 return sample >>(MAD_F_FRACBITS + 1 - 16);
}


static enum mad_flow output(void *data,
                            struct mad_header *header,
			    struct mad_pcm *pcm) {

 unsigned int nchannels, nsamples;
 mad_fixed_t *left_ch, *right_ch;

 nchannels = pcm->channels;
 nsamples =  pcm->length;
 
 output_pcm.rate = pcm->samplerate;
 output_pcm.channels=pcm->channels;

 left_ch =   pcm->samples[0];
 right_ch =  pcm->samples[1];

 while(nsamples--) {
  signed int sample;
  sample = scale(*left_ch++);

  write_char_fifo(&decode_fifo, (sample>>0) &0xff);
  write_char_fifo(&decode_fifo, (sample>>8) &0xff);
  song_len+=2; 
  if(nchannels==2) {
   sample = scale(*right_ch++);

   song_len+=2;
   write_char_fifo(&decode_fifo, (sample>>0) &0xff);
   write_char_fifo(&decode_fifo, (sample>>8) &0xff);

  }
 }
 return MAD_FLOW_CONTINUE;
}

static enum mad_flow error(void *data, 
                           struct mad_stream *stream,
			   struct mad_frame *frame) {

 mad_error = 1;
 return MAD_FLOW_CONTINUE;
 
}



void uncompress_mp3(char *filename) {
 struct buffer buffer;
 unsigned char *ret;
 unsigned char *mp3_data;
 int mp3_data_len = 0;
 int real_len = 0;
 int l, l2, i;
 SDL_RWops *rw;
 struct mad_decoder decoder;
 mad_error = 0;

 rw = SDL_RWFromFile(filename, "r");
 mp3_data = (unsigned char *)0; 
 l2 = 0;
 for(;;) {
  mp3_data_len += CHUNK;
  mp3_data = realloc(mp3_data, mp3_data_len);
  if((l=SDL_RWread(rw, &mp3_data[mp3_data_len - CHUNK], 1, CHUNK))==0) break;
  l2+=l;
  real_len+=(l*CHUNK);
 }
 buffer.start = mp3_data;
 buffer.length = l2;

 mad_decoder_init(&decoder, &buffer,
   input, 0,0, output, error, 0);
 mad_decoder_run(&decoder, MAD_DECODER_MODE_SYNC);
 mad_decoder_finish(&decoder);

 free(mp3_data);

}
