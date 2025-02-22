// tx_cic.v

// Generated using ACDS version 18.1 625

`timescale 1 ps / 1 ps
module tx_cic (
		input  wire [1:0]  in_error,          //  av_st_in.error
		input  wire        in_valid,          //          .valid
		output wire        in_ready,          //          .ready
		input  wire [15:0] in_data,           //          .in_data
		input  wire        in_startofpacket,  //          .startofpacket
		input  wire        in_endofpacket,    //          .endofpacket
		output wire [13:0] out_data,          // av_st_out.out_data
		output wire [1:0]  out_error,         //          .error
		output wire        out_valid,         //          .valid
		input  wire        out_ready,         //          .ready
		output wire        out_startofpacket, //          .startofpacket
		output wire        out_endofpacket,   //          .endofpacket
		output wire        out_channel,       //          .channel
		input  wire        clken,             //     clken.clken
		input  wire        clk,               //     clock.clk
		input  wire        reset_n            //     reset.reset_n
	);

	tx_cic_cic_ii_0 cic_ii_0 (
		.clk               (clk),               //     clock.clk
		.reset_n           (reset_n),           //     reset.reset_n
		.clken             (clken),             //     clken.clken
		.in_error          (in_error),          //  av_st_in.error
		.in_valid          (in_valid),          //          .valid
		.in_ready          (in_ready),          //          .ready
		.in_data           (in_data),           //          .in_data
		.in_startofpacket  (in_startofpacket),  //          .startofpacket
		.in_endofpacket    (in_endofpacket),    //          .endofpacket
		.out_data          (out_data),          // av_st_out.out_data
		.out_error         (out_error),         //          .error
		.out_valid         (out_valid),         //          .valid
		.out_ready         (out_ready),         //          .ready
		.out_startofpacket (out_startofpacket), //          .startofpacket
		.out_endofpacket   (out_endofpacket),   //          .endofpacket
		.out_channel       (out_channel)        //          .channel
	);

endmodule
