

#ifndef _MSP3400_H_
#define _MSP3400_H_






#define MSP_IN_SCART1		0  
#define MSP_IN_SCART2		1  
#define MSP_IN_SCART3		2  
#define MSP_IN_SCART4		3  
#define MSP_IN_MONO		6  
#define MSP_IN_MUTE		7  
#define MSP_SCART_TO_DSP(in)	(in)

#define MSP_IN_TUNER1		0  
#define MSP_IN_TUNER2		1  
#define MSP_TUNER_TO_DSP(in)	((in) << 3)


#define MSP_DSP_IN_TUNER	0  
#define MSP_DSP_IN_SCART	2  
#define MSP_DSP_IN_I2S1		5  
#define MSP_DSP_IN_I2S2		6  
#define MSP_DSP_IN_I2S3		7  
#define MSP_DSP_IN_MAIN_AVC	11 
#define MSP_DSP_IN_MAIN		12 
#define MSP_DSP_IN_AUX		13 
#define MSP_DSP_TO_MAIN(in)	((in) << 4)
#define MSP_DSP_TO_AUX(in)	((in) << 8)
#define MSP_DSP_TO_SCART1(in)	((in) << 12)
#define MSP_DSP_TO_SCART2(in)	((in) << 16)
#define MSP_DSP_TO_I2S(in)	((in) << 20)


#define MSP_SC_IN_SCART1	0  
#define MSP_SC_IN_SCART2	1  
#define MSP_SC_IN_SCART3	2  
#define MSP_SC_IN_SCART4	3  
#define MSP_SC_IN_DSP_SCART1	4  
#define MSP_SC_IN_DSP_SCART2	5  
#define MSP_SC_IN_MONO		6  
#define MSP_SC_IN_MUTE		7  
#define MSP_SC_TO_SCART1(in)	(in)
#define MSP_SC_TO_SCART2(in)	((in) << 4)


#define MSP_INPUT(sc, t, main_aux_src, sc_i2s_src) \
	(MSP_SCART_TO_DSP(sc) | \
	 MSP_TUNER_TO_DSP(t) | \
	 MSP_DSP_TO_MAIN(main_aux_src) | \
	 MSP_DSP_TO_AUX(main_aux_src) | \
	 MSP_DSP_TO_SCART1(sc_i2s_src) | \
	 MSP_DSP_TO_SCART2(sc_i2s_src) | \
	 MSP_DSP_TO_I2S(sc_i2s_src))
#define MSP_INPUT_DEFAULT MSP_INPUT(MSP_IN_SCART1, MSP_IN_TUNER1, \
				    MSP_DSP_IN_TUNER, MSP_DSP_IN_TUNER)
#define MSP_OUTPUT(sc) \
	(MSP_SC_TO_SCART1(sc) | \
	 MSP_SC_TO_SCART2(sc))

#define MSP_OUTPUT_DEFAULT (MSP_SC_TO_SCART1(MSP_SC_IN_SCART3) | \
			    MSP_SC_TO_SCART2(MSP_SC_IN_DSP_SCART1))













#endif 
