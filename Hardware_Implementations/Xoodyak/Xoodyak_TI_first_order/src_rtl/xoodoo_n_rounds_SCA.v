/*===========================================================================*\
           Filename : xoodoo_n_rounds_SCA.v
             Author : Shuying Yin <yinshuying@mail.tsinghua.edu.cn>
        Description : N rounds of Xoodoo
          Called by : xoodoo_SCA
   Revision History : 2022-04-18, Revision 0.1.0, Shuying Yin
                      2022-04-23, Revision 0.1.1, Shuying Yin
                        Change the port width of rdi from 768 bits to 384 bits.
                        Add rdi0_en & rdi1_en to fix the handshake of rdi.
                        Remove parameter roundsPerCycle.
            License : GNU General Public License v3.0 (GPL-3.0)
                      For more information please see:
                      https://spdx.org/licenses/GPL-3.0.html
\*===========================================================================*/

module xoodoo_n_rounds_SCA
(
    input           clk            ,
    input           rst            ,
    input  [ 383:0] state_in0      ,
    input  [ 383:0] state_in1      ,
    input  [ 383:0] rdi            ,
    input           rdi0_en        ,
    input           rdi1_en        ,
    output [ 383:0] state_out0     ,
    output [ 383:0] state_out1     ,
    input  [  12:0] j_in           ,
    output [  12:0] j_out
);
 
    wire   [  31:0] rc;

    xoodoo_rconst u_rconst(.clk(clk),.rst(rst),.j_in(j_in),.rc(rc),.j_out(j_out));
    xoodoo_round_SCA  u_round(.clk(clk),.rst(rst),.in_0(state_in0),.in_1(state_in1),.rdi(rdi),.rdi0_en(rdi0_en),.rdi1_en(rdi1_en),.rconst(rc),.out_0(state_out0),.out_1(state_out1));

endmodule

module xoodoo_rconst
(
    input                  clk    ,
    input                  rst    ,
    input         [  12:0] j_in   ,
    output  reg   [  31:0] rc     ,
    output  reg   [  12:0] j_out
);
 
    always @(posedge clk) begin
      if(rst) begin
         rc   [31:0] <= 32'b0;
         j_out[12:0] <= 13'b0;
      end 
      else begin
        rc[01] <= j_in[11];
        rc[02] <= |{j_in[7],j_in[5]};
        rc[03] <= |{j_in[7],j_in[1:0]};
        rc[04] <= |{j_in[11],j_in[9],j_in[5],j_in[3],j_in[1:0]};
        rc[05] <= |{j_in[10:9],j_in[7:6],j_in[4],j_in[1]};
        rc[06] <= |{j_in[9],j_in[6],j_in[3:2],j_in[0]};
        rc[07] <= |{j_in[10:8],j_in[3:2]};
        rc[08] <= |{j_in[10],j_in[8],j_in[4],j_in[2]};
        rc[09] <= |{j_in[8],j_in[2]};
        {rc[31:10],rc[0]} <= 23'b0;
        j_out  <= j_in<<1;
      end
    end

endmodule
