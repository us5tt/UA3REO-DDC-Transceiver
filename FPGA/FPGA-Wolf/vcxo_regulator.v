module vcxo_regulator(
pwm_clk_in,
VCXO_correction,

pump
);

input pwm_clk_in;
input signed [15:0] VCXO_correction;

output reg pump = 0;

reg unsigned [15:0] PWM_counter_MAX = 65500;
reg unsigned [15:0] PWM_counter = 0;

always @ (posedge pwm_clk_in)
begin
	//count PWM
	if(PWM_counter > PWM_counter_MAX)
		PWM_counter = 0;
	else
		PWM_counter = PWM_counter + 1;
	
	//do PWM
	if(PWM_counter <= ($signed(VCXO_correction) + 32750))
		pump = 1;
	else
		pump = 0;
end


endmodule
