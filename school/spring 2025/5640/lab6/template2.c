/*
(C) Copyright Utah State University 2014.  All rights reserved.
No part of this program may be photocopied, reproduced, or translated
to another program language without the prior written consent of
Utah State University.

Time-stamp: " "

(program) template2.c
(status) Under development
(description) Template file for output of samples using the OMAP-L138 Experimenter kit.
(keywords) ECE 5640, Real-Time DSP

(long_d)

This is a template file for use in the lab for ECE 5640.  It is used to provide the
initialization for the OMAP-L138 Experimenter kit that will send samples to the left
and right channels.

This template illustrates the use of interrupt service routines for timing.

(long_d)

(see_also) template2.h L138_aic3106_init.c L138_aic3106_init.h vectors_intr.asm

(bugs)
There are no known bugs at this time
(bugs)

(author) Andrew Pound

(modifications)
Written 04/01/13 by Andrew Pound
Modified: See subversion logs.
(modifications)


  Info: $Id: template2.c 755 2014-02-04 22:08:39Z scott $   USU
  Revision: $Rev: 755 $
 $LastChangedDate: 2014-02-04 15:08:39 -0700 (Tue, 04 Feb 2014) $
 $LastChangedBy: scott $

*/

#include "L138_aic3106_init.h"
#include "filter.h"
#include <math.h>

#define SECTIONS 5
#define B 16

// This data structure allows for outputting to both channels.
AIC31_data_type codec_data;

int32_t gl;
int32_t a_coeffs_int[SECTIONS][2];
int32_t b_coeffs_int[SECTIONS][3];
int32_t d[SECTIONS][2] = {
      {0,0},
      {0,0},
      {0,0},
      {0,0},
      {0,0}
};
const uint32_t mask = 0xFFFFFFFF << (32 - B);
const uint32_t round = 1 << (31 - B);
const uint32_t one = (0x7FFFFFFF >> (32 - B)) << (32 - B);

int32_t SOS_section(const int32_t *b, const int32_t *a, int32_t *d, int32_t x, int32_t mask, int32_t round, int32_t scale);

interrupt void interrupt4(void)
{
  int32_t yn,xn = 0;
  
  // Get the next sample of the input.
  yn = (((int32_t)(input_left_sample())) << 16) & mask;
  xn = (int32_t)input_right_sample(); // input from ADC

  yn = SOS_section(a[0], b[0], d[0], yn, mask, round, gl);
  yn = SOS_section(a[1], b[1], d[1], yn, mask, round, one);
  yn = SOS_section(a[2], b[2], d[2], yn, mask, round, one);
  yn = SOS_section(a[3], b[3], d[3], yn, mask, round, one);
  yn = SOS_section(a[4], b[4], d[4], yn, mask, round, one);
  yn /= (1 << 15);

  // output to BOTH right and left channels...
  codec_data.channel[LEFT] = (int16_t)(yn);
  codec_data.channel[RIGHT] = (int16_t)(xn);
  output_sample(codec_data.uint);  // output to L and R DAC

  return;
}


int main(void)
{
  int i;
  int j;
  g1 = (int32_t)(pow(2,30) * g + pow(2,31-B)) & mask;

  for (i = 0 ; i < SECTIONS; ++i) 
  {
	for(j = 0; j < 2; j++)
	{
		a_coeffs_int[i][j] = (int32_t)(pow(2,30) * a_coeffs[i][j] + pow(2,31-B)) & mask;
	}
		
	for(j = 0; j < 3; j++)
	{
		b_coeffs_int[i][j] = (int32_t)(pow(2,30) * b_coeffs[i][j] + pow(2,31-B)) & mask;
	}
  }
  L138_initialise_intr(FS_48000_HZ,ADC_GAIN_0DB,DAC_ATTEN_0DB);

  while(1);
}
