#define TFT_R0    26	//	1  T11		3.3V 		              
#define TFT_R1    45	//	2  R12		3.3V 		       
#define TFT_R2    47	//	3  U13		3.3V 			       
#define TFT_R3    70	//	4  R1 		3.3V 			       
#define TFT_R4    71	//	5  R2 		3.3V 			       
#define TFT_R5    72	//	6  R3 		3.3V 			       
#define TFT_R6    73	//	7  R4 		3.3V 			       
#define TFT_R7    74	//	8  T1 		3.3V 			       
#define TFT_G0    23	//	9  T10		3.3V 		       
#define TFT_G1    44	//	10 T12		3.3V 			      
#define TFT_G2    75	//	11 T2 		3.3V 			      
#define TFT_G3    76	//	12 T3 		3.3V 			      
#define TFT_G4    77	//	13 T4 		3.3V 			      
#define TFT_G5    78	//	14 U1 		3.3V 			      
#define TFT_G6    79	//	15 U2 		3.3V 			      
#define TFT_G7    80	//	16 U3 		3.3V 			      
                 	
#define TFT_B0    22	//	19 U10	3.3V output 		                
#define TFT_B1    27	//	20 U12	3.3V output 			      
#define TFT_B2    46	//	21 V13	3.3V output 		      
#define TFT_B3    81	//	22 U4 	3.3V output 		      
#define TFT_B4    8	//	23 V2 	3.3V output 		      
#define TFT_B5    9	//	24 V3 	3.3V output 		      
#define TFT_B6    10	//	25 V4 	3.3V output 		      
#define TFT_B7    11	//	26 T5 	3.3V output 		      
#define TFT_VCLK     88	//	27 V5 	3.3V output 			    
#define TFT_VSYNC    86	//	28 U5 	3.3V output				   
#define TFT_VDEN     89	//	29 R6 	3.3V output 			    
#define TFT_HSYNC    87	//	30 R5 	3.3V output				   
#define GPMC_nWE     68	//	31 U6 	3.3V output 			
//#define GND GND --  	// 	32   	 
#define GPMC_nOE     67	//	33 T7 	3.3V output		                
#define GPMC_nBE0_CLE 69//	34 T6 	3.3V output 			
#define GPMC_nWP      31//	35 U17	3.3Voutput 				    
#define GPMC_nAVD_ALE 66//	36 R7 	3.3V output 				 
#define GPMC_nBE1     60//	37 U18	3.3V input		                    
#define GPMC_nWAIT0   30//	38 T17	3.3V input                        	
                                     	
                                     	
#define GPMC_D0    32   //  41  U7 3.3V input/output		                
#define GPMC_D1    33	//	42 V7 	3.3V input/output	     
#define GPMC_D2    34	//	43 R8 	3.3V input/output		     
#define GPMC_D3    35	//	44 T8 	3.3V input/output		     
#define GPMC_D4    36	//	45 U8 	3.3V input/output		     
#define GPMC_D5    37	//	46 V8 	3.3V input/output	     
#define GPMC_D6    38	//	47 R9 	3.3V input/output		     
#define GPMC_D7    39	//	48 T9 	3.3V input/output		     
#define GPMC_A0    48	//	49 R1 	3.3V output		              
#define GPMC_A1    49	//	50 V1 	3.3V input		   
#define GPMC_A2    50	//	51 U1 	3.3V output		   
#define GPMC_A3    51	//	52 T1 	3.3V output		   
#define GPMC_A4    52	//	53 R1 	3.3V output		   
#define GPMC_A5    53	//	54 V1 	3.3V output		   
#define GPMC_A6    54	//	55 U1 	3.3V output		   
#define GPMC_A7    55	//	56 T1 	3.3V input		   
#define GPMC_A8    56	//	57 V1 	3.3V input		   
#define GPMC_A9    57	//	58 U1 	3.3V input		   
#define GPMC_A10    58	//	59 T1 	3.3V input	    
#define GPMC_A11    59	//	60 V1 	3.3V input	    
//#define EXT_WAKEUP    //	61 C5 	3.3V input	//no gpio            
//#define VDD_BAT 	  //	62  -- 	5.0V input	                   
//#define AI_0          //  63  B6 	1.8V input		//no gpio          
//#define AI_1		//	64 C7 	1.8V input 		//no gpio          
//#define AI_2		//	65 B7 	1.8V input 		//no gpio          
//#define AI_3		//	66 A7 	1.8V input 		//no gpio           
//#define AI_4		//	67 C8 	1.8V input 		//no gpio          
//#define AI_5		//	68 B8 	1.8V input 		//no gpio          
//#define AI_6		//	69 A8 	1.8V input 		//no gpio          
//#define AI_7		//	70 C9 	1.8V input 		//no gpio           
//#define               //                  
//#define               //
//#define JTAG_nTRST    //  	73 B10  3.3V input 		  //no gpio        
//#define JTAG_TDO 	//	74 A11  3.3V output		   //no gpio      
//#define JTAG_TDI 	//	75 B11  3.3V input 		   //no gpio       
//#define JTAG_TMS 	//	76 C11  3.3V input 		   //no gpio        
//#define JTAG_TCK 	//	77 A12  3.3V input 		  // no gpio       
#define MCASP0_AXR0  	112	// 78 		D12  3.3V input/output	           
#define MCASP0_AXR1 	116	// 79 		D13  3.3V input/output 	           
#define MCASP0_AHCLKR	113	// 80 		C12  3.3V output for pi only GPIO
#define MCASP0_FSR   	115	// 81 		C13  3.3V input/output             
#define MCASP0_ACLKR 	114	// 82 		B12  3.3V input/output 	            
#define MCASP0_FSX 	111	// 83 		B13  3.3V input/output            
#define MCASP0_ACLKX 	110	// 84 		A13  3.3V input/output                                                                               
//#define nWARMRSTIN  		// 85 		A10  3.3V input  //no gpio      
#define MCASP0_AHCLKX	117	// 86 		A14  3.3V input                                  
//#define nRSTOUT 		// 87 A10  3.3V output    //no gpio         
//#define GND GND     		// 88 		            //no gpio 
#define EMU1 	 	104	// 89		B14  3.3V input/output		   
#define EMU0 		103	// 90 		C14  3.3V input/output             
#define GPIO0_20	20	// 91 		D14  3.3V input/output            
#define GPIO0_19   	19      // 92 		A15  3.3V input	                    
#define GPIO0_7    	7       // 93 		C18  3.3V output                 
#define GPMC_nCS3   	64      // 94 		T13  3.3V output								                                   
#define I2C1_SCL	5	// 97 		A16  3.3V input/output	  
#define I2C1_SDA 	4	//	98 	B16  3.3V input/output	  
#define I2C2_SCL 	3	//	99 	B17  3.3V input/output	  
#define I2C2_SDA 	2	//	100	A17  3.3V input/output		  
//#define nRST_IN 		//	101	   	 3.3V input --
//#define EINT EINT 		//	102	B18	 3.3V input 	//no gpio
//#define SP706_WDO 		//	103		   3.3V output --
//#define PWR_nRST  		//	104		   3.3V output --
#define UART1_RXD	14	//	105	D16	 3.3V input 
#define UART1_TXD 	15	//	106	D15	 3.3V output		
#define UART0_RXD 	42	//	107	E15	 3.3V input 	
#define UART0_TXD 	43	//	108	E16	 3.3V output		
#define CAN0_RXD	13	//	109	D17	 3.3V input 	
#define CAN0_TXD 	12	//	110	D18	 3.3V output		
#define CAN1_RXD 	41	//	111	E17	 3.3V input 		
#define CAN1_TXD 	40	//	112	E18	 3.3V output		
#define MII_MDIO	0	//	113	M17	 3.3V input/output
#define MII_MDC 	1	//	114	M18	 3.3V output 		
                                   		
#define GPMC_nCS1	62	//	121	U9   3.3V output 	
//#define USB1_VBUS 		//	122	T18  5.0V input 	//no gpio
#define GPMC_nCS2	63	//	123	V9   3.3V output 	
//#define USB1_D_P 		//	124	R17  3.3V input/output 	//no gpio
//#define USB0_VBUS 		//	125	P15  5.0V input 	//no gpio
//#define USB1_D_N 		//	126	R18  3.3V input/output	//no gpio
//#define USB0_D_P 		//	127	N17  3.3V input/output	//no gpio
//#define USB1_ID 		//	128	P17  3.3V input 	//no gpio
//#define USB0_D_N 		//	129	N18  3.3V input/output 	//no gpio
//#define USB1_CE 		//	130	P18  3.3V output 	//no gpio
//#define USB0_ID 		//	131	P16  3.3V input 	//no gpio        
