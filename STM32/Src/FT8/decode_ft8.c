#include "gen_ft8.h"
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "constants.h"
#include "decode.h"
#include "encode.h"
#include "ldpc.h"
#include "unpack.h"
// #include "button.h"

#include "Process_DSP.h"
// #include "display.h"
// #include "options.h"
// #include "log_file.h"
#include "decode_ft8.h"
// #include "locator_ft8.h"
// #include "traffic_manager.h"

#include "FT8_main.h"
#include "lcd_driver.h"

// #include "stm32h7xx_hal.h"
// #include "main.h"

// char erase[] = "                   ";

const int kLDPC_iterations = 20;      // original 10
const int kMax_candidates = 80;       // original 20
const int kMax_decoded_messages = 30; // original 6
const int kMax_message_length = 20;
const int kMin_score = 20; // original 40 Minimum sync score threshold for candidates

int strindex(char s[], char t[]);

extern int ND;
extern int NS;
extern int NN;
// Define the LDPC sizes
extern int N;
extern int K;
extern int M;
extern int K_BYTES;

Decode new_decoded[40] = {0}; // Decode new_decoded[20]={0};

#if (defined(LAY_800x480))
int message_limit = 11;
#else
int message_limit = 6;
#endif

extern char Station_Call[];
char Target_Call[7]; // target station (partner) callsign
int Target_RSL;      // four character RSL  + /0
char Target_Grid[5]; // Grid square of the target station (partner)
char RapRcv_RSL[5];  // Recieved raport from our corespondent - used to determine if he got our "raport" mesage

int ft8_decode(void) {
	char ctmp[20] = {0}; // Debug

	// Find top candidates by Costas sync score and localize them in time and frequency
	Candidate candidate_list[kMax_candidates + 1];

	int num_candidates = find_sync(export_fft_power, ft8_msg_samples, ft8_buffer, kCostas_map, kMax_candidates, candidate_list, kMin_score);

	char decoded[kMax_decoded_messages + 1][kMax_message_length];

	const float fsk_dev = 6.25f; // tone deviation in Hz and symbol rate

	// Go over candidates and attempt to decode messages
	int num_decoded = 0;

	// Debug
	sprintf(ctmp, "Cand: %d ", num_candidates);
	LCDDriver_printText(ctmp, 10, 30, COLOR_GREEN, COLOR_BLACK, 2);

	//
	uint16_t chksumAray[kMax_decoded_messages]; // array containing the check sum of already decoded messages
	                                            // it is used to avoid duplication of messages

	for (int idx = 0; idx < num_candidates; ++idx) {
		Candidate cand = candidate_list[idx];
		float freq_hz = (cand.freq_offset + cand.freq_sub / 2.0f) * fsk_dev;

		float log174[N];
		extract_likelihood(export_fft_power, ft8_buffer, cand, kGray_map, log174);

		// bp_decode() produces better decodes, uses way less memory
		uint8_t plain[N];
		int n_errors = 0;
		bp_decode(log174, kLDPC_iterations, plain, &n_errors);
		print("FT8 Candidate: ", idx, " errors: ", n_errors);

		if (n_errors > 10) {
			println(" Many errors");
			continue;
		}

		// Extract payload + CRC (first K bits)
		uint8_t a91[K_BYTES];
		pack_bits(plain, K, a91);

		// Extract CRC and check it
		uint16_t chksum = ((a91[9] & 0x07) << 11) | (a91[10] << 3) | (a91[11] >> 5);
		a91[9] &= 0xF8;
		a91[10] = 0;
		a91[11] = 0;
		uint16_t chksum2 = crc(a91, 96 - 14);
		if (chksum != chksum2) {
			println(" CRC error");
			continue;
		}

		char message[kMax_message_length];

		char field1[14] = {0};
		char field2[14] = {0};
		char field3[7] = {0};
		int rc = unpack77_fields(a91, field1, field2, field3);
		if (rc < 0) {
			println(" RC error");
			continue;
		}

		sprintf(message, "%s %s %s ", field1, field2, field3);
		print(" Decoded ");
		println(field1, " ", field2, " ", field3);

		// Check for duplicate messages
		bool found = false;
		for (int i = 0; i < num_decoded; ++i) { // Alternative check for duplicate messages (should be much faster)
			if (chksumAray[i] == chksum) {        // the check sums are controlled
				found = true;                       // should be much faster!
				break;
			}
		}

		println(" Duplicate");

		int raw_RSL;
		int display_RSL;
		float distance;

		if (!found && num_decoded < kMax_decoded_messages) {
			if (strlen(message) > 4 && strlen(message) < kMax_message_length) {
				chksumAray[num_decoded] = chksum; // Used to check for duplicate messages
				// strcpy(decoded[num_decoded], message);
				// strcpy(decoded[num_decoded], field1);
				// memcpy(decoded[num_decoded], field1,6);    //Used for check only the first field (not the whole mesage)

				new_decoded[num_decoded].sync_score = cand.score;
				//           new_decoded[num_decoded].freq_hz = (int)freq_hz;
				strcpy(new_decoded[num_decoded].field1, field1);
				strcpy(new_decoded[num_decoded].field2, field2);
				strcpy(new_decoded[num_decoded].field3, field3);
				//            strcpy(new_decoded[num_decoded].decode_time, rtc_string);

				// memcpy(new_decoded[num_decoded].field1, field1,6);
				// memcpy(new_decoded[num_decoded].field2, field2,6);
				// memcpy(new_decoded[num_decoded].field3, field3,6);
				//            memcpy(new_decoded[num_decoded].decode_time, rtc_string,8);

				raw_RSL = new_decoded[num_decoded].sync_score;
				if (raw_RSL > 160) {
					raw_RSL = 160;
				}
				display_RSL = (raw_RSL - 160) / 6;
				new_decoded[num_decoded].snr = display_RSL;

				++num_decoded;

				println(" OK");
			} else {
				println(" Length error");
			}
		}

	} // End of big decode loop

	// Debug
	/*
	    for (int tmp = 0; tmp < num_decoded; ++tmp)
	        {
	        sprintf(ctmp,"%s %s %s", new_decoded[tmp].field1,new_decoded[tmp].field2,new_decoded[tmp].field3);
	        println(ctmp);	//Debug
	        }
	*/
	return num_decoded;
}

void display_messages(int decoded_messages) {

	char message[kMax_message_length];
	//      char big_gulp[60];

	//			LCDDriver_Fill_RectXY(0, 100, 240, 240,COLOR_BLACK);	//Clear the old mesages

	for (int i = 0; i < decoded_messages && i < message_limit; i++) {
		sprintf(message, "%s %s %s", new_decoded[i].field1, new_decoded[i].field2, new_decoded[i].field3);
		LCDDriver_printText(message, 10, 100 + i * 25, COLOR_GREEN, COLOR_BLACK, 2);
	}
}

void SetNew_TargetCall(int index) {

	char selected_station[18];
	char blank[] = "        ";
	strcpy(Target_Call, new_decoded[index].field2); // take the target (partner) callsign
	Target_RSL = new_decoded[index].snr;

	strcpy(Target_Grid, new_decoded[index].field3); // take the target (partner) gridsqare

	sprintf(selected_station, "%7s %3i", Target_Call, Target_RSL);
}

// take the recieved raport from the oposite side
// it is used to determine if the oposide side recieved our mesage and if yes we can send "73"
//"index" is the message index
//"CQ_Answer" showing if we check the answer when we call the "CQ" or we answer a "CQ"
int CheckRecievedRaportRSL(int index, char CQ_Answer) {
	strcpy(RapRcv_RSL, new_decoded[index].field3);

	if (CQ_Answer) {
		if ((RapRcv_RSL[0] == '-') || (RapRcv_RSL[0] == '+')) {
			return 1;
		} else {
			return 0;
		}
	} else {
		if ((RapRcv_RSL[0] == 'R') && ((RapRcv_RSL[1] == '-') || (RapRcv_RSL[1] == '+'))) {
			return 1;
		} else {
			return 0;
		}
	}
}

// it is used to determine if the oposide side recieved our last mesage
//"index" is the message index
//"CQ_Answer" showing if we check the answer when we call the "CQ" or we answer a "CQ"
int CheckRecieved73(int index, char CQ_Answer) {
	char RapRcv_73[7]; // Recieved raport from our corespondent - used to determine if he got our "raport" mesage

	strcpy(RapRcv_73, new_decoded[index].field3);

	if (CQ_Answer) {
		if ((RapRcv_73[0] == 'R') && (RapRcv_73[1] == 'R')) {
			return 1;
		} else {
			return 0;
		}
	} else {
		if ((RapRcv_73[0] == '7') && (RapRcv_73[1] == '3')) {
			return 1;
		} else {
			return 0;
		}
	}
}

/*
  Find the index of the partner we were talking till now

  The function is called after "Check_Calling_Stations" so for shure we know someodey called us
  we just have to be sure it is the station we were in contact till now!
*/
int FindPartnerIDX(int num_decoded) {
	uint8_t Partner_Idx;

	for (int i = 0; i < num_decoded; i++) {
		if (strindex(new_decoded[i].field2, Target_Call) >= 0) {
			Partner_Idx = i + 100; // Add 100 to be sure that match was found
		}
	}

	if (Partner_Idx > 0) {      // Match was found
		return Partner_Idx - 100; // return the index of the match
	} else {                    // no match was found
		return -1;
	}
}

void display_details(int decoded_messages) {

	char message[48];

	// tft.fillRect(0, 100, 500, 320, RA8875_BLACK);

	for (int i = 0; i < decoded_messages && i < message_limit; i++) {
		sprintf(message, "%7s %7s %4s %4i %3i %4i", new_decoded[i].field1, new_decoded[i].field2, new_decoded[i].field3, new_decoded[i].freq_hz, new_decoded[i].snr, new_decoded[i].distance);
	}
}

int strindex(char s[], char t[]) {
	int i, j, k, result;

	result = -1;

	for (i = 0; s[i] != '\0'; i++) {
		for (j = i, k = 0; t[k] != '\0' && s[j] == t[k]; j++, k++) {
			;
		}
		if (k > 0 && t[k] == '\0') {
			result = i;
		}
	}
	return result;
}

/*
 Run through all decoded messages (from the last cycle)
 Return "-1" if nobody called us
 Return the index of the message where we were called
If more stations called us it is returned the index of the last one
*/
int Check_Calling_Stations(int num_decoded) {
	// char big_gulp[60];
	char message[kMax_message_length];
	int Ans_Mess_IDX = 0;

	char To_meCallIdx = 0; // used to indicate on the right the mesages addresed to us

	for (int i = 0; i < num_decoded; i++) {
		if (strindex(new_decoded[i].field1, Station_Call) >= 0) {
			sprintf(message, "%s %s %s", new_decoded[i].field1, new_decoded[i].field2, new_decoded[i].field3);

			if (To_meCallIdx <= message_limit) // Display up to 6 mesages addrssed to us (shuld be enough)
			{
				LCDDriver_printText(message, 240, 100 + To_meCallIdx * 25, COLOR_YELLOW, COLOR_BLACK, 2);
				To_meCallIdx++;
			}

			Ans_Mess_IDX = i + 100; // used to detect change (0 is also valid return as it is used for indexing)
		}
	}

	if (Ans_Mess_IDX >= 100) {
		return Ans_Mess_IDX - 100;
	} else {
		return -1;
	}
}