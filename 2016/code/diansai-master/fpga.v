module zonghe(BIT,DATE,CLK,spi_clk,cs,spi_date,spi_out,fre,pwm,test);

     output [3:0]BIT;
     output [7:0]DATE;
	  output  spi_out,pwm;
     input CLK,cs,spi_date,spi_clk,fre;

	  output test;
	  reg test;
	  
     reg [3:0]BIT;
     reg [7:0]DATE;
     reg [7:0]DATE_0;
	  reg [7:0]DATE_1;
     reg [7:0]DATE_2;
     reg [7:0]DATE_3;
	  reg SHANSHUO_0;
	  reg SHANSHUO_1;
     reg SHANSHUO_2;
     reg SHANSHUO_3;
	  reg [8:0]shanshuo_i;
     reg [1:0]j;
     reg [15:0]i;
     reg CLK_100;
	  
	  reg [15:0]spi_receive;
	  reg [8:0]spi_receive_num;
	  reg receive_flag;
	  reg spi_xianshi_flag;
	  reg [31:0]send_date;
	  reg spi_out;
	  
	  
	  reg fre_flag;
	  reg [31:0]fre_num;
	  reg [31:0]fre_num1;
	  reg [31:0]duty_num;
	  
	  reg pwm;
	  reg [7:0]pwm_i;
	  reg [7:0]pwm_duty;
	  
	  
	  reg [31:0]tfre_time;

	  function [0:7] yima;
			input [4:0]num;
				begin
					case(num)
					0:begin yima=8'h3f;end
					1:begin yima=8'h06;end
					2:begin yima=8'h5b;end
					3:begin yima=8'h4f;end
					4:begin yima=8'h66;end
					5:begin yima=8'h6d;end
					6:begin yima=8'h7d;end
					7:begin yima=8'h07;end
					8:begin yima=8'h7f;end
					9:begin yima=8'h6f;end
					16:begin yima=8'hbf;end
					17:begin yima=8'h86;end
					18:begin yima=8'hdb;end
					19:begin yima=8'hcf;end
					20:begin yima=8'he6;end
					21:begin yima=8'hed;end
					22:begin yima=8'hfd;end
					23:begin yima=8'h87;end
					24:begin yima=8'hff;end
					25:begin yima=8'hef;end
					endcase
				end
		endfunction
		
		
     initial
     begin
         DATE_0=8'b00000000;
         DATE_1=8'b00000000;
         DATE_2=8'b00000000;
         DATE_3=8'b00000000;
         BIT=4'b0000;
         j=2'b00;
     end

     always@(posedge CLK)
     begin
         i<=i+1'b1;
         if(i==16'h0fff)
         begin
             CLK_100=~CLK_100;
             i<=16'h0000;
         end
			
			tfre_time=tfre_time+1;
			if(tfre_time==48000000)
			begin
				
				tfre_time=0;
			end
			
			if(fre_flag)
				begin
					fre_num1=fre_num1+1;
				end
			else
				begin
					fre_num1=0;
				end
			
			pwm_i=pwm_i+1;
			if(pwm_i>pwm_duty)
				pwm=0;
			else
				pwm=1;
			
     end
	  
	  
	  always@(posedge fre)
     begin
		  if(fre_flag)
			  begin
					fre_num=fre_num1;
			  end
		  fre_flag=fre_flag+1;
     end
	  
	  always@(negedge fre)
	  begin
		  if(fre_flag)
				duty_num=fre_num1;
	  end
	  

     always@(posedge CLK_100)
     begin
         case (j)
         2'b00:
             begin		 
		          BIT=4'b0001;//0濠电姷鏁搁崵鍫縏E_0=8'b11111111;
					 if(SHANSHUO_0==0)
							begin
							DATE=DATE_0;
							end
					else
						begin
							if(shanshuo_i>256)
								begin
									DATE=DATE_0;
								end
						else
							begin
								DATE=0;
							end
					end
					j<=2'b01;
             end		 
         2'b01: 
             begin		 
					BIT=4'b0010;//1濠电姷鏁告慨鐑剿囩粋鎿矱_1=8'b01111111;
						if(SHANSHUO_1==0)
							begin
								DATE=DATE_1;
							end
						else
							begin
								if(shanshuo_i>256)
									begin
										DATE=DATE_1;
									end
							else
								begin
									DATE=0;
							end
						end
					j<=2'b10;
             end
         2'b10:         
		     begin		 
					BIT=4'b0100;//2濠电姷鏁告慨鐑藉极閹间礁鍚归柛顐ょ瑣E_2=8'b10111111;
					if(SHANSHUO_2==0)
							begin
								DATE=DATE_2;
							end
						else
							begin
								if(shanshuo_i>256)
									begin
										DATE=DATE_2;
									end
							else
								begin
									DATE=0;
							end
						end
						j<=2'b11;
             end
         2'b11:          
		     begin		
					BIT=4'b1000;//3濠电姷鏁告慨鐑剿囩粋鎿矱_3=8'b00000000;
						if(SHANSHUO_3==0)
							begin
								DATE=DATE_3;
							end
						else
							begin
								if(shanshuo_i>256)
									begin
										DATE=DATE_3;
									end
							else
								begin
									DATE=0;
							end
						end
					j<=2'b00;
					shanshuo_i=shanshuo_i+1;
             end
         endcase
     end

		always @(posedge spi_clk)
		if(cs==0)
			if(receive_flag==0)
				begin
					spi_receive=spi_receive << 1;
					spi_receive[0]=spi_date;
					spi_receive_num=spi_receive_num+1;
					
					if(spi_receive_num==16)
							begin
								case(spi_receive[15:8])
									0:begin 
											DATE_0=yima(spi_receive[4:0]);
											if(spi_receive[5]==1)
												SHANSHUO_0=1;
											else
												SHANSHUO_0=0;
									  end
									1:begin 
											DATE_1=yima(spi_receive[4:0]);
											if(spi_receive[5]==1)
												SHANSHUO_1=1;
											else
												SHANSHUO_1=0;
										end
									2:begin 
											DATE_2=yima(spi_receive[4:0]);
											if(spi_receive[5]==1)
												SHANSHUO_2=1;
											else
												SHANSHUO_2=0;
										end
									3:begin 
											 DATE_3=yima(spi_receive[4:0]);
											 if(spi_receive[5]==1)
												SHANSHUO_3=1;
											 else
												SHANSHUO_3=0;
										end
									4:begin
											pwm_duty=spi_receive[7:0];
									  end
								endcase
							spi_receive_num=0;
						end
					
					
					
					if(spi_receive_num==8)
						begin
						if(spi_receive[7:0]==8'hbf)
							begin
							receive_flag=1;
							send_date=fre_num;
							spi_receive_num=0;
							end
						if(spi_receive[7:0]==8'hbe)
							begin
							receive_flag=1;
							send_date=duty_num;
							spi_receive_num=0;
							end
							if(spi_receive[7:0]<5)
								begin
								spi_xianshi_flag=1;
								end
							else
							begin
								spi_receive_num=0;
								spi_xianshi_flag=0;
							end
						end
				  end
			else
				begin
					spi_out=send_date[31];
					send_date = send_date<< 1;
					spi_receive_num=spi_receive_num+1;
					if(spi_receive_num==32)
						begin
							spi_receive_num=0;
							receive_flag=0;
						end
				end
		else
			begin
				spi_receive_num=0;
				receive_flag=0;
			end
////////////////////////////////////////////////////
endmodule
